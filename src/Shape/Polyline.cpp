#include "ViewProps.h"
#include "EntityVisitor.h"
#include "Polyline.h"


namespace PP3d{
  //********************************


  //--------------------------------------  
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
  void Polyline::drawGL( ViewProps & pViewProps )
  {     
    if( cMyProps.cVisible == false )
      {
        return;
      }    
    
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
      case SelectType::All:        
      case SelectType::Point:
        {	  
          glDisable(GL_LIGHTING);
          drawPointsLines(pViewProps);
        }
        break;
     
				
      case SelectType::Group:
      case SelectType::Object:
      case SelectType::Poly:
      case SelectType::Facet:
      case SelectType::Line:
	{      				
          glDisable(GL_LIGHTING);
          drawLines(pViewProps);
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  
 
    drawPointsLines(pViewProps);
  }
  //---------------------------		
  void Polyline::selectGL( ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
        return;
      }
      

    switch( pViewProps.cSelectType )
      { 
      case SelectType::All:
        {         
          pickingPointsLines(pViewProps);
        }
        break;
        
      case SelectType::Point:
        {
          pickingPoints(pViewProps);
        }
        break;
        
      case SelectType::Line:
	{				
          pickingLines(pViewProps);
	}
	break;
        
				
      default:  
	break;
      }
 
    //	drawInfoGL( pViewProps, cMyProps );
  }	
  //********************************
}
