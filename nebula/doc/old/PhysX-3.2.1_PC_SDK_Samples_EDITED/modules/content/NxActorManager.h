class NxActor
{
    PxActor* m_PxActor;
}

class NxRigidActor: public NxActor
{
	RenderBaseActor*	m_RenderBaseActor;
}

class NxRigidBody: public NxRigidActor
{
    
}

class NxRigidStatic: public NxRigidActor
{
    
}

class NxRigidDynamic: public NxRigidBody
{
    
}

class NxArticulationLink: public NxRigidBody
{
    
}

class NxParticleBaseActor
{
	PxParticleBaseActor*		m_PxParticleBaseActor;
}

class NxParticleSystemActor
{
	RenderParticleSystemActor*	m_RenderParticleSystemActor;
}

class NxClothActor
{
	PxClothActor*		m_PxClothActor;
	
	RenderClothActor*	m_RenderClothActor;
}

class NxCharacterController
{
	PxController*			m_PxController;

	PxCapsuleControllerDesc	m_PxCapsuleControllerDesc;
	PxBoxControllerDesc		m_PxBoxControllerDesc;
}

class NxActorManager :
	public NxAllocatable
{
public:
	NxActorManager();
	~NxActorManager();

protected:
	// PhysX Objects //////////////////////////////////////////////////////////////////////////////
	

	// My Objects /////////////////////////////////////////////////////////////////////////////////
	

	

	// PhysX Objects //////////////////////////////////////////////////////////////////////////////
private:
	// other
	//std::vector<RenderBaseActor*>			mDeletedRenderActors;
	//std::vector<RenderBaseActor*>			mRenderActors;

	

	// My Objects /////////////////////////////////////////////////////////////////////////////////
	std::vector<NxActor*>	m_actors;
	//std::vector<NxParticleSystemActor*>		m_NxParticleSystemActor;
	//std::vector<NxClothActor*>				m_NxClothActor;
	//std::vector<NxCharacterController*>		m_NxCharacterController;
	

}
