#include <jni.h>
#include <android_native_app_glue.h>
#include "AppWindow.hpp"

static void process_input( struct android_app* state, struct android_poll_source* source)
{
    AInputEvent* event = NULL;
    if (AInputQueue_getEvent(state->inputQueue, &event) >= 0) {
        int type = AInputEvent_getType(event);
        bool skip_predispatch
              =  AInputEvent_getType(event)  == AINPUT_EVENT_TYPE_KEY
              && AKeyEvent_getKeyCode(event) == AKEYCODE_BACK;

        if (!skip_predispatch && AInputQueue_preDispatchEvent(state->inputQueue, event)) {
            return;
        }

        int32_t handled = 0;
        if (state) handled = state->onInputEvent(state, event);
        AInputQueue_finishEvent(state->inputQueue, event, handled);
    }
}

static int32_t engine_handle_input(struct android_app* state, AInputEvent* event)
{
	AppWindow* app = (AppWindow*)state->userData;
	return app->HandleInput(event);
}

static void engine_handle_cmd(struct android_app* state, int32_t cmd)
{
	AppWindow* app = (AppWindow*)state->userData;
	return app->HandleCommand(cmd);
}

void android_main(struct android_app* state)
{
    app_dummy();
	AppWindow window(state);
    state->inputPollSource.process = process_input;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    state->userData = &window;
    window.Run();
}
