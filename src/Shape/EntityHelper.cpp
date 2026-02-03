#include "Object.h"
#include "ViewProps.h"
#include "VisitorDraw.h"
#include "VisitorPicking.h"
#include "Selection.h"

namespace PP3d {

  //*********************************************

  //---------------------------
  //----------- DRAW ----------
  //---------------------------
  void Entity::drawPointGL(ViewProps& pViewProps )
  {
    //        std:: cout << "Entity::drawPointGL " << pViewProps << std::endl;
    VisitorDrawPoints	lVisitP( pViewProps, cMyProps);
    execVisitor( lVisitP );
  }
  //---------------------------
  void Entity::drawLineGL(ViewProps& pViewProps )
  {
    //    std::cout << "********* Entity::drawLineGL Obj:" << getShapeType()  << std::endl;
    
    VisitorDrawLine	lVisitL( pViewProps, cMyProps);
    execVisitor( lVisitL );
  }
  //---------------------------
  void Entity::drawFacetGL(ViewProps& pViewProps )
  {    
    if( is( ShapeType::Facet ))
      {
        //        std::cout << "********* Entity::drawFacetGL Obj:" << getShapeType() << std::endl;
        VisitorDrawFacet	lVisitF( pViewProps, cMyProps);
        execVisitor( lVisitF );
      }
  }		
  //---------------------------
  void Entity::drawPolyGL(ViewProps& pViewProps )
  {
    //    std::cout << "********* Entity::drawPolyGL Obj:" << getShapeType()  << std::endl;
    VisitorDrawPoly	lVisitP( pViewProps, cMyProps);
    execVisitor( lVisitP );
  }
  //---------------------------
  void Entity::drawObjectGL(ViewProps& pViewProps )
  {
    //    std::cout << "********* Entity::drawObjectGL Obj:" << getShapeType()  << std::endl;
    VisitorDrawObject	lVisitO( pViewProps, cMyProps);
    execVisitor( lVisitO );
  }

  //---------------------------
  //--------- SELECT ----------
  //---------------------------
  void Entity::drawSelectPointGL(ViewProps& pViewProps )
  {
    VisitorPickingPoints	lVisitP( pViewProps, cMyProps);
    execVisitor( lVisitP );
  }
  //---------------------------
  void Entity::drawSelectLineGL(ViewProps& pViewProps )
  {
    VisitorPickingLine	lVisitL( pViewProps, cMyProps);
    execVisitor( lVisitL );
  }
  //---------------------------
  void Entity::drawSelectFacetGL(ViewProps& pViewProps )
  {    
    if( is( ShapeType::Facet ))
      {
        VisitorPickingFacet	lVisitF( pViewProps, cMyProps);
        execVisitor( lVisitF );
      }
  }		
  //---------------------------
  void Entity::drawSelectPolyGL(ViewProps& pViewProps )
  {
    VisitorPickingPoly	lVisitY( pViewProps, cMyProps);
    execVisitor( lVisitY );
  }
  //---------------------------
  void Entity::drawSelectObjectGL(ViewProps& pViewProps )
  {
    std::cout << "   Entity::drawSelectObjectGL id:" << getId() << std::endl;
    
    VisitorPickingObject	lVisitO( pViewProps, cMyProps);
    execVisitor( lVisitO );
  }
  
  //---------------------------
  void Entity::drawGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }
    //		std::cout << "Entity::drawGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;
	 
    //   if( pViewProps.cGLMode != GLMode::Select  )
      {
	//  CHANGER DE PLACE : a ne faire quand cas de changement de l'objet 
	///////	recomputeAll( cMyProps,  );
      }


      //      std::cout << "********* Entity::drawGL Obj:" << getShapeType()  << std::endl;

    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL )
            {
              glEnable(GL_LIGHTING);
               drawPolyGL(pViewProps);
            }
					
          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps	);		
	}
	break;

      case SelectType::All:
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL )
            {
              glEnable(GL_LIGHTING);
              drawPolyGL(pViewProps);
            }
					
          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps );
	    
	  drawPointGL( pViewProps);
				
	}
	break;
				
      case SelectType::Line:
	{
	  //	  std::cout << "Entity::drawGL line cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;	  if( pViewProps.cViewMode == ViewMode::FULL)

          if( pViewProps.cViewMode == ViewMode::FULL)		
            {
              glEnable(GL_LIGHTING);
             drawPolyGL(pViewProps);
            }
					
          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps );			
	}
	break;
				
				
      case SelectType::Facet:
	{
          std::cout << "Entity::drawGL Facet " <<  std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)
            {
              glEnable(GL_LIGHTING);
              drawPolyGL(pViewProps);
            }

          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps );		
	}
	break;

      case SelectType::Poly:
	{
          std::cout << "Entity::drawGL poly " <<  std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)
            {
              glEnable(GL_LIGHTING);
              drawPolyGL(pViewProps);
            }
	  
          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps );			
	}
	break;
				
      case SelectType::Group:
      case SelectType::Object:
	{
          std::cout << "Entity::drawGL object" << std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)
            {
              glEnable(GL_LIGHTING);
              drawObjectGL( pViewProps);
            }

          glDisable(GL_LIGHTING);
	  drawLineGL( pViewProps );			
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
    //---------------------------
  void Entity::selectGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }
    
    EntityId lId =  getId();
    ColorRGBA::Id( getId());


    
    ColorRGBA lCurCol;
    lCurCol.getCurrentColor();

    
    std::cout << "*** Entity::selectGL -> " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  " id:" << lId <<  "  -> " << lCurCol << std::endl;

    glDisable(GL_LIGHTING);
    
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;
	
      case SelectType::Point:
	{
  	  if( pViewProps.cViewMode == ViewMode::FULL)
            drawPolyGL( pViewProps);
	  
	  drawSelectPointGL( pViewProps );
	}
	break;
				
				
      case SelectType::Line:
	{
  	  if( pViewProps.cViewMode == ViewMode::FULL)
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
				
      case SelectType::Group:
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
    
  //*********************************************

}
