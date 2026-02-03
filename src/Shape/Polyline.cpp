#include "ViewProps.h"
#include "EntityVisitor.h"
#include "VisitorDraw.h"
#include "VisitorPicking.h"
#include "Polyline.h"


namespace PP3d{
   //********************************


  //- //-------------------------------------  
  Polyline* Polyline::duplicate() const
  {
    VectPoint3d lPts;    
    if( getCopyPointsWithDuplicate( lPts ) > 0 )
      {
        PolylinePtr lNewFac = new Polyline();

           
        PIndex lSize = lPts.getVector().size();
        if( isClose() )
          {
            lSize--;
          }
        
        for( PIndex i=0; i< lSize; i++ )
          {
            lNewFac->addPoint( lPts.getVector()[i] );
          }
          
        return lNewFac;
      }
    return nullptr;
  }
  //-------------------------		
  void Polyline::drawGL( ViewProps & iViewProps )
    {
      std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Polyline::drawGL " << std::endl;

      //      glDisable( GL_LIGHTING );	  	 

      drawLineGL( iViewProps );					
      drawPointGL( iViewProps);
      
      //      glEnable( GL_LIGHTING );
    }
  //---------------------------		
  void Polyline::selectGL( ViewProps& pViewProps )
    {
      std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SelectPolyline" << std::endl; 
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
	    //	    std::cout << ">>>>>>>>>>>>SelectPolyline" << std::endl;
	    VisitorPickingPolyline	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
	  }
	}
    }	
   //********************************
}
