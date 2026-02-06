#include "ViewProps.h"
#include "EntityVisitor.h"
#include "Facet.h"
#include "SortVisitor.h"
#include "Selection.h"
#include "Shape/DataBase.h"


namespace PP3d{
  //*********************************************
  //-------------------------------------
  // ATTENTION NE FAIT RIEN POUR LES OWNERS
  void Facet::deleteAll()
  {		    
    for( size_t i=0; i< getLines().size(); i++  )
      {
        LinePtr lLine = getLines()[i];
        if( i == 0 && lLine->isPoint() == false )
          {
            delete lLine->first();
          }
        delete lLine->second();
        delete lLine;
      }

    getLines().clear();
  }
  //-------------------------------------
 void Facet::inverseLines( )
  {		
    GLuint lNbDemi  = (GLuint)cLines.size()/2;
    GLuint lLastPos = (GLuint)cLines.size()-1;
    // si nombre impair, le milieu ne change pas !
    for( GLuint l=0; l< lNbDemi; l++)
      {
	LinePtr lTmp = cLines[l];
	cLines[l] = cLines[lLastPos-l];
	cLines[lLastPos-l] = lTmp;
      }
    // On inverse aussi les points dans les lignes !
    for( LinePtr lLine : cLines )
      {
	lLine->inversePoint();
      }		
  }
  //-------------------------------------
  void Facet::computeNormal()
  {
    std::vector<LinePtr> & lLines = getLines();
    
    size_t lNb = lLines.size();
    
    if( lNb ==  3 || lNb == 2 )
      {
	Calcul3d::Normal( lLines[0]->first()->get(),
			  lLines[0]->second()->get(),
			  lLines[1]->second()->get(), // pas first !!!	
                          cNorm );
	return;
      }
    
    if( lNb > 3 )
      {
        int i = lNb/3;
        int j = (lNb*2)/3;
        Calcul3d::Normal( lLines[0]->first()->get(),
                          lLines[i]->second()->get(),
                          lLines[j]->second()->get(), // pas first !!!		
                          cNorm );
        return;
      }
  }
  //------------------------------------
  void Facet::addPoint( const  Point3d & pPt )
  {    
     LinePtrVect& lLines = getLines();
     
     if( lLines.size() == 0 )
       {
         PointPtr lPoint= new Point( pPt );
         LinePtr lLine = new Line( lPoint, lPoint ); // Un point 
         lLines.push_back( lLine );
      }
     else if(lLines.size() == 1 && lLines[0]->isPoint() )  // Un  point !
      {
	if( lLines[0]->getFirst()->get() == pPt )
	  {
	    std::cerr << "*** ERROR : same point " << std::endl;
	    return ;
	  }
	//	std::cout << " is Point  "  << std::endl;	
        PointPtr lPoint= new Point( pPt );
	lLines[0]->getPoints().second = lPoint;  // on change le second point
      }
    else
      {
	if( lLines[lLines.size()-1]->getSecond()->get() == pPt )
	  {
	    std::cerr << "*** ERROR : same point " << std::endl;
	    return ;
	  }
	    
	//	std::cout << " new line   "  << std::endl;	
        PointPtr lPoint= new Point( pPt );
	LinePtr	lLine = new Line(  lLines[lLines.size()-1]->getPoints().second, lPoint);
	lLines.push_back( lLine );
      }		
  }
  //-------------------------------------  
  Facet* Facet::duplicate() const
  {
    VectPoint3d lPts;    
    if( getCopyPointsWithDuplicate( lPts ) > 0 )
      {
        FacetPtr lNewFac = new Facet();

           
        PIndex lSize = lPts.getVector().size();
        if( isClose() )
          {
            lSize--;
          }
        
        for( PIndex i=0; i< lSize; i++ )
          {
            lNewFac->addPoint( lPts.getVector()[i] );
          }
          if( isClose() )
            {
              lNewFac->closeFacet();
            }
          
        return lNewFac;
      }
    return nullptr;
  }
 //-------------------------------------
  Point3d Facet::getCenter3d()
  {
    
    PP3d::Point3d  lCenter;
    
    PointPtrSet lPoints;
    VisitorGetPoints lVisit(lPoints);
		
    execVisitor( lVisit );
    for(  const PP3d::PointPtr pPoint : lPoints )
      {
	lCenter += pPoint->cPt;
      }
    lCenter /= (double)lPoints.size();

    
    /*
    for( LinePtr lLine : cLines )
      {
	lCenter += lLine->first()->get();
	lCenter += lLine->second()->get();
      }
    
    lCenter /= cLines.size()*2;
    */
    
    return lCenter;
  }
 //-------------------------------------
  // Attention le dernier points est duplique si fermé !!!!
  PointPtr Facet::getPoint( PIndex iPos ) 
  {
    PointPtr lPt = nullptr;
    if( cLines.size() >= (size_t) iPos )
      {    
	if( iPos == 0 )
	  {
	    lPt = cLines[0]->getFirst();
	  }
	else
	  {
	    lPt = cLines[iPos-1]->getSecond();
	  } 
      }
    return lPt;
  }
  //-------------------------------------
  // Attention le dernier points est duplique  si fermé!!!!
   int  Facet::getCopyPointsWithDuplicate( VectDouble3 & oPts ) const
   {
    if(  cLines.size() > 0 )
      {        
        oPts.add( *(cLines[0]->getFirst()) );

        PIndex lSz =  cLines.size();        
        //       if( isClose() )
        //        lSz-- ; // le dernier point est le premier !
            
        for( PIndex i = 0; i< lSz; i++ )
          {            
            oPts.add( *(cLines[i]->getSecond()) );
          }
          //        for( LinePtr lLine : cLines )
          //          {
          //            oPts.add( *(lLine->getSecond()));
          //          }
      }
    
    return  oPts.size();
  }
  //-------------------------------------
  // Attention le dernier points est duplique si fermé !!!!
  int  Facet::getCopyPointsWithDuplicate( VectPoint3d & oPts ) const
  {
    if(  cLines.size() > 0 )
      {        
        oPts.add( *(cLines[0]->getFirst()) );

        PIndex lSz =  cLines.size();        
        //     if( isClose() )
        //         lSz-- ; // le dernier point est le premier !
            
        for( PIndex i = 0; i< lSz; i++ )
          {            
            oPts.add( *(cLines[i]->getSecond()) );
          }
      }
        /*
    if(  cLines.size() > 0 )
      {
        oPts.add( *(cLines[0]->getFirst()) );
        
        for( LinePtr lLine : cLines )
          {
            oPts.add( *(lLine->getSecond()));
          } 
      }
        */
    return  oPts.size();
  }
  //-------------------------------------
  void Facet::insertPoint( PIndex iPos, PointPtr lPt, DataBase & iBase )
  {
    if( lPt == nullptr )
      return;

    if(iPos > cLines.size() )
      {
	iPos = cLines.size();
      }

      
    //============== No point ============
    if( cLines.size() == 0)
      {
	LinePtr lNewLine = iBase.getNewLine( lPt, lPt);
	cLines.push_back( lNewLine );
	return;
      }
    //============== Along one point ============
    if( cLines.size() == 1 && cLines[0]->isPoint() )
      {
	cLines[0]->first() = lPt;
	return;
      }

    //============== Other case ================
    LinePtr lNewLine = iBase.getNewLine( lPt, cLines[iPos]->first() );
    
    if( iPos > 0 )
      cLines[iPos-1]->second() = lPt;
    cLines.insert( cLines.begin()+iPos, lNewLine );
  }
  //-------------------------------------
  void Facet::insertPoint( PIndex iPos, Point3d & lPt, DataBase & iBase )
  {    
    insertPoint( iPos, iBase.getNewPoint( lPt ), iBase );
  }
  //-------------------------------------
  bool Facet::delPoint( PIndex iPos, DataBase & iBase )
  {
    if( iPos >= cLines.size() )
      {
	return false;
      }

    //=========== Along one line ==========
    if( cLines.size() == 1 ) 
      {
	if( cLines[0]->isPoint() )
	  {
	    return true;   // the void !  must perhap delete the line ?
	  }
	
	if( iPos == 0 )
	  {
	    iBase.freePoint( cLines[0]->first() );
	    cLines[0]->first() = cLines[0]->second();	    
	  }
	else // iPos == 1
	  {
	    iBase.freePoint( cLines [0]->second() );
	    cLines[0]->second() = cLines[0]->first();
	  }
	return false;
      }		   


    //=========== The last point ==========   
    if( ((size_t)iPos) == cLines.size()-1 ) 
      {
	iBase.freePoint( cLines[iPos]->second() );
	iBase.freeLine( cLines.back() );
	cLines.pop_back();
	return false;
      }

    //============= First Point ============
    if( iPos == 0 )
      {
	iBase.freePoint( cLines[0]->first() );
	iBase.freeLine( cLines[0] );
	cLines.erase( cLines.begin() );
	return false;
      }
    
    //============ General case ============
    cLines[iPos-1]->second() =  cLines[iPos]->second();
    iBase.freeLine( cLines[iPos] );
    cLines.erase( cLines.begin()+iPos );
    
    return false;
  }
   
  //-------------------------------------
  void Facet::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginFacet( this );
	
    for( LinePtr lLine : getLines() )
      {
	lLine->execVisitor( pVisit );
      }
		
    pVisit.execEndFacet( this );
  }
  //-------------------------------------
  void Facet::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode(  this, nullptr );
    for( LinePtr lLine : getLines() )
      {
	pVisit.execNode( lLine, this );
	lLine->execVisitor( pVisit );
	pVisit.execEndNode( lLine, this );
      }		
    pVisit.execEndNode( this, nullptr );
  }
  //-------------------------------------
  void Facet::closeFacet()
  {
    if( isClosable() )
      {
	LinePtr lLine = new Line( cLines[ getNbLines() -1 ]->getSecond(),
				  cLines[0]->getFirst() );
	cLines.push_back( lLine);
      }
  }
  //-------------------------------------
  // il suffit d'un seul angle superieur ou egal a 180 pour que la facette soit concave
  /*
    bool Facet::computeConcave()
    {
    if( cLines.size() <= 3 )
    return false;

    std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;
    size_t i;
    for(  i = 0 ; i<  cLines.size()-1; i++ )
    {
    Point3d A = cLines[i]->second()->get()   - cLines[i]->first()->get();
    Point3d B = cLines[i+1]->second()->get() - cLines[i+1]->first()->get();

	
    std::cout <<  cLines[i]->second()->get() << " - "  cLines[i]->first()->get()
    << " -> " << A << std::endl;
	
    std::cout <<  cLines[i+1]->second()->get() << " - "  cLines[i+1]->first()->get()
    << " -> " << B << std::endl;

    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
	
    std::cout << lResult << std::endl;
    if( lResult >  M_PIl)
    {
    std::cout << "Facet::computeConcave  CONCAVE !  sz line:" << cLines.size() << "  i:" << i << " : " << lResult << " >>>>> " << M_PIl << std::endl;
    cIsConcave = true;
    return true;
    }
    }
    Point3d A = cLines[i]->second()->get() - cLines[i]->first()->get();
    Point3d B = cLines[0]->second()->get() - cLines[0]->first()->get();
    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
    if( lResult >  M_PIl)
    {
    std::cout << "Facet::computeConcave2   sz line:" << cLines.size() << "  i:" << i << " : " << lResult << " >>>>> " << M_PIl<< std::endl;
    cIsConcave = true;
    return true;
    }
    
    
    cIsConcave = false;
    return false;
    }
  */
  bool Facet::computeConcave()
  {
    if( cLines.size() <= 3 ){
      cIsConcave = false;
      return false;
    }

    //   std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;
    size_t i;
    for(  i = 0 ; i<  cLines.size()-1; i++ )
      {
	Point3d A = cLines[i]->second()->get()   - cLines[i]->first()->get();
	Point3d B = cLines[i+1]->second()->get() - cLines[i+1]->first()->get();

	
	//	std::cout <<  cLines[i]->second()->get() << " - "  <<cLines[i]->first()->get()
	//	  << " -> " << A << std::endl;
	
	//	std::cout <<  cLines[i+1]->second()->get() << " - "  <<cLines[i+1]->first()->get()
	//		  << " -> " << B << std::endl;

	if( Point3d::isNegAngle( A, B, cNorm )) // On prend la normale comme repere
	  {
	    //	    std::cout << "Facet::computeConcave  CONCAVE ! " << std::endl;
	    cIsConcave = true;
	    return true;
	  }
      }
    Point3d A = cLines[i]->second()->get() - cLines[i]->first()->get();
    Point3d B = cLines[0]->second()->get() - cLines[0]->first()->get();
    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
    if( Point3d::isNegAngle( A, B, cNorm )) // On prend la normale comme repere
      {
	std::cout << "Facet::computeConcave  CONCAVE ! " << std::endl;
	cIsConcave = true;
	return true;
      }

    
    cIsConcave = false;
    return false;
  }
  //----------------------------------------------
  // Mettre un Epsiloon !
  
  bool Facet::testFlat(){
  
    //   std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;
    size_t i;
    for(  i = 0 ; i<  cLines.size()-1; i++ )
      {     
	Point3d A = cLines[i]->second()->get()   - cLines[i]->first()->get();
	Point3d B = cLines[i+1]->second()->get() - cLines[i+1]->first()->get();

	if( Point3d::isPositifAngle( A, B, cNorm )) // On prend la normale comme repere
	  {
	    //    std::cout << "Facet::computeConcave  CONCAVE ! " << std::endl;
	    return false;
	  }
      }
    Point3d A = cLines[i]->second()->get() - cLines[i]->first()->get();
    Point3d B = cLines[0]->second()->get() - cLines[0]->first()->get();
  
    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
    if( Point3d::isPositifAngle( A, B, cNorm )) // On prend la normale comme repere
      {
	return false;
      }

    return true;
  }
  //*********************************************
  void Facet::drawConcavFacet()
  {
    // A FAIRE : OPTIMISER la recuperation des points !
	
    SortEntityVisitorPoint lVisit;
    execVisitor( lVisit );
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
  void Facet::drawNormalFacet()
  {
    PointPtrSet lPoints;
    VisitorGetPoints lVisit(lPoints);
		
    execVisitor( lVisit );
				
    Point3d lMiddle;
		
    for(  const PP3d::PointPtr pPoint : lPoints )
      {
	lMiddle += pPoint->cPt;
      }
    lMiddle /= (double)lPoints.size();
    
    ColorRGBA::Green();
    Point3d lNormMiddle( lMiddle );
    Point3d lNorm10( getNormal() );
		
    //////    lNorm10 *=  10; // facteur de zoom eventuel
		
    lNormMiddle += lNorm10;
		
    glLineWidth( 1 );
    //    glDisable( GL_LIGHTING );
		
    glBegin( GL_LINE_STRIP ); 
		
    glVertex3dv( lMiddle.vectForGL() );
    glVertex3dv( lNormMiddle.vectForGL() );
		
    glEnd();
		
    //    glEnable( GL_LIGHTING );			
  }
  //---------------------------		
  //---------------------------
  //---------------------------		
  void Facet::drawPoints(ViewProps &pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->drawPoints(pViewProps);
  }
  //---------------------------		
  void Facet::drawLines(ViewProps& pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->drawLine(pViewProps);
  }
  //---------------------------		
  void Facet::drawPointsLines(ViewProps& pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->drawPointsLine(pViewProps);
  }
  //---------------------------		
  void Facet::drawFacet(ViewProps& pViewProps)
  {
    if( pViewProps.cFlagViewNormal == true
	&& pViewProps.cGLMode == GLMode::Draw )
      {
	drawNormalFacet();
      }

    glNormal3dv( getNormal().vectForGL() );
    pViewProps.facetGL( isSelect(), isHighlight(), isMagnet());
        		

    if( getNbLines() == 3 )
      {
        glBegin(GL_TRIANGLES);
      }
    else
      {
        if( isConcave() )
          {
            drawConcavFacet();
            return;          //RETURN
          }  
        else
          if(  getNbLines() == 4 )
            {
              glBegin(GL_QUADS);
            }
          else
            {
              glBegin(GL_POLYGON);
            }
      }
    
    for( size_t i=0; i< cLines.size(); i++ )
      {
        glVertex3dv( cLines[i]->getFirst()->get().vectForGL() );
      }
    glVertex3dv( cLines[ getNbLines()-1 ]->getSecond()->get().vectForGL() );

    glEnd(); 
  }
  //---------------------------		
  void Facet::drawGL( ViewProps& pViewProps )
  {      
    if( cMyProps.cVisible == false )
      {
        return;
      }
      
    //   std::cout << "======= Facet::drawGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;

    
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;

      case SelectType::All:
      case SelectType::Point:
        {
	  if( pViewProps.cViewMode == ViewMode::FULL )
            {
              glEnable(GL_LIGHTING);
              drawFacet(pViewProps);
            }
          glDisable(GL_LIGHTING);
          drawPointsLines(pViewProps);
        }
        break;
     
				
      case SelectType::Group:
      case SelectType::Object:
      case SelectType::Poly:
      case SelectType::Facet:
      case SelectType::Line:
	{
          if( pViewProps.cViewMode == ViewMode::FULL)		
            {
              glEnable(GL_LIGHTING);
              drawFacet(pViewProps);
            }
					
          glDisable(GL_LIGHTING);
          drawLines(pViewProps);
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
    }
    //------------------------------------------------------		
    //------------------------------------------------------		
    //------------------------------------------------------		
 	
  void Facet::pickingPoints(ViewProps &pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->pickingPoints(pViewProps);
  }
  //---------------------------		
  void Facet::pickingLines(ViewProps& pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->pickingLine(pViewProps);
  }
  //---------------------------		
  void Facet::pickingPointsLines(ViewProps& pViewProps)
  {
    for( LinePtr lLine: getLines())
      lLine->pickingPointsLine(pViewProps);
  }
  //---------------------------		
  void Facet::pickingFacet()
  {    
    //   std::cout << "Facet::pickingFacet "  <<  std::endl;
    
    glNormal3dv( getNormal().vectForGL() );

 		
   if( getNbLines() == 3 )
      {
        glBegin(GL_TRIANGLES);
      }
    else
      {
        if( isConcave() ) // METTRE UN FLAG ET NE RECALCULE QUE S'IL Y A BESOIN (lors d'une mise a jour de l'objet)
          {
            drawConcavFacet();
            return;          //RETURN
          }  
        else
          if(  getNbLines() == 4 )
            {
              glBegin(GL_QUADS);
            }
          else
            {
              glBegin(GL_POLYGON);
            }
      }
    
    for( size_t i=0; i< cLines.size(); i++ )
      {
        glVertex3dv( cLines[i]->getFirst()->get().vectForGL() );
      }
    glVertex3dv( cLines[ getNbLines()-1 ]->getSecond()->get().vectForGL() );

    glEnd(); 
  }
  //---------------------------		
  void Facet::selectGL( ViewProps & pViewProps )
  {      
    if( cMyProps.cVisible == false )
      {
        std::cout << ">>>>>>>> Facet::pickingGL NOT VISIBLE " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;

        return;
      }
      

    
    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;

      case SelectType::All:
        {
	  if( pViewProps.cViewMode == ViewMode::FULL )
            {
              pViewProps.facetGL( isSelect(), isHighlight(), isMagnet()); //??????????????
              ColorRGBA::Id( getId()); //??????????????     
              pickingFacet();
            }
          pickingPointsLines(pViewProps);
        }
        break;
        
      case SelectType::Point:
        {
          pickingPoints(pViewProps);
        }
        break;
        
      case SelectType::Line:
	{				
          pickingLines(pViewProps);
	}
	break;
        
				
      case SelectType::Group:
      case SelectType::Object:
      case SelectType::Poly:
      case SelectType::Facet:
        if( pViewProps.cViewMode == ViewMode::FULL)		
            {
              pViewProps.facetGL( isSelect(), isHighlight(), isMagnet()); //??????????????
              ColorRGBA::Id( getId()); //??????????????     
             pickingFacet();
            }
	break;
      }
 
    //	drawInfoGL( pViewProps, cMyProps );
    }
   //********************************
}
