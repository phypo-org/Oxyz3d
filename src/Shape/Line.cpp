#include "ViewProps.h"
#include "EntityVisitor.h"
#include "Line.h"


namespace PP3d{
   //********************************
  void Line::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginLine( this );		
    pVisit.execPoint( getPoints().first );
    pVisit.execPoint( getPoints().second );
    pVisit.execEndLine( this );		
  }
  //-------------------------------------	
  void Line::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode(  this, nullptr );
    pVisit.execNode(  getPoints().first, this );
    pVisit.execEndNode(  getPoints().first, this );
    pVisit.execNode(  getPoints().second, this );
    pVisit.execEndNode(  getPoints().second, this );
    pVisit.execEndNode(  this, nullptr );
  }
  //-------------------------------------	
  /* OLD
  void Line::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode(  this, nullptr );
    pVisit.execNode(  getPoints().first, this );
    pVisit.execEndNode(  getPoints().first, this );
    pVisit.execNode(  getPoints().second, this );
    pVisit.execEndNode(  getPoints().second, this );
    pVisit.execEndNode(  this, nullptr );
  }
  */
  //------------------------------- 
  LinePtr Line::getReverseLineByPoint() {
    
    //    std::cout << "Line::getReverseLine         " << cPoints.first  << " : " << cPoints.second << std::endl;
    
    PointPtrPair lReverse =   Reverse( cPoints);
    //    std::cout << "Line::getReverseLine Reverse " << lReverse.first  << " : " << lReverse.second << std::endl;    
    
    FindPointPairVisitor lVisit( lReverse );

    
    if( getOwners().size() )
      {
	Entity *lEntity =  *getOwners().begin();
	ExecVisitor( lEntity->getOwners(), lVisit );
	if( lVisit.getLines().size() )
	  {
	    return *(lVisit.getLines().begin());
	  }
      }
    return nullptr;
  }
  //------------------------------- 

  
   //--------------------------------
  // Les lignes inverses partage les memes points
  // donc la ligne inverse est présente dans la liste des owners des points
  
  LinePtr Line::getReverseLineByOwner()
  {       
    for( EntityPtr lEnt : first()->getOwners())
      {
        if( lEnt != this
            && lEnt->getShapeType() == ShapeType::Line ) // Normalement c'est toujours le cas !
          {
            if( ((LinePtr)lEnt)->second() == first()
                && second() == ((LinePtr)lEnt)->first() )
              return (LinePtr)lEnt;
          }
      }
    return nullptr;
  }    
	
   //--------------------------------
  void Line::drawPoints( ViewProps& pViewProps )
    {      			
      pViewProps.pointGL(getFirst()->isSelect(), getFirst()->isHighlight(), getFirst()->isMagnet() );
      
      glBegin( GL_POINTS );
      glVertex3dv( getFirst()->cPt.vectForGL() );
      glEnd();

      if( isPoint()  == false)
        {
          pViewProps.pointGL(getSecond()->isSelect(), getSecond()->isHighlight(), getSecond()->isMagnet() );
          
          glBegin( GL_POINTS );
          glVertex3dv( getSecond()->get().vectForGL() );
          glEnd();               
      }
    }		
  //---------------------------		
  void Line::drawLine( ViewProps& pViewProps )
    {      			
      if( isPoint()  == false)
        {        
          pViewProps.lineGL( isSelect(), isHighlight(), isMagnet() );
      
          glBegin( GL_LINES );
          glVertex3dv( getFirst()->get().vectForGL() );
          glVertex3dv( getSecond()->get().vectForGL() );          
          glEnd();
      }
      else
        {      			
          pViewProps.pointGL(getFirst()->isSelect(), getFirst()->isHighlight(), getFirst()->isMagnet() );
      
          glBegin( GL_POINTS );
          glVertex3dv( getFirst()->cPt.vectForGL() );
          glEnd();
        }
    }
  //---------------------------		
  void Line::drawPointsLine( ViewProps& pViewProps )
  {      			
    if( isPoint()  == false)
      {          
        pViewProps.lineGL( isSelect(), isHighlight(), isMagnet() );
        glBegin( GL_LINES );
        glVertex3dv( getFirst()->get().vectForGL() );
        glVertex3dv( getSecond()->get().vectForGL() );          
        glEnd();

        pViewProps.pointGL(getSecond()->isSelect(), getSecond()->isHighlight(), getSecond()->isMagnet() );
          
        glBegin( GL_POINTS );
        glVertex3dv( getSecond()->get().vectForGL() );
        glEnd();       
      }
    
    pViewProps.pointGL(getFirst()->isSelect(), getFirst()->isHighlight(), getFirst()->isMagnet() );
      
    glBegin( GL_POINTS );
    glVertex3dv( getFirst()->cPt.vectForGL() );
    glEnd();
 
  }				
  //---------------------------		
  void Line::drawGL( ViewProps& pViewProps, bool iForceSelect, bool iSelect )
    {
      glDisable(GL_LIGHTING);
      
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	case SelectType::Point:
	  {
	    drawPointsLine( pViewProps );
	  }
	  break;
				
	case SelectType::Line:
	  drawLine( pViewProps );					
	  break;
				
	default:
	  {
            drawLine( pViewProps );		
	  }
	}
    }
  //---------------------------		
  //---------------------------		
  //---------------------------		
  void Line::pickingPoints( ViewProps& pViewProps)
    {      			
      pViewProps.pointGL( getFirst()->isSelect(), getFirst()->isHighlight(), getFirst()->isMagnet() );
      ColorRGBA::Id( getFirst()->getId());

      glBegin( GL_POINTS );
      glVertex3dv( getFirst()->cPt.vectForGL() );
      glEnd();

      if( isPoint()  == false)
        {
          pViewProps.pointGL( getSecond()->isSelect(), getSecond()->isHighlight(), getSecond()->isMagnet() );
          ColorRGBA::Id( getSecond()->getId());

          glBegin( GL_POINTS );
          glVertex3dv( getSecond()->get().vectForGL() );
          glEnd();               
      }
    }		
  //---------------------------		
  void Line::pickingLine( ViewProps& pViewProps )
    {      			
      if( isPoint()  == false)
        {        
          pViewProps.lineGL( isSelect(), isHighlight(), isMagnet() );
          ColorRGBA::Id( getId());

          glBegin( GL_LINES );
          glVertex3dv( getFirst()->get().vectForGL() );
          glVertex3dv( getSecond()->get().vectForGL() );          
          glEnd();
      }
      else
        {      			
          pViewProps.lineGL( isSelect(), isHighlight(), isMagnet() );
          ColorRGBA::Id( getFirst()->getId());

          glBegin( GL_POINTS );
          glVertex3dv( getFirst()->cPt.vectForGL() );
          glEnd();
        }
    }
  //---------------------------		
  void Line::pickingPointsLine( ViewProps& pViewProps )
  {      			
    if( isPoint()  == false)
      {          
        pViewProps.lineGL( isSelect(), isHighlight(), isMagnet() );
        ColorRGBA::Id( getId());
        glBegin( GL_LINES );
        glVertex3dv( getFirst()->get().vectForGL() );
        glVertex3dv( getSecond()->get().vectForGL() );          
        glEnd();

        pViewProps.pointGL(getSecond()->isSelect(), getSecond()->isHighlight(), getSecond()->isMagnet() );
        ColorRGBA::Id( getSecond()->getId());

        glBegin( GL_POINTS );
        glVertex3dv( getSecond()->get().vectForGL() );
        glEnd();       
      }
    
    pViewProps.pointGL(getFirst()->isSelect(), getFirst()->isHighlight(), getFirst()->isMagnet() );
    ColorRGBA::Id( getFirst()->getId());

    glBegin( GL_POINTS );
    glVertex3dv( getFirst()->cPt.vectForGL() );
    glEnd();
 
  }				
  //---------------------------		
  void Line::selectGL( ViewProps& pViewProps, EntityId iForceId  )
    {
      glDisable(GL_LIGHTING);
      
      switch( pViewProps.cSelectType )
	{
	case SelectType::All:
	  {
	    pickingPointsLine( pViewProps );					
	  }
	  break;
          
	case SelectType::Point:
	  {
	    pickingPoints( pViewProps );					
	  }
	  break;
				
	case SelectType::Line:
	  pickingLine( pViewProps );					
	  break;
				
	default:
	  {
            ;		
	  }
	}
    }

   //********************************
}
