#ifndef _ACTOR_ADMIN_RIGID_STATIC_H
#define _ACTOR_ADMIN_RIGID_STATIC_H

#include <content/Actor/Admin/CO_AC_AD_RigidActor.h>

/*
namespace physx {
	class PxRigidStatic;
}
*/

namespace Content {
	namespace Actor {
		namespace Admin {
			/// rigid static
			class RigidStatic: public RigidActor {
			public:
				/// Constructor
								RigidStatic();
				/// Destructor
								~RigidStatic();

				///@name Standard Functions
				///@{
				/// Initialize
				virtual	void	VInit( Void* data );
				/// ShutDown
				virtual	void	VShutDown();
				/// Update
				virtual	void	VUpdate();
				/// Render
				virtual	void	VRender( Void* data );
				/// Step
				virtual	void	VStep( Void* );
				///@}




				/// Create Shapes
				virtual	void	VCreateShape();


			protected:
			};
		};
	};
};

#endif





