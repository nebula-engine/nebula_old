



#define SAMPLE_CREATOR(className)	create##className
#define SAMPLE_STARTER(className)	className##Starter

#define SAMPLE_CREATOR_VAR(className)	g##className##creator

#define REGISTER_SAMPLE(className, fullPath)													\
	static PhysXSample* SAMPLE_CREATOR(className)(PhysXSampleApplication& app) {				\
		return SAMPLE_NEW(className)(app);														\
	}																							\
	static SampleFunctionCreator SAMPLE_CREATOR_VAR(className)(SAMPLE_CREATOR(className));		\
	struct SAMPLE_STARTER(className) {															\
		SAMPLE_STARTER(className)() {															\
			PhysXSampleApplication::registerSample(&SAMPLE_CREATOR_VAR(className), fullPath);	\
		}																						\
	} g##className##Starter;