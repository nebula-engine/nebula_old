// Manager of NxUI objects

class NxUIManager
{
	///////////////
	// FUNCTIONS //
	///////////////
public:
	NxUIManager();
	~NxUIManager();

	NxApplication*	getNxApplication(); { return m_NxApplication };

	NxUI*			getActive(); { return m_active };

	void			select( NxUI* ui );

	void			render();

	///////////////
	// VARIABLES //
	///////////////
private:
	NxApplication*	m_NxApplication;
	
	NxUI*			m_active;
}

