


#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"

#include "characterkinematic/PxControllerManager.h"
#include "characterkinematic/PxCapsuleController.h"

#include "SampleNorthPole.h"
#include "SampleNorthPoleCameraController.h"
#include "SampleNorthPoleInputEventIds.h"
#include <SamplePlatform.h>
#include <SampleUserInput.h>
#include <SampleUserInputIds.h>
#include <SampleUserInputDefines.h>

using namespace SampleRenderer;
using namespace SampleFramework;

void SampleNorthPole::tryStandup()
{
	// overlap with upper part
	PxReal r = mController->getRadius();
	PxReal dh = mStandingSize-mCrouchingSize-2*r;
	PxCapsuleGeometry geom(r, dh*.5f);

	PxExtendedVec3 position = mController->getPosition();
	PxVec3 pos((float)position.x,(float)position.y+mStandingSize*.5f+r,(float)position.z);
	PxQuat orientation(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f));

	PxShape* hit;
	if(getActiveScene().overlapMultiple(geom, PxTransform(pos,orientation),&hit,1) != 0) return;

	// if no hit, we can stand up
	resizeController(mStandingSize);
	mDoStandup = false;
}

void SampleNorthPole::resizeController(PxReal height)
{
	mController->resize(height);
}

PxCapsuleController* SampleNorthPole::createCharacter(const PxExtendedVec3& position)
{
	PxCapsuleControllerDesc cDesc;
	cDesc.material			= &getDefaultMaterial();
	cDesc.position			= position;
	cDesc.height			= mStandingSize;
	cDesc.radius			= mControllerRadius;
	cDesc.slopeLimit		= 0.0f;
	cDesc.contactOffset		= 0.1f;
	cDesc.stepOffset		= 0.02f;
	cDesc.callback			= this;
	cDesc.behaviorCallback	= this;

	mControllerInitialPosition = cDesc.position;

	PxCapsuleController* ctrl = static_cast<PxCapsuleController*>(mControllerManager->createController(getPhysics(),&getActiveScene(),cDesc));

	// remove controller shape from scene query for standup overlap test
	PxRigidDynamic* actor = ctrl->getActor();
	if(actor)
	{
		if(actor->getNbShapes())
		{
			PxShape* ctrlShape;
			actor->getShapes(&ctrlShape,1);
			ctrlShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE,false);
		}
		else
			fatalError("character actor has no shape");
	}
	else
		fatalError("character could not create actor");

	// uncomment the next line to render the character
	//createRenderObjectsFromActor(ctrl->getActor());

	return ctrl;
}


void SampleNorthPole::onShapeHit(const PxControllerShapeHit& hit)
{
	PxRigidDynamic* actor = hit.shape->getActor().is<PxRigidDynamic>();
	if(actor)
	{
		// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
		// useless stress on the solver. It would be possible to enable/disable vertical pushes on
		// particular objects, if the gameplay requires it.
		if(hit.dir.y==0.0f)
		{
			PxReal coeff = actor->getMass() * hit.length;
			PxRigidBodyExt::addForceAtLocalPos(*actor,hit.dir*coeff, PxVec3(0,0,0), PxForceMode::eIMPULSE);
		}
	}
}

void SampleNorthPole::resetScene()
{	
	mController->setPosition(mControllerInitialPosition);
	mNorthPoleCamera->setView(0,0);

	while(mPhysicsActors.size())
	{
		PxRigidActor* actor = mPhysicsActors.back();
		removeActor(actor);
	}

	createSnowMen();

	for(unsigned int b = 0; b < NUM_BALLS; b++)
	{
		PxRigidDynamic* ball = mSnowBalls[b];
		if(ball)
		{
			removeActor(ball);
			ball->release();
			mSnowBalls[b] = 0;
			
			// render actor is released inside of removeActor()
			mSnowBallsRenderActors[b] = 0;
		}
	}
}

void SampleNorthPole::collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents)
{
	PhysXSample::collectInputEvents(inputEvents);

	getApplication().getPlatform()->getSampleUserInput()->unregisterInputEvent(SPAWN_DEBUG_OBJECT);
	getApplication().getPlatform()->getSampleUserInput()->unregisterInputEvent(CAMERA_MOVE_BUTTON);

	//digital keyboard events
	DIGITAL_INPUT_EVENT_DEF(CROUCH,			SCAN_CODE_DOWN,	XKEY_C,	PS3KEY_C,	AKEY_UNKNOWN,	SCAN_CODE_DOWN,	PSP2KEY_UNKNOWN,	IKEY_UNKNOWN,	SCAN_CODE_DOWN);
	DIGITAL_INPUT_EVENT_DEF(RESET_SCENE,	WKEY_R,			XKEY_R,	PS3KEY_R,	AKEY_UNKNOWN,	OSXKEY_R,		PSP2KEY_UNKNOWN,	IKEY_UNKNOWN,	LINUXKEY_R);

	//digital gamepad events
	DIGITAL_INPUT_EVENT_DEF(CROUCH,			GAMEPAD_WEST,				GAMEPAD_WEST,				GAMEPAD_WEST,				AKEY_UNKNOWN,	GAMEPAD_WEST,				GAMEPAD_WEST,				IKEY_UNKNOWN, LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(RESET_SCENE,	GAMEPAD_NORTH,				GAMEPAD_NORTH,				GAMEPAD_NORTH,				AKEY_UNKNOWN,	GAMEPAD_NORTH,				GAMEPAD_NORTH,				IKEY_UNKNOWN, LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(THROW_BALL,		GAMEPAD_RIGHT_SHOULDER_BOT, GAMEPAD_RIGHT_SHOULDER_BOT, GAMEPAD_RIGHT_SHOULDER_BOT, AKEY_UNKNOWN,	GAMEPAD_RIGHT_SHOULDER_BOT, GAMEPAD_RIGHT_SHOULDER_BOT,	IKEY_UNKNOWN, LINUXKEY_UNKNOWN);

	//digital mouse events
	if (!isPaused())
	{
		DIGITAL_INPUT_EVENT_DEF(THROW_BALL,	MOUSE_BUTTON_LEFT, XKEY_UNKNOWN, PS3KEY_UNKNOWN, AKEY_UNKNOWN, MOUSE_BUTTON_LEFT, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, MOUSE_BUTTON_LEFT);
	}
    
    //touch events
    TOUCH_INPUT_EVENT_DEF(RESET_SCENE,			"Reset",			ABUTTON_5,		IBUTTON_5);
	TOUCH_INPUT_EVENT_DEF(CROUCH,               "Crouch",			ABUTTON_6,		IBUTTON_6);
	TOUCH_INPUT_EVENT_DEF(THROW_BALL,           "Throw Ball",   AQUICK_BUTTON_1,	IQUICK_BUTTON_1);
}

bool SampleNorthPole::onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val)
{
	switch (ie.m_Id)
	{
	case CROUCH:
		{
			if(val)
			{
				resizeController(mCrouchingSize);
			}
			else
			{
				mDoStandup = true;
			}
		}
		break;
 	case RESET_SCENE:
		{
			if(val)
			{
				resetScene();
			}
		}
		break;
	case THROW_BALL:
		{
			if(val)
			{
				throwBall();
			}
		}
		break;
	}

	PhysXSample::onDigitalInputEvent(ie,val);

	return true;
}
