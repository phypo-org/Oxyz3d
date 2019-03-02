#ifndef H__ColorRGBA__H
#define H__ColorRGBA__H


namespace PP3d {

  //********************************************************
	
  typedef float FColor         ;
     

     
  class ColorRGBA{

	public:
    FColor cR;		
		FColor cG;
		FColor cB;
		FColor cA;

  public :
    ColorRGBA()
      :cR(1.0f),cG(1.0f),cB(1.0f),cA(1.0f)
		{
		}

    ColorRGBA( FColor pR, FColor pG, FColor pB, FColor pA=1.0f)
      :cR(pR),cG(pG),cB(pB),cA(pA)
		{
		}
    void zero()
		{
			cR=cG=cB=cA=0;
		}
		bool isZero()
		{
			if( cR == 0 && cG == 0 && cB == 0 && cA == 0 )
				return true;
			return false;
		}
	
    void set(  FColor pR, FColor pG, FColor pB, FColor pA=1.0f )
    {
      cR = pR;
      cG = pG;
      cB = pB;
      cA = pA;
    }

		const FColor* vectForGL() const
		{
			return &cR;
		}	
		FColor* operator *()
		{
			
			return &cR;
		}
	
		void GL() const { glColor4fv( &cR); }
		void materialGL() { glMaterialfv( GL_FRONT_AND_BACK, 
																			GL_AMBIENT_AND_DIFFUSE, &cR);}

		void emissionGL() { glMaterialfv(  GL_FRONT, 
																			 GL_EMISSION,
																			 &cR);}
		
		void fogGL() { glFogfv( GL_FOG_COLOR,  &cR );
		}
		public:
			static void Red()	      { glColor3f(1.0,0.0,0.0); 	}
			static void Green()    	{ glColor3f(0.0,1.0,0.0); 	}
			static void Blue()      { glColor3f( 0.0, 0.0, 1.0 ); }
			static void LightBlue() { glColor3f( 0.5, 0.5, 1.0 ); }
		
			static void Yellow()	  { glColor3f(1.0,1.0,0.0); 	}
			static void Pink()	    { glColor3f(1.0,0.0,1.0); 	}
		
			static void White()	    { glColor3f(1.0,1.0,1.0); 	}
			static void Black()     { glColor3f( 0.0, 0.0, 0.0 ); }
		
			static void Grey()	    { glColor3f(0.5,0.5,0.5); 	}
			static void LightGrey()	{ glColor3f(0.7,0.7,0.7); 	}
			static void DarkGrey()  { glColor3f( 0.3, 0.3, 0.3 ); }

			friend  	std::ostream& operator <<( std::ostream& pOs, ColorRGBA pCol)
			{
				pOs << "r:" << pCol.cR << " g:" << pCol.cG << " b:" << pCol.cB << " a:" << pCol.cA; 
				return pOs;
			}
		};
	
		//********************************************************

	
	};

#endif
