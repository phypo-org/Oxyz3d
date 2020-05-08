
#include "PP3dType.h"

#include "MinMaxBox3d.h"

#include <cmath>

const double PI =  3.141592653589793238462 ; // std::acos(-1);


namespace PP3d {

  /*---------------------------------------------------------------------*/
  /*    Transf3d::Transf3d ...                                         */
  /*---------------------------------------------------------------------*/

  Transf3d::Transf3d()
  {
    raz();
  }
  /*---------------------------------------------------------------------*/
  /*    Transf3d::raz ...                                                */
  /*---------------------------------------------------------------------*/

  void 
  Transf3d::raz()
  {
    cPosition.raz();
    cAngle.raz();
    cScale.raz(1.0);
  }
  void
  Transf3d::raz45()
   {
     raz();
     cAngle[0] = - 10;	
     cAngle[1] = 45;	
     cAngle[2] = 0 ;	
  }    
  void 
  Transf3d::razX()
  {
    raz();
    cAngle[1] = 90;	
  }
  void 
  Transf3d::razY()
  {
    raz();
    cAngle[0] =  90;
  }
  void 
  Transf3d::razZ()
  {
    raz();
  }
  void 
  Transf3d::razXInv()
  {
    raz();
    cAngle[1] = 270;	
  }
  void 
  Transf3d::razYInv()
  {
    raz();
    cAngle[0] = 270;
  }
  void 
  Transf3d::razZInv()
  {
    raz();
    cAngle[2] = 180;
  }
  /*---------------------------------------------------------------------*/
  /*    Transf3d::exec ...                                               */
  /*---------------------------------------------------------------------*/

  void 
  Transf3d::execGL()
  {
    // ATTENTION A L ORDRE DES OPERATIONS
    std::cout << " P:" << cPosition << " R:" << cAngle << std::endl;
    glTranslated( cPosition[ 0 ], cPosition[1], cPosition[2] );

    glRotated( cAngle[0], 1.0, 0.0, 0.0 );
    glRotated( cAngle[1], 0.0, 1.0, 0.0 );
    glRotated( cAngle[2], 0.0, 0.0, 1.0 );

    glScaled( cScale[0], cScale[1], cScale[2]);
  }
  /*---------------------------------------------------------------------*/
  /*    AX3dTrans::exec ...                                              */
  /*---------------------------------------------------------------------*/
  /*
    void
    Transf3d::execBox( MinMaxBox3d & pBox )
    {
    glTranslated( cPosition[ 0 ], cPosition[1], cPosition[2] );


    Double3 lCenter = pBox.centerDbl();

    glTranslatef( lCenter[ 0 ], lCenter[ 1 ], lCenter[ 2 ] );

    glRotated( cAngle[0], 1.0, 0.0, 0.0 );
    glRotated( cAngle[1], 0.0, 1.0, 0.0 );
    glRotated( cAngle[2], 0.0, 0.0, 1.0 );

    //	glRotated( cAngle[0], 1.0, 0.0, 0.0 );
    //	glRotated( cAngle[1], 0.0, 1.0, 0.0 );
    //	glRotated( cAngle[1], 0.0, 0.0, 1.0 );

    glScaled( cScale[0], cScale[1], cScale[2]);

    glTranslatef( -lCenter[ 0 ], -lCenter[ 1 ], -lCenter[ 2 ] );	
    }
  */
  /*---------------------------------------------------------------------*/
  /*    Transf3d::centerTo ...                                          */
  /*---------------------------------------------------------------------*/
  void
  Transf3d::centerTo( MinMaxBox3d& pBox)
  {
    Double3 lCenter = 	pBox.centerDbl();

    for( int i= 0; i < 3; i++ )
      cPosition[i] = -lCenter[i]; 
  }
  //---------------------------------------------
  void
  Transf3d::scaleTo( double pSize )
  {
	
    for( int i = 0; i < 3; i++ )
      {
	cScale[i]  = pSize;		
      }
  }/*---------------------------------------------------------------------*/
  /*    Transf3d::scaleTo ...                                           */
  /*---------------------------------------------------------------------*/

  void
  Transf3d::scaleTo( MinMaxBox3d& pBox,  Double3& pSz )
  {


    cScale[0]  = pSz[ 0 ] / (pBox.getMax().cX-pBox.getMin().cX);		
    cScale[1]  = pSz[ 1 ] / (pBox.getMax().cY-pBox.getMin().cY);		
    cScale[2]  = pSz[ 2 ] / (pBox.getMax().cZ-pBox.getMin().cZ);		
			
  }

  /*---------------------------------------------------------------------*/
  /*    Transf3d::scaleTo ...                                           */
  /*---------------------------------------------------------------------*/

  void
  Transf3d::scaleTo( MinMaxBox3d& pBox, double pSz )
  {
    double lMax=-1e10;

    // ATTENTION AU DIVISION PAR ZERO ET DEPACEMENT DE CAPACITE !

    if( (pBox.getMax().cX-pBox.getMin().cX)>lMax)
      lMax = (pBox.getMax().cX-pBox.getMin().cX);		

    if( (pBox.getMax().cY-pBox.getMin().cY)>lMax)
      lMax = (pBox.getMax().cY-pBox.getMin().cY);		
			
    if( (pBox.getMax().cZ-pBox.getMin().cZ)>lMax)
      lMax = (pBox.getMax().cZ-pBox.getMin().cZ);		
	
    cScale[0] =
      cScale[1] =
      cScale[2] = pSz / lMax;
  }
  //****************************************************
}
