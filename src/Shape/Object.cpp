
#include "Object.h"


#include "ViewProps.h"


#include "GLUtility.h"
#include "Selection.h"
#include "PP3dType.h"


#include <algorithm>


namespace PP3d{


  //********************************

  Object::Object(  const char*pName)
    :cName(pName)
  {
  }
  Object::Object(  const std::string& pName)
    :cName(pName)
  {
  }
  //---------------------------
  Object::~Object()
  {
  }
  //-------------------------------
  void Object::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginObject( this );
		
    getShape()->execVisitor( pVisit );
			
    pVisit.execEndObject( this );
  }
  //-------------------------------
  void Object::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode( this, nullptr );	// Object n'a pas de owner !
    pVisit.execNode( getShape(), this );
		
    getShape()->execVisitor( pVisit );
			
    pVisit.execEndNode( getShape(), this );
    pVisit.execEndNode( this, nullptr );	// Object n'a pas de owner !
  }
  //---------------------------
  void Object::drawPointGL(ViewProps& pViewProps )
  {
    VisitorDrawPoints	lVisitP( pViewProps, cMyProps);
    execVisitor( lVisitP );
  }
  //---------------------------
  void Object::drawLineGL(ViewProps& pViewProps )
  {
    VisitorDrawLine	lVisitL( pViewProps, cMyProps);
    execVisitor( lVisitL );
  }
  //---------------------------
  void Object::drawFacetGL(ViewProps& pViewProps )
  {
    VisitorDrawFacet	lVisitF( pViewProps, cMyProps);
    execVisitor( lVisitF );
  }		
  //---------------------------
  void Object::drawPolyGL(ViewProps& pViewProps )
  {
    VisitorDrawPoly	lVisitY( pViewProps, cMyProps);
    execVisitor( lVisitY );
  }
  //---------------------------
  void Object::drawObjectGL(ViewProps& pViewProps )
  {
    VisitorDrawObject	lVisitO( pViewProps, cMyProps);
    execVisitor( lVisitO );
  }
  //---------------------------
  void Object::drawGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }
    //		std::cout << "Object::drawGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;
	 
    if( pViewProps.cGLMode != GLMode::Select  )
      {
	//  CHANGER DE PLACE : a ne faire quand cas de changement de l'objet 
	recomputeAll( cMyProps );
      }

    switch( pViewProps.cSelectType )
      {
      case SelectType::All:
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL(pViewProps);
					
	  drawLineGL( pViewProps );					
	  drawPointGL( pViewProps);
				
	}
	break;
				
      case SelectType::Line:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
				
				
      case SelectType::Facet:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps	);		
	}
	break;

      case SelectType::Poly:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawPolyGL(pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
				
      case SelectType::Object:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawObjectGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
  //---------------------------
  void Object::drawSelectPointGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectPoints	lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectLineGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectLine	lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectFacetGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectFacet lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectPolyGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectPoly lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectObjectGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectObject lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::selectGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }
    //		std::cout << "Object::selectGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;


    switch( pViewProps.cSelectType )
      {
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawPolyGL( pViewProps);
										
	  drawLineGL( pViewProps );									
	  drawSelectPointGL( pViewProps );
	}
	break;
				
				
      case SelectType::Line:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawPolyGL( pViewProps);
					
	  drawSelectLineGL( pViewProps );
	}
	break;
								
      case SelectType::Facet:
	{
	  drawLineGL( pViewProps);					
	  drawSelectFacetGL( pViewProps );
	}
	break;

      case SelectType::Poly:
	{
	  drawLineGL( pViewProps);					
	  drawSelectPolyGL( pViewProps );
	}
	break;
				
      case SelectType::Object:
	{
	  drawLineGL( pViewProps);
	  drawSelectObjectGL( pViewProps );
	}
	break;
			
      case SelectType::All:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawSelectFacetGL( pViewProps );
					
	  drawSelectLineGL( pViewProps);					
	  drawSelectPointGL( pViewProps );
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
  //---------------------------
  void Object::recomputeAll(ObjProps&pProps)
  {
    VisitorNormalFacet lVisit;

    execVisitor( lVisit );
  }
  //---------------------------
  /*
    void Object::drawInfoGL(  ViewProps& pViewProps, ObjProps& pObjProps ) 
    {
    VisitorDrawPoints	lVisit( pViewProps, pObjProps);;
    execVisitor( lVisit );
    }
  */
  //---------------------------
  //---------------------------
  /*
    void Object::drawSelectionGL( ViewProps& pViewProps )
    {
		
    if( cMyProps.cVisible == false )
    return;

    // Pas une bonne idée !
		
    //		drawGL( pViewProps );

	 
    Point3d lSize3d = cMyProps.cMinMaxBox.size();
    PDouble lSize = std::max(lSize3d.cX,lSize3d.cY )  ;
    lSize= std::max( lSize3d.cZ, lSize);
		
    ColorRGBA::Yellow();
    GLUtility::DrawCursorCruz( cMyProps.cCenter, lSize);
		
    //	GLUtility::DrawMinMaxBox( cMyProps.cMinMaxBox );
    }
  */
  //---------------------------

  //---------------------------
  PointPtrHash Object::getPoints()
  {
    VisitorGetPoints<PointPtrHash> lVisit;
    getShape()->execVisitor( lVisit );
		
    return lVisit.getPoints();
  }
  //********************************
}
