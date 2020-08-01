#ifndef H__Transf3d__H
#define H__Transf3d__H

#include "Double3.h"


namespace PP3d {
  
  //********************************************************
  class Rect3d;

  class Transf3d
  {

  protected:
    Double3 cPosition;
    Double3 cAngle;
    Double3 cScale;
  public:
    Transf3d();

    Double3& position() { return cPosition; }
    Double3& angle()    { return cAngle; }
    Double3& scale()    { return cScale; }
    
    void setPosition( Double3 & iPos );

    void raz();
    void raz45();
    void razX();
    void razY();
    void razZ();
    void razXInv();
    void razYInv();
    void razZInv();

    void centerTo( Rect3d& pBox);
    void scaleTo( Rect3d & pBox, Double3 & pSz );
    void scaleTo( Rect3d& pBox, double pSz );

    void scaleTo( double pSize );
		
    void execGL();
    //	void execBox( MinMaxBox3d & pBox );

  };


  //********************************************************

}



#endif
