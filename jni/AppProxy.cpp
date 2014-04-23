/*
 * AppProxy.cpp
 *
 *  Created on: Apr 22, 2014
 */

#include "Types.h"
#include "AppProxy.h"
#include "AppRunner.h"
#include "TouchEventMessage.h"

AppProxy::AppProxy(AppRunner& appRunner)
: m_appRunner(appRunner)
{
}

void AppProxy::Pause(PersistentAppState* pPersistentState)
{
	m_appRunner.SendMessage(m_pauseMessage);
	m_appRunner.WaitForStop();
}

void AppProxy::Resume()
{
	m_appRunner.Start();
}

void AppProxy::ActivateSurface()
{
	m_appRunner.SendMessage(m_displayAvailableMessage);
}

void AppProxy::SendTouchEvent(const InputMessages::TouchEventMessage& touchEvent)
{
	m_appRunner.SendMessage(touchEvent);
}


