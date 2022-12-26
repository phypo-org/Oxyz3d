#ifndef H__ObjSpline__H
#define H__ObjSpline__H

#include "Shape/ObjectPolylines.h"
#include "SplineCalcul.h"

namespace PP3d {

  //******************************
  class ObjBSpline : public ObjectPolylines
  {
    VectPoint3d cPoles;  // poles are store localy 
    Facet*      cSplinePts = nullptr ; // Polyline result

  public:	
     ObjBSpline(  const char*pName, Facet* pFacet)
      : ObjectPolylines( pName, pFacet )
    {;}
    ObjBSpline(  const std::string &pName, Facet* pFacet)
      : ObjectPolylines( pName, pFacet )
    {;}
		
    ObjectType getObjType() const override { return ObjectType::ObjBSpline; }


    

   void drawGL( ViewProps& pViewProps ) override
    {
      makePoles();
      
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	case SelectType::Point:
	  {
	    drawLineGL( pViewProps );					
	    drawPointGL( pViewProps);					
	  }
	  break;
				
	case SelectType::Line:
	  drawLineGL( pViewProps );					
	  drawPointGL( pViewProps);					
	  break;
				
	default:
	  {
	  drawLineGL( pViewProps );					
	  drawPointGL( pViewProps);					
	    //	    std::cout << ">>>>>>>>>>>>Polyline" << std::endl;
	  //	    VisitorDrawBSpline	lVisitL( pViewProps, cMyProps);
	  //	    execVisitor( lVisitL );			
	  }
	}
    }
     //---------------------------		
    void selectGL( ViewProps& pViewProps ) override
    {
      // std::cout << ">>>>>>>>>>>>SelectBSpline" << std::endl; 
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	  drawSelectLineGL( pViewProps );					
	  drawSelectPointGL( pViewProps);
	  break;
	case SelectType::Point:
	  {
	    //	    drawLineGL( pViewProps );					
	    drawSelectPointGL( pViewProps);					
	  }
	  break;
	case SelectType::Line:
	  drawSelectLineGL( pViewProps );					
	  break;
				
	default:
	  {
	    //	    std::cout << ">>>>>>>>>>>>SelectBSpline" << std::endl;
	    VisitorDrawSelectPolyline	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
	  }
	}
    }		 
 
     //---------------------------		
    void makePoles(){
      if( getFacet() == nullptr )
        return ;

     
      VectDouble3 lNodes;
      getFacet()->getPoints( lNodes ); // get the originals points
      std::cout << "*********** makePoles "  << lNodes.size() << std::endl;
     
      VectDouble3 lDest;
      lDest.resize( lNodes.size() ); // reserve size for pole

      for( int i = 0; i < 3; i++ ) {
        std::cout << "******* makePoles "  << i << std::endl;
        SplineCalcul::Thomas( lNodes[i],  lDest[i]);
      }
      
      cPoles.clear();
      
      // On double le premier point       
      cPoles.add( lDest.getPoint3d(0) );
      
      for( size_t j = 0; j <  lNodes.size(); j++ )
        cPoles.add( lDest.getPoint3d( j ) );
      
      // On double le dernier point
      cPoles.add( lDest.getPoint3d( lNodes.size()- 1 ) );        
      std::cout << "******* makePoles END "  << cPoles.size() << std::endl;

      VectDouble3 oResult;
      std::cout << "BSpline  call CalculBSpline poles:" << cPoles.size() << std::endl;
      CalculBSpline( SplineCalcul::Maille, cPoles, VectDouble3 oResult );
      std::cout << "BSpline  call CalculBSpline result:" << oResult.size() << std::endl;
            
      
   }
    
  };
  //******************************

}
#endif
