namespace SampleUI
{
	class Object: public NxUIObject
	{
		Object();
		~Object();
		
		// SETTINGS
		setNext( Object* _next );
		setPrev( Object* _prev );

		// RENDER
		void render( PxU32 x, PxU32 y )
	}
    
    
    
	class Samples: public NxUI
	{
		Samples();
		~Samples();

		NxUIObject* m_SampleHelloWorld;
        NxUIObject* m_SampleBridges;
        NxUIObject* m_SampleCharacterCloth;
        NxUIObject* m_SampleVehicle;
        NxUIObject* m_SampleParticles;
        NxUIObject* m_SampleSubmarine;
        NxUIObject* m_SampleNorthPole;
        NxUIObject* m_SampleCustomGravity;
	}
}

namespace SampleUI
{
	
	
	Samples::Samples()
	{
		m_SampleHelloWorld      = new Object( NULL,                     NULL, _T("SampleHelloWorld") );
        m_SampleBridges         = new Object( m_SampleHelloWorld,       NULL, _T("SampleBridges") );
        m_SampleCharacterCloth  = new Object( m_SampleBridges,          NULL, _T("SampleCharacterCloth") );
        m_SampleVehicle         = new Object( m_SampleCharacterCloth,   NULL, _T("SampleVehicle") );
        m_SampleParticles       = new Object( m_SampleVehicle,          NULL, _T("SampleParticles") );
        m_SampleSubmarine       = new Object( m_SampleParticles,        NULL, _T("SampleSubmarine") );
        m_SampleNorthPole       = new Object( m_SampleSubmarine,        NULL, _T("SampleNorthPole") );
        m_SampleCustomGravity   = new Object( m_SampleNorthPole,        NULL, _T("SampleCustomGravity") );
        
        m_SampleHelloWorld->setNext( m_SampleBridges );
        m_SampleBridges->setNext( m_SampleCharacterCloth );
        m_SampleCharacterCloth->setNext( m_SampleVehicle );
        m_SampleVehicle->setNext( m_SampleParticles );
        m_SampleParticles->setNext( m_SampleSubmarine );
        m_SampleSubmarine->setNext( m_SampleNorthPole );
        m_SampleNorthPole->setNext( m_SampleCustomGravity );
        
        m_Help = new Object( NULL,NULL,_T(""))
        
	}
    Samples::render()
    {
        PxU32 y = (PxU32)(-8);
        
        m_SampleHelloWorld->render(y += yInc;);
        m_SampleBridges->render(y += yInc;);
        m_SampleCharacterCloth->render(y += yInc;);
        m_SampleVehicle->render(y += yInc;);
        m_SampleParticles->render(y += yInc;);
        m_SampleSubmarine->render(y += yInc;);
        m_SampleNorthPole->render(y += yInc;);
        m_SampleCustomGravity->render(y += yInc;);
        
        y += yInc;
    }
    
    Object::render( PxU32 x, PxU32 y )
    {
        // y += yInc;
        NxApplication* app = m_ui->m_manager->m_application;
        
		const RendererColor textColor(255, 255, 255, 255);
		const char* msg;
        
		msg = app->inputInfoMsg("Press "," for help", SHOW_HELP, -1);
		if(msg)
        {
			app->m_renderer->print(x, y, msg, scale, shadowOffset, textColor);
        }
        
		// y += yInc;
        
    }
    
    
}

