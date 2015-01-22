class SampleFunctionCreator : public PhysXSampleCreator
	{
	public:
		SampleFunctionCreator(FunctionCreator func) : mFunc(func) {}
		virtual PhysXSample* operator()(PhysXSampleApplication& app) const { return (*mFunc)(app); }
	private:
		FunctionCreator	mFunc;
	};