#include "VisitorPicking.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"


namespace PP3d {



  //*********************************************
  void VisitorPickingObjectLine::execBeginObject( ObjectPtr pObj )
  {
    std::cout << " VisitorPickingObjectLine::execBeginObject " << pObj->getId() << std::endl;
    ColorRGBA::Id(  pObj->getId()); // PP 
    //    glLoadName( pObj->getId() ); //PP
    VisitorDrawObjectLine::execBeginObject( pObj );	 
  }
  //*********************************************
  //*********************************************
  //*********************************************
  void VisitorPickingPoints::execPoint( PointPtr iVar )
  {	
    

    std::cout << " VisitorPickingPoints::execPoint " << iVar->getId() << std::endl;
   ColorRGBA::Id( iVar->getId());
     VisitorDrawPoints::execPoint( iVar );
    ColorRGBA::Zero();			
  }
  //--------------------------- ---	
  void VisitorPickingPoints::execEndObject( Object * iVar )
  {
    // Neutralise le glEnable(GL_LIGHTING);
  }		
  //*********************************************

  void VisitorPickingLine::execBeginLine( LinePtr iVar )
  {		
    std::cout << "VisitorPickingLine::execBeginLine" << iVar->getId() << std::endl;
    ColorRGBA::Id( iVar->getId());
    VisitorDrawLine::execBeginLine( iVar );
    ColorRGBA::Zero();			
  }
  void VisitorPickingLine::execEndObject( ObjectPtr pObj )
  {
    // Neutralise le glEnable(GL_LIGHTING);
  }
  //*********************************************
  void VisitorPickingFacet::execBeginFacet( FacetPtr iVar )
  {
    glDisable(GL_LIGHTING); 
    VisitorDrawFacet::execBeginFacet( iVar );				
  }
  //---------------------------	----	
  void VisitorPickingFacet::execAfterBegin( EntityPtr iVar)
  {
    std::cout << "VisitorPickingFacet::execAfterBegin" << iVar->getId() << std::endl;
    ColorRGBA::Id( iVar->getId());
  }
  //---------------------------	----	
  void VisitorPickingFacet::execEndFacet( FacetPtr iVar  )
  {
    glEnd();
    std::cout << "VisitorPickingPoly::execEndFacet" << iVar->getId() << std::endl;
    ColorRGBA::Zero();
    // Neutralise le     glEnable(GL_LIGHTING);
  }
  //*********************************************
  void VisitorPickingPoly::execBeginPoly( Poly* iVar )
  {
    std::cout << "VisitorPickingPoly::execBeginPoly id:" << iVar->getId() << std::endl;
    
    ColorRGBA::Id( iVar->getId());

    VisitorDrawPoly::execBeginPoly( iVar );				
  }
  //---------------------------	----	
  void VisitorPickingPoly::execEndPoly( Poly* iVar )
  {
    glEnd();
    ColorRGBA::Zero();							
    // Neutralise le     glEnable(GL_LIGHTING);
  }
  //*********************************************
  void VisitorPickingPolyline::execBeginFacet( FacetPtr iVar )
  {
    std::cout << "VisitorPickingPolyline::execBeginFacet" << std::endl;
    ColorRGBA::Id( iVar->getId());
    VisitorDrawPolyline::execBeginFacet( iVar );						
  }
  //---------------------------	
  void VisitorPickingPolyline::execEndFacet( FacetPtr iVar )
  {
    //	glVertex3dv( pLine->getSecond()->get().vectForGL() );
    VisitorDrawPolyline::execEndFacet( iVar );						
    std::cout << "VisitorPickingPolyline::execEndFacet" << std::endl;
    ColorRGBA::Zero();							
 }
 //*********************************************
  void VisitorPickingObject::execBeginObject( ObjectPtr iVar )
  {
    cId = iVar->getId();

    //    glDisable(GL_LIGHTING); 
 
    std::cout << " VisitorPickingObject::execBeginObject : "  << cId << std::endl;
    ColorRGBA::Id( cId );
        
   VisitorDrawObject::execBeginObject( iVar );				
  }
  //---------------------------	----	
  void VisitorPickingObject::execEndObject( ObjectPtr iVar )
  {
    glEnd();
    ColorRGBA::Zero();											
  }
  //---------------------------	----	
  /*  void VisitorPickingColorObject::execBeginFacet( FacetPtr iVar )
  {
    glDisable(GL_LIGHTING); 
    VisitorDrawFacet::execBeginFacet( iVar );				
  }
  */
  //---------------------------	----	
  void VisitorPickingObject::execAfterBegin( EntityPtr iVar)
  {
    std::cout << " VisitorPickingColorObject::execAfterBegin : "  << cId << std::endl;

    ColorRGBA::Id( cId ); // L'id de l'objet
  }

  //*********************************************
  

} // namespace
