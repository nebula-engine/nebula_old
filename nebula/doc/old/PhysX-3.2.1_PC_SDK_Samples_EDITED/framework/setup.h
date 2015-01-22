struct SampleSetup
{
	SampleSetup() : 
		mName		(NULL),
		mWidth		(0),
		mHeight		(0),
		mFullscreen	(false)
	{
	}
	const char*	mName;
	PxU32		mWidth;
	PxU32		mHeight;
	bool		mFullscreen;
};