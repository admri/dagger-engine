#ifndef DAGGER_CORE_EVENT_BUS_H
#define DAGGER_CORE_EVENT_BUS_H

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace dagger
{

class EventBus
{
public:
    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> callback)
    {
        auto type = std::type_index(typeid(EventType));
        
        // Create a new handler in the map if nobody subscribes to it yet
        if (handlers_.find(type) == handlers_.end())
        {
            handlers_[type] = std::make_unique<EventHandler<EventType>>();
        }

        // Add callback to the list
        auto* handler = static_cast<EventHandler<EventType>*>(handlers_[type].get());
        handler->callbacks.push_back(std::move(callback));
    }

    template <typename EventType, typename... Args>
    void emit(Args&&... args)
    {
        auto type = std::type_index(typeid(EventType));
        auto it = handlers_.find(type);
        
        // If somebody is subscribed to this type, we notify them
        if (it != handlers_.end())
        {
            // Create an event instance with forwarded arguments
            EventType event{std::forward<Args>(args)...};
            
            auto* handler = static_cast<EventHandler<EventType>*>(it->second.get());
            
            // Call all registered listeners
            for (auto& callback : handler->callbacks)
            {
                callback(event);
            }
        }
    }

    void clear()
    {
        handlers_.clear();
    }

private:
    // EventHandler interface to allow for storing
    // different event type handlers in one map
    struct IEventHandler
    {
        virtual ~IEventHandler() = default;
    };

    // Struct that holds the event type and a list of callbacks
    template <typename EventType>
    struct EventHandler : public IEventHandler
    {
        std::vector<std::function<void(const EventType&)>> callbacks;
    };

    // Map with EventTypes (keys) and lists of listeners (values)
    std::unordered_map<std::type_index, std::unique_ptr<IEventHandler>> handlers_;
};

} // namespace dagger

#endif // DAGGER_CORE_EVENT_BUS_H
