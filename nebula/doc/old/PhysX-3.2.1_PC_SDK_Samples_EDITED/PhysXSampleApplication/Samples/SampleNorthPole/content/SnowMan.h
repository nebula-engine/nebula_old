

struct SnowMan
{
	PxShape* eyeL;
	PxShape* eyeR;

	void changeMood()
	{
		PxTransform tl = eyeL->getLocalPose();
		PxTransform tr = eyeR->getLocalPose();

		eyeL->setLocalPose(PxTransform(tl.p,tr.q));
		eyeR->setLocalPose(PxTransform(tr.p,tl.q));
	}
};