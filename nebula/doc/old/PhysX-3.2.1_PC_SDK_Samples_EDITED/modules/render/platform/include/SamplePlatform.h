#ifndef SAMPLE_PLATFORM_H
#define SAMPLE_PLATFORM_H

#include <RendererDesc.h>
#include <RendererWindow.h>
#include <RendererTexture2D.h>

namespace Framework
{
	class Application;
	class UserInput;

	/* This class declares and partly implements platform-abstraction level.
		Do not use platform-specific types and functions here.
	*/
	class Platform 
	{
	protected:
		SampleRenderer::RendererWindow*		m_app;		
		static Platform*					m_platform;

	public:
		// access
		static Platform*					platform();
		static void							setPlatform(Platform*);

		// creation
		explicit							Platform(SampleRenderer::RendererWindow* _app);
		virtual								~Platform() = 0;
		// System
		virtual void						showCursor(bool);

		virtual size_t						getCWD(char* path, size_t len);
		virtual void						setCWDToEXE(void);
		virtual void						popPathSpec(char *path);
		virtual bool						preOpenWindow(void * ptr);
		virtual bool						openWindow(physx::PxU32& width, physx::PxU32& height, const char* title, bool fullscreen);
		virtual void						update();
		virtual bool						closeWindow();
		virtual bool						updateWindow();
		virtual bool						hasFocus() const;
		virtual void						setFocus(bool b);
		virtual bool						isOpen();
		virtual physx::PxU64				getWindowHandle();
		virtual void						setWindowSize(physx::PxU32 width, physx::PxU32 height);
		virtual void						getWindowSize(physx::PxU32& width, physx::PxU32& height);
		virtual void						getTitle(char *title, physx::PxU32 maxLength) const;
		virtual void						setTitle(const char *title);

		virtual void						setMouseCursorRecentering(bool val) {}
		virtual bool						getMouseCursorRecentering() const { return false; }

		virtual void						showMessage(const char* title, const char* message);
		virtual void*						compileProgram(void * context, const char* assetDir, const char *programPath, physx::PxU64 profile, const char* passString, const char *entry, const char **args);
		virtual void*						initializeD3D9();
		virtual bool						isD3D9ok();
		virtual const char*					getPathSeparator();
		// Rendering
		virtual void						initializeCGRuntimeCompiler();
		virtual void						initializeOGLDisplay(const SampleRenderer::RendererDesc& desc,
																physx::PxU32& width, 
																physx::PxU32& height);
		virtual physx::PxU32				initializeD3D9Display(void * presentParameters, 
																char* m_deviceName, 
																physx::PxU32& width, 
																physx::PxU32& height,
																void * m_d3dDevice_out);
		virtual physx::PxU32				D3D9Present();
		virtual void						D3D9BlockUntilNotBusy(void * resource);
		virtual void						D3D9DeviceBlockUntilIdle();
		virtual physx::PxU64				getD3D9TextureFormat(SampleRenderer::RendererTexture2D::Format format);
		virtual void						postInitializeOGLDisplay();
		virtual bool						makeContextCurrent();
		virtual bool						isContextValid();
		virtual void						freeDisplay();
		virtual void						swapBuffers();
		virtual void						postRendererRelease();
		virtual void						preRendererSetup();
		virtual void						postRendererSetup(SampleRenderer::Renderer* renderer);
		virtual void						setupRendererDescription(SampleRenderer::RendererDesc& renDesc);
		// Input
		virtual void						doInput();	

		virtual const SampleUserInput*		getSampleUserInput() const = 0;
		virtual SampleUserInput*			getSampleUserInput() = 0;

		virtual const char*					getPlatformName() const { return NULL; }
		// File System
		virtual bool						makeSureDirectoryPathExists(const char* dirPath);
	};

	Platform*		createPlatform(SampleRenderer::RendererWindow* _app);
}

#endif
