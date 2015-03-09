#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class BlockingQueue
{
private:
	std::mutex				m_mutex;
	std::condition_variable	m_condition;
	std::deque<T>			m_queue;
public:
	BlockingQueue();
	~BlockingQueue();

	BlockingQueue(BlockingQueue const&) = delete;
	BlockingQueue(BlockingQueue&&) = delete;
	BlockingQueue&	operator=(BlockingQueue const&) = delete;
	BlockingQueue&	operator=(BlockingQueue&&) = delete;

	void push(T const& value) 
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_queue.push_front(value);
		}
		m_condition.notify_one();
	}

	T pop() 
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [=]{ return !m_queue.empty(); });
		T rc(std::move(m_queue.back()));
		m_queue.pop_back();
		return rc;
	}

	bool tryPop(T & v, std::chrono::milliseconds dur) 
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (!m_condition.wait_for(lock, dur, [=]{ return !m_queue.empty(); }))
			return false;
		v = m_queue.back();
		m_queue.pop_back();
		return true;
	}

	int size() 
	{
		return static_cast<int>(m_queue.size());
	}
};

template <typename T>
BlockingQueue<T>::~BlockingQueue()
{

}

template <typename T>
BlockingQueue<T>::BlockingQueue()
{

}
