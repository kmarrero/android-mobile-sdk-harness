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
	//Eegeo_TTY("Example Address: %d, Factory Address: %d, Keyboard Type: %d, Return Key Type: %d", this, &keyboardInputFactory, KeyboardTypeDefault, ReturnKeySearch);
    //m_pKeyboardInput = keyboardInputFactory.CreateKeyboardInput(*this, *this, KeyboardTypeDefault, ReturnKeySearch);
}

void KeyboardInputExample::HandleKeyboardInputDismissed(const std::string& input)
{
    Eegeo_TTY("%s", "\nDismissed the keyboard.");
}

void KeyboardInputExample::HandleKeyboardInputKeyPressed(const std::string& input)
{
    Eegeo_TTY("%s", input.c_str());

    //Optionally dismiss the keyboard on pressing Return key
    if (input == "\n")
    {
        m_pKeyboardInput->Dismiss();
    }
}
