#include "ViewProps.h"
#include "EntityVisitor.h"
#include "VisitorPicking.h"
#include "VisitorDraw.h"
#include "Line.h"


namespace PP3d{
   //********************************
		
    //---------------------------		
  void Line::drawGL( ViewProps& pViewProps )
    {
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
	  break;
				
	default:
	  {
	    VisitorDrawObjectLine	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
						
	  }
	}
    }
    //---------------------------		
    void Line::selectGL( ViewProps& pViewProps ) 
    {
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	  drawSelectLineGL( pViewProps );					
	  drawSelectPointGL( pViewProps);
	  break;
	case SelectType::Point:
	  {
	    drawLineGL( pViewProps );					
	    drawSelectPointGL( pViewProps);					
	  }
	  break;
	case SelectType::Line:
	  drawSelectLineGL( pViewProps );					
	  break;
				
	default:
	  {
	    VisitorPickingObjectLine	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
					
	  }
	}
    }    

   //********************************
}
