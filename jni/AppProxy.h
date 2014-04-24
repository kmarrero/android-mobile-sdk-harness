/*
 * AppProxy.h
 *
 *  Created on: Apr 22, 2014
 */

#ifndef APPPROXY_H_
#define APPPROXY_H_

#include "AppRunner.h"
#include "InputMessages.h"
#include "GameId.h"
#include "Thread.h"

class AppProxy
{
public:
	AppProxy(AppRunner& appRunner);
	~AppProxy();

	void Pause();
	void Resume();
	void ActivateSurface();

	void StartGame(Game::GameId::Values gameId);

	void SendTouchEvent(const InputMessages::TouchEventMessage* pTouchEvent);

private:
	AppRunner& m_appRunner;
	Thread m_thread;
};

#endif /* APPPROXY_H_ */
