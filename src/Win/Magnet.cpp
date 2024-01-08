#include "Magnet.h"
#include "Shape/PrimitivFactory.h"

#include "Application.h"

using namespace PP3d;

namespace M3d {

  
  //  on pourrait aussi ajouter une ou des torsions ...

  
  //****************************
  bool Magnet::execOn( Point3d & ioPt, double iCoef )
  {
 
    
    // il faut calculer le vecteur entre le point et le magnet
    Point3d lVect = ioPt - cPos;
    double lSquareDest = lVect.square();
    if( lSquareDest > cSize2 )
      {        
        //     std::cout << "  Magnet::execOn Pt:" << ioPt << " Magnet : " <<  cPos << " Dist2:" <<lSquareDest << " Size2:" << cSize2 << " Too far away!!!< " << std::endl;
        return false;            // hors porté
      }
    // ajouter log, exponentiel ... 

    double lVal=0;
    
    switch( cAlgo )
      {
      case MagnetAlgo::MAGNET_ALGO_NO_DIST:
        lVal = (cPower*iCoef);
        break;

      case MagnetAlgo::MAGNET_ALGO_SQUARE_DIST:
        lVal = (cPower*iCoef) / lSquareDest;
        break;

      case MagnetAlgo::MAGNET_ALGO_DIST:
        lVal = (cPower*iCoef) / std::sqrt(lSquareDest);
        break;

      case MagnetAlgo::MAGNET_ALGO_SQUARE_ROOT:
        lVal = (cPower*iCoef) / std::sqrt( std::sqrt(lSquareDest));
        break;
      }   

    //   std::cout << "  Magnet::execOn coef:"  << iCoef << " Val:" << lVal << " Pt:" << ioPt ;

    
        
    if( cAction == MagnetAction::MAGNET_ACTION_ATTRACK )
      {
        if( isUsingX() )  ioPt.x() += lVal;
        if( isUsingY() )  ioPt.y() += lVal;
        if( isUsingZ() )  ioPt.z() += lVal;
      }
    else
      {
        if( isUsingX() )  ioPt.x() -= lVal;
        if( isUsingY() )  ioPt.y() -= lVal;
        if( isUsingZ() )  ioPt.z() -= lVal;
      }

    std::cout << " => " << ioPt << std::endl;
    
    return true;
  }
  //--------------------------------------
  PP3d::Poly* Magnet::prepareMagnetDraw(){

    PrimitivParam lParam;
    
    lParam.cNbU = 16;
    lParam.cNbV = 8;
    lParam.cHeight = cSize;
    lParam.cWidth  = cSize;

    std::string lName="Magnet";

    PP3d::Poly* lShape = PP3d::PrimitivFactory::CreateSphere( &lParam, lName );
    
    TheInput.swapCurrentCreation( new PP3d::ObjectPoly( "Primitiv", lShape ) );  

     
    //  lShape->move(lPos );
      		
    //PP3d::Point3d lScale( lSz, lSz, lSz );
    //      lShape->scale( lScale );
     
      PP3d::Mat4 lMatTran;
      lMatTran.identity();
 
      lMatTran.initMove( cPos );
      lShape->modify( lMatTran );
      return lShape;
  }
  //--------------------------------------
  PP3d::Poly* Magnet::releaseMagnet(){

    
    // TheInput.swapCurrentCreation( nullptr );  

      return nullptr;
  }
  //---------------------------
  void Magnet::drag(Canvas3d & iCanvas)
  {     
    //  if( iCanvas.setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
      {
	PP3d::Point3d lPos = TheAppli.getDatabase()->getCursorPosition();

	TheAppli.roundInput( lPos );
        setPosition( lPos );	
      }
  }
  //---------------------------
  void Magnet::prepareDrawMagnet()
  {  
    //  if( cShapeMagnet == nullptr )
    //    {
        PP3d::PrimitivParam lParam;
        lParam.cNbU    = 20;
        lParam.cNbV    = 15;
        lParam.cHeight = getSize();
        lParam.cWidth  = getSize();
        
        std::string lName = "Magnet";
        
        cShapeMagnet = PP3d::PrimitivFactory::Create(  PP3d::PrimitivFactory::Type::SPHERE,
                                                                lName, &lParam);
        if( cShapeMagnet != nullptr )
          {
            cObjectPoly =  new PP3d::ObjectPoly( lName, cShapeMagnet );    
            TheInput.swapCurrentCreation( cObjectPoly  );
          }
    
    PP3d::Mat4 lMatTran;
    lMatTran.identity();
              
    lMatTran.initMove( getPosition() );
    cShapeMagnet->modify( lMatTran );
  }
  //****************************
  void VisitorMagnet::execEndFacet( PP3d::Facet* pEntity) 
    {
      //   std::cout << "  VisitorMagnet::execEndFacet:" << cModifPt.size() << std::endl;
      VisitorModifPoints::execEndFacet(pEntity); 
      //   std::cout << " VisitorMagnet::execEndFacet2 2:" << cModifPt.size() << std::endl;

      if( cMode == Mode::MODIF )
	{	  
	  for( PP3d::Point* lPt:cModifPt )
	    {
              double lNb=1;
	      auto lIter = lNbOwner.find( lPt->getId() );
	      if(  lIter != lNbOwner.end() )
		lNb = lIter->second;

              //       std::cout << "  VisitorMagnet::execEndFacet call execOn:" << cCoef << std::endl;
              cMagnet.execOn( lPt->get(), cCoef/lNb );
              //     std::cout << "Visit:" <<  lPt->get() << std::endl;
	    }
	  cModifPt.clear(); 
	}        
    }
  
  //****************************

} // Namespace 

