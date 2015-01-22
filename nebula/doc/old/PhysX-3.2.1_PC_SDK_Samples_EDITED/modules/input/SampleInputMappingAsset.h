


#ifndef SAMPLE_INPUT_MAPPING_ASSET_H
#define SAMPLE_INPUT_MAPPING_ASSET_H

#include <SampleUserInput.h>
#include <ODBlock.h>
#include "SampleAllocator.h"

class SampleInputMappingAsset : public SampleAllocateable
{	
public:
	SampleInputMappingAsset(FILE* file, const char *path, bool empty,PxU32 userInputCS, PxU32 inputEventCS);
	virtual ~SampleInputMappingAsset(void);
	
	virtual bool isOk(void) const;

	const SampleFramework::T_SampleInputData& getSampleInputData() const { return mSampleInputData; }

	void	addMapping(const char* uiName, const char* ieName);
	void	saveMappings();

private:
	void loadData(ODBlock* odsSettings);	
	bool createNewFile(bool rewriteFile);
	bool checksumCheck();

private:
	SampleFramework::T_SampleInputData mSampleInputData;
	ODBlock*			mSettingsBlock;
	ODBlock*			mMapping;
	FILE*				mFile;
	const char*			mPath;
	bool				mIsOk;
	PxU32				mUserInputCS;
	PxU32				mInputEventCS;
};	

#endif
