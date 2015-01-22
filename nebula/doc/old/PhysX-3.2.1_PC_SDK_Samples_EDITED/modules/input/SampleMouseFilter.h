

#ifndef SAMPLE_MOUSE_FILTER_H
#define SAMPLE_MOUSE_FILTER_H

#include "Allocateable.h"

	class MouseFilter : public Allocateable
	{
		public:
								MouseFilter();
								MouseFilter(PxU32 length, PxReal weightModifier);
								~MouseFilter();

		PX_INLINE	PxReal		GetWeightModifier()						const	{ return mWeightModifier;		}
		PX_INLINE	void		SetWeightModifier(PxReal modifier)				{ mWeightModifier = modifier;	}

		PX_INLINE	PxU32		GetHistoryBufferLength()				const	{ return mHistoryBufferLength;	}
					bool		SetHistoryBufferLength(PxU32 length);

					void		Apply(PxReal& deltaMouseX, PxReal& deltaMouseY);
		private:
					PxReal		mWeightModifier;
					PxU32		mHistoryBufferLength;
					PxReal*		mHistoryBufferX;
					PxReal*		mHistoryBufferY;
	};

#endif

