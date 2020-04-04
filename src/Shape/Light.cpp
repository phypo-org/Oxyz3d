#include "Light.h"


namespace PP3d {

	Light Light::sLights[MaxLight];	


  //******************************
	Light::Light()
		:cEnable(false)
		,cLightId(-1)
	{
	}
	
	//---------------------------
	void Light::raz()
	{
		cLightId = -1;
		cPosition.zero();
		cDiffuse.zero();
		cAmbient.zero();
		cSpecular.zero();

		cEnable   = false;		
		
	}
	//---------------------------
	void Light::disable()
	{
		cPosition.zero();
		cDiffuse.zero();
		cAmbient.zero();
		cSpecular.zero();

		cEnable   = false;		
	}
	//---------------------------
	int Light::ConvLightToPos( int pPos )
	{
		switch( pPos )
			{
			case GL_LIGHT0 : return 0;
			case GL_LIGHT1 : return 1;
			case GL_LIGHT2 : return 2;
			case GL_LIGHT3 : return 3;
			case GL_LIGHT4 : return 4;
			case GL_LIGHT5 : return 5;
			case GL_LIGHT6 : return 6;
			case GL_LIGHT7 : return 7;
			}
		return -1; //BOF !!!!
	}
	//---------------------------
	int Light::ConvPosToLight( int pPos )
	{
		switch( pPos )
			{
			case 0: return GL_LIGHT0 ;
			case 1: return GL_LIGHT1 ;
			case 2: return GL_LIGHT2 ;
			case 3: return GL_LIGHT3 ;
			case 4: return GL_LIGHT4 ;
			case 5: return GL_LIGHT5 ;
			case 6: return GL_LIGHT6 ;
			case 7: return GL_LIGHT7 ;
			}
		return -1; //BOF !!!!
	}
	//---------------------------
	Light* Light::GetNewLight()
	{
		for( int i=0; i<MaxLight; i++)
			{
				if( sLights[i].cLightId == -1 )
					{
						sLights[i].raz();
						sLights[i].cLightId = ConvPosToLight( i );
						return &sLights[i];
					}
			}
		return nullptr;
	}

	//---------------------------
	// On ne fait aucun vrai control 
	// mais on pourrait si la cLightId est bon

	void DestroyLight( Light* pLight )
	{
		if( pLight )
			pLight->raz();
	}

	//---------------------------
	void Light::GL( )
	{
		if( cLightId == -1 )
			return ;

		if( cEnable )
			{
				//	if( cAmbient.isZero() == false )
					glLightfv( cLightId, GL_AMBIENT,  cAmbient.vectForGL() );

					//			if( cDiffuse.isZero() == false )
					glLightfv( cLightId, GL_DIFFUSE,  cDiffuse.vectForGL() );

					//		if( cSpecular.isZero() == false )
					glLightfv( cLightId, GL_SPECULAR, cSpecular.vectForGL() );

		
				glLightfv( cLightId, GL_POSITION, cPosition.vectForGL() );

				glEnable( cLightId );
			}
		else
			{
				glDisable( cLightId );
			}
	}
	//---------------------------
	void Light::AllGL()
	{
		glEnable(GL_LIGHTING); 
		
		for( int i=0; i<MaxLight; i++)
			{
				sLights[i].GL();
			}
	}
	//---------------------------
	void Light::RazAll()
	{
		glEnable(GL_LIGHTING); 
		
		for( int i=0; i<MaxLight; i++)
			{
				sLights[i].raz();
			}
	}
	//---------------------------
	void Light::DisableAll()
	{
		glEnable(GL_LIGHTING); 
		
		for( int i=0; i<MaxLight; i++)
			{
				sLights[i].disable();
			}
	}
	//---------------------------

	void Light::RainbowOn()
	{
		Light* l1= Light::GetNewLight();
	
		l1->cAmbient.set(  0.1f, 0.1f, 0.1f,   1.0f );
		l1->cDiffuse.set(  1,  0, 0, 1.0 );
		l1->cSpecular.set(  0.1f,  0.1f, 0.1f, 1.0f );
		l1->cPosition.set(  1000000, 0, 0  );
		l1->cEnable= true ;


		// On reutilise l1
		l1= Light::GetNewLight();
		l1->cDiffuse.set( 0, 1000000, 0, 0.0  );
		l1->cPosition.set( 0, 1000000, 0 );
		l1->cEnable= true ;

		// On reutilise l1
		l1= Light::GetNewLight();
		l1->cDiffuse.set(  0.0, 0.0, 1.0, 1);
		l1->cPosition.set(   0, 0, 1000000);
		l1->cEnable= true ;

		// On reutilise l1
		l1= Light::GetNewLight();
		l1->cDiffuse.set(  1,  1, 0, 1.0 );
		l1->cPosition.set(  -1000000, 0, 0);
		l1->cEnable= true ;

		// On reutilise l1
		l1= Light::GetNewLight();
		l1->cDiffuse.set(  1, 0, 1, 1 );
		l1->cPosition.set(   0, -1000000, 0);
		l1->cEnable= true ;

		// On reutilise l1
		l1= Light::GetNewLight();
		l1->cDiffuse.set( 0.0, 1.0, 1.0, 1  );
		l1->cPosition.set( 0, 0, -1000000);
		l1->cEnable= true ;
		
	}
	//---------------------------
	void Light::GreyOn()
	{
		Light* l1= Light::GetNewLight();
	
		l1->cAmbient.set(  0.8f, 0.8f, 0.8f,   1.0f );
		l1->cDiffuse.set(  0.2f,  0.2f, 0.2f,  1.0f );
		l1->cSpecular.set(  0.1f,  0.1f, 0.1f, 1.0f );
		l1->cPosition.set(  1000000,1000000, 0  );
		l1->cEnable= true ;

		l1= Light::GetNewLight();
		l1->cDiffuse.set(  0.1f,  0.1f, 0.1f,  1.0f );
		l1->cSpecular.set(  0.1f,  0.1f, 0.1f, 1.0f );
		l1->cPosition.set(  -1000000,1000000, 0  );
		l1->cEnable= true ;

		l1= Light::GetNewLight();
		l1->cDiffuse.set(  0.3f,  0.3f, 0.3f,  1.0f );
		l1->cSpecular.set(  0.1f,  0.1f, 0.1f, 1.0f );
		l1->cPosition.set(  1000000, -1000000, 10000000 );
		l1->cEnable= true ;

	}
  //******************************

}

