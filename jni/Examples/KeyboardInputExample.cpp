/*
 * KeyboardInputExample.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: stephenphillips
 */

#include "KeyboardInputExample.h"
#include "IKeyboardInputFactory.h"
#include <android_native_app_glue.h>
#include "Logger.h"

using namespace Examples;
using namespace Eegeo::UI::NativeInput;

KeyboardInputExample::KeyboardInputExample(Eegeo::UI::NativeInput::IKeyboardInputFactory& keyboardInputFactory)
: keyboardInputFactory(keyboardInputFactory)
, dismissed(true)
{
}

void KeyboardInputExample::Start()
{
	EXAMPLE_LOG("Creating keyboard with Input Type: %d and Return Key type: %d", this, &keyboardInputFactory, KeyboardTypeDefault, ReturnKeySearch);
	m_pKeyboardInput = keyboardInputFactory.CreateKeyboardInput(*this, *this, KeyboardTypeDefault, ReturnKeySearch);
	dismissed = false;
}

void KeyboardInputExample::Suspend()
{
	if(!dismissed) {
		m_pKeyboardInput->Dismiss();
	}
}

void KeyboardInputExample::HandleKeyboardInputDismissed()
{
	EXAMPLE_LOG("%s", "\nDismissed the keyboard.");
}

void KeyboardInputExample::Dismiss()
{
	if(!dismissed) {
		m_pKeyboardInput->Dismiss();
	}
}

bool KeyboardInputExample::HandleKeyboardInputKeyPressed(const AppInterface::KeyboardData& data)
{
	if (data.printable)
    {
		EXAMPLE_LOG("%c", data.keyCode);
    }

    //Optionally dismiss the keyboard on pressing Return key
    if (data.keyCode == '\n' || data.keyCode == AKEYCODE_BACK)
    {
    	Dismiss();
    }

    return false;
}
