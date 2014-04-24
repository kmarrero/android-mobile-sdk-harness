/*
 * AppRunner.cpp
 *
 *  Created on: Apr 21, 2014
 */

#include "AppRunner.h"
#include "Graphics.h"
#include "AndroidThreadHelper.h"
#include "AppDisplayAvailableMessage.h"
#include "TouchEventMessage.h"
#include "AppStartGameMessage.h"

AppRunner::AppRunner(const std::string& apiKey, AndroidNativeState* pNativeState)
: m_apiKey(apiKey)
, m_pNativeState(pNativeState)
, m_pAppHost(NULL)
{
}

AppRunner::~AppRunner()
{
	bool destroyEGL = true;
	m_displayService.ReleaseDisplay(destroyEGL);

	if(m_pAppHost != NULL)
	{
		Eegeo_DELETE(m_pAppHost);
	}
}

void AppRunner::OnStarted()
{
	Eegeo::Helpers::ThreadHelpers::SetThisThreadAsMainThread();

	if(m_pAppHost != NULL)
	{
		m_pAppHost->OnStarted();
	}
}

void AppRunner::OnStopped()
{
	if(m_pAppHost != NULL)
	{
		m_pAppHost->OnStopped();
	}
}

bool AppRunner::HandleMessage(const AppLifecycleMessages::AppPauseMessage& message)
{
	ReleaseDisplay();

	const bool continueRunning = false;
	return continueRunning;
}

bool AppRunner::HandleMessage(const AppLifecycleMessages::AppDisplayAvailableMessage& message)
{
	ReleaseDisplay();

	if(!TryBindDisplay())
	{
		SendMessage(&message);
	}

	CreateAppHost();

	const bool continueRunning = true;
	return continueRunning;
}

bool AppRunner::HandleMessage(const InputMessages::TouchEventMessage& message)
{
	if(m_pAppHost != NULL)
	{
		m_pAppHost->ProcessTouchInputEvent(message.GetTouchInputEvent());
	}

	const bool continueRunning = true;
	return continueRunning;
}

bool AppRunner::HandleMessage(const AppMessages::AppStartGameMessage& message)
{
	Eegeo_TTY("Change game %d\n", message.GetGameId());

	m_pAppHost->StartGame(message.GetGameId());

	const bool continueRunning = true;
	return continueRunning;
}

void AppRunner::ReleaseDisplay()
{
	if(m_displayService.IsDisplayAvailable())
	{
		const bool teardownEGL = false;
		m_displayService.ReleaseDisplay(teardownEGL);
	}
}

bool AppRunner::TryBindDisplay()
{
	if(m_displayService.TryBindDisplay(*(m_pNativeState->window)))
	{
		if(m_pAppHost != NULL)
		{
			m_pAppHost->SetSharedSurface(m_displayService.GetSharedSurface());
			m_pAppHost->SetViewportOffset(0, 0);
		}

		return true;
	}

	return false;
}

void AppRunner::CreateAppHost()
{
	if(m_pAppHost == NULL && m_displayService.IsDisplayAvailable())
	{
		m_pAppHost = Eegeo_NEW(AppHost)
		(
			m_apiKey,
			*m_pNativeState,
			m_displayService.GetDisplayWidth(),
			m_displayService.GetDisplayHeight(),
			m_displayService.GetDisplay(),
			m_displayService.GetSharedSurface(),
			m_displayService.GetResourceBuildSharedContext()
		);
	}
}

void AppRunner::OnUpdate()
{
	if(m_pAppHost != NULL && m_displayService.IsDisplayAvailable())
	{
	    Eegeo_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		float dt = 1.0f/30.0f;
		m_pAppHost->Update(dt);

		Eegeo_GL(glFinish());
		Eegeo_GL(eglSwapBuffers(m_displayService.GetDisplay(), m_displayService.GetSurface()));
	}
}

void AppRunner::OnMessageHandled(const AppMessages::IAppMessage* message)
{
	Eegeo_DELETE(message);
}





