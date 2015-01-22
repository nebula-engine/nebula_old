
#ifndef SAMPLE_APPLICATION_H
#define SAMPLE_APPLICATION_H

#include <Renderer.h>
#include <RendererWindow.h>

#include <FrameworkFoundation.h>

#include <AssetManager.h>
#include <PsTime.h>
#include "foundation/PxMat44.h"
#include <SampleUserInput.h>

namespace physx { 
	class PxProfileZone;
	class PxProfileZoneManager;
	class PxProfileEventSender;
}

#define SMOOTH_CAM

namespace Framework
{

	class NxCommandLine;

	class NxApplication
	{
	public:
		//------------------------------------------------------
		// Functions
		//------------------------------------------------------
		// constructor, destructor
														NxApplication(const NxCommandLine& cmdline, const char* assetPathPrefix="media", PxI32 camMoveButton = -1);
		virtual											~NxApplication(void);
		// init, shutdown
		virtual			void							onInit(void) = 0;
		virtual			void							onShutdown(void) = 0;
		// modules
		const			NxCommandLine&					getCommandLine(void) const	{ return m_cmdline; }
						NxContentManager*				getContentManager();		{ return m_contentManager; }
						NxInterfaceManager*				getInterfaceManager()		{ return m_userInterfaceManager; }
						NxPhysics*						getPhysics()				{ return m_physics; }
						SampleRenderer::Renderer*		getRenderer(void)			{ return m_renderer; }
						NxRender*						getRender()					{ return m_render; }
						AssetManager*					getAssetManager(void)		{ return m_assetManager; }
		
		// time
		virtual			float							tweakElapsedTime(float dtime)									{ return dtime;	}
		
		// window???
		virtual			void							onOpen(void);
		virtual			bool							onClose(void);
		
		// error
		virtual			void							fatalError(const char * msg);
		
	private:
		// get rid of stupid C4512
						void							operator=(const NxApplication&);
		//------------------------------------------------------
		// Variables
		//------------------------------------------------------
	protected:
		// modules
						NxAssetManager*					m_assetManager;		
						NxCommandLine&					m_cmdline;
						NxContentManager*				m_contentManager;
						NxInput*						m_input;
						NxPhysics*						m_physics;
						NxRender*						m_render;
						NxUserInterfaceManager*			m_userInterfaceManager;


		// time
						physx::PxU64					m_timeCounter;
						physx::shdfnd::Time				m_time;
		// android
						void*							android_window_ptr;
	};

} //namespace Framework

#endif


