#pragma once

#include <queue>
#include <condition_variable>
#include <mutex>
#include <optional>
#include "Log.h"

namespace Schism::System
{
	// Multiple producers one consumer
	template<typename T>
	class SyncQueue
	{
	public:
		SyncQueue() = default;

		void Push(T&& obj)
		{
			{
				std::scoped_lock lck(m_Mutex);
				m_Queue.push(std::forward<T>(obj));
			}
			m_Condition.notify_one();
		}

		void Push(const T& obj)
		{
			{
				std::scoped_lock lck(m_Mutex);
				m_Queue.push(obj);
			}
			m_Condition.notify_one();
		}

		void Pop()
		{
			std::scoped_lock lck(m_Mutex);
			m_Queue.pop();
		}

		T Peek()
		{
			std::unique_lock lck(m_Mutex);

			if (!m_Queue.empty())
			{
				return m_Queue.front();
			}

			m_Condition.wait(lck, [this]()
				{
					return !m_Queue.empty();
				});

			return m_Queue.front();
		}

		T PeekPop()
		{
			std::unique_lock lck(m_Mutex);

			if (!m_Queue.empty())
			{
				T o = m_Queue.front();
				m_Queue.pop();
				return o;
			}

			m_Condition.wait(lck, [this]()
				{
					return !m_Queue.empty();
				});

			T result = m_Queue.front();
			m_Queue.pop();
			return result;
		}

		std::optional<T> WaitForPeek(std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
		{
			if (timeout == std::chrono::milliseconds(0))
			{
				std::scoped_lock lck(m_Mutex);

				if (m_Queue.empty())
				{
					return std::nullopt;
				}

				return m_Queue.front();
			}

			std::unique_lock lck(m_Mutex);

			bool hasTimedout = !m_Condition.wait_for(lck, timeout, [this]()
			{
				return !m_Queue.empty();
			});

			if (hasTimedout)
			{
				return std::nullopt;
			}
			return m_Queue.front();
		}

		std::optional<T> WaitForPeekPop(std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
		{
			if (timeout == std::chrono::milliseconds(0))
			{
				std::scoped_lock lck(m_Mutex);

				if (m_Queue.empty())
				{
					return std::nullopt;
				}

				T result = m_Queue.front();
				m_Queue.pop();
				return result;
			}

			std::unique_lock lck(m_Mutex);

			bool hasTimedout = !m_Condition.wait_for(lck, timeout, [this]()
				{
					return !m_Queue.empty();
				});

			if (hasTimedout)
			{
				return std::nullopt;
			}

			T result = m_Queue.front();
			m_Queue.pop();
			return result;
		}

        template<bool ShouldLockOnLoop = false> // True only if functor is a function that takes a long time
        void Drain(typename std::queue<T>::size_type count, std::function<void(T&&)> functor)
        {
            if constexpr (!ShouldLockOnLoop)
            {
                std::scoped_lock lck(m_Mutex);

                if (m_Queue.empty())
                {
                    return; // Log out error
                }

                for (typename std::queue<T>::size_type i = 0; i < count; i++)
                {
                    if (m_Queue.empty())
                    {
                        return;
                    }
                    T obj = m_Queue.front();
                    m_Queue.pop();
                    functor(std::move(obj));
                }
            }
            else
            {
                for (typename std::queue<T>::size_type i = 0; i < count; i++)
                {
                    std::scoped_lock lck(m_Mutex);

                    if (m_Queue.empty())
                    {
                        continue;
                    }

                    T obj = m_Queue.front();
                    m_Queue.pop();
                    functor(std::move(obj));
                }
            }

        }

		typename std::queue<T>::size_type Size()
		{
			std::scoped_lock lck(m_Mutex);
			return m_Queue.size();
		}
	private:
		std::queue<T> m_Queue;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
	};
}