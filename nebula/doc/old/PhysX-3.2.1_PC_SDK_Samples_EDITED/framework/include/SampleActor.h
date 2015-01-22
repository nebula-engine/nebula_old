
#ifndef SAMPLE_ACTOR_H
#define SAMPLE_ACTOR_H

#include "FrameworkFoundation.h"

namespace SampleFramework
{

	class SampleActor
	{
	public:
							SampleActor(void) {}
		virtual				~SampleActor(void) {}

					void	release(void) { delete this; }

	public:
		virtual		void	tick(float dtime, bool rewriteBuffers = false) {}
		virtual		void	render(bool rewriteBuffers = false) {}
		virtual		int		getType() { return -1; }
	};

} // namespace SampleFramework

#endif


//virtual void render(const PxMat44 &eyeT) { render(); } // TODO: provide a version of render() with some info about the current scene like camera pose...