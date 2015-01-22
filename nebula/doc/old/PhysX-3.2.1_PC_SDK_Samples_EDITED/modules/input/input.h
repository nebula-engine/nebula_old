
class NxInput: public InputEventListener
{
public:
	virtual		void	onAnalogInputEvent(const InputEvent& , float val);
	virtual		bool	onDigitalInputEvent(const InputEvent& , bool val);
	virtual		void	onPointerInputEvent(const InputEvent&, physx::PxU32 x, physx::PxU32 y, physx::PxReal dx, physx::PxReal dy, bool val);
	virtual		void	doInput(void);
private:
				PxI32	m_camMoveButton;
}
