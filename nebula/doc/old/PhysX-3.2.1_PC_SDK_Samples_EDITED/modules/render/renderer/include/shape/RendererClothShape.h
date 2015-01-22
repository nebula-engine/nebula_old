

#ifndef RENDERER_CLOTH_SHAPE_H
#define RENDERER_CLOTH_SHAPE_H

#include <RendererShape.h>

namespace SampleRenderer
{
	class RendererVertexBuffer;
	class RendererIndexBuffer;

	class RendererClothShape : public RendererShape
	{
	public:
		RendererClothShape(Renderer& renderer, 
			const PxVec3* verts, PxU32 numVerts,
			const PxVec3* normals,
			const PxReal* uvs,
			const PxU16* faces, PxU32 numFaces, bool flipWinding=false);
        
        void update(const PxVec3* verts, PxU32 numVerts, const PxVec3* normals);

		virtual ~RendererClothShape();

	private:
		RendererVertexBuffer*	m_vertexBuffer;
		RendererIndexBuffer*	m_indexBuffer;
	};

} // namespace SampleRenderer

#endif
