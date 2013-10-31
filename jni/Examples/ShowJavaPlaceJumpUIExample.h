/*
 * ShowJavaPlaceJumpUIExample.h
 *
 *  Created on: Oct 30, 2013
 *      Author: kimbleoperations
 */

#ifndef SHOWJAVAPLACEJUMPUIEXAMPLE_H_
#define SHOWJAVAPLACEJUMPUIEXAMPLE_H_

#include "IExample.h"
#include "AndroidNativeState.h"

namespace Examples
{
    class ShowJavaPlaceJumpUIExample : public IExample
    {
    	AndroidNativeState& m_nativeState;

    public:
    	ShowJavaPlaceJumpUIExample(AndroidNativeState& pNativeState);

        void Start();
        void Update() {}
        void Draw() {}
        void Suspend() {}
    };
}


#endif /* SHOWJAVAPLACEJUMPUIEXAMPLE_H_ */
