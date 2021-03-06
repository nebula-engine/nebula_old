// This code contains NVIDIA Confidential Information and is disclosed to you 
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and 
// any modifications thereto. Any use, reproduction, disclosure, or 
// distribution of this software and related documentation without an express 
// license agreement from NVIDIA Corporation is strictly prohibited.
// 
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2012 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#include "PhysXSampleApplication.h"

#include "PxTkNamespaceMangle.h"
#include "PxTkStream.h"

#include "RendererMemoryMacros.h"
#include "SampleUtils.h"
#include "SampleStepper.h"
#include "SampleActor.h"
#include "SampleCommandLine.h"
#include "SampleConsole.h"
#include "SampleAllocator.h"
#include "SampleAllocatorSDKClasses.h"

#include "SamplePlatform.h"

#include "Renderer.h"
#include "RendererDirectionalLightDesc.h"

#include "RenderBoxActor.h"
#include "RenderSphereActor.h"
#include "RenderCapsuleActor.h"
#include "RenderMeshActor.h"
#include "RenderGridActor.h"
#include "RenderMaterial.h"
#include "RenderTexture.h"
#include "RenderPhysX3Debug.h"

#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "PsFile.h"
#include "PsShare.h"
#include "pxtask/PxTask.h"
#include "PxToolkit.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "PxFiltering.h"

#include "SampleBaseInputEventIds.h"

#include <algorithm>
#include "PxTkBmpLoader.h"
#include <ctype.h>

#include "PhysXSample.h"
#include "TestGroup.h"

#include <SampleUserInputIds.h>
#include "SampleUserInputDefines.h"

#ifdef PX_PS3
#include "extensions/ps3/PxPS3Extension.h"
#endif

using namespace physx;
using namespace SampleRenderer;
using namespace SampleFramework;
using namespace PxToolkit;

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
//#define SCREEN_WIDTH	1024
//#define SCREEN_HEIGHT	768
#define	CAMERA_FOV		70.0f
#define	CAMERA_NEAR		0.1f
#define	CAMERA_FAR		10000.0f


#ifdef SN_TARGET_PSP2
#include <stdlib.h>
// User main thread parameters
extern const char			sceUserMainThreadName[]		= "physx_sample_main_thr";

// Libc parameters
unsigned int	sceLibcHeapSize	= 100*1024*1024;
#endif

#define SAMPLE_FRAMEWORK_MEDIA_DIR	"media"

static char gSampleMediaFolder[512];
///////////////////////////////////////////////////////////////////////////////
static PhysXSampleApplication* gBase = NULL;
///////////////////////////////////////////////////////////////////////////////
			PhysXSampleApplication::PhysXSampleApplication(const SampleCommandLine& cmdline) :
	SampleApplication					(cmdline, SAMPLE_FRAMEWORK_MEDIA_DIR),
	mTextAlphaHelp						(0.0f),
	mTextAlphaDesc						(0.0f),
	mMenuType							(MenuType::TESTS),
	mMenuVisualizationsIndexSelected	(0),
	mIsCloseRequested					(false),
	mDebugRenderer						(NULL),
	mPause								(false),
	mOneFrameUpdate						(false),
	mShowHelp							(false),
	mShowDescription					(false),
	mShowExtendedHelp					(false),
	mHideMouseCursor					(false),
	mDrawScreenQuad						(true),
	mMenuExpand							(false),
	mRunning							(NULL),
	mSelected							(NULL),
	mSample								(NULL),
	mDefaultSamplePath					(NULL)
{
	mConsole = SAMPLE_NEW(Console)(getPlatform());
	if(mConsole)
	{
		gBase = this;
		mConsole->addCmd("nearplane", gNearPlane);
		mConsole->addCmd("farplane", gFarPlane);
		mConsole->addCmd("drawcameradebug", gDrawCameraDebug);
		mConsole->addCmd("freezefrustum", gFreezeFrustum);
		mConsole->addCmd("vfc", gVFC);
	}

	mScreenQuadTopColor		= RendererColor(0x00, 0x00, 0x80);
	mScreenQuadBottomColor	= RendererColor(0xff, 0xf0, 0xf0);

	// Projection
	mCamera.setFOV(CAMERA_FOV);
	mCamera.setNearPlane(CAMERA_NEAR);
	mCamera.setFarPlane(CAMERA_FAR);
	mCamera.setScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);

	// View
	mCamera.setPos(PxVec3(0.0f, 10.0f, 0.0f));
	mCameraController.init(PxVec3(0.0f, 10.0f, 0.0f), PxVec3(0.0f, 0.0f, 0.0f));
	setDefaultCameraController();

	// PT: search path for samples' media files
	strcpy(gSampleMediaFolder, getAssetPathPrefix());
	strcat(gSampleMediaFolder, SAMPLE_MEDIA_DIR);
	addSearchPath(gSampleMediaFolder);

	for (PxU32 i = 0; i < MATERIAL_COUNT; ++i)
	{
		mManagedMaterials[i] = NULL;
	}

    mSelected = getSampleTreeRoot().getFirstTest();
}
			PhysXSampleApplication::~PhysXSampleApplication()
{
	DELETESINGLE(mConsole);

	PX_ASSERT(!mLights.size());
}
const char* PhysXSampleApplication::inputInfoMsg(const char* firstPart,const char* secondPart, physx::PxI32 inputEventId1, physx::PxI32 inputEventId2)
{
	const char* keyNames1[5];

	PxU32 inputMask = 0;
	if(m_platform->getSampleUserInput()->gamepadSupported() && m_platform->getSampleUserInput()->keyboardSupported())
	{
		inputMask = KEYBOARD_INPUT | GAMEPAD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
	}
	else
	{
		if(m_platform->getSampleUserInput()->gamepadSupported())
		{
			inputMask = GAMEPAD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
		}

		if(m_platform->getSampleUserInput()->keyboardSupported())
		{
			inputMask = KEYBOARD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
		}
	}

	PxU16 numNames1 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId1,keyNames1,5,inputMask);

	const char* keyNames2[5];
	PxU16 numNames2 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId2,keyNames2,5,inputMask);

	if(!numNames1 && !numNames2)
		return NULL;

	strcpy(m_Msg, firstPart);
	if(numNames1 && numNames2)
	{
		for (PxU16 i = 0; i < numNames1; i++)
		{
			strcat(m_Msg, keyNames1[i]);
			strcat(m_Msg, " or ");
		}		

		for (PxU16 i = 0; i < numNames2; i++)
		{
			strcat(m_Msg, keyNames2[i]);
			if(i != numNames2 - 1)
				strcat(m_Msg, " or ");
		}				
	}
	else
	{
		if(numNames1)
		{
			for (PxU16 i = 0; i < numNames1; i++)
			{
				strcat(m_Msg, keyNames1[i]);
				if(i != numNames1 - 1)
					strcat(m_Msg, " or ");
			}		
		}
		else
		{
			for (PxU16 i = 0; i < numNames2; i++)
			{
				strcat(m_Msg, keyNames2[i]);
				if(i != numNames2 - 1)
					strcat(m_Msg, " or ");
			}		
		}
	}

	strcat(m_Msg, secondPart);
	return m_Msg;
}
const char* PhysXSampleApplication::inputInfoMsg_Aor_BandC(const char* firstPart,const char* secondPart, physx::PxI32 inputEventIdA, physx::PxI32 inputEventIdB, physx::PxI32 inputEventIdC)
{
	PxU32 inputMask = TOUCH_PAD_INPUT | MOUSE_INPUT;
	if(m_platform->getSampleUserInput()->gamepadSupported())
		inputMask |= GAMEPAD_INPUT;
	if(m_platform->getSampleUserInput()->keyboardSupported())
		inputMask |= KEYBOARD_INPUT;

	const char* keyNamesA[5];
	PxU16 numNamesA = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventIdA,keyNamesA,5,inputMask);

	const char* keyNamesB[5];
	PxU16 numNamesB = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventIdB,keyNamesB,5,inputMask);

	const char* keyNamesC[5];
	PxU16 numNamesC = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventIdC,keyNamesC,5,inputMask);

	PX_ASSERT(numNamesB!=0 == numNamesC!=0);

	strcpy(m_Msg, firstPart);
	{

		for (PxU16 i = 0; i < numNamesA; i++)
		{
			strcat(m_Msg, keyNamesA[i]);
			if(i != numNamesA - 1)
				strcat(m_Msg, " or ");
		}	

		if(numNamesB && numNamesC)
		{
			if(numNamesA)
				strcat(m_Msg, " or (");

			for (PxU16 i = 0; i < numNamesB; i++)
			{
				strcat(m_Msg, keyNamesB[i]);
				if(i != numNamesB - 1)
					strcat(m_Msg, " or ");
			}		

			strcat(m_Msg, " and ");

			for (PxU16 i = 0; i < numNamesC; i++)
			{
				strcat(m_Msg, keyNamesC[i]);
				if(i != numNamesC - 1)
					strcat(m_Msg, " or ");
			}		
			if(numNamesA)
				strcat(m_Msg, ")");
		}		
	}

	strcat(m_Msg, secondPart);
	return m_Msg;
}
const char* PhysXSampleApplication::inputMoveInfoMsg(const char* firstPart,const char* secondPart, physx::PxI32 inputEventId1, physx::PxI32 inputEventId2,physx::PxI32 inputEventId3,physx::PxI32 inputEventId4)
{
	const char* keyNames1[5];
	const char* keyNames2[5];
	const char* keyNames3[5];
	const char* keyNames4[5];

	PxU32 inputMask = 0;
	if(m_platform->getSampleUserInput()->gamepadSupported() && m_platform->getSampleUserInput()->keyboardSupported())
	{
		inputMask = KEYBOARD_INPUT | GAMEPAD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
	}
	else
	{
		if(m_platform->getSampleUserInput()->gamepadSupported())
		{
			inputMask = GAMEPAD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
		}

		if(m_platform->getSampleUserInput()->keyboardSupported())
		{
			inputMask = KEYBOARD_INPUT | TOUCH_PAD_INPUT | MOUSE_INPUT;
		}
	}

	PxU16 numNames1 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId1,keyNames1,5,inputMask);
	PxU16 numNames2 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId2,keyNames2,5,inputMask);
	PxU16 numNames3 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId3,keyNames3,5,inputMask);
	PxU16 numNames4 = getPlatform()->getSampleUserInput()->getUserInputKeys(inputEventId4,keyNames4,5,inputMask);


	if(!numNames1 && !numNames2 && !numNames3 && !numNames4)
		return NULL;

	bool firstVal = true;

	strcpy(m_Msg, firstPart);
	if(numNames1)
	{
		for (PxU16 i = 0 ; i < numNames1; i++)
		{
			if(!firstVal)
			{
				strcat(m_Msg, ",");				
			}

			firstVal = false;
			strcat(m_Msg, keyNames1[i]);
		}		
	}
	if(numNames2)
	{
		for (PxU16 i = 0 ; i < numNames2; i++)
		{
			if(!firstVal)
			{
				strcat(m_Msg, ",");				
			}

			firstVal = false;
			strcat(m_Msg, keyNames2[i]);
		}		
	}
	if(numNames3)
	{
		for (PxU16 i = 0 ; i < numNames3; i++)
		{
			if(!firstVal)
			{
				strcat(m_Msg, ",");				
			}

			firstVal = false;
			strcat(m_Msg, keyNames3[i]);
		}		
	}
	if(numNames4)
	{
		for (PxU16 i = 0 ; i < numNames4; i++)
		{
			if(!firstVal)
			{			
				strcat(m_Msg, ",");				
			}

			firstVal = false;
			strcat(m_Msg, keyNames4[i]);
		}		
	}

	strcat(m_Msg, secondPart);
	return m_Msg;
}
void		PhysXSampleApplication::collectInputEvents(std::vector<const InputEvent*>& inputEvents)
{	
	//digital keyboard events
	DIGITAL_INPUT_EVENT_DEF(MENU_SAMPLES,			WKEY_RETURN,	XKEY_RETURN,	PS3KEY_RETURN,	AKEY_UNKNOWN,	OSXKEY_RETURN,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_RETURN);
	DIGITAL_INPUT_EVENT_DEF(MENU_QUICK_UP,			WKEY_UP,		XKEY_UP,		PS3KEY_UP,		AKEY_UNKNOWN,	OSXKEY_UP,		PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_UP);
	DIGITAL_INPUT_EVENT_DEF(MENU_QUICK_DOWN,		WKEY_DOWN,		XKEY_DOWN,		PS3KEY_DOWN,	AKEY_UNKNOWN,	OSXKEY_DOWN,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_DOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_QUICK_LEFT,		WKEY_LEFT,		XKEY_LEFT,		PS3KEY_LEFT,	AKEY_UNKNOWN,	OSXKEY_LEFT,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_LEFT);
	DIGITAL_INPUT_EVENT_DEF(MENU_QUICK_RIGHT,		WKEY_RIGHT,		XKEY_RIGHT,		PS3KEY_RIGHT,	AKEY_UNKNOWN,	OSXKEY_RIGHT,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_RIGHT);
	DIGITAL_INPUT_EVENT_DEF(MENU_SELECT,			WKEY_RETURN,	XKEY_RETURN,	PS3KEY_RETURN,	AKEY_UNKNOWN,	OSXKEY_RETURN,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_RETURN);
	DIGITAL_INPUT_EVENT_DEF(MENU_ESCAPE,			WKEY_ESCAPE,	XKEY_ESCAPE,	PS3KEY_ESCAPE,	AKEY_UNKNOWN,	OSXKEY_ESCAPE,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_ESCAPE);
	DIGITAL_INPUT_EVENT_DEF(QUIT,					WKEY_ESCAPE,	XKEY_ESCAPE,	PS3KEY_ESCAPE,	AKEY_UNKNOWN,	OSXKEY_ESCAPE,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_ESCAPE);
	DIGITAL_INPUT_EVENT_DEF(SHOW_EXTENDED_HELP,		WKEY_F1,		XKEY_F1,		PS3KEY_F1,		AKEY_UNKNOWN,	OSXKEY_F1,		PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_F1);
#if PX_SUPPORT_GPU_PHYSX
	DIGITAL_INPUT_EVENT_DEF(TOGGLE_CPU_GPU,			WKEY_F2,		XKEY_UNKNOWN,	PS3KEY_UNKNOWN,	AKEY_UNKNOWN,	OSXKEY_UNKNOWN,	PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
#endif
	DIGITAL_INPUT_EVENT_DEF(MENU_VISUALIZATIONS,	WKEY_F3,		XKEY_F3,		PS3KEY_F3,		AKEY_UNKNOWN,	OSXKEY_F3,		PSP2KEY_UNKNOWN, IKEY_UNKNOWN,	LINUXKEY_F3);
	
	//digital mouse events
	DIGITAL_INPUT_EVENT_DEF(CAMERA_MOVE_BUTTON, MOUSE_BUTTON_LEFT, XKEY_UNKNOWN, PS3KEY_UNKNOWN, AKEY_UNKNOWN, MOUSE_BUTTON_LEFT, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, MOUSE_BUTTON_LEFT);

	//digital gamepad events
	DIGITAL_INPUT_EVENT_DEF(MENU_SAMPLES,	GAMEPAD_START,		GAMEPAD_START,		GAMEPAD_START,		AKEY_UNKNOWN,	GAMEPAD_START,		GAMEPAD_START,		IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_UP,		GAMEPAD_DIGI_UP,	GAMEPAD_DIGI_UP,	GAMEPAD_DIGI_UP,	AKEY_UNKNOWN,	GAMEPAD_DIGI_UP,	GAMEPAD_DIGI_UP,	IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_DOWN,		GAMEPAD_DIGI_DOWN,	GAMEPAD_DIGI_DOWN,	GAMEPAD_DIGI_DOWN,	AKEY_UNKNOWN,	GAMEPAD_DIGI_DOWN,	GAMEPAD_DIGI_DOWN,	IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_LEFT,		GAMEPAD_DIGI_LEFT,	GAMEPAD_DIGI_LEFT,	GAMEPAD_DIGI_LEFT,	AKEY_UNKNOWN,	GAMEPAD_DIGI_LEFT,	GAMEPAD_DIGI_LEFT,	IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);	
	DIGITAL_INPUT_EVENT_DEF(MENU_RIGHT,		GAMEPAD_DIGI_RIGHT,	GAMEPAD_DIGI_RIGHT,	GAMEPAD_DIGI_RIGHT,	AKEY_UNKNOWN,	GAMEPAD_DIGI_RIGHT,	GAMEPAD_DIGI_RIGHT,	IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_SELECT,	GAMEPAD_SOUTH,		GAMEPAD_SOUTH,		GAMEPAD_SOUTH,		AKEY_UNKNOWN,	GAMEPAD_SOUTH,		GAMEPAD_SOUTH,		IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);
	DIGITAL_INPUT_EVENT_DEF(MENU_ESCAPE,	GAMEPAD_EAST,		GAMEPAD_EAST,		GAMEPAD_EAST,		AKEY_UNKNOWN,	GAMEPAD_EAST,		GAMEPAD_EAST,		IKEY_UNKNOWN,	LINUXKEY_UNKNOWN);

	//analog mouse events
	ANALOG_INPUT_EVENT_DEF(CAMERA_MOUSE_LOOK,GAMEPAD_DEFAULT_SENSITIVITY, MOUSE_MOVE, XKEY_UNKNOWN, PS3KEY_UNKNOWN, AKEY_UNKNOWN, MOUSE_MOVE, PSP2KEY_UNKNOWN, IKEY_UNKNOWN, MOUSE_MOVE);

	// touchscreen events for sampleall
	if(mRunning->getParent()->numChildren() > 1)
	{
		TOUCH_INPUT_EVENT_DEF(RUN_NEXT_SAMPLE,		">>",  ASELECTOR_BUTTON2,	ISELECTOR_BUTTON2);
		TOUCH_INPUT_EVENT_DEF(RUN_PREVIOUS_SAMPLE,	"<<",  ASELECTOR_BUTTON1,	ISELECTOR_BUTTON1);
	}

	if(mCurrentCameraController)
	{
		mCurrentCameraController->collectInputEvents(inputEvents);
	}

	if(mConsole)
	{
		mConsole->collectInputEvents(inputEvents);
	}

	if(mSample)
	{
		mSample->collectInputEvents(inputEvents);
	}
}
void		PhysXSampleApplication::refreshVisualizationMenuState(PxVisualizationParameter::Enum p)
{
	PxScene& scene = mSample->getActiveScene();
	
	for(PxU32 i=0; i < mMenuVisualizations.size(); i++)
	{
		if(mMenuVisualizations[i].toggleCommand == p)
		{
			mMenuVisualizations[i].toggleState = !!scene.getVisualizationParameter(p);
			break;
		}
	}
}
void		PhysXSampleApplication::applyDefaultVisualizationSettings()
{
	PxScene& scene = mSample->getActiveScene();
	PxReal debugRenderScale = mSample->getDebugRenderScale();

	for(PxU32 i=0; i < mMenuVisualizations.size(); i++)
	{
		bool enabled = mMenuVisualizations[i].toggleState;
		
		PxVisualizationParameter::Enum p = static_cast<PxVisualizationParameter::Enum>(mMenuVisualizations[i].toggleCommand);
		
		if (p != PxVisualizationParameter::eSCALE)
			scene.setVisualizationParameter(p, enabled ? 1.0f : 0.0f);
		else
			scene.setVisualizationParameter(p, enabled ? debugRenderScale: 0.0f); 		
	}
}
///////////////////////////////////////////////////////////////////////////////
void		PhysXSampleApplication::customizeSample(SampleSetup& setup)
{
	setup.mName			= "PhysXSampleApplication";
	setup.mWidth		= SCREEN_WIDTH;
	setup.mHeight		= SCREEN_HEIGHT;
	setup.mFullscreen	= false;
}
///////////////////////////////////////////////////////////////////////////////
bool		PhysXSampleApplication::initLogo()
{
/*	RAWTexture data;
	data.mName = "physx_logo.bmp";
	RenderTexture* logoTexture = createRenderTextureFromRawTexture(data);

	mRockMaterial = SAMPLE_NEW(RenderMaterial)(*getRenderer(), PxVec3(1.0f, 1.0f, 1.0f), 1.0f, false, 0xffffffff, rockTexture);
	mRenderMaterials.push_back(mRockMaterial);
*/
	return true;
}
float		PhysXSampleApplication::tweakElapsedTime(float dtime)
{
	if(dtime>1.0f)
		dtime = 1.0f/60.0f;

	if(mOneFrameUpdate)
	{
		mPause = false;
		dtime = 1.0f/60.0f;
	}

/*	if(mOneFrameUpdate)
		mPause = false;

	if(mPause)
		return 0.0f;*/

	return dtime;
}
void		PhysXSampleApplication::baseResize(PxU32 width, PxU32 height)
{
//	printf("Resize: %d | %d\n", width, height);

	SampleApplication::onResize(width, height);

	updateCameraViewport(width, height);
}
void		PhysXSampleApplication::updateCameraViewport(PxU32 clientWidth, PxU32 clientHeight)
{
	Renderer* renderer = getRenderer();

//	PxU32 clientWidth, clientHeight;
//	renderer->getWindowSize(clientWidth, clientHeight);

#if defined(RENDERER_WINDOWS)
	PxU32 width, height;
	m_platform->getWindowSize(width, height);
//	const PxReal ratio = PxReal(width) / PxReal(height);
//	const PxReal ratio2 = PxReal(Rect.right - Rect.left) / PxReal(Rect.bottom - Rect.top);
	mCamera.setScreenSize(clientWidth, clientHeight, width, height);
#else
	mCamera.setScreenSize(clientWidth, clientHeight, clientWidth, clientHeight);
#endif
}
void		PhysXSampleApplication::onRender()
{
	Renderer* renderer = getRenderer();
	if(renderer)
	{
		mCamera.BuildFrustum();
#if 0
		PxScene* mScene = &mSample->getActiveScene();
		if(mScene)
		{
			const PxVec3* frustumVerts = mCamera.getFrustumVerts();
			PxBounds3 cameraBounds(frustumVerts[0], frustumVerts[0]);
			for(PxU32 i=0;i<8;i++)
				cameraBounds.include(frustumVerts[i]);
			mScene->setVisualizationCullingBox(cameraBounds);
			mScene->setVisualizationParameter(PxVisualizationParameter::eCULL_BOX, 1.0f);
		}
#endif
		renderer->clearBuffers();
        
		if(mDrawScreenQuad)
		{
			ScreenQuad sq;
			sq.mLeftUpColor		= mScreenQuadTopColor;
			sq.mRightUpColor	= mScreenQuadTopColor;
			sq.mLeftDownColor	= mScreenQuadBottomColor;
			sq.mRightDownColor	= mScreenQuadBottomColor;

			renderer->drawScreenQuad(sq);
		}

		for(PxU32 i=0; i<mLights.size(); i++)
		{
			renderer->queueLightForRender(*mLights[i]);
		}
		if ( mSample->getPhysics().getPvdConnectionManager() )
		{
			PxVec3 camPos( mCamera.getPos() );
			PxVec3 camDir = mCamera.getViewDir();
			PxVec3 camUp = PxVec3( 0, 1, 0 );
			PxVec3 camTarget = camPos + camDir * 50.0f;
			mSample->getPhysics().getPvdConnectionManager()->setCamera( "SampleCamera", camPos, camUp, camTarget );
		}


		mSample->render();

		renderer->render(mCamera.getViewMatrix(), mCamera.getProjMatrix());

	{
		
        
        
		m_uimanager->render();
        
		/*
		if (mMenuExpand)
		{
            // render menu for Tests or Visualtions
		}
		else
		{
			// print specific Sample menu
        }
		*/


		// PT: "customizeRender" is NOT just for text render, it's a generic render callback that should be called all the time,
		// not just when "mTextAlpha" isn't 0.0
		mSample->customizeRender();

		if(mDrawScreenQuad)
		{
			ScreenQuad sq;
			sq.mLeftUpColor		= mScreenQuadTopColor;
			sq.mRightUpColor	= mScreenQuadTopColor;
			sq.mLeftDownColor	= mScreenQuadBottomColor;
			sq.mRightDownColor	= mScreenQuadBottomColor;

			renderer->drawScreenControls(sq);
		}

	}
    
	mSample->displayFPS();

	if(isConsoleActive())
		mConsole->render(getRenderer());
        
    getRenderer()->finalizeTextRender();
	}
}
/*
void PhysXSampleApplication::advanceSimulation(float dtime)
{
	const PxReal timestep = 1.0f/60.0f;
	while(dtime>0.0f)
	{
		const PxReal dt = dtime>=timestep ? timestep : dtime;
		mScene->simulate(dt);
		mScene->fetchResults(true);
		dtime -= dt;
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
void		PhysXSampleApplication::onAnalogInputEvent(const SampleFramework::InputEvent& ie, float val)
{
	if(mCurrentCameraController)
	{
		mCurrentCameraController->onAnalogInputEvent(ie,val);
	}

	SampleApplication::onAnalogInputEvent(ie,val);

	if(NULL != mSample)
		mSample->onAnalogInputEvent(ie,val);
}
///////////////////////////////////////////////////////////////////////////////
bool		PhysXSampleApplication::isConsoleActive() const
{
	return mConsole && mConsole->isActive();
}
///////////////////////////////////////////////////////////////////////////////
void		PhysXSampleApplication::baseTickPreRender(float dtime)
{
	if(mCurrentCameraController && !isConsoleActive())
		mCurrentCameraController->update(getCamera(), dtime);
}
void		PhysXSampleApplication::baseTickPostRender(float dtime)
{
}
//float tweakElapsedTime(float dtime);
void		PhysXSampleApplication::onTickPreRender(float dtime)
{
	if(!mShowHelp)
	{
		mTextAlphaHelp -= dtime;
		if(mTextAlphaHelp<0.0f)
			mTextAlphaHelp = 0.0f;
	}
	else if(0.0f==mTextAlphaDesc)
	{
		mTextAlphaHelp += dtime;
		if(mTextAlphaHelp>1.0f)
			mTextAlphaHelp = 1.0f;
	}

	if(!mShowDescription)
	{
		mTextAlphaDesc -= dtime;
		if(mTextAlphaDesc<0.0f)
			mTextAlphaDesc = 0.0f;
	}
	else if(0.0f==mTextAlphaHelp)
	{
		mTextAlphaDesc += dtime;
		if(mTextAlphaDesc>1.0f)
			mTextAlphaDesc = 1.0f;
	}


	if(mHideMouseCursor)
	{
		if(hasFocus())
			showCursor(false);
		else
			showCursor(true);
	}

	if (mSample) mSample->onTickPreRender(dtime);
}
void		PhysXSampleApplication::onShutdown()
{
	Renderer* renderer = getRenderer();

	if (renderer)
	{
		renderer->finishRendering();
	}

	if (mSample)
	{
		mSample->onShutdown();
		delete mSample;
		mSample = NULL;
	}

	if (renderer)
	{
		renderer->closeScreenquad();
		renderer->closeTexter();
	}

	for(PxU32 i=0; i<mLights.size(); i++)
		mLights[i]->release();
	mLights.clear();
	for(PxU32 i=0;i<MATERIAL_COUNT;i++)
	{
		mManagedMaterials[i]->release();
		mManagedMaterials[i] = NULL;
	}
	
	DELETESINGLE(mDebugRenderer);
}
void		PhysXSampleApplication::onInit()
{
	Renderer* renderer = getRenderer();

	getPlatform()->getSampleUserInput()->registerInputEventListerner(this);

	PxU32 clientWidth, clientHeight;
	renderer->getWindowSize(clientWidth, clientHeight);
	updateCameraViewport(clientWidth, clientHeight);

	RendererDirectionalLightDesc lightdesc;
	lightdesc.intensity = 1;

	lightdesc.color     = RendererColor(250, 250, 250, 255);
	lightdesc.direction = PxVec3(-4.0f, -5.0f, -3.0f);
	lightdesc.direction.normalizeFast();
	
	mLights.push_back(renderer->createLight(lightdesc));

	renderer->initTexter();	
	renderer->initScreenquad();

	mDebugRenderer = SAMPLE_NEW(RenderPhysX3Debug)(*renderer, *getAssetManager());
	// Create managed materials
	{
		const PxReal c = 0.75f;
		const PxReal opacity = 1.0f;
		const bool doubleSided = false;
		const PxU32 id = 0xffffffff;

		mManagedMaterials[MATERIAL_GREY]	= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(0.5f, 0.5f, 0.5f),	opacity, doubleSided, id, NULL);
		mManagedMaterials[MATERIAL_RED]		= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(c, 0.0f, 0.0f),		opacity, doubleSided, id, NULL);
		mManagedMaterials[MATERIAL_GREEN]	= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(0.0f, c, 0.0f),		opacity, doubleSided, id, NULL);
		mManagedMaterials[MATERIAL_BLUE]	= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(0.0f, 0.0f, c),		opacity, doubleSided, id, NULL);
		mManagedMaterials[MATERIAL_YELLOW]	= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(c, c, 0.0f),			opacity, doubleSided, id, NULL);
		mManagedMaterials[MATERIAL_FLAT]	= SAMPLE_NEW(RenderMaterial)(*renderer, PxVec3(0.5f, 0.5f, 0.5f),	opacity, doubleSided, id, NULL, true, true);
	}

	getNextSample();
	if (mSample)
	{
		mSample->onInit(false);
		mSample->registerInputEvents();
	}
}
void		PhysXSampleApplication::onTickPostRender(float dtime)
{
	if (mSample)
		mSample->onTickPostRender(dtime);
}
void		PhysXSampleApplication::showCursor(bool show)
{ 
	if(m_platform) 
		m_platform->showCursor(show); 
}
void		PhysXSampleApplication::setMouseCursorHiding(bool hide)
{ 
	if(hide != mHideMouseCursor)
	{
		mHideMouseCursor = hide;
		showCursor(!hide);
	}						
}
void		PhysXSampleApplication::setMouseCursorRecentering(bool recenter)
{
	PX_ASSERT(SamplePlatform::platform());
	SamplePlatform::platform()->setMouseCursorRecentering(recenter);
}
void		PhysXSampleApplication::onPointerInputEvent(const InputEvent& ie, physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val)
{
	SampleApplication::onPointerInputEvent(ie,x,y,dx,dy,val);

	if(mSample)
	{
		mSample->onPointerInputEvent(ie,x,y,dx,dy,val);
	}

	if(mCurrentCameraController)
	{
		mCurrentCameraController->onPointerInputEvent(ie,x,y,dx,dy,val);
	}
}
void		PhysXSampleApplication::onKeyDownEx(SampleFramework::SampleUserInput::KeyCode keyCode, physx::PxU32 wParam)
{ 
	if(mSample) 
		mSample->onKeyDownEx(keyCode, wParam);

	if(!mConsole)
		return;

	if(mConsole->mIsActive)
	{
		mConsole->onKeyDown(keyCode, wParam);
	}
	else
	{
		if(keyCode == SampleFramework::SampleUserInput::KEY_TAB)
			mConsole->mIsActive = true;
	}
}
void		PhysXSampleApplication::onResize(PxU32 width, PxU32 height)										
{ 
	if(mSample)
		mSample->onResize(width, height); 
}
///////////////////////////////////////////////////////////////////////////////
bool		PhysXSampleApplication::onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val)
{ 
	if(mCurrentCameraController)
	{
		mCurrentCameraController->onDigitalInputEvent(ie,val);
	}

	SampleApplication::onDigitalInputEvent(ie,val);

	if (isConsoleActive())
	{
		return mConsole->onDigitalInputEvent(ie,val);
	}
    
    
    
    return m_uimanager->onDigitalInputEvent(ie,val);
    
    
}
void		PhysXSampleApplication::toggleDebugRenderer()
{
	PxScene& scene = mSample->getActiveScene();
	PxReal debugRendering = scene.getVisualizationParameter(PxVisualizationParameter::eSCALE);
	scene.setVisualizationParameter(PxVisualizationParameter::eSCALE, mSample->getDebugRenderScale());
	mMenuVisualizations[0].toggleState = true;
}
void		PhysXSampleApplication::handleSettingMenuKey(MenuKey::Enum menuKey)
{
	// transfered to SampleUI::Visualizations
}
bool		PhysXSampleApplication::handleMenuKey(MenuKey::Enum menuKey)
{
    // transfered to SampleUI
    
    // CREATE SPECIAL TEXT TREE MENU AS AN IMPLEMENTATION OF NxUI
    
    TreeGroup* parent = mSelected->getParent(), *child = NULL;
	PX_ASSERT(parent);
	
    
	switch (menuKey)
	{
	case MenuKey::NAVI_LEFT:
		if (NULL != parent->getParent())
			mSelected = parent;
		break;
	case MenuKey::NAVI_RIGHT:
		if (NULL != (child = mSelected->getFirstChild()))
			mSelected = child;
		break;
	case MenuKey::NAVI_UP:
		if (NULL != (child = parent->getPreviousChild(*mSelected)))
			mSelected = child;
		else if (NULL != (child = parent->getLastChild()))
			mSelected = child;
		break;
	case MenuKey::NAVI_DOWN:
		if (NULL != (child = parent->getNextChild(*mSelected)))
			mSelected = child;
		else if (NULL != (child = parent->getFirstChild()))
			mSelected = child;
		break;
	case MenuKey::SELECT:
		if (mSelected->isTest())
		{
			mMenuExpand = false;
			switchSample();
			return false;
		}
		else
		{
			if (NULL != (child = mSelected->getFirstChild()))
				mSelected = child;
		}
		break;
	default:
		//mSelected = mRunning;
		break;
	}

	return true;
    
}
void		PhysXSampleApplication::switchSample()
{
	bool isRestart = mRunning == mSelected;
	bool togglePause = isPaused();

	if (mSample && togglePause)
		mSample->togglePause();

	setDefaultCameraController();

	Renderer* renderer = getRenderer();
	if (renderer)
	{
		renderer->finishRendering();
	}

	if (mSample)
	{
		mSample->onShutdown();
		delete mSample;
		mSample = NULL;
	}

	if (getNextSample())
	{	
		mSample->onInit(isRestart);
		mSample->registerInputEvents();
		if (mCurrentCameraController)
			mCurrentCameraController->update(getCamera(), 0.0f);
		mSample->onTickPreRender(0.0f);
		mSample->onSubstep(0.0f);
		mSample->initRenderObjects();
	}

	if (mSample && togglePause)
		mSample->togglePause();
}
//=============================================================================
// PhysXSampleManager
//-----------------------------------------------------------------------------
TreeGroup*	PhysXSampleApplication::mSampleTreeRoot = NULL;
TreeGroup&	PhysXSampleApplication::getSampleTreeRoot()
{
	if (!mSampleTreeRoot)
	{
		mSampleTreeRoot = new TreeGroup("");
	}
	return *mSampleTreeRoot;
}
bool		PhysXSampleApplication::addSample(TreeGroup &root, SampleCreator creator, const char *fullPath)
{
	PX_ASSERT(fullPath);

	do
	{
		if ('\0' == fullPath[0] || '/' == fullPath[0])
		{
			printf("invalid name: %s\n", fullPath);
			break;
		}

		const char* p = fullPath;
		while ('\0' != *p && '/' != *p)
			++p;

		if ('\0' == *p) // test name
		{
			if (root.getChildByName(fullPath))	// duplicated name
			{
				printf("test \"%s\" exists.\n", fullPath);
				break;
			}
			root.addTest(creator, fullPath);
		}
		else // group name
		{
			TreeGroup* group = root.getChildByName(fullPath, p - fullPath);
			if (group)
				return addSample(*group, creator, p + 1);

			group = new TreeGroup(fullPath, p - fullPath);
			if (!addSample(*group, creator, p + 1))
			{
				delete group;
				break;
			}
			root.addGroup(group);
		}

		return true;
	} while (false);

	return false;
}
bool		PhysXSampleApplication::getNextSample()
{
	TreeGroup& root = PhysXSampleApplication::getSampleTreeRoot();

	if (NULL == mSelected)
		mSelected = root.getFirstTest();

	if (NULL == mSelected)
		return false;

	mRunning = mSelected;
	mSample = (*mRunning->getCreator())(*this);
	return true;
}
void		PhysXSampleApplication::saveCameraState()
{
	mSavedView = getCamera().getViewMatrix();
}
void		PhysXSampleApplication::restoreCameraState()
{
	getCamera().setView(mSavedView);
}
