/*
 * AppRunner.h
 *
 *  Created on: Apr 21, 2014
 */

#ifndef APPRUNNER_H_
#define APPRUNNER_H_

#include "Thread.h"
#include "Graphics.h"
#include "AppHost.h"
#include "GlDisplayService.h"
#include "IMessageQueue.h"
#include "MessageHandlerChain.h"
#include "AppMessageQueue.h"
#include <algorithm>

class AppRunner : public Thread, private AppMessages::IAppMessageDispatcher
{
public:
	AppRunner(const std::string& apiKey, AndroidNativeState* pNativeState, PersistentAppState* pPersistentState);
	~AppRunner();

	void SendMessage(const AppMessages::IAppMessage* pMessage);

protected:
	bool operator()();
	void OnStarted();
	void OnStopped();

private:
	const std::string& m_apiKey;
	AndroidNativeState* m_pNativeState;
	PersistentAppState m_persistentState;

    AppMessageQueue m_messageQueue;
	bool HandleMessage(const AppLifecycleMessages::AppPauseMessage& message);
	bool HandleMessage(const AppLifecycleMessages::AppDisplayAvailableMessage& message);
	bool HandleMessage(const InputMessages::TouchEventMessage& message);

    GlDisplayService m_displayService;
	void ReleaseDisplay();
	bool TryBindDisplay();

    AppHost* m_pAppHost;
	void CreateAppHost();

	void Update();
};

#endif /* APPRUNNER_H_ */
