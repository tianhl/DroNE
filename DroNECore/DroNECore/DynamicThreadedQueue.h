#ifndef DYNAMIC_THREADED_QUEUE_h 
#define DYNAMIC_THREADED_QUEUE_h 
// @(#)DroNE/DroNECore:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DynamicThreadedQueue.h
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <queue>

template<typename T>
class DataItem{
	public:
		DataItem(T p, size_t s):m_pData(p),m_size(s){};
		virtual ~DataItem(){};

		T        getData(){return m_pData;};
		size_t   getSize(){return m_size;};
	private:
		DataItem(){};
		// set const?
		T         m_pData;
		size_t    m_size;
};

template<typename T>
class DynamicThreadedQueue
:boost::noncopyable
{
	public:
		DynamicThreadedQueue() : queue_(), cond_(), mutex_() {}
		~DynamicThreadedQueue(){}

		void put(const T& obj) {
			boost::unique_lock<boost::mutex> lock(mutex_); 
			queue_.push(obj);
			cond_.notify_all();
		}

		T get() {
                        // If queue size is zero, process is blocked.
			boost::unique_lock<boost::mutex> lock(mutex_); 
			while(queue_.size()== 0) cond_.wait(lock);
			T front(queue_.front());
			queue_.pop();
			return front;
		}

		T getUB() {
                        // If queue size is zero, process returns NULL.
			boost::unique_lock<boost::mutex> lock(mutex_); 
			if(0 == queue_.size()) return NULL; 
			T front(queue_.front());
			queue_.pop();
			return front;
		}

		unsigned size() { return queue_.size(); }

		void notify_all() { cond_.notify_all(); }
	private:
		std::queue<T> queue_;
		boost::condition_variable_any cond_;
		boost::mutex mutex_;
};


#endif
