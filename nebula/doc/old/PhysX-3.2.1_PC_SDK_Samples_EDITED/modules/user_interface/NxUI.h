// User Interface class
// controls appearence and functionality of all stationary 2D graphics (menus, HUD, etc.)
//
// m_NxUIObjects	vector of pointers to PERSISTENT NxUI objects

class NxUI;

class NxUIObject
{
public:
	NxUIObject();
	NxUIObject( NxUIObject* _prev, NxUIObject* _next, const TCHAR* _text );
	~NxUIObject();
    
    // OBJECT EVENTS
	virtual void onSetFocus() {}
	virtual void onKillFocus() {}
	
    // PLATFORM INPUT
	virtual bool onDigitalInputEvent(const SampleFramework::InputEvent& , bool val) { return false; }
	virtual void collectInputEvents(std::vector<const SampleFramework::InputEvent*>& inputEvents) {}

	// RENDER
	virtual void render() {}

	//	SETTINGS
	setEnabled( bool _enabled ): m_enabled(_enabled) {};
	setNext( NxUIObject* _next ): m_next(_next) {};
	setPrev( NxUIObject* _prev ): m_prev(_prev) {};

	// OBJECT DIRECTIVES
	selectNext();
	selectPrev();

private:
	NxUI*			m_ui;

	TCHAR*			m_text;
	TCHAR*			m_name;

	bool			m_enabled;

	NxUIObject*		m_next;
	NxUIObject*		m_prev;
}

class NxUI
{
	///////////////
	// FUNCTIONS //
	///////////////
public:
	NxUIObject();
	~NxUIObject();

	// INTERFACE EVENTS
	virtual void onSetFocus() {}
	virtual void onKillFocus() {}
	
	// PLATFORM INPUT


	// RENDER
	virtual void render() {}

	///////////////
	// VARIABLES //
	///////////////
	NxUIManager*				m_manager;

	TCHAR*						m_name;

	NxUIObject*					m_active;

	//std::vector<NxUIObject*>	m_NxUIObjects; // custom implementation can come up with their own method of storing NxUI objects
}
