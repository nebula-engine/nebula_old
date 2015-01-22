
#ifndef TREE_H
#define TREE_H

#include "PhysXSampleApplication.h"
#include "SampleArray.h"


/**
Group of T. The groups can be linked to a tree.
If a T is added to a group, a leaf group is automatically created, that 
contains the T.
*/

template <class T> class TreeGroup
{
	static const size_t MAX_COUNT = ~0;
public:
	explicit		TreeGroup(const char* name, size_t count = MAX_COUNT);
	explicit		TreeGroup(TreeGroup* group);
					TreeGroup(SampleCreator test, const char* name, size_t count = MAX_COUNT);
					~TreeGroup();
	const char*		getName() const;
	void			getPathName(char* strBuffer, unsigned strBufferMaxLength, bool omitRoot);
	void			addGroup(TreeGroup* group);
	void			addLeaf(T leaf, const char* name, size_t count = MAX_COUNT);
	TreeGroup*		addPath(SampleArray<TreeGroup*>& path);
	TreeGroup*		deepCopy();
	void			getPath(SampleArray<TreeGroup*>& path);
	TreeGroup*		getGroupFromPathName(const char* pathName, bool omitRoot);
	TreeGroup*		getNextChild(TreeGroup& current);
	TreeGroup*		getPreviousChild(TreeGroup& current);
	TreeGroup*		getChildByName(const char* name, size_t count = MAX_COUNT);
	TreeGroup*		getFirstLeaf();
	TreeGroup*		getLastLeaf();
	TreeGroup*		getNextLeaf(TreeGroup* current);
	TreeGroup*		getPreviousLeaf(TreeGroup* current);
	SampleCreator	getCreator() { PX_ASSERT(isTest()); return mTest; }
	bool			isLeaf() const;
	PxU32			numChildren() const;

private:
	unsigned		getChildIndex(TreeGroup& child);
	TreeGroup*		getFirstLeaf();
	TreeGroup*		getLastLeaf();
	TreeGroup*		getNextLeaf(TreeGroup* current);
	TreeGroup*		getPreviousLeaf(TreeGroup* current);

private:
	
	T m_data;
	
	char* mName;
	SampleArray<TreeGroup*> mChildren;
	TreeGroup* mParent;
};

#endif // TEST_CATEGORY
