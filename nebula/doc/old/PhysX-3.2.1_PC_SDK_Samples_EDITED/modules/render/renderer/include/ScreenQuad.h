class ScreenQuad
{
	public:
					ScreenQuad();

	RendererColor	mLeftUpColor;		//!< Color for left-up vertex
	RendererColor	mLeftDownColor;		//!< Color for left-down vertex
	RendererColor	mRightUpColor;		//!< Color for right-up vertex
	RendererColor	mRightDownColor;	//!< Color for right-down vertex
	PxReal			mAlpha;				//!< Alpha value
	PxReal			mX0, mY0;			//!< Up-left coordinates
	PxReal			mX1, mY1;			//!< Bottom-right coordinates
};