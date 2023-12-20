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
    ObjectLine( const char*pName, Line* pLine, bool iTransform=false )
      : Object( pName, iTransform )
      ,cShape( pLine)
    {;}
    ObjectLine( const std::string& pName, Line* pLine, bool iTransform=false )
      : Object( pName, iTransform )
      ,cShape( pLine)
    {;}
   
    EntityPtr  getShape() override { return cShape; }
    LinePtr    getLine()           { return cShape; }
    PointPtr   first()  const { return cShape->first();}
    PointPtr   second() const { return cShape->second();}
    Point3d &  first3d()  const { return cShape->first()->get();}
    Point3d &  second3d() const { return cShape->second()->get();}
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

    
    //---------------------------		
    PP3d::Point3d getAxis()
    {
      PP3d::Point3d  lAxis = second3d() - first3d();
      return lAxis;
    }
    //---------------------------		
    PP3d::Point3d getVectorOrigin() { return first3d(); }
    //---------------------------		
    PP3d::Point3d getVector() { return getAxis();}

  };
  //******************************  
  inline ObjectLine * MakeObjectLine( const char*pName, const Point3d & iA,  const Point3d & iB, bool iIsTransform  )
  {
    return new ObjectLine( pName, MakeLine( iA, iB ), iIsTransform );
  }
}

#endif
