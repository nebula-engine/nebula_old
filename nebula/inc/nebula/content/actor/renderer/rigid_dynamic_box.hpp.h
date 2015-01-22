#ifndef _ACTOR_RENDERER_RIGID_DYNAMIC_BOX_H
#define _ACTOR_RENDERER_RIGID_DYNAMIC_BOX_H

#include <content/Actor/Renderer/CO_AC_RE_RigidDynamic.h>

namespace Content {
	namespace Actor {
		namespace Renderer {
			/// rigid dynamic box
			class RigidDynamicBox: public CO_AC_RE_RigidDynamic {
			public:
				/// Default Constructor
										RigidDynamicBox();
				/// destructor
										~RigidDynamicBox();
		



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
			private:
				/// x
						float			m_hx;
				/// y
						float			m_hy;
				/// z
						float			m_hz;
			};
		}
	}
}

#endif







