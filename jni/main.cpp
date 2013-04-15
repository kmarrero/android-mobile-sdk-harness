#include <jni.h>
#include <android_native_app_glue.h>
#include "AppWindow.hpp"
#include "AppOnMap.h"
#include "AndroidInputProcessor.h"

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
    MyApp app;
    AndroidInputProcessor inputProcessor(&app);
	AppWindow window(state, &app, &inputProcessor);
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    state->userData = &window;
    window.Run();
}


#include "new"
#include <stdlib.h>

const std::nothrow_t std::nothrow = {};

void* operator new(std::size_t size)
{
    void* p = calloc(1, size);
    if (p == NULL) {
        abort();
    }
    return p;
}

void* operator new[](std::size_t size)
{
    void* p = calloc(1, size);
    if (p == NULL) {
        abort();
    }
    return p;
}

void  operator delete(void* ptr)
{
    free(ptr);
}

void  operator delete[](void* ptr)
{
    free(ptr);
}

void* operator new(std::size_t size, const std::nothrow_t&)
{
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&)
{
    return malloc(size);
}

void  operator delete(void* ptr, const std::nothrow_t&)
{
    free(ptr);
}

void  operator delete[](void* ptr, const std::nothrow_t&)
{
    free(ptr);
}
