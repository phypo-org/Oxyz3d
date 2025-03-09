#include "Magnet.h"
#include "Shape/PrimitivFactory.h"

#include "Creation.h"

using namespace PP3d;
using namespace std;

namespace M3d {

  
  //  on pourrait aussi ajouter une ou des torsions ...

  
  //****************************

  //----------------------------
  double Magnet::getSquareDist( const Point3d & ioPt )
  {
    Point3d lVect = ioPt - cPos;
    double lSquareDest = lVect.square();
    if( lSquareDest > cSize2 )
      {
        return -1;
      }
    
    return lSquareDest;
  }
  //----------------------------
 
  bool Magnet::execOn( Point3d & ioPt, double iCoef, double iDx, double iDy )
  {
    if( cLastHightLight == nullptr )
      return false;
    
    // il faut calculer le vecteur entre le point et le magnet
    double lSquareDest = getSquareDist( ioPt );
    if( lSquareDest < 0 )
      {
        //     std::cout << "  Magnet::execOn Pt:" << ioPt << " Magnet : " <<  cPos << " Dist2:" <<lSquareDest << " Size2:" << cSize2 << " Too far away!!!< " << std::endl;
        return false;            // hors porté
      }
    // ajouter log, exponentiel ... 

    //  double lVal= cTransformPos.dot( cPos);
    double lVal=1;

      
    Point3d lDecal;

    if( cDirection == MagnetDirection::MAGNET_DIR_NORMAL)
      {
        lDecal = getLastHightLigth()->getNormal3d();
        iCoef *= iDx*10.0;
      }
    else
      if( cDirection == MagnetDirection::MAGNET_DIR_FREE )
        {
          lDecal   = cTransformPos;
          lDecal  += getLastHightLigth()->getNormal3d()*10.0;
          iCoef   *= std::sqrt(iDx*iDx+iDy*iDy);
        }
    
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
        if( isUsingX() )  ioPt.x() += lVal*lDecal.x();
        if( isUsingY() )  ioPt.y() += lVal*lDecal.y();
        if( isUsingZ() )  ioPt.z() += lVal*lDecal.z();
      }
    else
      {
        if( isUsingX() )  ioPt.x() -= lVal*lDecal.x();
        if( isUsingY() )  ioPt.y() -= lVal*lDecal.y();
        if( isUsingZ() )  ioPt.z() -= lVal*lDecal.z();
      }

    //    std::cout << " => " << ioPt << std::endl;
    
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
    
    TheInput.swapCurrentCreation( new PP3d::Object( "Primitiv", ObjectType::ObjPoly, lShape ));  

     
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
  PP3d::Poly* Magnet::releaseMagnet()
  {    
    // TheInput.swapCurrentCreation( nullptr );  

    return nullptr;
  }
  //---------------------------
  void Magnet::unMagnetise()
  {
    cout << "    --- unMagnetise" << endl;
    for( PointPtr lPt : cVectPoints )
      {
        lPt->setMagnet(false);
      }
    for( LinePtr lLine : cVectLines )
      {
        lLine->setMagnet(false);
      }
    cVectPoints.clear();
    cVectLines.clear();
  }
  //---------------------------
  // il faut une selection non vide et une entity hightlight
  
  bool Magnet::magnetise( PP3d::DataBase & iBase, PP3d::Selection & iSel )
  {
    unMagnetise();
    
    cout << "*** magnetise " << iSel.getNbSelected() <<  endl;

    if( iSel.getNbSelected() == 0)
      {
       
        cout << "    no select" << endl;
        return false;
      }
    
    if( cLastHightLight == nullptr )
      {
        cout << "    no hightlight" << endl;
        return false;
      }


    PP3d::SortEntityVisitorPoint cVisit;
        
    TheSelect.execVisitorOnEntity(cVisit);
    if( cVisit.cVectPoints.size() == 0 )
      {
        cout << "    no point found in selection " << endl;
        unMagnetise();
        return false;
      }    


    //    cout << "    magnetise2 " << iSel.getNbSelected();

    // on magnetise ceux qui sont dans le champ 
    for( PointPtr lPt : cVisit.cVectPoints )
      {
        if( getSquareDist( lPt->get() ) > 0 )
          {
            lPt->setMagnet(true);
            //  cout << '.';
            cVectPoints.push_back( lPt );
            auto lOwners = lPt->getOwners();
            for( auto lOwn : lOwners )  // Et aussi les lines des points 
              {
                if( lOwn->getType() == ShapeType::Line )
                  {
                    //     cout << '-';

                    lOwn->setMagnet( true );
                    cVectLines.push_back( (LinePtr)lOwn );
                  }
              }
          }        
      }
    //   cout << endl;

    //   Il faut prendre le plan constitué de l'ensemble des objets de la selection comme ???
    
    
    // Point3d lCenter = lHighLight->getCenter3d();
    //  Point3d lNormal = lHighLight->getNormal3d(); 

                                       
    
    return true;
  }
    
  //---------------------------
  void Magnet::setPos3d()
  {     
    //  if( iCanvas.setCursor3dPosition(Fl::event_x(), Fl::event_y() ))
    {
      PP3d::Point3d lPos = TheCreat.getDatabase()->getCursorPosition();

      TheCreat.roundInput( lPos );
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
        cObjectPoly =  new PP3d::Object( lName, ObjectType::ObjPoly, cShapeMagnet );    
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
            cMagnet.execOn( lPt->get(), cCoef/lNb, cDx, cDy );
            //     std::cout << "Visit:" <<  lPt->get() << std::endl;
          }
        cModifPt.clear(); 
      }        
  }
  
  //****************************

} // Namespace 

