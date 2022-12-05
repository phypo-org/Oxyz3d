#ifndef H__ColorRGBA__H
#define H__ColorRGBA__H

#include "PP3dType.h"

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
    ColorRGBA( const ColorRGBA & iCol)
      :cR(iCol.cR),cG(iCol.cG),cB(iCol.cB),cA(iCol.cA)
    {      
    }
    ColorRGBA( GLuint iId)
      :cR(((iId & 0x000000FF) >>  0)/255.0f)
      ,cG(((iId & 0x0000FF00) >>  8)/255.0f)
      ,cB(((iId & 0x00FF0000) >>  16)/255.0f)
      ,cA(((iId & 0xFF000000) >>  24)/255.0f)
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
   void setId( GLuint iId )
    {
      cR =  ((iId & 0x000000FF) >>  0)/255.0f;
      cG =  ((iId & 0x0000FF00) >>  8)/255.0f;
      cB =  ((iId & 0x00FF0000) >>  16)/255.0f;
      cA =  ((iId & 0xFF000000) >>  24)/255.0f;
    }

    static GLuint GetId( unsigned char iData[4]  )
    { 
      return 	iData[0] + (iData[1]<<8)  + (iData[2]<<16) ; ////// + (iData[3]<<24); 
    }

 

    const FColor* vectForGL() const
    {
      return &cR;
    }	
    FColor* operator *()
    {			
      return &cR;
    }

    void operator = ( const ColorRGBA & iCol )
    {
      cR = iCol.cR;
      cG = iCol.cG;
      cB = iCol.cB;
      cA = iCol.cA;
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
    static void Id( GLuint iId){
      /*
          std::cout << "Id: " << std::hex
		<< ((iId & 0x000000FF) >>  0) <<" "
		<< ((iId & 0x0000FF00) >>  8) <<" "
		<< ((iId & 0x00FF0000) >>  16)<<" "
		<< std::endl;      
      */
      /*
      float r = ((iId & 0x000000FF) >>  0)/255.0f;
      float g = ((iId & 0x0000FF00) >>  8)/255.0;
      float b = ((iId & 0x00FF0000) >>  16)/255.0f;
      std::cout << "Id: " << std::hex <<(int) r*255.0f
		<< " " << std::hex << (int) g*255.0f
		<< " " << std::hex << (int) b*255.0f
		<< std::endl;
    
	glColor4f(r, g, b, 1);
        */
	
    glColor4f(
		((iId & 0x000000FF) >>  0)/255.0f,
		((iId & 0x0000FF00) >>  8)/255.0f,
		((iId & 0x00FF0000) >>  16)/255.0f,
		1);
	
      
	}
    static void Zero()     { glColor4f( 0.0, 0.0, 0.0, 0.0); }
    static void Red()	    { glColor3f(1.0,0.0,0.0); 	}
    static void Green()     { glColor3f(0.0,1.0,0.0); 	}
    static void Blue()      { glColor3f( 0.0, 0.0, 1.0 ); }
    static void LightBlue() { glColor3f( 0.5, 0.5, 1.0 ); }
		
    static void Yellow()    { glColor3f(1.0,1.0,0.0); 	}
    static void Pink()	    { glColor3f(1.0,0.0,1.0); 	}
		
    static void White()	    { glColor3f(1.0,1.0,1.0); 	}
    static void Black()     { glColor3f( 0.0, 0.0, 0.0 ); }
		
    static void Grey()	    { glColor3f(0.5,0.5,0.5); 	}
    static void LightGrey()	{ glColor3f(0.7f,0.7f,0.7f); 	}
    static void DarkGrey()  { glColor3f( 0.3f, 0.3f, 0.3f ); }

    friend  	std::ostream& operator <<( std::ostream& pOs, ColorRGBA pCol)
    {
      pOs << "r:" << pCol.cR << " g:" << pCol.cG << " b:" << pCol.cB << " a:" << pCol.cA; 
      return pOs;
    }
  };
	
  //********************************************************

	
}

#endif
