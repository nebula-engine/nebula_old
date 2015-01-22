

//
// RendererGridShape : convenience class for generating a grid mesh.
//
#ifndef RENDERER_GRID_SHAPE_H
#define RENDERER_GRID_SHAPE_H

#include <RendererShape.h>

namespace SampleRenderer
{

	class RendererVertexBuffer;
#ifdef RENDERER_PSP2
	class RendererIndexBuffer;
#endif

	class RendererGridShape : public RendererShape
	{
	public:
		enum UpAxis
		{
			UP_X,
			UP_Y,
			UP_Z,
		};

		RendererGridShape(Renderer &renderer, PxU32 size, float cellSize, bool showAxis = false, UpAxis = UP_Y);
		virtual ~RendererGridShape(void);

		UpAxis getUpAxis() { return m_UpAxis; }

	private:
		RendererVertexBuffer *m_vertexBuffer;
#ifdef RENDERER_PSP2
		RendererIndexBuffer  *m_indexBuffer;
#endif
		UpAxis                m_UpAxis;
	};

} // namespace SampleRenderer

#endif
