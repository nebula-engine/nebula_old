static void gNearPlane(Console* console, const char* text, void* userData)
{
	if(!text)
	{
		console->out("Usage: nearplane <float>");
		return;
	}

	const float val = (float)::atof(text);
	gBase->getCamera().setNearPlane(val);
}

static void gFarPlane(Console* console, const char* text, void* userData)
{
	if(!text)
	{
		console->out("Usage: farplane <float>");
		return;
	}

	const float val = (float)::atof(text);
	gBase->getCamera().setFarPlane(val);
}

static void gDrawCameraDebug(Console* console, const char* text, void* userData)
{
	gBase->getCamera().mDrawDebugData = !gBase->getCamera().mDrawDebugData;
}

static void gFreezeFrustum(Console* console, const char* text, void* userData)
{
	gBase->getCamera().mFreezeFrustum = !gBase->getCamera().mFreezeFrustum;
}

static void gVFC(Console* console, const char* text, void* userData)
{
	gBase->getCamera().mPerformVFC = !gBase->getCamera().mPerformVFC;
}

const char* getSampleMediaFilename(const char* filename)
{
	return findPath(filename);
}

const char*				getSampleMediaFilename(const char* filename);
PxToolkit::BasicRandom& getSampleRandom();
PxErrorCallback&		getSampleErrorCallback();


static PxMat44 convertViewMatrix(const PxTransform& eye)
{
	PxTransform viewMatrix = eye.getInverse();
	PxMat44 mat44 = PxMat44(viewMatrix).getTranspose();

	float m[16];
	memcpy(m, mat44.front(), sizeof m);

	PxMat44 view44;
	view44.column0.x = m[0];
	view44.column0.y = m[1];
	view44.column0.z = m[2];
	view44.column0.w = m[3];
	view44.column1.x = m[4];
	view44.column1.y = m[5];
	view44.column1.z = m[6];
	view44.column1.w = m[7];
	view44.column2.x = m[8];
	view44.column2.y = m[9];
	view44.column2.z = m[10];
	view44.column2.w = m[11];
	view44.column3.x = m[12];
	view44.column3.y = m[13];
	view44.column3.z = m[14];
	view44.column3.w = m[15];

PxMat44 tmpmat = view44.getTranspose(); view44 = tmpmat;

	return view44;
}

static PxMat44 convertProjMatrix(const RendererProjection& proj)
{
	float renderProjMatrix[16];
	proj.getColumnMajor44(renderProjMatrix);

	PxMat44 proj44;
	proj44.column0.x = renderProjMatrix[0];
	proj44.column0.y = renderProjMatrix[1];
	proj44.column0.z = renderProjMatrix[2];
	proj44.column0.w = renderProjMatrix[3];
	proj44.column1.x = renderProjMatrix[4];
	proj44.column1.y = renderProjMatrix[5];
	proj44.column1.z = renderProjMatrix[6];
	proj44.column1.w = renderProjMatrix[7];
	proj44.column2.x = renderProjMatrix[8];
	proj44.column2.y = renderProjMatrix[9];
	proj44.column2.z = renderProjMatrix[10];
	proj44.column2.w = renderProjMatrix[11];
	proj44.column3.x = renderProjMatrix[12];
	proj44.column3.y = renderProjMatrix[13];
	proj44.column3.z = renderProjMatrix[14];
	proj44.column3.w = renderProjMatrix[15];

//PxMat44 tmpmat = proj44.getTranspose(); proj44 = tmpmat;

	return proj44;
}


static PX_FORCE_INLINE bool planesAABBOverlap(const PxBounds3& a, const PxPlane* p, PxU32& out_clip_mask, PxU32 in_clip_mask)
{
	/*
	Convert the AABB from (minimum,maximum) form into (center,half-diagonal).
	Note that we could get rid of these six subtractions and three
	multiplications if the AABB was originally expressed in (center,
	half-diagonal) form.
	*/

	PxVec3 m = a.getCenter();		// get center of AABB ((minimum+maximum)*0.5f)
	PxVec3 d = a.maximum - m;		// get positive half-diagonal (maximum - center)

	/*
	Evaluate through all active frustum planes. We determine the relation 
	between the AABB and a plane by using the concept of "near" and "far"
	vertices originally described by Zhang (and later by Moeller). Our
	variant here uses 3 fabs ops, 6 muls, 7 adds and two floating point
	comparisons per plane. The routine early-exits if the AABB is found
	to be outside any of the planes. The loop also constructs a new output
	clip mask. Most FPUs have a native single-cycle fabsf() operation.
	*/

	PxU32 Mask				= 1;			// current mask index (1,2,4,8,..)
	PxU32 TmpOutClipMask	= 0;			// initialize output clip mask into empty. 

	while(Mask<=in_clip_mask)				// keep looping while we have active planes left...
	{
		if(in_clip_mask & Mask)				// if clip plane is active, process it..
		{               
			const float NP = d.x*PxAbs(p->n.x) + d.y*PxAbs(p->n.y) + d.z*PxAbs(p->n.z);
			const float MP = m.x*p->n.x + m.y*p->n.y + m.z*p->n.z + p->d;

			if(NP < MP)						// near vertex behind the clip plane... 
				return false;				// .. so there is no intersection..
			if((-NP) < MP)					// near and far vertices on different sides of plane..
				TmpOutClipMask |= Mask;		// .. so update the clip mask...
		}
		Mask+=Mask;							// mk = (1<<plane)
		p++;								// advance to next plane
	}

	out_clip_mask = TmpOutClipMask;			// copy output value (temp used to resolve aliasing!)
	return true;							// indicate that AABB intersects frustum
}

static PxMat33 EulerToMat33(const PxVec3 &e)
{
	float c1 = cosf(e.z);
	float s1 = sinf(e.z);
	float c2 = cosf(e.y);
	float s2 = sinf(e.y);
	float c3 = cosf(e.x);
	float s3 = sinf(e.x);
	PxMat33 m(PxVec3(c1*c2,              -s1*c2,             s2),
			  PxVec3((s1*c3)+(c1*s2*s3), (c1*c3)-(s1*s2*s3),-c2*s3),
			  PxVec3((s1*s3)-(c1*s2*c3), (c1*s3)+(s1*s2*c3), c2*c3));

	return m;
}