#include "EntityVisitorPicking.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"


namespace PP3d {



  //*********************************************
  void VisitorDrawSelectObjectLine::execBeginObject( ObjectPtr pObj )
  {
    glLoadName( pObj->getId() );
    VisitorDrawObjectLine::execBeginObject( pObj );	 
  }
  //*********************************************
  //*********************************************
  //*********************************************
  void VisitorDrawSelectPoints::execPoint( PointPtr iVar )
  {	
    ColorRGBA::Id( iVar->getId());
    VisitorDrawPoints::execPoint( iVar );
    ColorRGBA::Zero();			
  }
  //*********************************************

  void VisitorDrawSelectLine::execBeginLine( LinePtr iVar )
  {		
    ColorRGBA::Id( iVar->getId());
    VisitorDrawLine::execBeginLine( iVar );
    ColorRGBA::Zero();			
  }	
  //*********************************************
  void VisitorDrawSelectFacet::execBeginFacet( FacetPtr iVar )
  {
    glDisable(GL_LIGHTING); 
    VisitorDrawFacet::execBeginFacet( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectFacet::execAfterBegin( EntityPtr iVar)
  {
    ColorRGBA::Id( iVar->getId());
  }
  //---------------------------	----	
  void VisitorDrawSelectFacet::execEndFacet( FacetPtr iVar  )
  {
    glEnd();
    ColorRGBA::Zero();			
  }
  //*********************************************
  void VisitorDrawSelectPoly::execBeginPoly( Poly* iVar )
  {
    ColorRGBA::Id( iVar->getId());

    VisitorDrawPoly::execBeginPoly( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectPoly::execEndPoly( Poly* iVar )
  {
    glEnd();
    ColorRGBA::Zero();							
  }
  //*********************************************
  void VisitorDrawSelectPolyline::execBeginFacet( FacetPtr iVar )
  {
    ColorRGBA::Id( iVar->getId());
    VisitorDrawPolyline::execBeginFacet( iVar );						
  }
  //---------------------------	
  void VisitorDrawSelectPolyline::execEndFacet( FacetPtr iVar )
  {
    //	glVertex3dv( pLine->getSecond()->get().vectForGL() );
    VisitorDrawPolyline::execEndFacet( iVar );						
     ColorRGBA::Zero();							
 }
 //*********************************************
  void VisitorDrawSelectObject::execBeginObject( ObjectPtr iVar )
  {
    cId = iVar->getId();
    glDisable(GL_LIGHTING); 
 
    //    std::cout << " VisitorDrawSelectColorObject::execBeginObject : "  << cId << std::endl;
    
   VisitorDrawObject::execBeginObject( iVar );				
  }
  //---------------------------	----	
  void VisitorDrawSelectObject::execEndObject( ObjectPtr iVar )
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
  void VisitorDrawSelectObject::execAfterBegin( EntityPtr iVar)
  {
    //    std::cout << " VisitorDrawSelectColorObject::execAfterBegin : "  << cId << std::endl;

    ColorRGBA::Id( cId ); // L'id de l'objet
  }

  //*********************************************
  

} // namespace
