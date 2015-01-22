
//
// RendererCapsuleShape : convenience class for generating a capsule mesh.
//
#ifndef RENDERER_CAPSULE_SHAPE_H
#define RENDERER_CAPSULE_SHAPE_H

#include <RendererShape.h>
#include <RendererBoxShape.h>

namespace SampleRenderer
{

	class RendererVertexBuffer;
	class RendererIndexBuffer;

	class RendererCapsuleShape : public RendererShape
	{
	public:
		RendererCapsuleShape(Renderer &renderer, PxF32 halfHeight, PxF32 radius);
		
		// resize the capsule
		void setDimensions(PxF32 halfHeight, PxF32 radius0, PxF32 radius1);

		virtual ~RendererCapsuleShape(void);

	private:
		RendererVertexBuffer *m_vertexBuffer;
		RendererIndexBuffer  *m_indexBuffer;
	};

} // namespace SampleRenderer

#endif
