class NxPhysics :
	public NxAllocatable
{
public:
	NxPhysics();
	~NxPhysics();

protected:
	///////////////
	// Functions //
	///////////////
	// PhysX //////////////////////////////////////////////////////////////////////////////////////
	PxFoundation*			getPxFoundation()			const { return m_PxFoundation; }
	PxProfileZoneManager*	getPxProfileZoneManager()	const { return m_PxProfileZoneManager; }
	PxPhysics*				getPxPhysics()				const { return m_PxPhysics; }
	PxCooking*				getPxCooking()				const { return m_PxCooking; }




	//Must be called when foundation is setup.
	virtual			physx::PxProfileZone*			getOrCreateProfileZone(PxFoundation& inFoundation);
	virtual			physx::PxProfileZone*			getOrCreateProfileZone(physx::PxProfileZoneManager& zoneManager);
	virtual			physx::PxProfileZone*			getPxProfileZone() const { return m_PxProfileZone; }
		
	//Must be called when foundation is setup.
	virtual			void							releaseProfileZone();
	virtual			physx::PxProfileEventSender*	getEventBuffer();



	
	// My stuff ///////////////////////////////////////////////////////////////////////////////////

	NxSceneManager*			getNxSceneManager()			const { return m_NxSceneManager; }
	NxMaterialManager*		getNxVehicleManager()		const { return m_NxVehicleManager; }
	NxActorManager*			getNxActorManager()			const { return m_NxActorManager; }
	NxVehicleManager*		getNxVehicleManager()		const { return m_NxVehicleManager; }

	///////////////
	// Variables //
	///////////////

	// PhysX Objects //////////////////////////////////////////////////////////////////////////////
	PxFoundation*				m_PxFoundation;
	PxPhysics*					m_PxPhysics;
	PxCooking*					m_PxCooking;
	PxControllerManager*		m_PxControllerManager;

	PxDefaultCpuDispatcher*		m_CpuDispatcher;
#ifdef PX_PS3
	PxDefaultSpuDispatcher*		m_SpuDispatcher;
#endif
#ifdef PX_WINDOWS
	pxtask::CudaContextManager*	m_CudaContextManager;
#endif

	PxProfileZoneManager*		m_PxProfileZoneManager;
	PxProfileZone*				m_PxProfileZone;



	PxActiveTransform*			mBufferedActiveTransforms;
	std::vector<PxActor*>		mDeletedActors;
	PxU32						mActiveTransformCount;
	PxU32						mActiveTransformCapacity;


	// My Objects /////////////////////////////////////////////////////////////////////////////////
	NxSceneManager*		m_NxSceneManager;
	NxMaterialManager*	m_NxMaterialManager;
	NxGeometryManager*	m_NxGeometryManager;

	NxActorManager*		m_NxActorManager;
	NxVehicleManager*	m_NxVehicleManager;

}
