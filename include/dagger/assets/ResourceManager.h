#ifndef DAGGER_ASSETS_RESOURCE_MANAGER_H
#define DAGGER_ASSETS_RESOURCE_MANAGER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace dagger
{

template <typename T, typename HandleType>
class ResourceManager
{
public:
    bool has(const std::string& id) const
    {
        return nameToHandle_.find(id) != nameToHandle_.end();
    }

    HandleType getHandle(const std::string& id)
    {
        auto it = nameToHandle_.find(id);
        if (it != nameToHandle_.end())
        {
            return it->second;
        }
        return HandleType{};
    }

    HandleType add(const std::string& id, T resource)
    {
        // Check if resource already exists
        auto it = nameToHandle_.find(id);
        if (it != nameToHandle_.end())
        {
            return it->second;
        }

        uint32_t index = getFreeSlot();
        Slot& slot = slots_[index];

        slot.resource = std::move(resource);
        slot.generation++;
        slot.active = true;

        HandleType handle{index, slot.generation};
        nameToHandle_[id] = handle;
        return handle;
    }

    void replace(const std::string& id, T newResource)
    {
        auto it = nameToHandle_.find(id);
        if (it != nameToHandle_.end())
        {
            Slot& slot = slots_[it->second.index];
            slot.resource.destroy();
            slot.resource = std::move(newResource);
        }
    }

    T* resolve(HandleType handle)
    {
        if (!handle.isValid() || handle.index >= slots_.size())
        {
            return nullptr;
        }

        Slot& slot = slots_[handle.index];
        if (slot.active && slot.generation == handle.generation)
        {
            return &slot.resource;
        }

        return nullptr;
    }

    void release(HandleType handle)
    {
        if (!handle.isValid() || handle.index >= slots_.size()) return;
        Slot& slot = slots_[handle.index];

        if (slot.active && slot.generation == handle.generation)
        {
            slot.resource.destroy();
            slot.active = false;

            for (auto it = nameToHandle_.begin(); it != nameToHandle_.end(); ++it)
            {
                if (it->second.index == handle.index && it->second.generation == handle.generation)
                {
                    nameToHandle_.erase(it);
                    break;
                }
            }
        }
    }

    void clear()
    {
        for (auto& slot : slots_)
        {
            if (slot.active)
            {
                slot.resource.destroy();
                slot.active = false;
            }
        }
        nameToHandle_.clear();
    }

private:
    struct Slot
    {
        T resource;
        uint32_t generation = 0;
        bool active = false;
    };

    std::vector<Slot> slots_;
    std::unordered_map<std::string, HandleType> nameToHandle_;

    uint32_t getFreeSlot()
    {
        for (size_t i = 0; i < slots_.size(); ++i)
        {
            if (!slots_[i].active)
            {
                return static_cast<uint32_t>(i);
            }
        }

        slots_.push_back(Slot{});
        return static_cast<uint32_t>(slots_.size() - 1);
    }
};

} // namespace dagger

#endif // DAGGER_ASSETS_RESOURCE_MANAGER_H
