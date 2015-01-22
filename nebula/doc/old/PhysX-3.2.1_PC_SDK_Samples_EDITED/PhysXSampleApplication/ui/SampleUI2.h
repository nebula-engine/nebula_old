
namespace SampleUI
{
    class Tests: public NxUI
    {
        m_selected;
        
        void render();
        
    }
    
    class TestBase: public NxUI
    {
        
        
        
        void render();
    }
    
    class Visualizations: public NxUI
    {

        
        void render();
		void handleSettingMenuKey(MenuKey::Enum menuKey);
    }
}

namespace SampleUI
{
    void Tests::render()
    {
        const PxReal scale = 0.5f;
    	const PxReal shadowOffset = 6.0f;

		Renderer* renderer = getRenderer();
		PxU32 x = 10;
		PxU32 y = (PxU32)(-8);
		const PxU32 yInc = 18;

		char strbuf[512] = "";
        
        
        
        
        if (mMenuExpand)
    	{

			const RendererColor textColor(255, 255, 255, 255);
			const RendererColor highlightTextColor(255, 255, 0, 255);
			
			
			Test::TestGroup* parent = mSelected->getParent();
			parent->getPathName(strbuf, sizeof strbuf - 1, true);
			renderer->print(x, y += yInc, strbuf, scale, shadowOffset, textColor);

			for (Test::TestGroup* child = parent->getFirstChild(); child != NULL; child = parent->getNextChild(*child))
			{
				sprintf(strbuf, "%s%s", child->getName(), child->isTest() ? "" : "/...");
				renderer->print(30, y += yInc, strbuf, scale, shadowOffset, (mSelected == child ? highlightTextColor : textColor));
			}
			
			
            //print minimal information
    		{
				y += yInc;

				const RendererColor textColor(255, 255, 255, 255);
				const char* msg;

				msg = inputInfoMsg("Press "," for help", SHOW_HELP, -1);
				if(msg)
					renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);

				y+=yInc;
			}
            if (mTextAlphaHelp != 0.0f)
    		{
				const RendererColor textColor(255, 255, 255, PxU32(mTextAlphaHelp*255.0f));
				const char* msg;

				if(m_platform->getSampleUserInput()->keyboardSupported() && m_platform->getSampleUserInput()->gamepadSupported()) 
					renderer->print(x, y += yInc, "Use arrow keys or D-Pad to navigate between the items", scale, shadowOffset, textColor);
				else
				{
					if(m_platform->getSampleUserInput()->keyboardSupported())
					{
						renderer->print(x, y += yInc, "Use arrow keys to navigate between the items", scale, shadowOffset, textColor);
					}
					else
					{
						if(m_platform->getSampleUserInput()->gamepadSupported())
						{
							renderer->print(x, y += yInc, "Use D-Pad to navigate between the items", scale, shadowOffset, textColor);
						}
					}
				}
				msg = inputInfoMsg("Press "," to run the selected sample", MENU_SELECT, -1);
				if(msg)
					renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);
				msg = inputInfoMsg("Press "," to exit sample selector", MENU_ESCAPE, -1);
				if(msg)
					renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);
			}
    	}
        
        
    }
    
    void TestBase::render()
    {
        const PxReal scale = 0.5f;
    	const PxReal shadowOffset = 6.0f;

		Renderer* renderer = getRenderer();
		PxU32 x = 10;
		PxU32 y = (PxU32)(-8);
		const PxU32 yInc = 18;

		char strbuf[512] = "";
        
        
        
        
        const RendererColor highlightTextColor(255, 255, 0, 255);
        const RendererColor textColor(255, 255, 255, 255);
        const RendererColor textAlphaHelpColor(255, 255, 255, PxU8(mTextAlphaHelp*255.0f));
        
        //print minimal information
		mRunning->getPathName(strbuf, sizeof strbuf - 1, true);
		if (mPause) strncat(strbuf, "  <PAUSED>", 10);
		renderer->print(x, y += yInc, strbuf, scale, shadowOffset, highlightTextColor);
		y += yInc;
        
		const char* msg;
        
		msg = inputInfoMsg("Press "," for description", SHOW_DESCRIPTION, -1);
		if(msg)
			renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);

		msg = inputInfoMsg("Press "," for help", SHOW_HELP, -1);
		if(msg)
			renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);

		y += yInc;
		
        // print description
		mSample->descriptionRender(x, y+=yInc, PxU8(mTextAlphaDesc*255.0f));
		
		//print help
		if (mTextAlphaHelp != 0.0f)
		{
			//print common help
			
			const char* msg;

			msg = inputInfoMsg("Press "," to enter sample selector", MENU_SAMPLES, -1);
			if(msg)
				renderer->print(x, y += yInc, msg, scale, shadowOffset, textAlphaHelpColor);

			msg = inputInfoMsg("Press "," to quit", QUIT,-1);
			if(msg)
				renderer->print(x, y += yInc, msg, scale, shadowOffset, textAlphaHelpColor);

			//print sample specific help
			mSample->helpRender(x, y += yInc, PxU8(mTextAlphaHelp*255.0f));
		}
    }
    
	void Visualixation::Visualizations()
	{
		mMenuVisualizations.push_back(MenuTogglableItem(PxVisualizationParameter::eSCALE, "Debug Rendering"));

		#define MENU_PUSHV(var, title) mMenuVisualizations.push_back( MenuTogglableItem(PxVisualizationParameter::var, title))

		MENU_PUSHV(eCOLLISION_AABBS,					"Collision AABBs"				);
		MENU_PUSHV(eCOLLISION_SHAPES,					"Collision Shapes"				);
		MENU_PUSHV(eCOLLISION_AXES,						"Collision Axes"				);
		MENU_PUSHV(eCOLLISION_FNORMALS,					"Collision Face Normals"		);
		MENU_PUSHV(eCOLLISION_EDGES,					"Collision Edges"				);
		MENU_PUSHV(eCOLLISION_PAIRS,					"Collision Pairs"				);
		MENU_PUSHV(eCONTACT_POINT,						"Contact Points"				);
		MENU_PUSHV(eCONTACT_NORMAL,						"Contact Normals"				);
		MENU_PUSHV(eCONTACT_ERROR,						"Contact Error"					);
		MENU_PUSHV(eCONTACT_FORCE,						"Contact Force"					);
		MENU_PUSHV(eACTOR_AXES,							"Actor Axes"					);
		MENU_PUSHV(eBODY_AXES,							"Body Axes"						);
		MENU_PUSHV(eBODY_LIN_VELOCITY,					"Linear Velocity"				);
		MENU_PUSHV(eBODY_ANG_VELOCITY,					"Angular Velocity"				);
		MENU_PUSHV(eBODY_MASS_AXES,						"Mass Axes"						);
		MENU_PUSHV(eJOINT_LIMITS,						"Joint Limits"					);
		MENU_PUSHV(eJOINT_LOCAL_FRAMES,					"Joint Local Frames"			);
		MENU_PUSHV(ePARTICLE_SYSTEM_BROADPHASE_BOUNDS,	"Particle System Broadphase Bounds");
		MENU_PUSHV(ePARTICLE_SYSTEM_GRID,				"Particle System Grid"			);	
		MENU_PUSHV(ePARTICLE_SYSTEM_BOUNDS,				"Particle System Bounds"		);	
		MENU_PUSHV(ePARTICLE_SYSTEM_COLLISION_NORMAL,	"Particle System Collision Normals");		
		MENU_PUSHV(eCULL_BOX,							"Culling Box"					);
		MENU_PUSHV(eCOLLISION_STATIC,					"Static pruning structure"		);
		MENU_PUSHV(eCOLLISION_DYNAMIC,					"Dynamic pruning structure"		);
		MENU_PUSHV(eCOLLISION_COMPOUNDS,				"Compounds"						);
	}

    void Visualizations::render()
    {
        const PxReal scale = 0.5f;
    	const PxReal shadowOffset = 6.0f;

		Renderer* renderer = getRenderer();
		PxU32 x = 10;
		PxU32 y = (PxU32)(-8);
		const PxU32 yInc = 18;

		char strbuf[512] = "";
        
        
        
        
        const RendererColor color(0, 90, 90);
		ScreenQuad sq;
		sq.mLeftUpColor		= color;
		sq.mRightUpColor	= color;
		sq.mLeftDownColor	= color;
		sq.mRightDownColor	= color;
		sq.mAlpha			= 0.75;

		getRenderer()->drawScreenQuad(sq);

		for (PxU32 i = 0; i < mMenuVisualizations.size(); i++)
		{
			bool selected = mMenuVisualizationsIndexSelected == i;
			sprintf(strbuf, "%d  (%s)  %s", i+1, mMenuVisualizations[i].toggleState ? "ON " : "OFF", mMenuVisualizations[i].name);
			renderer->print(30, y += yInc, strbuf, scale, shadowOffset, (selected ? highlightTextColor : textColor));
		}
        
        //print minimal information
		{
			y += yInc;

			const RendererColor textColor(255, 255, 255, 255);
			const char* msg;

			msg = inputInfoMsg("Press "," for help", SHOW_HELP, -1);
			if(msg)
				renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);

			y+=yInc;
		}

		if (mTextAlphaHelp != 0.0f)
		{
			const RendererColor textColor(255, 255, 255, PxU32(mTextAlphaHelp*255.0f));
			const char* msg;

			if(m_platform->getSampleUserInput()->keyboardSupported() && m_platform->getSampleUserInput()->gamepadSupported()) 
				renderer->print(x, y += yInc, "Use arrow keys or D-Pad to navigate between the items", scale, shadowOffset, textColor);
			else
			{
				if(m_platform->getSampleUserInput()->keyboardSupported())
				{
					renderer->print(x, y += yInc, "Use arrow keys to navigate between the items", scale, shadowOffset, textColor);
				}
				else
				{
					if(m_platform->getSampleUserInput()->gamepadSupported())
					{
						renderer->print(x, y += yInc, "Use D-Pad to navigate between the items", scale, shadowOffset, textColor);
					}
				}
			}
			msg = inputInfoMsg("Press "," to run the selected sample", MENU_SELECT, -1);
			if(msg)
				renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);
			msg = inputInfoMsg("Press "," to exit sample selector", MENU_ESCAPE, -1);
			if(msg)
				renderer->print(x, y += yInc, msg, scale, shadowOffset, textColor);
		}
        
    }
    
    void Visualizations::handleSettingMenuKey(MenuKey::Enum menuKey)
    {
        size_t numEntries = mMenuVisualizations.size();
    	switch(menuKey)
    	{
    	case MenuKey::NAVI_LEFT:
    	case MenuKey::NAVI_UP:
    		mMenuVisualizationsIndexSelected = (mMenuVisualizationsIndexSelected > 0) ? (mMenuVisualizationsIndexSelected - 1) : numEntries - 1;
    		break;
    	case MenuKey::NAVI_RIGHT:
    	case MenuKey::NAVI_DOWN:
    		mMenuVisualizationsIndexSelected = (mMenuVisualizationsIndexSelected + 1) % numEntries;
    		break;
    	case MenuKey::SELECT:
    		{
    			MenuTogglableItem& togglableItem = mMenuVisualizations[mMenuVisualizationsIndexSelected];
    			PxU32 menuVisIndex = togglableItem.toggleCommand;
    			PX_ASSERT(menuVisIndex < PxVisualizationParameter::eNUM_VALUES);
    			PxScene& scene = mSample->getActiveScene();
    			PxVisualizationParameter::Enum p = static_cast<PxVisualizationParameter::Enum>(menuVisIndex);
    			scene.setVisualizationParameter(p, 1.0f - scene.getVisualizationParameter(p));
    			
    			bool enabled = scene.getVisualizationParameter(p) != 0.0f;
    			if(enabled && scene.getVisualizationParameter(PxVisualizationParameter::eSCALE) == 0.0f)
    			{
    				toggleDebugRenderer();
    			}					
    			togglableItem.toggleState = scene.getVisualizationParameter(p) != 0.0f;
    		}
    		break;
    	default:
    		break;			
    	}
    }
    
    bool Tests::onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val)
    {
        MenuKey::Enum menuKey = MenuKey::NONE;

        switch (ie.m_Id)
    	{
    	case RUN_NEXT_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_DOWN);
    				handleMenuKey(MenuKey::SELECT);
    			}
    		}
    		break;
    	case RUN_PREVIOUS_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_UP);
    				if(!handleMenuKey(MenuKey::SELECT))
    					return false;
    			}
    		}
    		break;
    	default:
    			break;
    	}
    
    	if(val)
    	{		
    		switch (ie.m_Id)
    		{
    		case MENU_UP:			if(mMenuExpand)		menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_DOWN:			if(mMenuExpand)		menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_LEFT:			if(mMenuExpand)		menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_RIGHT:		if(mMenuExpand)		menuKey = MenuKey::NAVI_RIGHT;	break;
    		case MENU_SELECT:		if(mMenuExpand)		menuKey = MenuKey::SELECT;		break;		
    		case MENU_ESCAPE:		if(mMenuExpand)		menuKey = MenuKey::ESCAPE;		break;		
    		case MENU_QUICK_UP:		mMenuExpand = true; menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_QUICK_DOWN:	mMenuExpand = true; menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_QUICK_LEFT:	mMenuExpand = true; menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_QUICK_RIGHT:	mMenuExpand = true; menuKey = MenuKey::NAVI_RIGHT;	break;
    		}
    
    		
			if (ie.m_Id == SHOW_HELP)
			{
				mShowHelp = !mShowHelp;
				return true;
			}
            
			
			if(!handleMenuKey(menuKey))
				return false;
			
			return true;
    		
    	}
    	else
    	{
    		if (mMenuExpand)
    		{
    			if (MENU_ESCAPE == ie.m_Id)
    			{
    				mMenuExpand = false;
    				mMenuType = MenuType::TESTS;
    			}
    		}
    		if(mSample)
    		{
    			if(!mSample->onDigitalInputEvent(ie,val))
    				return false;
    		}
    	}
        
    	return true;
    }
    
    bool TestBase::onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val)
    {
        MenuKey::Enum menuKey = MenuKey::NONE;

        switch (ie.m_Id)
    	{
    	case RUN_NEXT_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_DOWN);
    				handleMenuKey(MenuKey::SELECT);
    			}
    		}
    		break;
    	case RUN_PREVIOUS_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_UP);
    				if(!handleMenuKey(MenuKey::SELECT))
    					return false;
    			}
    		}
    		break;
    	default:
    			break;
    	}
    
    	if(val)
    	{		
    		switch (ie.m_Id)
    		{
    		case MENU_UP:			if(mMenuExpand)		menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_DOWN:			if(mMenuExpand)		menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_LEFT:			if(mMenuExpand)		menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_RIGHT:		if(mMenuExpand)		menuKey = MenuKey::NAVI_RIGHT;	break;
    		case MENU_SELECT:		if(mMenuExpand)		menuKey = MenuKey::SELECT;		break;		
    		case MENU_ESCAPE:		if(mMenuExpand)		menuKey = MenuKey::ESCAPE;		break;		
    		case MENU_QUICK_UP:		mMenuExpand = true; menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_QUICK_DOWN:	mMenuExpand = true; menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_QUICK_LEFT:	mMenuExpand = true; menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_QUICK_RIGHT:	mMenuExpand = true; menuKey = MenuKey::NAVI_RIGHT;	break;
    		}
            
    		switch (ie.m_Id)
    		{
    		case SHOW_EXTENDED_HELP:
    			{	
    				mShowExtendedHelp = !mShowExtendedHelp;
    				if(mSample)
    					mSample->resetExtendedHelpText();
    				break;
    			}
    		case MENU_VISUALIZATIONS:
    			{
    				mShowExtendedHelp = false;
    				mMenuExpand = true;	
    				mMenuType = MenuType::VISUALIZATIONS;
    				break;
    			}		
    		case MENU_SAMPLES:
    			{
    				mShowExtendedHelp = false;
    				mMenuExpand = true;	
    				mMenuType = MenuType::TESTS;
    				break;
    			}
    		case QUIT:
    			{
    				requestToClose();
    				break;
    			}
    		default:
    			if(NULL != mSample)
    			{
    				if(!mSample->onDigitalInputEvent(ie,val))
    					return false;
    			}
    			break;
    		}
    	}
    	else
    	{
    		
    		if(mSample)
    		{
    			if(!mSample->onDigitalInputEvent(ie,val))
    				return false;
    		}
    	}
        
    	return true;
    }
    
	bool Visualizations::onDigitalInputEvent(const SampleFramework::InputEvent& ie, bool val)
    {
        MenuKey::Enum menuKey = MenuKey::NONE;

        switch (ie.m_Id)
    	{
    	case RUN_NEXT_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_DOWN);
    				handleMenuKey(MenuKey::SELECT);
    			}
    		}
    		break;
    	case RUN_PREVIOUS_SAMPLE:
    		{
    			if(val)
    			{
    				handleMenuKey(MenuKey::NAVI_UP);
    				if(!handleMenuKey(MenuKey::SELECT))
    					return false;
    			}
    		}
    		break;
    	default:
    			break;
    	}
    
    	if(val)
    	{		
    		switch (ie.m_Id)
    		{
    		case MENU_UP:			if(mMenuExpand)		menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_DOWN:			if(mMenuExpand)		menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_LEFT:			if(mMenuExpand)		menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_RIGHT:		if(mMenuExpand)		menuKey = MenuKey::NAVI_RIGHT;	break;
    		case MENU_SELECT:		if(mMenuExpand)		menuKey = MenuKey::SELECT;		break;		
    		case MENU_ESCAPE:		if(mMenuExpand)		menuKey = MenuKey::ESCAPE;		break;		
    		case MENU_QUICK_UP:		mMenuExpand = true; menuKey = MenuKey::NAVI_UP;		break;
    		case MENU_QUICK_DOWN:	mMenuExpand = true; menuKey = MenuKey::NAVI_DOWN;	break;
    		case MENU_QUICK_LEFT:	mMenuExpand = true; menuKey = MenuKey::NAVI_LEFT;	break;
    		case MENU_QUICK_RIGHT:	mMenuExpand = true; menuKey = MenuKey::NAVI_RIGHT;	break;
    		}
    
    		
			if (ie.m_Id == SHOW_HELP)
			{
				mShowHelp = !mShowHelp;
				return true;
			}
			else
			{
				handleSettingMenuKey(menuKey);
			}
			return true;
    		
    	}
    	else
    	{
    		
			if (MENU_ESCAPE == ie.m_Id)
			{
				mMenuExpand = false;
				mMenuType = MenuType::TESTS;
			}
    		
    		if(mSample)
    		{
    			if(!mSample->onDigitalInputEvent(ie,val))
    				return false;
    		}
    	}
        
    	return true;
    }
    
    bool handleMenuKey(MenuKey::Enum menuKey)
    {
        // CREATE SPECIAL TEXT TREE MENU AS AN IMPLEMENTATION OF NxUI
        
        Test::TestGroup* parent = mSelected->getParent(), *child = NULL;
        PX_ASSERT(parent);
    	
        
    	switch (menuKey)
    	{
    	case MenuKey::NAVI_LEFT:
    		if (parent->getParent())
    			mSelected = parent; // only select your parent if you have a grandparent (do not select the root)
    		break;
    	case MenuKey::NAVI_RIGHT:
    		if (NULL != (child = mSelected->getFirstChild()))
    			mSelected = child;
    		break;
    	case MenuKey::NAVI_UP:
    		if (NULL != (child = parent->getPreviousChild(*mSelected)))
    			mSelected = child;
    		else if (NULL != (child = parent->getLastChild()))
    			mSelected = child;
    		break;
    	case MenuKey::NAVI_DOWN:
    		if (NULL != (child = parent->getNextChild(*mSelected)))
    			mSelected = child;
    		else if (NULL != (child = parent->getFirstChild()))
    			mSelected = child;
    		break;
    	case MenuKey::SELECT:
    		if (mSelected->isTest())
    		{
    			// for general tree class, replace with functions pointer to custom onSelect function
                mMenuExpand = false;
    			switchSample();
                
                
    			return false;
    		}
    		else
    		{
    			if (NULL != (child = mSelected->getFirstChild()))
    				mSelected = child;
    		}
    		break;
    	default:
    		//mSelected = mRunning;
    		break;
    	}
    
    	return true;
        
    }
}

