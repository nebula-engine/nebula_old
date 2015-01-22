



#ifndef PICKING_H
#define PICKING_H

#include "foundation/PxSimpleTypes.h"
#include "foundation/PxVec3.h"
#include "extensions/PxD6Joint.h"
#include "extensions/PxSphericalJoint.h"

#include "PhysXSample.h"
#include "RendererProjection.h"

#define USE_D6_JOINT_FOR_MOUSE 0			// PT: please keep it 0 for interactive tests where one needs to rotate objects
#define USE_SPHERICAL_JOINT_FOR_MOUSE 0
//#define VISUALIZE_PICKING_RAYS

namespace physx
{
	class PxActor;
	class PxDistanceJoint;
	class PxRigidDynamic;
	class PxScene;
	class PxPhysics;
}

namespace physx {

	
	
	class Picking 
	{
	public:
													Picking(PhysXSample& frame);
													~Picking();

		PX_FORCE_INLINE	void						lazyPick() { pick(mMouseScreenX, mMouseScreenY); }
						bool						isPicked() const;
						bool						pick(int x, int y);
						void						moveCursor(PxI32 x, PxI32 y);
//						void						moveCursor(PxReal deltaDepth);

						void						computeCameraRay(PxVec3& orig, PxVec3& dir, PxI32 x, PxI32 y)	const;

		// returns picked actor
						PxActor*					letGo();
						void						tick();

#ifdef VISUALIZE_PICKING_RAYS
						
	PX_FORCE_INLINE		const						std::vector<Ray>&		getRays()	const	{ return mRays;	}
#endif

	private:
						void						grabActor(const PxVec3& worldImpact, const PxVec3& rayOrigin);
						void						moveActor(int x, int y);
						PxVec3						unProject( int x, int y, float depth)	const;
						void						project(const physx::PxVec3& v, int& xi, int& yi, float& depth)	const;

						PxActor*					mSelectedActor;

#if USE_D6_JOINT_FOR_MOUSE
						PxD6Joint*					mMouseJoint; // was PxDistanceJoint, PxSphericalJoint, PxD6Joint
#elif USE_SPHERICAL_JOINT_FOR_MOUSE
						PxSphericalJoint*			mMouseJoint;
#else
						PxDistanceJoint*			mMouseJoint;
#endif
						PxRigidDynamic*				mMouseActor;
						PxRigidDynamic*				mMouseActorToDelete;
						PxReal						mDistanceToPicked;
						int							mMouseScreenX, mMouseScreenY;
						PhysXSample&				mFrame;
#ifdef VISUALIZE_PICKING_RAYS
						std::vector<Ray>			mRays;
#endif
	};
}

#endif