#pragma once

#include <type_traits>
#include <condition_variable>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <parallel_hashmap/phmap.h>
#include <any>
#include <memory>
#include "schism/System/SyncQueue.h"
#include "schism/System/Log.h"

namespace Schism::GameEvent
{
	class SyncBus;
	class SyncListener
	{
		using Queue = System::SyncQueue<std::any>;
	public:
		SyncListener() = default;
		virtual ~SyncListener() = default;

		template<typename T>
		void RegisterGameEvent()
		{
			m_QueueMap[typeid(T)] = std::make_unique<Queue>();
		}

		template<typename T>
		[[nodiscard]] bool GameEventRegistered()
		{
			return m_QueueMap.find(typeid(T)) != m_QueueMap.end();
		}

		template<typename T>
		[[nodiscard]] T ListenGameEvent()
		{
			return std::any_cast<T>(m_QueueMap[typeid(T)]->PeekPop());
		}

        template<typename T>
        void ListenAllEvents()
        {

        }

		template<typename T>
		[[nodiscard]] std::optional<T> ListenGameEventFor(std::chrono::milliseconds timeout)
		{
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                std::any_cast<T>(i->second->WaitForPeekPop(timeout));
            }
            SC_CORE_ERROR("(SyncListener) Event not registered {0}", typeid(T).name());
		}

	private:
		template<typename T>
		void ProduceGameEvent(T&& o)
		{
			if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second->Push(std::forward<T>(o));
            }

            SC_CORE_ERROR("(SyncListener) Event not registered {0}", typeid(T).name());
		}

		template<typename T>
		void ProduceGameEvent(const T& o)
		{
            if (auto i = m_QueueMap.find(typeid(T)); i != m_QueueMap.end())
            {
                i->second->Push(o);
            }
		}


        phmap::parallel_flat_hash_map<std::type_index, std::unique_ptr<Queue>> m_QueueMap;
		//std::unordered_map<std::type_index, Queue> m_QueueMap;
        //std::mutex m_Mutex;
		friend SyncBus;
	};
}