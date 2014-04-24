/*
 * AppProxy.cpp
 *
 *  Created on: Apr 22, 2014
 */

#include "Types.h"
#include "AppProxy.h"
#include "AppRunner.h"
#include "TouchEventMessage.h"
#include "AppPauseMessage.h"
#include "AppDisplayAvailableMessage.h"
#include "AppStartGameMessage.h"

AppProxy::AppProxy(AppRunner& appRunner)
: m_appRunner(appRunner)
{
}

void AppProxy::Pause()
{
	AppLifecycleMessages::AppPauseMessage* pMessage = Eegeo_NEW(AppLifecycleMessages::AppPauseMessage)();
	m_appRunner.SendMessage(pMessage);
	m_appRunner.WaitForStop();
}

void AppProxy::Resume()
{
	m_appRunner.Start();
}

void AppProxy::ActivateSurface()
{
	AppLifecycleMessages::AppDisplayAvailableMessage* pMessage = Eegeo_NEW(AppLifecycleMessages::AppDisplayAvailableMessage)();
	m_appRunner.SendMessage(pMessage);
}

void AppProxy::StartGame(GameId::Values gameId)
{
	AppMessages::AppStartGameMessage* pMessage = Eegeo_NEW(AppMessages::AppStartGameMessage)(gameId);
	m_appRunner.SendMessage(pMessage);
}

void AppProxy::SendTouchEvent(const InputMessages::TouchEventMessage* pTouchEvent)
{
	m_appRunner.SendMessage(pTouchEvent);
}


