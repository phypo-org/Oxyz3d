#ifndef H__MAGNET__H
#define H__MAGNET__H


#include "Shape/Entity.h"
#include "Shape/ObjectPoly.h"

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
    MAGNET_ALGO_SQUARE_ROOT,
    MAGNET_ALGO_NO_DIST
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

    double             cMemSize = 1;
    double             cSize    = 1;
    double             cSize2   = 1;

    double             cPower  =1;
    
    MagnetAction       cAction     = MagnetAction::MAGNET_ACTION_ATTRACK;
    MagnetActionAxis   cActionAxis = MagnetActionAxis::ACTIONAXIS_XYZ;
    MagnetAlgo         cAlgo       = MagnetAlgo::MAGNET_ALGO_DIST;
    
    bool               cFlagX=true;
    bool               cFlagY=true;
    bool               cFlagZ=true;
 
  public:
 
    void          setTransformPosition( PP3d::Point3d & iPos ) { cTransformPos = iPos; }
    PP3d::Point3d getTransformPosition()                const  { return cTransformPos; }
    void          setPosition ( PP3d::Point3d & iPos ) { cPos = iPos;  cTransformPos = iPos;}
    PP3d::Point3d getPosition ()                const  { return cPos; }
   
    void          setMemSize     ( double iSz)            { cMemSize = iSz; }
    double        getMemSize     ()                const  { return cMemSize; }

    void          setSize     ( double iSz)            { cSize = iSz; cSize2 = cSize*cSize; }
    double        getSize     ()                const  { return cSize; }
    
    void          setAction   ( MagnetAction iAct  )   { cAction = iAct;  }
    MagnetAction  getAction()                   const  { return cAction;  }
    void          setAlgo     ( MagnetAlgo   iAlgo )   { cAlgo   = iAlgo; }
    MagnetAlgo    getAlgo     ()                       { return cAlgo;    }

    void          setPower( double iPower )            { cPower = iPower; }
    double        getPower()                    const  { return cPower; }


    bool          execOn( PP3d::Point3d & ioPt , double iCoef );
    
    PP3d::Poly*   prepareMagnetDraw();
    PP3d::Poly*   releaseMagnet();
    void          drag( Canvas3d & iCanvas );

  protected:
    PP3d::Poly       * cShapeMagnet=nullptr;
    PP3d::ObjectPoly * cObjectPoly=nullptr;
 
    
  public:
    PP3d::Poly       * getMyShape ()                            { return cShapeMagnet; }
    PP3d::ObjectPoly * getMyObject()                            { return cObjectPoly;  }
    void               setMyShape (PP3d::Poly       * iShape)   { cShapeMagnet = iShape; }
    void               setMyObject(PP3d::ObjectPoly * iObject)  { cObjectPoly  = iObject; }
    void               prepareDrawMagnet();

    bool               isUsingX() const{ return cFlagX;}
    bool               isUsingY() const{ return cFlagY;}
    bool               isUsingZ() const{ return cFlagZ;}
    void               useX(bool iFlag = true ) { cFlagX = iFlag; }
    void               useY(bool iFlag = true ) { cFlagY = iFlag; }
    void               useZ(bool iFlag = true ) { cFlagZ = iFlag; }

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
    void execEndFacet( PP3d::Facet* pEntity) override;
    
  };


} // Namespace 

#endif
