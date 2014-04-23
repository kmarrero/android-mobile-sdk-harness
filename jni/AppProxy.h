/*
 * AppProxy.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef APPPROXY_H_
#define APPPROXY_H_

#include "AppRunner.h"
#include "AppPauseMessage.h"
#include "AppDisplayAvailableMessage.h"
#include "InputMessages.h"

class AppProxy
{
public:
	AppProxy(AppRunner& appRunner);
	~AppProxy();

	void Pause(PersistentAppState* pPersistentState);
	void Resume();

	// Should be try acquire display.
	void ActivateSurface();

	void SendTouchEvent(const InputMessages::TouchEventMessage& touchEvent);

private:
	AppRunner& m_appRunner;

	const AppLifecycleMessages::AppPauseMessage m_pauseMessage;
	const AppLifecycleMessages::AppDisplayAvailableMessage m_displayAvailableMessage;
};

#endif /* APPPROXY_H_ */
