#include "EntityVisitorDraw.h"

#include "Entity.h"
#include "Object.h"
#include "ViewProps.h"
#include "ObjProps.h"
#include "SortVisitor.h"

namespace PP3d {


  //*********************************************

  void DrawNormalFacet( Facet* pFacet )
  {

    PointPtrSet lPoints;
    VisitorGetPoints lVisit(lPoints);
		
    pFacet->execVisitor( lVisit );
		
    //		std::cout << " <DEBUG:" << lPoints.size() << '>' ;
		
    Point3d lMiddle;
		
    for(  const PP3d::PointPtr pPoint : lPoints )
      {
	lMiddle += pPoint->cPt;
      }
    lMiddle /= (double)lPoints.size();
		
    ColorRGBA::Green();
    Point3d lNormMiddle( lMiddle );
    Point3d lNorm10( pFacet->getNormal() );
		
    //////    lNorm10 *=  10; // facteur de zoom eventuel
		
    lNormMiddle += lNorm10;
		
    glLineWidth( 1 );
    glDisable( GL_LIGHTING );
		
    glBegin( GL_LINE_STRIP ); 
		
    glVertex3dv( lMiddle.vectForGL() );
    glVertex3dv( lNormMiddle.vectForGL() );
		
    glEnd();
		
    glEnable( GL_LIGHTING );
			
  }
  //---------------------------	
  void VisitorDrawPoints::execBeginObject( ObjectPtr pObj )
  {
    glDisable( GL_LIGHTING );	 
  }
  //---------------------------
  void VisitorDrawPoints::execPoint( PointPtr pPt )
  {
    //		cViewProps.dragMat( pPt );
			
    cViewProps.pointGL(pPt->isSelect(), pPt->isHighlight() );
    glBegin( GL_POINTS );
    glVertex3dv( pPt->cPt.vectForGL() );
    glEnd();
		
    //		cViewProps.undragMat();
  }
  //---------------------------	
  void VisitorDrawPoints::execEndObject(  ObjectPtr pObj )
  {
    glEnable(GL_LIGHTING);
  }		

  //*********************************************
  void VisitorDrawLine::execBeginObject( ObjectPtr pObj )
  {
    glDisable( GL_LIGHTING );	 
  }
  //---------------------------	
  void VisitorDrawLine::execBeginLine( LinePtr pLine )
  {
    //		std::cout << " VisitorDrawLine::execBeginLine : " << pLine->getFirst()->get()
    //							<< " " <<pLine->getSecond()->get() <<  std::endl;
    //		cViewProps.dragMat( pLine );

				
    if( pLine->isPoint() )
      {
	cViewProps.pointGL(pLine->isSelect(), pLine->isHighlight());
	glBegin( GL_POINTS );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glEnd();
				
      }
    else
      {
	cViewProps.lineGL( pLine->isSelect(), pLine->isHighlight() );
	glBegin( GL_LINES );
	glVertex3dv( pLine->getFirst()->get().vectForGL() );
	glVertex3dv( pLine->getSecond()->get().vectForGL() );
	glEnd();
      }		
    //		cViewProps.undragMat();
  }
  //*********************************************
  void VisitorDrawLine::execEndObject( ObjectPtr pObj )
  {
    glEnable( GL_LIGHTING );	 
  }
  //*********************************************
  void VisitorDrawObjectLine::execBeginObject( ObjectPtr pObj )
  {
    cViewProps.lineGL( pObj->isSelect(), pObj->isHighlight() );
    glDisable( GL_LIGHTING );	 
  }
  //---------------------------	
  void VisitorDrawObjectLine::execBeginLine( LinePtr pLine )
  {
    //		std::cout << " VisitorDrawLine::execBeginLine : " << pLine->getFirst()->get()
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
  void VisitorDrawPolyline::execBeginFacet( FacetPtr pFacet )
  {
    glDisable( GL_LIGHTING );	  
    cViewProps.lineGL( pFacet->isSelect(), pFacet->isHighlight()  );
  }
  //---------------------------	
  void VisitorDrawPolyline::execBeginLine( LinePtr pLine )
  {			
    if( pLine->isPoint() )
      {
	cViewProps.pointGL(pLine->isSelect(), pLine->isHighlight());
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
  void VisitorDrawPolyline::execEndFacet( FacetPtr pFacet )
  {
    //	glVertex3dv( pLine->getSecond()->get().vectForGL() );
    glEnd();
    glEnable(GL_LIGHTING);
    //		cViewProps.undragMat();
  }

  //*********************************************
  void VisitorDrawFacet::execBeginFacet( FacetPtr pFacet )
  {
    //		cViewProps.dragMat( pFacet);
    //==================================
    if( cViewProps.cFlagViewNormal == true
	&& cViewProps.cGLMode == GLMode::Draw )
      {
	DrawNormalFacet( pFacet );
      }
    //==================================
		
    glNormal3dv( pFacet->getNormal().vectForGL() );
    cViewProps.facetGL( pFacet->isSelect(), pFacet->isHighlight()  );

    cNumLineEnd = (GLuint)(pFacet->getLines().size()-1);
    cNumLine = 0;
		
    if( pFacet->getLines().size() == 3 )
      {
	glBegin(GL_TRIANGLES);
	execAfterBegin(pFacet);	
      }
    	
    else if(  pFacet->getLines().size() == 4 )
      {   
	glBegin(GL_QUADS);
	execAfterBegin(pFacet);				       
      }
    
    /*
     else 
      {
		glBegin( GL_POLYGON );
		execAfterBegin(pFacet);
	    }
    */

    //	cNoDraw =
    /*
    else{
      // prepare stencil buffer
      glEnable(GL_STENCIL_TEST);
      glClear(GL_STENCIL_BUFFER_BIT);
    
      // set stencil buffer to invert value on draw, 0 to 1 and 1 to 0
      glStencilFunc(GL_ALWAYS, 0, 1);
      glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
    
      // disable writing to color buffer
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

      SortEntityVisitorPoint lVisit;
      pFacet->execVisitor( lVisit );
      std::cout << "Points:" << lVisit.cVectPoints.size()  << std::endl;

      // draw polygon into stenci      glDisable(GL_STENCIL_TEST);
l buffer  drawVertices(GL_TRIANGLE_FAN, vertices, n_vertices);
      glBegin( GL_TRIANGLE_FAN);
      for( Point * lPt : lVisit.cVectPoints )
	{
	  glVertex3dv( lPt->get().vectForGL() );
	}
     
      // set stencil buffer to only keep pixels when value in buffer is 
      glStencilFunc(GL_EQUAL, 1, 1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
      // enable color again
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    
      glBegin( GL_TRIANGLE_FAN);
      for( Point * lPt : lVisit.cVectPoints )
	{
	  glVertex3dv( lPt->get().vectForGL() );
	}
      glDisable(GL_STENCIL_TEST);
    }
    */
   else 
      {
	// REGUPERER TOUT LES POINTS DE LA FACETTES
	
	SortEntityVisitorPoint lVisit;
	pFacet->execVisitor( lVisit );
	//	std::cout << "Points:" << lVisit.cVectPoints.size()  << std::endl;
	
	//    	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
    
	// set stencil buffer to invert value on draw, 0 to 1 and 1 to 0
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
	
	//	glStencilFunc(GL_ALWAYS, 0, 1);
	//	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
   
	// disable writing to color buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  
	// On trace tout les triangles entre le premier point et les autres
	
	glBegin( GL_TRIANGLES );
	for( size_t i = 1; i+1 < lVisit.cVectPoints.size()/2; ++i )
	  {
	//	std::cout << "\t" << 0 <<" " << i <<" "<< i+1  << sFL_DOUBLEtd::endl;
	    glVertex3dv( lVisit.cVectPoints[ 0]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[ i ]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[i+1]->get().vectForGL() );	  
	  }
	glEnd();
	
	// enable color again
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	
	// set stencil buffer to only keep pixels when value in buffer is 1
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	
	glColor3f(0.1, 0.8, 0.1);
	
	glBegin( GL_TRIANGLES );
	for( size_t i = 1; i+1 < lVisit.cVectPoints.size(); ++i )
	  {
	    glVertex3dv( lVisit.cVectPoints[ 0 ]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[ i ]->get().vectForGL() );
	    glVertex3dv( lVisit.cVectPoints[i+1]->get().vectForGL() );	  
	  }
	glEnd();
	
	//	glClear(GL_STENCIL_BUFFER_BIT);	  

	glDisable(GL_STENCIL_TEST);
	//	  glBegin( GL_POLYGON );
	//	execAfterBegin(pFacet);
      }
  }

  //---------------------------	
  void VisitorDrawFacet::execBeginLine( LinePtr pLine )
  {	
    glVertex3dv( pLine->getFirst()->get().vectForGL() );
    
    if( cNumLineEnd == cNumLine++ ){
      glVertex3dv( pLine->getSecond()->get().vectForGL() );
    }
  }
  //---------------------------	
  void VisitorDrawFacet::execEndFacet( FacetPtr pFacet )
  {
      glEnd();
 
    //		cViewProps.undragMat();
  }
  //*********************************************

  void VisitorDrawPoly::execBeginPoly(	Poly* pPoly )
  {	
    cViewProps.facetGL( pPoly->isSelect(), pPoly->isHighlight()  );
  }
  //---------------------------	
  void VisitorDrawPoly::execEndPoly(	Poly* pPoly )
  {
  }
  //---------------------------	
  void VisitorDrawPoly::execBeginFacet( FacetPtr pFacet )
  {
    //==================================
    if( cViewProps.cFlagViewNormal == true
	&& cViewProps.cGLMode == GLMode::Draw )
      {
	DrawNormalFacet( pFacet );
      }
    //==================================

    glNormal3dv( pFacet->getNormal().vectForGL() );

    cNumLineEnd = (GLuint)(pFacet->getLines().size()-1);
    cNumLine = 0;

    if(   pFacet->getLines().size() == 3 )
      {
	glBegin(GL_TRIANGLES);
	execAfterBegin(pFacet);
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
  void VisitorDrawPoly::execEndFacet( FacetPtr pFacet )
  {
    glEnd();
  }
  void VisitorDrawObject::execBeginObject( ObjectPtr pObject )
  {
    //		cViewProps.dragMat( pObject );
    cViewProps.facetGL( pObject->isSelect(), pObject->isHighlight()  );
  }
  //---------------------------	
  void VisitorDrawObject::execEndObject( ObjectPtr pObject )
  {
    //		cViewProps.undragMat();
  }
  //---------------------------	
  void VisitorDrawObject::execBeginPoly( Poly* pPoly )
  {
  }
  void VisitorDrawObject::execEndPoly( Poly* pPoly )
  {
  }
  //*********************************************
} // namespace
