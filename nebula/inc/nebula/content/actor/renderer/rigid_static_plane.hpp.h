#ifndef _ACTOR_RENDERER_RIGID_STATIC_PLANE_H
#define _ACTOR_RENDERER_RIGID_STATIC_PLANE_H

#include <utilities/Types/Utilities.h>

#include <content/Actor/Renderer/CO_AC_RE_RigidStatic.h>



namespace Content {
	namespace Actor {
		namespace Renderer {
			/// rigid static plane
			class RigidStaticPlane: public CO_AC_RE_RigidStatic {
			public:
				/// constructor
								RigidStaticPlane();
				/// destructor
								~RigidStaticPlane();



				///@name Standard Functions
				///@{
				/// Initialize
				virtual	void									VInit( Void* data );
				/// ShutDown
				virtual	void									VShutDown();
				/// Update
				virtual	void									VUpdate();
				/// Render
				virtual	void									VRender( Void* );
				/// Step
				virtual	void									VStep( Void* );
				///@}


			protected:
		
			};
		}
	}
}

#endif





