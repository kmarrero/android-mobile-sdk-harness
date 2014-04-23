/*
 * TouchInputMessage.h
 *
 *  Created on: Apr 23, 2014
 */

#ifndef TOUCHEVENTMESSAGE_H_
#define TOUCHEVENTMESSAGE_H_

#include "AppMessages.h"
#include "IAppMessage.h"
#include "TouchEventType.h"
#include "TouchEventWrapper.h"

namespace InputMessages
{
	class TouchEventMessage : public AppMessages::IAppMessage
	{
	public:
		TouchEventMessage(const Eegeo::Android::Input::TouchInputEvent& touchInputEvent);

		bool Handle(AppMessages::IAppMessageDispatcher& dispatcher) const;

		const Eegeo::Android::Input::TouchInputEvent& GetTouchInputEvent() const;

		/*
		static TouchEventMessage* CreatePointerDown
		(
			JNIEnv* jenv,
			jint primaryActionIndex,
			jint primaryActionIdentifier,
			jint numPointers,
			jfloatArray x,
			jfloatArray y,
			jintArray pointerIdentity,
			jintArray pointerIndex
		);

		static TouchEventMessage* CreatePointerUp
		(
			JNIEnv* jenv,
			jint primaryActionIndex,
			jint primaryActionIdentifier,
			jint numPointers,
			jfloatArray x,
			jfloatArray y,
			jintArray pointerIdentity,
			jintArray pointerIndex
		);

		static TouchEventMessage* CreatePointerMove
		(
			JNIEnv* jenv,
			jint primaryActionIndex,
			jint primaryActionIdentifier,
			jint numPointers,
			jfloatArray x,
			jfloatArray y,
			jintArray pointerIdentity,
			jintArray pointerIndex
		);
		*/

	private:
		Eegeo::Android::Input::TouchInputEvent m_touchInputEvent;
	};
}

#endif /* TOUCHEVENTMESSAGE_H_ */
