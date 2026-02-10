
#include "Shape/DataBase.h"
#include "Shape/ViewProps.h"

#include "BSpline.h"
#include "SplineCalcul.h"

namespace PP3d {
  //******************************
  BSpline::~BSpline()
  {
    if( cSplinePts !=nullptr )    cSplinePts->deleteAll();
  }
  //------------------------------------------
  void BSpline::drawGL( ViewProps& pViewProps, bool iForceSelect, bool iSelect )
  {
    //   std::cout << ">>>>>>>>>>>>> BSpline::drawGL" << std::endl;
    if( cMyProps.cVisible == false )
      {
        return;
      }    
       
    remakeAll(cMyMaille); // PAS TRES ECONOMIQUE !!!!!!!!

      
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
      case SelectType::All:
      case SelectType::Point:
        {
          glDisable(GL_LIGHTING);
            
          cSplinePts->drawLines(pViewProps);
          drawPointsLines(pViewProps);;  
        }
        break;
          
      case SelectType::Group:
      case SelectType::Object:
      case SelectType::Poly:
      case SelectType::Facet:
      case SelectType::Line:
        {
          glDisable(GL_LIGHTING);
          
          cSplinePts->drawLines(pViewProps);                    
          drawLines(pViewProps);;
 	}
      }      
  }
  //---------------------------		
  // phipo 20231204
  void BSpline::selectGL( ViewProps& pViewProps, EntityId iForceId )
  {
    std::cout << "  BSpline::selectGL" << std::endl;
    
    if( cMyProps.cVisible == false )
      {
	return;
      }
     
    std::cout << "  BSpline::selectGL 2 " << std::endl;
     

    //    remakeAll(cMyMaille); // PAS TRES ECONOMIQUE !!!!!!!!
    Polyline::selectGL( pViewProps, iForceId );      
  }
  //---------------------------		
  void BSpline::makePoles()
  {   
    VectDouble3 lNodes;
    getCopyPointsWithDuplicate( lNodes ); // get the originals points
    std::cout << "*********** makePoles nodes:"  << lNodes.size() << std::endl;

    //     for( size_t j = 0; j <  lNodes.size(); j++ )
    //       {
    //         std::cout << "lNodes "<< j << " -> "  <<lNodes.getPoint3d( j ) << std::endl;
    //      }
    if( cClosedSpline )
      {
        lNodes.add( lNodes[0]);
      }
     
    VectDouble3 lDest;
    lDest.resize( lNodes.size()  ); // reserve size for pole

    for( int i = 0; i < 3; i++ ) {
      //  std::cout << "******* makePoles poles:"  << i << std::endl;
      SplineCalcul::Thomas( lNodes.getAxePoint(i), lDest.getAxePoint(i));
    }
      
    cPoles.clear();
      
    // On double le premier point       
    cPoles.add( lDest.getPoint3d(0) );
      
    for( size_t j = 0; j <  lNodes.size(); j++ )
      {
        //   std::cout << "Poles " << j << " ==>> " << lDest.getPoint3d( j ) << std::endl;
        cPoles.add( lDest.getPoint3d( j ) );
      }
         
    // On double le dernier point
    cPoles.add( lDest.getPoint3d( lNodes.size()- 1 ) );
  }      
  //---------------------------		
  void BSpline::makePtsFromPoles( size_t iMaille  )
  {    
    if( iMaille == 0 ) iMaille = cMyMaille;
    if( iMaille == 0 ) iMaille = SplineCalcul::BMaille;
    
      
    cMyMaille = iMaille;
    
    if( cPoles.size() == 0 )
      {
        makePoles();
      }
      
    VectDouble3 oResult;
    std::cout << "BSpline makePtsFromPoles  call CalculBSpline poles:" << cPoles.size() << std::endl;
    SplineCalcul::CalculBSpline( iMaille, cPoles, oResult );
    std::cout << "BSpline makePtsFromPoles  call CalculBSpline result:" << oResult.size() << std::endl;            
 
    if( oResult.size() > 1 )
      {
        
        if( cSplinePts != nullptr )
          {
            cSplinePts->deleteAll();
          }
        
        cSplinePts = new Polyline();
        
        PointPtr lPt0 = new Point( oResult.getPoint3d( 0 )  );     
        
        for( size_t p =1; p< oResult.size();  p++)
          {        
            //     std::cout <<  p << "  =--> " <<  oResult.getPoint3d( p )  << "   ";
            
            PointPtr lPt   = new Point( oResult.getPoint3d( p ) );
            LinePtr  lLine = new Line( lPt0, lPt );
            //       std::cout << (*lPt0) << " " << (* lPt) << std::endl;
            cSplinePts->addLine( lLine );
            lPt0 = lPt;      
          }
      }
  }
  //---------------------------
  Polyline *  BSpline::CreatePolyline( BSpline * iSpline, size_t iMaille)
  {
    //      std::cout << "*********** BSpline::CreatePolyline" << std::endl;
    if( iMaille == 0 ) iMaille = iSpline->cMyMaille;
    if( iMaille == 0 ) iMaille = SplineCalcul::BMaille;
    
   
    if( iSpline != nullptr )
      {
        iSpline->cMyMaille = iMaille;
        if( iSpline->cSplinePts != nullptr )
          {
            delete iSpline->cSplinePts;
            iSpline->cSplinePts = nullptr;
          }
        iSpline->makePtsFromPoles(iMaille);
      }                  
    
    return new Polyline( iSpline->getLines() );
  }
//---------------------------		
   

//******************************

}
