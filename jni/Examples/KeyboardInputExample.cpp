/*
 * KeyboardInputExample.cpp
 *
 *  Created on: Jul 23, 2013
 *      Author: stephenphillips
 */

#include "KeyboardInputExample.h"
#include "IKeyboardInputFactory.h"

using namespace Examples;
using namespace Eegeo::UI::NativeInput;

KeyboardInputExample::KeyboardInputExample(Eegeo::UI::NativeInput::IKeyboardInputFactory& keyboardInputFactory) :
keyboardInputFactory(keyboardInputFactory)
{
}

void KeyboardInputExample::Start()
{
	Eegeo_TTY("Creating keyboard with Input Type: %d and Return Key type: %d", this, &keyboardInputFactory, KeyboardTypeDefault, ReturnKeySearch);
	m_pKeyboardInput = keyboardInputFactory.CreateKeyboardInput(*this, *this, KeyboardTypeDefault, ReturnKeySearch);
}

void KeyboardInputExample::HandleKeyboardInputDismissed()
{
    Eegeo_TTY("%s", "\nDismissed the keyboard.");
}

void KeyboardInputExample::HandleKeyboardInputKeyPressed(const AppInterface::KeyboardData& data)
{
	if (data.printable)
    {
		Eegeo_TTY("%c", data.keyCode);
    }

    //Optionally dismiss the keyboard on pressing Return key
    if (data.keyCode == '\n')
    {
        m_pKeyboardInput->Dismiss();
    }
}
