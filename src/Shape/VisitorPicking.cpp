#include "VisitorPicking.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"
#include "SortVisitor.h"

namespace PP3d {


  using namespace std;

  //*********************************************

  void VisitorPickingFacet::PickingConcavFacet( Facet* pFacet ){

    // A FAIRE : OPTIMISER la recuperation des points !
	
	SortEntityVisitorPoint lVisit;
	pFacet->execVisitor( lVisit );
	//	std::cout << "Points:" << lVisit.cVectPoints.size()  << std::endl;
	
	glDepthMask( GL_FALSE );  // disable depth testing SINON ça donne n'importe quoi !!!!!!!!!!!!!!!!
	// disable writing to color buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
 
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
    
	// set stencil buffer to invert value on draw, 0 to 1 and 1 to 0
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
	   
  
	// We draw all triangles possible from 0 to other point two by two
	// use GL_TRIANGLE_FAN
	
	glBegin( GL_TRIANGLES );
	for( size_t i = 1; i+1 < lVisit.cVectPoints.size(); ++i )
	  {
	//	std::cout << "\t" << 0 <<" " << i <<" "<< i+1  << sFL_DOUBLEtd::endl;
	    glVertex3dv( lVisit.cVectPoints[0]->get().vectForGL() );	  
	    glVertex3dv( lVisit.cVectPoints[ i ]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[ i+1]->get().vectForGL() );
	  }
	glEnd();
	
	// enable color again
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	
	glDepthMask( GL_TRUE );  // !!!!!!!!!!!!!!!!!!!!!

	// set stencil buffer to only keep pixels when value in buffer is 1
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
 	
	glBegin( GL_TRIANGLES );
	for( size_t i = 1; i+1 < lVisit.cVectPoints.size(); ++i )
	  {
	    glVertex3dv( lVisit.cVectPoints[ 0]->get().vectForGL() );	  
	    glVertex3dv( lVisit.cVectPoints[ i ]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[i+1 ]->get().vectForGL() );
	  }
	glEnd();

	glDisable(GL_STENCIL_TEST);
  }
  //---------------------------	
  void VisitorPickingPoints::execBeginObject( ObjectPtr pObj )
  {
    //    std::cout << "============= VisitorPickingPoints::execBeginObject glDisable( GL_LIGHTING ) " << std::endl;
    //    glDisable( GL_LIGHTING );	 
  }
  //---------------------------
  void VisitorPickingPoints::execPoint( PointPtr pPt )
  {
    //		cViewProps.dragMat( pPt );
			
    cViewProps.pointGL(pPt->isSelect(), pPt->isHighlight(), pPt->isMagnet() );
    ColorRGBA::Id( pPt->getId());
    
    glBegin( GL_POINTS );
    glVertex3dv( pPt->cPt.vectForGL() );
    glEnd();
		
    //		cViewProps.undragMat();
  }
  //---------------------------	
  void VisitorPickingPoints::execEndObject(  ObjectPtr pObj )
  {
    //    glEnable(GL_LIGHTING);
  }		

  //*********************************************
  void VisitorPickingLine::execBeginObject( ObjectPtr pObj )
  {
    
    cViewProps.lineGL( pObj->isSelect(), pObj->isHighlight(), pObj->isMagnet() );
     ColorRGBA::Id( pObj->getId());
   
    //    glDisable( GL_LIGHTING );	 
  }
  //---------------------------	
  void VisitorPickingLine::execBeginLine( LinePtr pLine )
  {
    std::cout << " VisitorPickingLine::execBeginLine  "  << std::endl;
      //<< pLine->getFirst()->get()
      //						<< " " <<pLine->getSecond()->get() <<  std::endl;
    //		cViewProps.dragMat( pLine );

				
    if( pLine->isPoint() )
      {
	cViewProps.pointGL(pLine->isSelect(), pLine->isHighlight(), pLine->isMagnet());
        ColorRGBA::Id( pLine->getId());
        
	glBegin( GL_POINTS );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glEnd();
				
      }
    else
      {
        // std::cout << VisitorPickingLine::execBeginLine" << std::endl;
          
	cViewProps.lineGL( pLine->isSelect(), pLine->isHighlight(), pLine->isMagnet() );
        ColorRGBA::Id( pLine->getId());
   
	glBegin( GL_LINES );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glVertex3dv( pLine->getSecond()->get().vectForGL() );
	glEnd();
      }
    
    //		cViewProps.undragMat();
  }
  //*********************************************
  void VisitorPickingLine::execEndObject( ObjectPtr pObj )
  {
    //    glEnable( GL_LIGHTING );	 
  }
  //*********************************************
  
  void VisitorPickingObjectLine::execBeginObject( ObjectPtr pObj )
  {
    cViewProps.lineGL( pObj->isSelect(), pObj->isHighlight(), pObj->isMagnet() );
    ColorRGBA::Id( pObj->getId());

    //    glDisable( GL_LIGHTING );	 
  }
  //---------------------------	
  void VisitorPickingObjectLine::execBeginLine( LinePtr pLine )
  {
    		std::cout << " VisitorPickingLine::execBeginLine : " <<  std::endl;
                  //<< pLine->getFirst()->get()
    //							<< " " <<pLine->getSecond()->get() <<  std::endl;
    //		cViewProps.dragMat( pLine );
				
    if( pLine->isPoint() )
      {
	glBegin( GL_POINTS );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glEnd();
				
      }
    else
      {
	glBegin( GL_LINES );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glVertex3dv( pLine->getSecond()->get().vectForGL() );
	glEnd();
      }
		
    //		cViewProps.undragMat();
  }
  //*********************************************
  void VisitorPickingPolyline::execBeginFacet( FacetPtr pFacet )
  {
     //   glDisable( GL_LIGHTING );	  
    cViewProps.lineGL( pFacet->isSelect(), pFacet->isHighlight(), pFacet->isMagnet()  );
    ColorRGBA::Id( pFacet->getId());

  }
  //---------------------------	
  void VisitorPickingPolyline::execBeginLine( LinePtr pLine )
  {			
    if( pLine->isPoint() )
      {
	cViewProps.pointGL(pLine->isSelect(), pLine->isHighlight(), pLine->isMagnet());
        ColorRGBA::Id( pLine->getId());
	glBegin( GL_POINTS );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glEnd();
				 
      }
    glBegin( GL_LINES );
    glVertex3dv( pLine->getFirst()->get().vectForGL() );
    glVertex3dv( pLine->getSecond()->get().vectForGL() );
    glEnd();
  }
  //---------------------------	
  void VisitorPickingPolyline::execEndFacet( FacetPtr pFacet )
  {
    //	glVertex3dv( pLine->getSecond()->get().vectForGL() );
    glEnd();
    //    glEnable(GL_LIGHTING);
    //		cViewProps.undragMat();
  }

  //*********************************************
  void VisitorPickingFacet::execBeginFacet( FacetPtr pFacet )
  {
    std::cout << "VisitorPickingFacet::execBeginFacet select:" <<  pFacet->isSelect() << std::endl;
    //		cViewProps.dragMat( pFacet);
    //==================================    
		
    glNormal3dv( pFacet->getNormal().vectForGL() );
    
    /////// cViewProps.facetGL( pFacet->isSelect(), pFacet->isHighlight() , pFacet->isMagnet());
    ColorRGBA::Id( pFacet->getId());

    cNumLineEnd = (GLuint)(pFacet->getLines().size()-1);
    cNumLine = 0;
		
    execAfterBegin(pFacet);	
    if( pFacet->getLines().size() == 3 )
      {
	glBegin(GL_TRIANGLES);
	execAfterBegin(pFacet);	
      }
    else
      if( pFacet->isConcave() )
      {
	//////////	std::cout << "VisitorPickingPoly::execBeginFacet Concav:" <<  pFacet->getLines().size() << std::endl;
	execAfterBegin(pFacet);
	PickingConcavFacet( pFacet );    
      }      
    else if(  pFacet->getLines().size() == 4 )
      {   
	glBegin(GL_QUADS);
	execAfterBegin(pFacet);				       
      }
   else 
      {
	glBegin(GL_POLYGON);
	execAfterBegin(pFacet);
      }
  }

  //---------------------------	
  void VisitorPickingFacet::execBeginLine( LinePtr pLine )
  {

    glVertex3dv( pLine->getFirst()->get().vectForGL() );
    
    if( cNumLineEnd == cNumLine++ ){
      glVertex3dv( pLine->getSecond()->get().vectForGL() );
    }
  }
  //---------------------------	
  void VisitorPickingFacet::execEndFacet( FacetPtr pFacet )
  {
      glEnd();
 
    //		cViewProps.undragMat();
  }
  //*********************************************

  void VisitorPickingPoly::execBeginPoly(	Poly* pPoly )
  {
    //    glDisable( GL_LIGHTING );   // PP
    /////  cViewProps.facetGL( pPoly->isSelect(), pPoly->isHighlight(), pPoly->isMagnet()  );
    ColorRGBA::Id( pPoly->getId());
  }
  //---------------------------	
  void VisitorPickingPoly::execEndPoly(	Poly* pPoly )
  {
    //    glEnable( GL_LIGHTING ); // PP
  }
  //---------------------------	
  void VisitorPickingPoly::execBeginFacet( FacetPtr pFacet )
  {

    ColorRGBA lCurCol;
    lCurCol.getCurrentColor(); 
      
    std::cout << "VisitorPickingPoly::execBeginFacet " << lCurCol << std::endl;
    
    glNormal3dv( pFacet->getNormal().vectForGL() );

    cNumLineEnd = (GLuint)(pFacet->getLines().size()-1);
    cNumLine = 0;
    execAfterBegin(pFacet);	

    if(   pFacet->getLines().size() == 3 )
      {
	glBegin(GL_TRIANGLES);
	execAfterBegin(pFacet);
      }
    else
      if( pFacet->isConcave() )
      {
	//	std::cout << "VisitorPickingPoly::execBeginFacet Concav:" <<  pFacet->getLines().size() << std::endl;
	execAfterBegin(pFacet);
	PickingConcavFacet( pFacet );    
      }         
    else if(  pFacet->getLines().size() == 4 )
      {   
	glBegin(GL_QUADS);
	execAfterBegin(pFacet);
      }
    else 
      {
	glBegin( GL_POLYGON );
	execAfterBegin(pFacet);
      }					
  }
  //---------------------------	
  void VisitorPickingPoly::execEndFacet( FacetPtr pFacet )
  {
    glEnd();
  }
  //---------------------------	
  void VisitorPickingObject::execBeginObject( ObjectPtr pObject )
  {
    cout << "VisitorPickingObject::execBeginObject" << endl;
    //    cViewProps.facetGL( pObject->isSelect(), pObject->isHighlight(), pObject->isMagnet()  );
    ColorRGBA::Id( pObject->getId());
  }
  //---------------------------	
  void VisitorPickingObject::execEndObject( ObjectPtr pObject )
  {
    //		cViewProps.undragMat();
  }
  //---------------------------	
  void VisitorPickingObject::execBeginPoly( Poly* pPoly )
  {
  }
  void VisitorPickingObject::execEndPoly( Poly* pPoly )
  {
  }
  //*********************************************
} // namespace
