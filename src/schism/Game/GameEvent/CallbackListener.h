#pragma once

#include <parallel_hashmap/phmap.h>
#include <type_traits>
#include <typeindex>
#include <any>
#include "schism/System/SyncQueue.h"
#include "schism/System/Log.h"

namespace Schism::GameEvent
{
    class CallbackBus;
    class CallbackListener
    {
        using Queue = System::SyncQueue<std::any>;
    public:
        template<typename T>
        using EventCallback = std::function<void(T&&)>;

        template<typename T>
        [[nodiscard]] bool GameEventRegistered()
        {
            return m_QueueMap.find(typeid(T)) != m_QueueMap.end();
        }

        template<typename T>
        void ListenGameEvent(EventCallback<T>&& eventFunction)
        {
            if (!GameEventRegistered<T>())
            {
                m_QueueMap[typeid(T)] = std::make_unique<Queue>();
            }
            m_CallbackMap[typeid(T)] = [functor = std::move(eventFunction)](std::any ev)
            {
                functor(std::any_cast<T>(ev));

            };
        }

        void ProcessGameEvents(size_t count = 5)
        {
            for (auto& i : m_QueueMap)
            {
                auto& callback = m_CallbackMap[i.first];
                i.second->Drain(count, callback);
            }
        }
    private:
        friend CallbackBus;

        template<typename T>
        void ProduceGameEvent(T&& o)
        {
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second->Push(std::forward<T>(o));
                return;
            }
            SC_CORE_WARN("(CallbackListener) ProduceGameEvent: type not found");
        }

        template<typename T>
        void ProduceGameEvent(const T& o)
        {
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second->Push(o);
                return;
            }
            SC_CORE_WARN("(CallbackListener) ProduceGameEvent: type not found");
        }

        phmap::parallel_node_hash_map<std::type_index, std::unique_ptr<Queue>> m_QueueMap;
        phmap::parallel_node_hash_map<std::type_index, std::function<void(std::any)>> m_CallbackMap;
    };
}