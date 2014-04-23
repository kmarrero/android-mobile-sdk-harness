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

AppRunner::AppRunner(const std::string& apiKey, AndroidNativeState* pNativeState, PersistentAppState* pPersistentState)
: m_apiKey(apiKey)
, m_pNativeState(pNativeState)
, m_persistentState(pPersistentState)
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

void AppRunner::SendMessage(const AppMessages::IAppMessage* pMessage)
{
	m_messageQueue.Enqueue(pMessage);
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
		m_messageQueue.Enqueue(&message);
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
			m_persistentState,
			m_displayService.GetDisplayWidth(),
			m_displayService.GetDisplayHeight(),
			m_displayService.GetDisplay(),
			m_displayService.GetSharedSurface(),
			m_displayService.GetResourceBuildSharedContext()
		);
	}
}

bool AppRunner::operator()()
{
	const AppMessages::IAppMessage* pMessage = NULL;

	while(m_messageQueue.TryDequeue(pMessage))
	{
		Eegeo_ASSERT(pMessage);

		bool continueRunning = pMessage->Handle(*this);
		Eegeo_DELETE(pMessage);

		if(!continueRunning)
		{
			return false;
		}
	}

	Update();

	return true;
}

void AppRunner::Update()
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







