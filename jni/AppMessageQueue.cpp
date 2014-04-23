/*
 * AppMessageQueue.cpp
 *
 *  Created on: Apr 22, 2014
 */

#include "AppMessageQueue.h"

void AppMessageQueue::Enqueue(const AppMessages::IAppMessage* const message)
{
	m_queue.Enqueue(message);
}

bool AppMessageQueue::TryDequeue(const AppMessages::IAppMessage*& out_message)
{
	return m_queue.TryDequeue(out_message);
}





