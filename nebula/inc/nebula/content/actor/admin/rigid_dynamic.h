#ifndef _ACTOR_ADMIN_RIGID_DYNAMIC_H
#define _ACTOR_ADMIN_RIGID_DYNAMIC_H

#include <content/Actor/Admin/CO_AC_AD_RigidBody.h>
#include <content/Actor/Admin/CO_AC_AD_RigidDynamic_PP.h>

namespace Content {
	namespace Actor {
		namespace Admin {
			/// rigid dynamic
			class RigidDynamic:
				public CO_AC_AD_RigidBody,
				public CO_AC_AD_RigidDynamic_PP,
				public CO_AC_AD_RigidDynamic_CP
			{
			public:
				///@name Ctor and Dtor
				///@{
				// Ctor
								RigidDynamic();
				/// Copy Ctor
								RigidDynamic(const RigidDynamic&);
				/// Dtor
								~RigidDynamic();
				///@}

				///@name Operator
				///@{
				/// Assignment
				RigidDynamic&	operator=(const RigidDynamic&);
				///@}

				///@name Standard
				///@{
				/// Init
				virtual	void	VInit(Void*);
				/// Shutdown
				virtual	void	VShutdown(Void*);
				/// Update
				virtual	void	VUpdate(Void*);
				/// Step
				virtual	void	VStep(Void*);
				/// Render
				virtual	void	VRender(Void*);
				///@}



				/// create shapes
				virtual	void	VCreateShape();
			};
		};
	};
};

#endif








