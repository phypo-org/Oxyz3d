#ifndef H__MAGNET__H
#define H__MAGNET__H


#include "Shape/Entity.h"


namespace M3d {
  
  class Canvas3d;

  enum class MagnetAction{
    MAGNET_ACTION_ATTRACK,
    MAGNET_ACTION_REPEL
  };
  enum class MagnetAlgo  {
    MAGNET_ALGO_DIST,
    MAGNET_ALGO_SQUARE_DIST,
    MAGNET_ALGO_SQUARE_ROOT
  };

  //****************************
  class Magnet{
    
    PP3d::Point3d      cPos;    
    double       cSize    = 100;
    double       cSize2   = cSize*cSize;

    double       cPower  =1;
    
    MagnetAction cAction = MagnetAction::MAGNET_ACTION_ATTRACK;
    MagnetAlgo   cAlgo   = MagnetAlgo::MAGNET_ALGO_DIST;
    
  public:
 
    void          setPosition ( PP3d::Point3d & iPos ) { cPos = iPos; }
    PP3d::Point3d getPosition ()                const  { return cPos; }
    
    void          setSize     ( double iSz)            { cSize = iSz; cSize2 = cSize*cSize; }
    double        getSize     ()                const  { return cSize; }
    
    void          setAction   ( MagnetAction iAct  )   { cAction = iAct;  }
    MagnetAction  getAction()                   const  { return cAction;  }
    void          setAlgo     ( MagnetAlgo   iAlgo )   { cAlgo   = iAlgo; }

    void          setPower( double iPower )            { cPower = iPower; }
    double        getPower()                    const  { return cPower; }


    bool          exec( PP3d::Point3d & ioPt );
    
    PP3d::Poly*   prepareMagnetDraw();
    PP3d::Poly*   releaseMagnet();
    void          drag( Canvas3d & iCanvas );


  };
  //****************************



} // Namespace 

#endif
