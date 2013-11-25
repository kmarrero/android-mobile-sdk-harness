/*
 * RouteSimulationExample.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: kimbleoperations
 */

#define ENABLE_TTY

#include "Types.h"
#include "RouteSimulationExample.h"
#include "RouteBuilder.h"
#include "VectorMath.h"
#include "RouteStyle.h"
#include "VectorMath.h"

using namespace Examples;
using namespace Eegeo;
using namespace Eegeo::Routes;
using namespace Eegeo::Routes::Simulation;
using namespace Eegeo::Routes::Simulation::View;
using namespace Eegeo::Routes::Simulation::Camera;

RouteSimulationExample::RouteSimulationExample(RouteService& routeService,
                                               RouteSimulationService& routeSimulationService,
                                               RouteSimulationViewService& routeSimulationViewService,
                                               Eegeo::Rendering::GLState& glState,
                                               Eegeo::Helpers::IFileIO& fileIO,
                                               Eegeo::Helpers::ITextureFileLoader& textureLoader,
                                               Eegeo::Camera::GlobeCamera::GlobeCameraController& defaultCamera,
                                               Eegeo::Location::IInterestPointProvider& interestPointProvider,
                                               RouteSimulationGlobeCameraControllerFactory routeSimulationGlobeCameraControllerFactory,
                                               AndroidNativeState& nativeState,
                                               EegeoWorld& world)
:m_routeService(routeService)
,m_routeSimulationService(routeSimulationService)
,m_routeSimulationViewService(routeSimulationViewService)
,m_glState(glState)
,m_fileIO(fileIO)
,m_textureLoader(textureLoader)
,m_defaultCamera(defaultCamera)
,m_interestPointProvider(interestPointProvider)
,m_routeSimulationGlobeCameraControllerFactory(routeSimulationGlobeCameraControllerFactory)
,m_nativeState(nativeState)
,m_world(world)
,m_initialised(false)
,m_route(NULL)
,m_usingFollowCamera(false)
{
}

void RouteSimulationExample::Initialise()
{
	int j = 0;
	for(int i = 0; i < 10000000; ++ i)
	{
		j = (i * i) + i*4;
	}
	__android_log_print(ANDROID_LOG_ERROR,"Eegeo", "%d\n", j);

    //Load a model containing the nodes which will be bound to our route simulation sessions. For
    //a detailed explation see http://sdk.eegeo.com/developers/mobiledocs/loading_rendering_models
    //or see LoadModelExample.cpp.
    Eegeo::Node *pVehicle1, *pVehicle2, *pVehicle3;
    m_pModel = LoadModelVehicleNodes(pVehicle1, pVehicle2, pVehicle3);

    //Build the route - see RouteDrawingExample.cpp for a detailed explanation of building routes, or
    //check out http://sdk.eegeo.com/developers/mobiledocs/routes
    m_route = BuildRoute();

    //Create three simulation sessions for the same route. This first illustrates a session which
    //we will control such that it just loops around in a cycle forever, to illustrate route 'playback'.
    //The first session will just obey the link speed for the route links.
    m_pSessionCycle = m_routeSimulationService.BeginRouteSimulationSession(*m_route);

    //The second session we will control such that it oscillates back and forward to illustrate
    //'rewinding' a route. We will vary the speed dynamically to illustrate 'fast-forward' playback.
    m_pSessionAlternatingSpeedChanger = m_routeSimulationService.BeginRouteSimulationSession(*m_route);

    //The final session will be used to illustrate mapping a point on to the route. A useful application
    //of this might be to map a GPS location on to the route, but for illustrative purposes we map the
    //camera focus point on to the route, so that the effect is clear without relying on sampling the
    //GPS (we should not require you to catch a bus to test this example!).
    m_pSessionCamera = m_routeSimulationService.BeginRouteSimulationSession(*m_route);

    //Start playback on the first two routes from the beginning - we will not start playback on the
    //m_pSessionCamera session as we want to control this session manually by setting the position
    //ourselves (and having the session select the closest route point to this position).
    m_pSessionCycle->StartPlaybackFromBeginning();
    m_pSessionAlternatingSpeedChanger->StartPlaybackFromBeginning();

    //Dynamically double the playback speed for the second route to illustrate fast-forward.
    m_pSessionAlternatingSpeedChanger->UseLinkSpeedValueWithMultiplier(2.f);

    //Create a local transform for the views which will be bound to the route simulation
    //session. The source vehicle models are small so must be scaled up - a scale factor
    //of 15 works well for this case. In the mode we are using, the vehicles are also
    //facing backward, so we can rotate them in local space by 180 degrees so they face
    //forwards.
    Eegeo::m44 scale, rotation, transform;
    scale.Scale(15.f);
    rotation.RotateY(M_PI);
    m44::Mul(transform, scale, rotation);

    //Bind a view to each of the three sessions using a different vehicle node from the model.
    //A pointer is stored to the view bindings, such that we can access them to change the local
    //model transform as the vehicle changes direction (when the route alternates between rewind
    //and regular playback), or to disable and enable the rendering of the views.
    m_pViewBindingForCycleSession = m_routeSimulationViewService.CreateBinding(*m_pSessionCycle, pVehicle1, transform);
    m_pViewBindingForOscillatingSession = m_routeSimulationViewService.CreateBinding(*m_pSessionAlternatingSpeedChanger, pVehicle2, transform);
    m_pViewBindingForCameraSession = m_routeSimulationViewService.CreateBinding(*m_pSessionCamera, pVehicle3, transform);

    m_pRouteSessionFollowCameraController = m_routeSimulationGlobeCameraControllerFactory.Create(false, *m_pSessionAlternatingSpeedChanger);
    m_pRouteSessionFollowCameraController->SetView(37.7858, -122.401, 0, 1781.0f);
    m_pRouteSessionFollowCameraController->StartFollowingSession();

    //Create some UI buttons for controlling the simulation...
    CreateAndBindUI();
}

void RouteSimulationExample::Update(float dt)
{
    //Defer initialisation until the loading state is over.
    if(m_world.Initialising()) {
        return;
    }

    //Just create the routes once.
    if(!m_initialised)
    {
        //Load the model, build the route, create and configure the simulation settings.
        Initialise();

        //We have initialised so don't need to do so again.
        m_initialised = true;
    }

    //If we are using a follow camera bound to a route simulation session, we should update this camera.
    //Otherwise, the default camera should be used.
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->Update(dt);
        m_world.SetCamera(m_pRouteSessionFollowCameraController->GetCamera());
    }
    else
    {
        m_world.SetCamera(m_defaultCamera.GetCamera());
    }

    //The route session for which we want to project a position to (in this case, the ecef interest
    //point) should be updated giving it the latest position.
    Eegeo::dv3 ecefPositionToProjectToRoute = m_interestPointProvider.GetEcefInterestPoint();
    m_pSessionCamera->SetCurrentPositionSnappedToRoute(ecefPositionToProjectToRoute);

    //For the session which should just cycle the route forever, when it has completed simply end
    //the session and restart playback from the beginning.
    if(m_pSessionCycle->IsRouteCompleted())
    {
        m_pSessionCycle->EndPlayback();
        m_pSessionCycle->StartPlaybackFromBeginning();
    }

    //For the session which should oscillate, when the route is finished we change the playback direction
    //and unpause the session (such that it restarts from where it left off), and also demonstrate that
    //we can change playback speed by selecting a random speed multiplier. This multiplier applies to the
    //link speed (such that it is like a 'fast-forward' function). If we wanted to, we could also override
    //the playback speed such that the link speed is ignored by calling the UseCustomSpeedValue method.
    if(m_pSessionAlternatingSpeedChanger->IsRouteCompleted())
    {
        m_pSessionAlternatingSpeedChanger->TogglePlaybackDirection();
        m_pSessionAlternatingSpeedChanger->Unpause();

        float linkSpeedMultiplier = 1.f + ((rand() % 300)/100.f);
        m_pSessionAlternatingSpeedChanger->UseLinkSpeedValueWithMultiplier(linkSpeedMultiplier);

        //Change the direction of the vehicle when we alternate playback directions, so the vehice
        //is always facing forward. If we did not do this, the vehicle would appear to reverse in
        //rewind mode.
        const m44& currentTransform = m_pViewBindingForOscillatingSession->GetModelTransform();
        m44 rotation, newTransform;
        rotation.RotateY(M_PI);
        m44::Mul(newTransform, rotation, currentTransform);
        m_pViewBindingForOscillatingSession->SetModelTransform(newTransform);
    }
}

void RouteSimulationExample::Suspend()
{
    m_routeSimulationViewService.DestroyBinding(m_pViewBindingForCycleSession);
    m_routeSimulationViewService.DestroyBinding(m_pViewBindingForOscillatingSession);
    m_routeSimulationViewService.DestroyBinding(m_pViewBindingForCameraSession);

    m_routeSimulationService.EndRouteSimulationSession(m_pSessionCycle);
    m_routeSimulationService.EndRouteSimulationSession(m_pSessionAlternatingSpeedChanger);
    m_routeSimulationService.EndRouteSimulationSession(m_pSessionCamera);

    m_pSessionCycle = NULL;
    m_pSessionAlternatingSpeedChanger = NULL;
    m_pSessionCamera = NULL;

    m_routeService.DestroyRoute(m_route);
    m_route = NULL;

    delete m_pModel;
    m_pModel = NULL;

    delete m_pRouteSessionFollowCameraController;
    m_pRouteSessionFollowCameraController = NULL;

    m_initialised = false;
}

void RouteSimulationExample::ToggleFollowCamera()
{
    m_usingFollowCamera = !m_usingFollowCamera;

    //Disable the rendering of the non-followed session view bindings if we are following,
    //otherwise enable them.
    m_pViewBindingForCycleSession->SetEnabled(!m_usingFollowCamera);
    m_pViewBindingForCameraSession->SetEnabled(!m_usingFollowCamera);
}

void RouteSimulationExample::ChangeFollowDirection()
{
    Eegeo_ASSERT(m_usingFollowCamera);

    m_pSessionAlternatingSpeedChanger->TogglePlaybackDirection();

    //Change the direction of the vehicle when we alternate playback directions, so the vehice
    //is always facing forward. If we did not do this, the vehicle would appear to reverse in
    //rewind mode.
    const m44& currentTransform = m_pViewBindingForOscillatingSession->GetModelTransform();
    m44 rotation, newTransform;
    rotation.RotateY(M_PI);
    m44::Mul(newTransform, rotation, currentTransform);
    m_pViewBindingForOscillatingSession->SetModelTransform(newTransform);
}

void RouteSimulationExample::IncreaseSpeedFollowed()
{
    Eegeo_ASSERT(m_usingFollowCamera);

    float newSpeed = (m_pSessionAlternatingSpeedChanger->GetPlaybackSpeedMultiplier() + 0.5f);
    m_pSessionAlternatingSpeedChanger->UseLinkSpeedValueWithMultiplier(newSpeed);
}

void RouteSimulationExample::DecreaseSpeedFollowed()
{
    Eegeo_ASSERT(m_usingFollowCamera);

    float newSpeed = (m_pSessionAlternatingSpeedChanger->GetPlaybackSpeedMultiplier() - 0.5f);

    if(newSpeed < 0.5f) { newSpeed = 0.5f; }

    m_pSessionAlternatingSpeedChanger->UseLinkSpeedValueWithMultiplier(newSpeed);
}

Route* RouteSimulationExample::BuildRoute() const
{
    const float halfWidth = 5.f;
    const float routeSpeedMetersPerSecond = 20.f;
    const Eegeo::v4 routeColor(1, 0, 1, 0.6f);
    const float altitudeMeters = 3.f;

    RouteBuilder builder;

    std::vector<RouteVertex> points = builder.Start(routeColor, halfWidth, routeSpeedMetersPerSecond)
    .AddPoint(37.795729,-122.401698, altitudeMeters)
    .AddPoint(37.794873,-122.401516, altitudeMeters)
    .AddPoint(37.794728,-122.403179, altitudeMeters)
    .AddPoint(37.794483,-122.404863, altitudeMeters)
    .AddPoint(37.793618,-122.404670, altitudeMeters)
    .AddPoint(37.793813,-122.403007, altitudeMeters)
    .AddPoint(37.792940,-122.402825, altitudeMeters)
    .AddPoint(37.793109,-122.401108, altitudeMeters)
    .AddPoint(37.792143,-122.400990, altitudeMeters)
    .AddPoint(37.790303,-122.400603, altitudeMeters)
    .AddPoint(37.790324,-122.400126, altitudeMeters)
    .AddPoint(37.794449,-122.394906, altitudeMeters)
    .AddPoint(37.793253,-122.393238, altitudeMeters)
    .AddPoint(37.793707,-122.392578, altitudeMeters)
    .FinishRoute();

    const Eegeo::Routes::RouteStyle style(Eegeo::Routes::RouteStyle::JoinStyleArc);
    return m_routeService.CreateRoute(points, style);
}

Eegeo::Model* RouteSimulationExample::LoadModelVehicleNodes(Eegeo::Node*& pVehicle1,
                                                            Eegeo::Node*& pVehicle2,
                                                            Eegeo::Node*& pVehicle3) const
{
    Eegeo::Model* pModel = new Eegeo::Model(m_glState, m_textureLoader, m_fileIO);

    pModel->Load("SanFrancisco_Vehicles.pod");
    Eegeo::Node* parentNode = pModel->FindNode("Vehicles");

    Eegeo_ASSERT(parentNode);
    Eegeo_ASSERT(parentNode->GetNumChildNodes() >= 3);

    pVehicle1 = parentNode->GetChildNode(0);
    pVehicle2 = parentNode->GetChildNode(1);
    pVehicle3 = parentNode->GetChildNode(2);

    return pModel;
}

//For each of the events our follow camera should intercept, we should handle this event
//and pass it to the follow camera if it is enabled. If the follow camera is enabled, we
//should block this event from being passed up to the default camera by declaring the
//event handled (by returning true from the even handler).
bool RouteSimulationExample::Event_TouchRotate(const AppInterface::RotateData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchRotate(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchRotate_Start(const AppInterface::RotateData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchRotate_Start(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchRotate_End(const AppInterface::RotateData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchRotate_End(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPinch(const AppInterface::PinchData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPinch(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPinch_Start(const AppInterface::PinchData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPinch_Start(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPinch_End(const AppInterface::PinchData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPinch_End(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPan(const AppInterface::PanData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPan(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPan_Start(const AppInterface::PanData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPan_Start(data);
    }

    return m_usingFollowCamera;
}

bool RouteSimulationExample::Event_TouchPan_End(const AppInterface::PanData& data)
{
    if(m_usingFollowCamera)
    {
        m_pRouteSessionFollowCameraController->GetTouchController().Event_TouchPan_End(data);
    }

    return m_usingFollowCamera;
}

//Create some UI to let us toggle aspects of the simulation, such as camera follow, speed, etc.
//The native iOS UI requires an objective-c object to bind to, so we create an objective-c object
//to route our button click events though to call back the example.
void RouteSimulationExample::CreateAndBindUI()
{
	//get an env for the current thread
	//
	//AndroidSafeNativeThreadAttachment will detach the thread if required at the end of the method
	AndroidSafeNativeThreadAttachment attached(m_nativeState);
	JNIEnv* env = attached.envForThread;

	//get the java HudPinController class
	jstring strClassName = env->NewStringUTF("com/eegeo/examples/RouteSimulationExampleHud");
	jclass routeSimulationExampleHudClass = m_nativeState.LoadClass(env, strClassName);
	env->DeleteLocalRef(strClassName);

	//create a persistent reference to the class
	m_routeSimulationExampleHudClass = static_cast<jclass>(env->NewGlobalRef(routeSimulationExampleHudClass));

	//get the constructor for the HudPinController, which takes the activity, a pointer to 'this' as
	//a parameter, and a flag to indicate if currently in follow mode.
	jmethodID routeSimulationExampleHudConstructor = env->GetMethodID(routeSimulationExampleHudClass, "<init>", "(Lcom/eegeo/MainActivity;JZ)V");

	//construct an instance of the HudPinController, and create and cache a persistent reference to it.
	//we will make calls on to this instance, and it will add elements to the UI for us form Java.
	jlong pThis = (jlong)(this);

	jobject instance = env->NewObject(
    		m_routeSimulationExampleHudClass,
    		routeSimulationExampleHudConstructor,
    		m_nativeState.activity,
    		pThis,
    		m_usingFollowCamera);

    m_routeSimulationExampleHud = env->NewGlobalRef(instance);
}

JNIEXPORT void JNICALL Java_com_eegeo_examples_RouteSimulationExampleHud_ToggleFollowCamera(
		JNIEnv* jenv, jobject obj,
		jlong nativeObjectPtr)
{
	Examples::RouteSimulationExample* example = (Examples::RouteSimulationExample*)(nativeObjectPtr);
	example->ToggleFollowCamera();
}

JNIEXPORT void JNICALL Java_com_eegeo_examples_RouteSimulationExampleHud_ChangeFollowDirection(
		JNIEnv* jenv, jobject obj,
		jlong nativeObjectPtr)
{
	Examples::RouteSimulationExample* example = (Examples::RouteSimulationExample*)(nativeObjectPtr);
	example->ChangeFollowDirection();
}

JNIEXPORT void JNICALL Java_com_eegeo_examples_RouteSimulationExampleHud_IncreaseSpeedFollowed(
		JNIEnv* jenv, jobject obj,
		jlong nativeObjectPtr)
{
	Examples::RouteSimulationExample* example = (Examples::RouteSimulationExample*)(nativeObjectPtr);
	example->IncreaseSpeedFollowed();
}

JNIEXPORT void JNICALL Java_com_eegeo_examples_RouteSimulationExampleHud_DecreaseSpeedFollowed(
		JNIEnv* jenv, jobject obj,
		jlong nativeObjectPtr)
{
	Examples::RouteSimulationExample* example = (Examples::RouteSimulationExample*)(nativeObjectPtr);
	example->DecreaseSpeedFollowed();
}
