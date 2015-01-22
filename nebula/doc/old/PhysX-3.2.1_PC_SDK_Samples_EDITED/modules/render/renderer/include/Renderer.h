

#ifndef RENDERER_H
#define RENDERER_H

#include <RendererConfig.h>

#include <RendererMaterial.h>
#include <RendererWindow.h>
#include <RendererColor.h>
#include <SampleUserInput.h>

#include <vector>
#include <string>
#include <map>

#include <foundation/PxVec2.h>

namespace physx {
	class PxProfileEventSender;
	class PxProfileZone;
}

namespace SampleRenderer
{

	class RendererDesc;
	class RendererWindow;
	class RendererVertexBuffer;
	class RendererVertexBufferDesc;
	class RendererIndexBuffer;
	class RendererIndexBufferDesc;
	class RendererInstanceBuffer;
	class RendererInstanceBufferDesc;
	class RendererTexture2D;
	class RendererTexture2DDesc;
	class RendererTarget;
	class RendererTargetDesc;
	class RendererMaterial;
	class RendererMaterialDesc;
	class RendererMesh;
	class RendererMeshDesc;
	class RendererMeshContext;
	class RendererLight;
	class RendererLightDesc;

	class RendererColor;
	class RendererProjection;

	

	class Renderer
	{
	public:
		

		

	public:
		// assetDir should point to the directory where shaders can be found in the assetDir/shaders/ subdirectory and textures are in assetDir/textures
		static Renderer *createRenderer(const RendererDesc &desc, const char* assetDir);

		static const char *getDriverTypeName(DriverType type);

	protected:
		Renderer(DriverType driver, const char* shaderDir);
		virtual ~Renderer(void);

	public:
		void release(void);
		DriverType getDriverType(void) const;				// get the driver type for this renderer.
		PxF32 getPixelCenterOffset(void) const;				// get the offset to the center of a pixel relative to the size of a pixel (so either 0 or 0.5).
		const char *getDeviceName(void) const;				// get the name of the hardware device.
		void queueMeshForRender(RendererMeshContext &mesh);	// adds a mesh to the render queue.
		void queueLightForRender(RendererLight &light);		// adds a light to the render queue.
		void render(const physx::PxMat44 &eye, const RendererProjection &proj, RendererTarget *target=0, bool depthOnly=false);// renders the current scene to the offscreen buffers. empties the render queue when done.
		void setFog(const RendererColor &fogColor, float fogDistance);// sets fog
		void setAmbientColor(const RendererColor &ambientColor);// sets the ambient lighting color.
		void setClearColor(const RendererColor &clearColor);// sets the clear color.
		RendererColor& getClearColor() { return m_clearColor; }// get and set the output message stream
		void setErrorCallback(PxErrorCallback* errc) { m_errorCallback = errc; }
		PxErrorCallback* getErrorCallback() { return m_errorCallback; }
		virtual void clearBuffers(void) = 0;// clears the offscreen buffers.
		virtual bool swapBuffers(void) = 0;// presents the current color buffer to the screen. returns true on device reset and if buffers need to be rewritten.
        virtual void finalizeTextRender(void) {}
		virtual void *getDevice() = 0;// get the device pointer (void * abstraction)
		virtual bool captureScreen(const char* filename) = 0;
		virtual void getWindowSize(PxU32 &width, PxU32 &height) const = 0;
		virtual PxU32 convertColor(const RendererColor& color) const = 0;
		virtual void finishRendering() {}

		virtual RendererVertexBuffer   *createVertexBuffer(  const RendererVertexBufferDesc   &desc) = 0;
		virtual RendererIndexBuffer    *createIndexBuffer(   const RendererIndexBufferDesc    &desc) = 0;
		virtual RendererInstanceBuffer *createInstanceBuffer(const RendererInstanceBufferDesc &desc) = 0;
		virtual RendererTexture2D      *createTexture2D(     const RendererTexture2DDesc      &desc) = 0;
		virtual RendererTarget         *createTarget(        const RendererTargetDesc         &desc) = 0;
		virtual RendererMaterial       *createMaterial(      const RendererMaterialDesc       &desc) = 0;
		virtual RendererMesh           *createMesh(          const RendererMeshDesc           &desc) = 0;
		virtual RendererLight          *createLight(         const RendererLightDesc          &desc) = 0;

		// These two methods are only necessary for internal caching of compiled materials
	protected:
		RendererMaterial*				hasMaterialAlready(const RendererMaterialDesc& desc);
		void							registerMaterial(const RendererMaterialDesc& desc, RendererMaterial* mat);
	public:
		void                            unregisterMaterial(RendererMaterial* mat);

		// Text rendering
		virtual	bool					initTexter();
		virtual	void					closeTexter();
		void							print(PxU32 x, PxU32 y, const char* text, PxReal scale=0.5f, PxReal shadowOffset=6.0f, RendererColor textColor = RendererColor(255, 255, 255, 255));
		const char*						getAssetDir() { return m_assetDir.c_str(); }
		
		// assetDir should point to the directory where shaders can be found in the assetDir/shaders/ subdirectory and textures are in assetDir/textures
		void							setAssetDir( const char * assetDir ) { m_assetDir = assetDir; }

		// On-screen controls/sticks for tablets
#if defined(RENDERER_TABLET)
		struct TabletButton
		{
			PxVec2						leftBottom;
			PxVec2						rightTop;
			physx::PxU8					pressedCount;
			physx::PxU16				emulatedKeyCode;
			std::string					text;
			RendererMesh*				mesh;
			RendererMaterial*			material;
			RendererMaterialInstance*	materialInstance;
			
			PxVec4						defaultColor, pressedColor;
			
			void						(*callback)();
			
			TabletButton();
			void 						setPressedCount(physx::PxU8);
			void 						incPressed();
			void 						decPressed();
		};
		
		virtual bool					initControls(RendererMaterial* controlMaterial, RendererMaterialInstance* controlMaterialInstance);
		void							setControlPosition(int ctrl_idx, const PxVec2&);
		void							setControlDefaultPosition(int ctrl_idx);
		PxBounds3						getControlBounds(int ctrl_idx);
		PxBounds3						getCenteredControlBounds(int ctrl_idx);
		PxVec2							getControlPosition(int ctrl_idx);
		PxVec2							getCenteredControlPosition(int ctrl_idx);
		// On-screen buttons for tablets
		void							addButton(const PxVec2& leftBottom, 
												const PxVec2& rightTop, 
												void (*func_ptr)(), 
												RendererMaterial* controlMaterial, 
												RendererMaterialInstance* controlMaterialInstance);
		void							releaseAllButtons();
		void							bindButtonToUserInput(size_t buttonIndex, physx::PxU16 userInputId, const char* buttonName);
		std::vector<TabletButton>& 		screenButtons();
#endif

		// Screenquad
		virtual	bool					initScreenquad();
		virtual	void					closeScreenquad();
				bool					drawScreenQuad(const ScreenQuad& screenQuad);
                bool					drawScreenControls(const ScreenQuad& screenQuad);
				bool					drawLines2D(PxU32 nbVerts, const PxReal* vertices, const RendererColor& color);
				bool					drawLines2D(PxU32 nbVerts, const PxReal* vertices, const RendererColor* colors);

		// Disable this performance optimization when CUDA/Graphics Interop is in use
		void setVertexBufferDeferredUnlocking( bool enabled );
		bool getVertexBufferDeferredUnlocking() const;
		

		static void setProfileZone( physx::PxProfileZone* inSDK ) { mProfileZone = inSDK; }
		static physx::PxProfileEventSender*	getEventBuffer();

	private:
		void renderMeshes(std::vector<RendererMeshContext*> & meshes, RendererMaterial::Pass pass);
		void renderDeferredLights(void);

	private:		
		RendererMesh*				initControl(PxReal* vertices, PxReal* texcoords, PxU32 verticesCount);

		virtual bool beginRender(void) { return true;}
		virtual void endRender(void) {}
		virtual void bindViewProj(const physx::PxMat44 &eye, const RendererProjection &proj)    = 0;
		virtual void bindFogState(const RendererColor &fogColor, float fogDistance)      = 0;
		virtual void bindAmbientState(const RendererColor &ambientColor)                 = 0;
		virtual void bindDeferredState(void)                                             = 0;
		virtual void bindMeshContext(const RendererMeshContext &context)                 = 0;
		virtual void beginMultiPass(void)                                                = 0;
		virtual void endMultiPass(void)                                                  = 0;
		virtual void renderDeferredLight(const RendererLight &light)                     = 0;

		virtual bool isOk(void) const = 0;
	public:
		virtual	void setupTextRenderStates()		= 0;
		virtual	void resetTextRenderStates()		= 0;
		virtual	void renderTextBuffer(const void* vertices, PxU32 nbVerts, const PxU16* indices, PxU32 nbIndices, RendererMaterial* material) = 0;

#if defined(RENDERER_PSP2)
		virtual void renderLines2D(const void* vertices, PxU32 nbVerts, RendererMaterial* material)	= 0; 
#else
		virtual	void renderLines2D(const void* vertices, PxU32 nbVerts)	= 0;
#endif
		
#if defined(RENDERER_TABLET)
		virtual void renderControls(const ScreenQuad& screenQuad);
		virtual void renderButtons(const ScreenQuad& screenQuad);
#endif
		virtual	void setupScreenquadRenderStates()	= 0;
		virtual	void resetScreenquadRenderStates()	= 0;

	private:
		Renderer &operator=(const Renderer&) { return *this; }


		static physx::PxProfileZone*	mProfileZone;

		const DriverType				m_driver;
	
	protected:
		PxErrorCallback*				m_errorCallback;

	private:
		std::vector<RendererMeshContext*>	m_visibleLitMeshes;
		std::vector<RendererMeshContext*>	m_visibleUnlitMeshes;
		std::vector<RendererMeshContext*>	m_screenSpaceMeshes;
		std::vector<RendererLight*>			m_visibleLights;

		RendererColor					m_fogColor;
		float							m_fogDistance;

		RendererColor					m_ambientColor;
		RendererColor					m_clearColor;

		// Texter data
		RendererMaterial*				m_textMaterial;
		RendererMaterialInstance*		m_textMaterialInstance;

		// Screenquad data
		RendererMaterial*				m_screenquadOpaqueMaterial;
		RendererMaterialInstance*		m_screenquadOpaqueMaterialInstance;
		RendererMaterial*				m_screenquadAlphaMaterial;
		RendererMaterialInstance*		m_screenquadAlphaMaterialInstance;

#if defined(RENDERER_TABLET)			
		RendererMaterial*				m_controlMaterial;
		RendererMaterialInstance*		m_controlMaterialInstance;
		RendererMesh*					m_controlMesh[2];
		PxVec2							m_controlHalfSize;
		PxVec2							m_controlCenteredPos[2];
		PxVec2							m_controlPos[2];
		std::vector<TabletButton>		m_buttons;
#endif		

	protected:
		bool                              m_deferredVBUnlock;
		PxF32                             m_pixelCenterOffset;
		char                              m_deviceName[256];
		bool                              m_useShadersForTextRendering;
		std::string                       m_assetDir;

#if RENDERER_EXPERIMENTAL_MATERIAL_CACHING
		struct CompareRenderMaterialDesc
		{
			bool operator()(const RendererMaterialDesc& desc1, const RendererMaterialDesc& desc2) const;
		};
		typedef std::map<RendererMaterialDesc, RendererMaterial*, CompareRenderMaterialDesc> tMaterialCache;
		tMaterialCache                    m_materialCache;
#endif // RENDERER_EXPERIMENTAL_MATERIAL_CACHING
	};

	inline physx::PxProfileEventSender*	getEventBuffer() { return Renderer::getEventBuffer(); }

} // namespace SampleRenderer

#endif
