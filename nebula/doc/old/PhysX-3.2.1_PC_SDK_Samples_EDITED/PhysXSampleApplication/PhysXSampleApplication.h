#ifndef PHYSX_SAMPLE_APPLICATION_H
#define PHYSX_SAMPLE_APPLICATION_H

#include "SamplePreprocessor.h"
#include "SampleApplication.h"
#include "SampleCamera.h"
#include "SampleCameraController.h"
#include "SampleAllocator.h"
#include "PxFiltering.h"
#include "RawLoader.h"
#include "RendererMeshContext.h"
#include "PxTkFPS.h"
#include "physxvisualdebuggersdk/PvdConnectionManager.h"
#include "PxVisualizationParameter.h"
#include "PxTkRandom.h"

#include "PsHashMap.h"
#include "PsUtilities.h"
#include "SampleArray.h"

#ifdef PX_WINDOWS
#include "pxtask/PxCudaContextManager.h"
#endif

#define	PHYSX_VERSION_STRING	PX_STRINGIZE(PX_PHYSICS_VERSION_MAJOR)"."PX_STRINGIZE(PX_PHYSICS_VERSION_MINOR)

#define	SAMPLE_MEDIA_DIR		"/PhysX/"PHYSX_VERSION_STRING"/Samples"
#define	SAMPLE_OUTPUT_DIR		"media"SAMPLE_MEDIA_DIR"/user"

namespace physx
{
	class PxDefaultCpuDispatcher;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	class PxGeometry;
	class PxMaterial;
	class PxRigidActor;
};

	class RenderPhysX3Debug;
	class RenderBaseActor;
	class RenderMaterial;
	class RenderMeshActor;
	class RenderTexture;
	class Stepper;
	class Console;

	class PhysXSampleApplication;
	class PhysXSample;

	class PhysXSampleCreator
	{
	public:
		virtual ~PhysXSampleCreator() {}
		virtual PhysXSample* operator()(PhysXSampleApplication& app) const = 0;
	};

	typedef PhysXSampleCreator *SampleCreator;
	typedef PhysXSample* (*FunctionCreator)(PhysXSampleApplication& app);
//	typedef PhysXSample* (*SampleCreator)(PhysXSampleApplication& app);

	class PhysXSampleApplication :	public SampleFramework::SampleApplication,
						public SampleAllocateable
	{
		friend class PhysXSample;

		struct MenuKey
		{
			enum Enum
			{
				NONE,
				ESCAPE,
				SELECT,
				NAVI_UP,
				NAVI_DOWN,
				NAVI_LEFT,
				NAVI_RIGHT
			};
		};
		
		// menu events
		struct MenuType
		{
			enum Enum
			{
				NONE,
				HELP,
				SETTINGS,
				VISUALIZATIONS,
				TESTS
			};
		};
		
		struct MenuTogglableItem
		{
			MenuTogglableItem(PxU32 c, const char* n) : toggleCommand(c), toggleState(false), name(n)  {}
			PxU32 toggleCommand; 
			bool toggleState;
			const char* name;
		};

		public:
														PhysXSampleApplication(const SampleFramework::SampleCommandLine& cmdline);
		virtual											~PhysXSampleApplication();

		///////////////////////////////////////////////////////////////////////////////

		// Implements SampleApplication/RendererWindow
		virtual			void							onInit();
		virtual			void							onShutdown();

		virtual			float							tweakElapsedTime(float dtime);
		virtual			void							onTickPreRender(float dtime);
		virtual			void							onRender();
		virtual			void							onTickPostRender(float dtime);
		
		virtual			void							onKeyDownEx(SampleFramework::SampleUserInput::KeyCode keyCode, physx::PxU32 wParam);
		virtual			void							onAnalogInputEvent(const SampleFramework::InputEvent& , float val);
		virtual			bool							onDigitalInputEvent(const SampleFramework::InputEvent& , bool val);        
		virtual			void							onPointerInputEvent(const SampleFramework::InputEvent&, physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val);

		virtual			void							onResize(PxU32 width, PxU32 height);

						void							baseTickPreRender(float dtime);
						void							baseTickPostRender(float dtime);

						void							baseResize(PxU32 width, PxU32 height);

		///////////////////////////////////////////////////////////////////////////////

						void							customizeSample(SampleSetup&);
//						void							onSubstep(float dtime);

		///////////////////////////////////////////////////////////////////////////////
						void							applyDefaultVisualizationSettings();
						void							saveCameraState();
						void							restoreCameraState();

		// Camera functions
		PX_FORCE_INLINE	void							setDefaultCameraController()				{ mCurrentCameraController = &mCameraController;	}
		PX_FORCE_INLINE	void							setCameraController(CameraController* c)	{ mCurrentCameraController = c;						}

		PX_FORCE_INLINE	PxReal							getTextAlpha1()						const	{ return mTextAlphaHelp;								}
		PX_FORCE_INLINE	PxReal							getTextAlpha2()						const	{ return mTextAlphaDesc;								}
		PX_FORCE_INLINE	bool							isPaused()							const	{ return mPause;									}
		PX_FORCE_INLINE	Camera&							getCamera()									{ return mCamera;									}
		PX_FORCE_INLINE	RenderPhysX3Debug*				getDebugRenderer()					const	{ return mDebugRenderer;							}

						bool							isConsoleActive()					const;
						void							showCursor(bool show);
						void							setMouseCursorHiding(bool hide);
						void							setMouseCursorRecentering(bool recenter);


		PX_FORCE_INLINE	void							registerLight(SampleRenderer::RendererLight* light)	{ mLights.push_back(light);					}
						void							collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents);
						const char*						inputInfoMsg(const char* firstPart,const char* secondPart, physx::PxI32 inputEventId1, physx::PxI32 inputEventId2);
						const char*						inputInfoMsg_Aor_BandC(const char* firstPart,const char* secondPart, physx::PxI32 inputEventIdA, physx::PxI32 inputEventIdB, physx::PxI32 inputEventIdC);
						const char*						inputMoveInfoMsg(const char* firstPart,const char* secondPart, physx::PxI32 inputEventId1, physx::PxI32 inputEventId2,physx::PxI32 inputEventId3,physx::PxI32 inputEventId4);
						void							requestToClose() { mIsCloseRequested = true; }
						bool							isCloseRequested() { return mIsCloseRequested; }
						

		private:
						PxReal							mTextAlphaHelp;
						PxReal							mTextAlphaDesc;
						MenuType::Enum					mMenuType;
						std::vector<MenuTogglableItem>	mMenuVisualizations;
						size_t							mMenuVisualizationsIndexSelected;
						void							setMenuVisualizations(MenuTogglableItem& togglableItem);
						char							m_Msg[512];




						PxTransform						mSavedView;
						bool							mIsCloseRequested;
										


		protected:
						Console*										mConsole;


						Camera											mCamera;
						DefaultCameraController							mCameraController;
						CameraController*								mCurrentCameraController;

						std::vector<SampleRenderer::RendererLight*>		mLights;
						RenderMaterial*									mManagedMaterials[MATERIAL_COUNT];

						
						bool											mPause;
						bool											mOneFrameUpdate;

						bool											mShowHelp;
						bool											mShowDescription;
						bool											mShowExtendedHelp;
						bool											mHideMouseCursor;
						//

						bool											mDrawScreenQuad;
						SampleRenderer::RendererColor					mScreenQuadTopColor;
						SampleRenderer::RendererColor					mScreenQuadBottomColor;						
						
						void											updateCameraViewport(PxU32 w, PxU32 h);
						bool											initLogo();

	private:
						bool											handleMenuKey(MenuKey::Enum menuKey);
						
						void											refreshVisualizationMenuState(PxVisualizationParameter::Enum p);
						void											toggleDebugRenderer();
		
//-----------------------------------------------------------------------------
// PhysXSampleManager
//-----------------------------------------------------------------------------
	public:
		static bool registerSample(SampleCreator creator, const char* fullPath)
		{
			return addSample(getSampleTreeRoot(), creator, fullPath);
		}
		bool getNextSample();
		void switchSample();
	protected:
		static TreeGroup*	mSampleTreeRoot;
		static TreeGroup&	getSampleTreeRoot();
		static bool addSample(TreeGroup& root, SampleCreator creator, const char* fullPath);
	public:
		bool											mMenuExpand;
		TreeGroup*								mRunning;
		TreeGroup*								mSelected;
		PhysXSample*									mSample;
		const char*										mDefaultSamplePath;
	};


#endif

