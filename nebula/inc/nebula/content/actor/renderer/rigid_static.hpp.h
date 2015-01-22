#ifndef _ACTOR_RENDERER_RIGID_STATIC_H
#define _ACTOR_RENDERER_RIGID_STATIC_H

#include <content/Actor/Renderer/CO_AC_RE_RigidActor.h>


namespace Content {
	namespace Actor {
		namespace Renderer {
			/// rigid static
			class RigidStatic: public CO_AC_RE_RigidActor {
			public:
				/// constructor
								RigidStatic();
				/// destructor
								~RigidStatic();



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





