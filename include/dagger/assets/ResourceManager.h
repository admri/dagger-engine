#ifndef DAGGER_ASSETS_RESOURCE_MANAGER_H
#define DAGGER_ASSETS_RESOURCE_MANAGER_H

#include <concepts>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace dagger
{

template <typename T>
concept IsResourceHandle = requires(T handle)
{
    { handle.index } -> std::convertible_to<uint32_t>;
    { handle.generation } -> std::convertible_to<uint32_t>;
    { handle.isValid() } -> std::same_as<bool>;
};

/// @brief Internal container managing the lifecycle, storage, and generational indices of a specific resource type.
template <typename T, IsResourceHandle HandleType>
class ResourceManager
{
public:
    /// @brief Checks if a resource with the given string ID is currently loaded.
    bool has(const std::string& id) const
    {
        return nameToHandle_.find(id) != nameToHandle_.end();
    }

    /// @brief Retrieves the generational handle associated with a string ID.
    HandleType getHandle(const std::string& id)
    {
        auto it = nameToHandle_.find(id);
        if (it != nameToHandle_.end())
        {
            return it->second;
        }
        return HandleType{};
    }

    /// @brief Takes ownership of a newly loaded resource and issues a handle for it.
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

    /// @brief Replaces the resource data at a specific ID, keeping existing handles valid.
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

    /// @brief Safely translates a handle into a raw pointer. Returns nullptr if generations mismatch.
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

    /// @brief Destroys the resource and marks its slot as free, invalidating all existing handles to it.
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

    /// @brief Destroys all managed resources and clears the registry.
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
