#ifndef H__ObjectPolylines__H
#define H__ObjectPolylines__H

#include "ObjectFacet.h"
#include "Calcul3d.h"
#include "ViewProps.h"

#include "EntityVisitorDraw.h"
#include "EntityVisitorPicking.h"

namespace PP3d {

  //******************************

  class ObjectPolylines  : public ObjectFacet
  {

  public:
    ObjectPolylines(  const char*pName, Facet* pFacet)
      : ObjectFacet( pName, pFacet )
    {;}
    ObjectPolylines(  const std::string &pName, Facet* pFacet)
      : ObjectFacet( pName, pFacet )
    {;}
		
    ObjectType getObjType() const override { return ObjectType::ObjPolyline; }


    //---------------------------		
    void drawGL( ViewProps& pViewProps ) override
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
	  drawPointGL( pViewProps);					
	  break;
				
	default:
	  {
	  drawLineGL( pViewProps );					
	  drawPointGL( pViewProps);					
	    //	    std::cout << ">>>>>>>>>>>>Polyline" << std::endl;
	  //	    VisitorDrawPolyline	lVisitL( pViewProps, cMyProps);
	  //	    execVisitor( lVisitL );			
	  }
	}
    }
    //---------------------------		
    void selectGL( ViewProps& pViewProps ) override
    {
      // std::cout << ">>>>>>>>>>>>SelectPolyline" << std::endl; 
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
	    VisitorDrawSelectPolyline	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
	  }
	}
    }		 
  };
  //******************************

}

#endif
