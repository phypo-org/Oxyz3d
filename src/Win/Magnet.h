#ifndef H__MAGNET__H
#define H__MAGNET__H


#include "Shape/Entity.h"

#include "Modif/VisitorModif.h"

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

  enum class MagnetActionAxis{
    ACTIONAXIS_X,
    ACTIONAXIS_Y,
    ACTIONAXIS_Z,
    ACTIONAXIS_XY,
    ACTIONAXIS_XZ,
    ACTIONAXIS_YZ,
    ACTIONAXIS_XYZ,
  };

  //****************************
  class Magnet{
    
    PP3d::Point3d      cPos;    
    PP3d::Point3d      cTransformPos;    
    double             cSize    = 100;
    double             cSize2   = cSize*cSize;

    double             cPower  =1;
    
    MagnetAction       cAction     = MagnetAction::MAGNET_ACTION_ATTRACK;
    MagnetActionAxis   cActionAxis = MagnetActionAxis::ACTIONAXIS_XYZ;
    MagnetAlgo         cAlgo       = MagnetAlgo::MAGNET_ALGO_DIST;

    
  public:
 
    void          setTransformPosition( PP3d::Point3d & iPos ) { cTransformPos = iPos; }
    PP3d::Point3d getTransformPosition()                const  { return cTransformPos; }
    void          setPosition ( PP3d::Point3d & iPos ) { cPos = iPos;  cTransformPos = iPos;}
    PP3d::Point3d getPosition ()                const  { return cPos; }
   
    void          setSize     ( double iSz)            { cSize = iSz; cSize2 = cSize*cSize; }
    double        getSize     ()                const  { return cSize; }
    
    void          setAction   ( MagnetAction iAct  )   { cAction = iAct;  }
    MagnetAction  getAction()                   const  { return cAction;  }
    void          setAlgo     ( MagnetAlgo   iAlgo )   { cAlgo   = iAlgo; }

    void          setPower( double iPower )            { cPower = iPower; }
    double        getPower()                    const  { return cPower; }


    bool          execOn( PP3d::Point3d & ioPt );
    
    PP3d::Poly*   prepareMagnetDraw();
    PP3d::Poly*   releaseMagnet();
    void          drag( Canvas3d & iCanvas );

  };
  //****************************

  struct VisitorMagnet : public  PP3d::VisitorModifPoints
  {
    Magnet & cMagnet;
    //---------------------------------			  
    VisitorMagnet( PP3d::Selection & iSelect, Magnet & iMagnet )
      :VisitorModifPoints( iSelect )
      ,cMagnet( iMagnet )
    {

    }
    //---------------------------------		
    void execEndFacet( PP3d::Facet* pEntity) override
    {
      // 	  std::cout << " cModifPt00000000222:" << cModifPt.size() << std::endl;
      VisitorModifPoints::execEndFacet(pEntity); 
      //	  std::cout << " cModifPt222:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{
	  //	  std::cout << " cModifPt:" << cModifPt.size() << std::endl;
	  
	  for( PP3d::Point* lPt:cModifPt )
	    {
              cMagnet.execOn( lPt->get() );
	    }
	  cModifPt.clear(); 
	}        
    }
  };


} // Namespace 

#endif
