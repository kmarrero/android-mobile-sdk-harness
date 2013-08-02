/*
 * KeyboardInputExample.h
 *
 *  Created on: Jul 23, 2013
 *      Author: stephenphillips
 */

#ifndef KEYBOARDINPUTEXAMPLE_H_
#define KEYBOARDINPUTEXAMPLE_H_

#include <string>

#include "IExample.h"
#include "KeyboardInput.h"
#include "IKeyboardInputDismissedHandler.h"
#include "IKeyboardInputKeyPressedHandler.h"

namespace Examples
{
    class KeyboardInputExample : public IExample, public Eegeo::UI::NativeInput::IKeyboardInputDismissedHandler, public Eegeo::UI::NativeInput::IKeyboardInputKeyPressedHandler
    {
    private:
        Eegeo::UI::NativeInput::IKeyboardInput* m_pKeyboardInput;
        Eegeo::UI::NativeInput::IKeyboardInputFactory& keyboardInputFactory;

    public:
        KeyboardInputExample(Eegeo::UI::NativeInput::IKeyboardInputFactory& keyboardInputFactory);

        void Start();
        void Update() {}
        void Draw() {}
        void Suspend() {}

        void HandleKeyboardInputDismissed();
        void HandleKeyboardInputKeyPressed(const AppInterface::KeyboardData& data);
    };
}

#endif /* KEYBOARDINPUTEXAMPLE_H_ */
