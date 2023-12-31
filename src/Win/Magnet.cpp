#include "Magnet.h"
#include "Shape/PrimitivFactory.h"

#include "Application.h"

using namespace PP3d;

namespace M3d {


  //  on pourrait aussi ajouter une ou des torsions ...

  
  //****************************
  bool Magnet::execOn( Point3d & ioPt )
  {
    /*
    Point3d lDiff = ioPos - TransformPos;
    
    // il faut calculer le vecteur entre le point et le magnet
    Point3d lVect = ioPt - cPos;
    double lSquareDest = lVect.square();
    if( lSquareDest > cSize2 )
      return false;            // hors porté

    // ajouter log, exponentiel ... 

    double lVal=0;
    
    switch( cAlgo )
      {
      case MagnetAlgo::MAGNET_ALGO_SQUARE_DIST:
        lVal = cPower / lSquareDest;
        break;

      case MagnetAlgo::MAGNET_ALGO_DIST:
        lVal = cPower / std::sqrt(lSquareDest);
        break;

      case MagnetAlgo::MAGNET_ALGO_SQUARE_ROOT:
        lVal = cPower / std::sqrt( std::sqrt(lSquareDest));
        break;
      }   

         
    if( cAction == MagnetAction::MAGNET_ACTION_ATTRACK )
      {
        ioPt -= lVal;
      }
    else
      {
        ioPt += lVal;
      }
    */
    return true;
  }
  //--------------------------------------
  PP3d::Poly* Magnet::prepareMagnetDraw(){

    PrimitivParam lParam;
    lParam.cNbU = 16;
    lParam.cNbV = 8;
    lParam.cHeight = cSize; lParam.cWidth = cSize;

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
  //****************************

} // Namespace 

