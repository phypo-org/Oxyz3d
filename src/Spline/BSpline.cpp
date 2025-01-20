
#include "Shape/DataBase.h"

#include "BSpline.h"
#include "SplineCalcul.h"

namespace PP3d {
  //******************************
  ObjBSpline::~ObjBSpline()
  {
    if( cSplinePts !=nullptr )    cSplinePts->deleteAll();
  }
  //------------------------------------------
  void ObjBSpline::drawGL( ViewProps& pViewProps )
    {
      //   std::cout << ">>>>>>>>>>>>> ObjBSpline::drawGL" << std::endl;
      
      remakeAll(cMyMaille); // PAS TRES ECONOMIQUE !!!!!!!!

      
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	case SelectType::Point:
	  {
          drawSplinePointGL( pViewProps);
          
          drawLineGL( pViewProps );					
           
            //   drawSplineLineGL( pViewProps);
             
          drawPointGL( pViewProps);
   	  }
	  break;
				
	case SelectType::Line:
         
          drawSplineLineGL( pViewProps);
         
 	  drawLineGL( pViewProps );					
          drawPointGL( pViewProps);
 	  break;
				
	default:
	  {       
          drawSplineLineGL( pViewProps);
          
	  drawLineGL( pViewProps );					
          drawPointGL( pViewProps);
  
	    //	    std::cout << ">>>>>>>>>>>>Polyline" << std::endl;
	  //	    VisitorDrawBSpline	lVisitL( pViewProps, cMyProps);
	  //	    execVisitor( lVisitL );			
	  }
	}
    }
  //---------------------------		
  // phipo 20231204
  void ObjBSpline::selectGL( ViewProps& pViewProps )
    {
      std::cout << "  ObjBSpline::selectGL" << std::endl;
    
     if( cMyProps.cVisible == false )
      {
	return;
      }
      std::cout << "  ObjBSpline::selectGL 2 " << std::endl;
  
     remakeAll(cMyMaille); // PAS TRES ECONOMIQUE !!!!!!!!
     ObjectPolylines::selectGL( pViewProps );
      
    if( cSplinePts != nullptr )
      {
      std::cout << "  ObjBSpline::selectGL 3" << std::endl;
        switch( pViewProps.cSelectType )
          {
          case SelectType::Point:
            {
              std::cout << "  ObjBSpline::selectGL 4" << std::endl;
              //        auto lMemo = pViewProps.deflatePointSize();
              VisitorDrawSelectPoints	lVisitS( pViewProps, cMyProps);
              cSplinePts->execVisitor( lVisitS );
              //   pViewProps.setPointSize(lMemo);
            }
            break;
          
          default: ;
          }
      }
    }
  //------------------------------------------------------
  void ObjBSpline::drawSplinePointGL( ViewProps & pViewProps )
  {
     std::cout << "  ObjBSpline::drawSplinePointGL" << std::endl;
   if( cSplinePts != nullptr )
      {
        std::cout << "    ObjBSpline::drawSplinePointGL OK" << std::endl;
        VisitorDrawPoints	lVisitP( pViewProps, cMyProps);

        auto lMemo = pViewProps.deflatePointSize();
        cSplinePts->execVisitor( lVisitP );
        pViewProps.setPointSize(lMemo);
      }
  }
  //---------------------------
  void ObjBSpline::drawSplineLineGL ( ViewProps& pViewProps )
  {
       std::cout << "  ObjBSpline::drawSplineLineGL" << std::endl;
    if( cSplinePts != nullptr )
      {
        std::cout << "    ObjBSpline::drawSplineLineGL OK" << std::endl;
        VisitorDrawLine	lVisitL( pViewProps, cMyProps);
       
        auto lMemo = pViewProps.deflatePointSize();
        cSplinePts->execVisitor( lVisitL );
        pViewProps.setPointSize(lMemo);
      }
  }	 

  //---------------------------		
  void ObjBSpline::makePoles()
  {
    if( getFacet() == nullptr )
      return ;
     
    VectDouble3 lNodes;
    getFacet()->getCopyPointsWithDuplicate( lNodes ); // get the originals points
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
  void ObjBSpline::makePtsFromPoles( size_t iMaille  ) {

    
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
 
    if( oResult.size() > 1 ) {
      
      if( cSplinePts != nullptr ) {
        cSplinePts->deleteAll();
      }
      
      cSplinePts = new Facet();
       
      PointPtr lPt0 = new Point( oResult.getPoint3d( 0 )  );     
  
      for( size_t p =1; p< oResult.size();  p++){
        
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
  ObjectPolylines* ObjBSpline::CreatePolyline( ObjBSpline * iSpline, size_t iMaille)
    {
      //      std::cout << "*********** ObjBSpline::CreatePolyline" << std::endl;
    if( iMaille == 0 ) iMaille = iSpline->cMyMaille;
    if( iMaille == 0 ) iMaille = SplineCalcul::BMaille;
    
    iSpline->cMyMaille = iMaille;

    ObjectPolylines* lPoly = nullptr;
    
    if( iSpline != nullptr )
      {
        if( iSpline->cSplinePts == nullptr ) {
          iSpline->makePtsFromPoles(iMaille);
        }
        
        lPoly = new ObjectPolylines( "Poly from BSpline", iSpline->cSplinePts );
        iSpline->cSplinePts = nullptr;
        
        iSpline->makePtsFromPoles(iMaille); 
      }

    return lPoly;
    }  
    //---------------------------		


  //******************************

}
