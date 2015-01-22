#ifndef WINDOWS_SAMPLE_PLATFORM_H
#define WINDOWS_SAMPLE_PLATFORM_H

#include <SamplePlatform.h>
#include <windows/WindowsSampleUserInput.h>

struct IDirect3D9;
struct IDirect3DDevice9;

namespace SampleFramework
{
	class WindowsPlatform : public SamplePlatform 
	{
	public:
		explicit							WindowsPlatform(SampleRenderer::RendererWindow* _app);
		virtual								~WindowsPlatform();
		// System
		virtual void						showCursor(bool);
		virtual void						postRendererSetup(SampleRenderer::Renderer* renderer);
		virtual size_t						getCWD(char* path, size_t len);
		virtual void						setCWDToEXE(void);
		virtual bool						openWindow(physx::PxU32& width, physx::PxU32& height, const char* title, bool fullscreen);
		virtual void						update();
		virtual bool						isOpen();
		virtual bool						closeWindow();
		virtual bool						hasFocus() const;
		virtual void						setFocus(bool b);
		virtual void						getTitle(char *title, physx::PxU32 maxLength) const;
		virtual void						setTitle(const char *title);
		virtual physx::PxU64				getWindowHandle();
		virtual void						setWindowSize(physx::PxU32 width, physx::PxU32 height);
		virtual void						getWindowSize(physx::PxU32& width, physx::PxU32& height);
		virtual void						showMessage(const char* title, const char* message);
		virtual void*						compileProgram(void * context, const char* assetDir, const char *programPath, physx::PxU64 profile, const char* passString, const char *entry, const char **args);
		virtual void*						initializeD3D9();
		virtual bool						isD3D9ok();
		// Rendering
		virtual void						initializeOGLDisplay(const SampleRenderer::RendererDesc& desc, physx::PxU32& width, physx::PxU32& height);
		virtual void						postInitializeOGLDisplay();
		virtual physx::PxU32				initializeD3D9Display(void * presentParameters, char* m_deviceName, physx::PxU32& width, physx::PxU32& height, void * m_d3dDevice_out);
		virtual physx::PxU32				D3D9Present();
		virtual physx::PxU64				getD3D9TextureFormat(SampleRenderer::RendererTexture2D::Format format);
		virtual bool						makeContextCurrent();
		virtual void						freeDisplay();
		virtual bool						isContextValid();
		virtual void						swapBuffers();
		virtual void						setupRendererDescription(SampleRenderer::RendererDesc& renDesc);
		// Input
		virtual void						doInput();		
		// File System
		virtual bool						makeSureDirectoryPathExists(const char* dirPath);
		virtual const SampleUserInput*		getSampleUserInput() const { return &m_windowsSampleUserInput; }
		virtual SampleUserInput*			getSampleUserInput() { return &m_windowsSampleUserInput; }
		WindowsSampleUserInput&				getWindowsSampleUserInput() { return m_windowsSampleUserInput; }
		const WindowsSampleUserInput&		getWindowsSampleUserInput() const { return m_windowsSampleUserInput; }
		virtual const char*					getPlatformName() const { return m_platformName; }
		virtual void						setMouseCursorRecentering(bool val);
		virtual bool						getMouseCursorRecentering() const;
		physx::PxVec2						getMouseCursorPos() const { return m_mouseCursorPos; }
		void								setMouseCursorPos(const physx::PxVec2& pos) { m_mouseCursorPos = pos; }
		void								recenterMouseCursor(bool generateEvent);
	protected:	
		IDirect3D9*							m_d3d;
		IDirect3DDevice9*					m_d3dDevice;
		HWND								m_hwnd;
		HDC									m_hdc;
		HGLRC								m_hrc;
		HMODULE		                        m_library;
		bool								m_isHandlingMessages;
		bool								m_destroyWindow;
		bool								m_hasFocus;
		bool								m_showCursor;
		char								m_platformName[256];
		WindowsSampleUserInput				m_windowsSampleUserInput;
		physx::PxVec2						m_mouseCursorPos;
		bool								m_recenterMouseCursor;
	};
}

#endif
