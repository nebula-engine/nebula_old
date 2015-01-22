
//
// RendererBoxShape : convenience class for generating a box mesh.
//
#ifndef RENDERER_BOX_SHAPE_H
#define RENDERER_BOX_SHAPE_H

#include <RendererShape.h>

namespace SampleRenderer
{

	class RendererVertexBuffer;
	class RendererIndexBuffer;

	class RendererBoxShape : public RendererShape
	{
	public:
		RendererBoxShape(Renderer &renderer, const PxVec3 &extents, const PxReal* uvs=NULL);
		virtual ~RendererBoxShape(void);

	private:
		RendererVertexBuffer *m_vertexBuffer;
		RendererIndexBuffer  *m_indexBuffer;
	};

} // namespace SampleRenderer

#endif
