

#ifndef RENDER_MESH_ACTOR_H
#define RENDER_MESH_ACTOR_H

#include "RenderBaseActor.h"
#include "foundation/PxVec3.h"

namespace SampleRenderer
{
	class Renderer;
}

	class RenderMeshActor : public RenderBaseActor
	{
		public:
					RenderMeshActor(SampleRenderer::Renderer& renderer,
								const PxVec3* verts, PxU32 numVerts,
								const PxVec3* vertexNormals,
								const PxReal* uvs,
								const PxU16* faces, PxU32 numFaces, bool flipWinding=false
								);
					RenderMeshActor(const RenderMeshActor&);
		virtual		~RenderMeshActor();
	};

#endif