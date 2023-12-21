
#include "Object.h"


#include "ViewProps.h"


#include "GLUtility.h"
#include "Selection.h"
#include "PP3dType.h"

#include "EntityVisitorPicking.h"

#include <algorithm>


namespace PP3d{


  //********************************

  Object::Object(  const char*pName, ClassType  iClass )
    :cName(pName)
    ,cClassType( iClass )
    ,cDateCreation( PPu::PPDate::GetCurrentDateTime70() )
  {
  }
  Object::Object(  const std::string& pName,  ClassType  iClass)
    :cName(pName)
    ,cClassType( iClass )
    ,cDateCreation( PPu::PPDate::GetCurrentDateTime70() )
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
	///////	recomputeAll( cMyProps,  );
      }

    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps	);		
	}
	break;

      case SelectType::All:
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == 0 )
	    drawFacetGL(pViewProps);
					
	  drawLineGL( pViewProps );
	    
	  drawPointGL( pViewProps);
				
	}
	break;
				
      case SelectType::Line:
	{
	  //	  std::cout << "Object::drawGL line cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
				
				
      case SelectType::Facet:
	{
	  if( pViewProps.cViewMode == 0)						
	    drawFacetGL( pViewProps);

	  //	  std::cout << "Object::drawGL facet  cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
   
	  drawLineGL( pViewProps );		
	}
	break;

      case SelectType::Poly:
	{
	  //	  std::cout << "Object::drawGL poly cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == 0)						
	    drawPolyGL(pViewProps);
	  
	  drawLineGL( pViewProps );			
	}
	break;
				
      case SelectType::Object:
	{
	  //	  std::cout << "Object::drawGL object cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == 0)						
	    drawObjectGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
 
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
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

    //    std::cout << "*** Object::selectGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;

    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;
	
      case SelectType::Point:
	{
  	  if( pViewProps.cViewMode == 0)						
	    drawPolyGL( pViewProps);
	  
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
	  drawSelectFacetGL( pViewProps );
	}
	break;

      case SelectType::Poly:
	{
	  drawSelectPolyGL( pViewProps );
	}
	break;
				
      case SelectType::Object:
	{
	  drawSelectObjectGL( pViewProps );
	}
	break;
			
      case SelectType::All:
	{
	  drawSelectFacetGL( pViewProps );					
	  drawSelectLineGL( pViewProps);					
	  drawSelectPointGL( pViewProps );
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
  //---------------------------
  int Object::recomputeAll(ObjProps&pProps, Compute iCompute)
  {

    switch( iCompute ){
    case Compute::Nothing :
      break;
      
    case Compute::FacetNormal:
      {
	//    std::cout << "   Object::recomputeAll Normal" << std::endl;
    VisitorNormalFacet lVisitN;
	execVisitor( lVisitN );
      }
      break;
      
    case  Compute::FacetConcave:
      {
	VisitorConcaveFacet lVisitC;
	//    std::cout << "   Object::recomputeAll Cancav" << std::endl;
	execVisitor( lVisitC );
	return lVisitC.count();
      }
      break;
      
    case  Compute::FacetAll:
      {
	VisitorNormalFacet lVisitN;
	execVisitor( lVisitN );
	VisitorConcaveFacet lVisitC;
	//    std::cout << "   Object::recomputeAll All" << std::endl;
	execVisitor( lVisitC );
	return lVisitC.count();
      }
      break;
    }
    
    return 0;
  }

 //********************************
}
