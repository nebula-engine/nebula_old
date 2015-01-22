
class NxRender: public Renderer::RendererWindow
{
public:
	virtual		void					onTickPreRender(float dtime) = 0;
	virtual		void					onRender(void) = 0;
	virtual		void					onTickPostRender(float dtime) = 0;
	virtual		void					onDraw(void);


	const		PxMat44&				getEyeTransform(void)									const	{ return m_worldToView.getInverseTransform(); }
				void					setEyeTransform(const PxMat44& eyeTransform);
				void					setEyeTransform(const PxVec3& pos, const PxVec3& rot);
				void					setViewTransform(const PxMat44& viewTransform);
	const		PxMat44&				getViewTransform(void)									const;


	virtual		void					rotateCamera(PxF32 dx, PxF32 dy);
private:
				bool					m_disableRendering;
	

				PxVec3					m_eyeRot;
				Transform<PxMat44>		m_worldToView;

				PxF32					m_sceneSize;

#if defined(SMOOTH_CAM)
				PxVec3					m_targetEyeRot;
				PxVec3					m_targetEyePos;
#endif

				Renderer::Renderer*		m_renderer;
				RenderPhysX3Debug*		m_debugRenderer;

}


 