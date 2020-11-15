#include "EntityVisitorPicking.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"


namespace PP3d {


  //*********************************************
  void VisitorDrawSelectPoints::execPoint( PointPtr pPt )
  {	
    glLoadName( pPt->getId() );
    VisitorDrawPoints::execPoint( pPt );
    glLoadName( 0 );			
  }
  //*********************************************

  void VisitorDrawSelectLine::execBeginLine( LinePtr pLine )
  {		
    glLoadName( pLine->getId() );
    VisitorDrawLine::execBeginLine( pLine );
    glLoadName( 0 );				
  }	
  //*********************************************
  void VisitorDrawSelectFacet::execBeginFacet( FacetPtr pFacet )
  {
    glLoadName( pFacet->getId() );
    VisitorDrawFacet::execBeginFacet( pFacet );				
  }
  //---------------------------	----	
  void VisitorDrawSelectFacet::execEndFacet( FacetPtr pFacet )
  {
    glEnd();
    glLoadName( 0 );				
  }
  //*********************************************
  void VisitorDrawSelectPoly::execBeginPoly( Poly* pPoly )
  {
    glLoadName( pPoly->getId() );
    VisitorDrawPoly::execBeginPoly( pPoly );				
  }
  //---------------------------	----	
  void VisitorDrawSelectPoly::execEndPoly( Poly* pPoly )
  {
    glEnd();
    glLoadName( 0 );				
  }
  //*********************************************
  void VisitorDrawSelectPolyline::execBeginFacet( FacetPtr pFacet )
  {
    glLoadName( pFacet->getId() );
    VisitorDrawPolyline::execBeginFacet( pFacet );						
  }
  //---------------------------	
  void VisitorDrawSelectPolyline::execEndFacet( FacetPtr pFacet )
  {
    //	glVertex3dv( pLine->getSecond()->get().vectForGL() );
    VisitorDrawPolyline::execEndFacet( pFacet );						
    glLoadName( 0 );
  }
  //*********************************************
  void VisitorDrawSelectObject::execBeginObject( ObjectPtr pObject )
  {
    glLoadName( pObject->getId() );
    VisitorDrawObject::execBeginObject( pObject );				
  }
  //---------------------------	----	
  void VisitorDrawSelectObject::execEndObject( ObjectPtr pObject )
  {
    glEnd();
    glLoadName( 0 );				
  }
  //*********************************************
  void VisitorDrawSelectObjectLine::execBeginObject( ObjectPtr pObj )
  {
    glLoadName( pObj->getId() );
    VisitorDrawObjectLine::execBeginObject( pObj );	 
  }
  //*********************************************
  //*********************************************
  //*********************************************
  void VisitorDrawSelectColorPoints::execPoint( PointPtr iVar )
  {	
    ColorRGBA::Id( iVar->getId());
    VisitorDrawPoints::execPoint( iVar );
    ColorRGBA::Zero();			
  }
  //*********************************************

  void VisitorDrawSelectColorLine::execBeginLine( LinePtr iVar )
  {		
    ColorRGBA::Id( iVar->getId());
    VisitorDrawLine::execBeginLine( iVar );
    ColorRGBA::Zero();			
  }	
  //*********************************************
  void VisitorDrawSelectColorFacet::execBeginFacet( FacetPtr iVar )
  {
    glDisable(GL_LIGHTING); 
    VisitorDrawFacet::execBeginFacet( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectColorFacet::execAfterBegin( EntityPtr iVar)
  {
    ColorRGBA::Id( iVar->getId());
  }
  //---------------------------	----	
  void VisitorDrawSelectColorFacet::execEndFacet( FacetPtr iVar  )
  {
    glEnd();
    ColorRGBA::Zero();			
  }
  //*********************************************
  void VisitorDrawSelectColorPoly::execBeginPoly( Poly* iVar )
  {
    ColorRGBA::Id( iVar->getId());

    VisitorDrawPoly::execBeginPoly( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectColorPoly::execEndPoly( Poly* iVar )
  {
    glEnd();
    ColorRGBA::Zero();							
  }
  //*********************************************
  void VisitorDrawSelectColorObject::execBeginObject( ObjectPtr iVar )
  {
    cId = iVar->getId();
    glDisable(GL_LIGHTING); 
 
    std::cout << " VisitorDrawSelectColorObject::execBeginObject : "  << cId << std::endl;
    
   VisitorDrawObject::execBeginObject( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectColorObject::execEndObject( ObjectPtr iVar )
  {
    glEnd();
    ColorRGBA::Zero();											
  }
  //---------------------------	----	
  /*  void VisitorDrawSelectColorObject::execBeginFacet( FacetPtr iVar )
  {
    glDisable(GL_LIGHTING); 
    VisitorDrawFacet::execBeginFacet( iVar );				
  }
  */
  //---------------------------	----	
  void VisitorDrawSelectColorObject::execAfterBegin( EntityPtr iVar)
  {
    std::cout << " VisitorDrawSelectColorObject::execAfterBegin : "  << cId << std::endl;

    ColorRGBA::Id( cId ); // L'id de l'objet
  }

  //*********************************************
  

} // namespace
