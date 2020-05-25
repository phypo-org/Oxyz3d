#ifndef H__ObjectLine__H
#define H__ObjectLine__H


#include "Calcul3d.h"

#include "Object.h"
#include "Entity.h"


namespace PP3d {

	
  class Line;
  //******************************
	
  class ObjectLine  : public Object {

    Line*      cShape;

  public:
    ObjectLine( const char*pName, Line* pLine )
      : Object( pName )
      ,cShape( pLine)
    {;}
    ObjectLine( const std::string& pName, Line* pLine )
      : Object( pName )
      ,cShape( pLine)
    {;}
   
    EntityPtr  getShape() override { return cShape; }
    LinePtr    getLine()           { return cShape; }
    ObjectType getObjType() const override { return ObjectType::ObjLine; }
		
    ShapeType getSubType() const override { return ShapeType::Line; }

		
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
	  break;
				
	default:
	  {
	    VisitorDrawObjectLine	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
						
	  }
	}
    }
    //---------------------------		
    void selectGL( ViewProps& pViewProps ) override
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
	    VisitorDrawSelectObjectLine	lVisitL( pViewProps, cMyProps);
	    execVisitor( lVisitL );			
					
	  }
	}
    }
  };
  //******************************  
  inline ObjectLine * MakeObjectLine( const char*pName, const Point3d & iA,  const Point3d & iB )
  {
    return new ObjectLine( pName, MakeLine( iA, iB ));
  }
}

#endif
