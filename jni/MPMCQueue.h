// Copyright (c) 2014 eeGeo. All rights reserved.

#ifndef MPMCQUEUE_H_
#define MPMCQUEUE_H_

#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <list>

namespace Eegeo
{
	namespace Android
	{
		namespace Queues
		{
			template<typename T>
			class MPMCQueue
			{
				public:
				   MPMCQueue();
				   ~MPMCQueue();

				   size_t Size() const;
				   void Enqueue(T message);
				   T Dequeue();
				   bool TryDequeue(T& message);
				private:
				   T DequeueMessageFromHead();
				   std::queue<T, std::list<T> > m_queue;
				   pthread_mutex_t m_mutex;
				   sem_t m_signal;
			};
		}
	}
}

#include "MPMCQueue.impl"

#endif
