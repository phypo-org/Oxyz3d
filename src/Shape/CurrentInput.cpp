#include "DataBase.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPoly.h"
#include "ObjectLine.h"


#include "ViewProps.h"


#include "PrimitivFactory.h"
#include "Light.h"
#include "Selection.h"
#include "EntityVisitor.h"
#include "SubDiv.h"
#include "DebugVars.h"
#include "SortVisitor.h"

#include "CurrentInput.h"



namespace PP3d {
  //***********************************
  
  CurrentInput::CurrentInput()
    :cCurrentCreation(nullptr)
    ,cCurrentLine(nullptr)
  {
  }
  //---------------------------
  CurrentInput::~CurrentInput()
  {
    while( cCurrentLine != nullptr )
      {
	delLastPointToCurrentLine();
      }
  }
  //------------------------------------------
  void CurrentInput::renumberPoints()
  {
    cCurrentLineId = kMinCurrentLineId;
    if( cCurrentLine == nullptr )
      return;
    
    PP3d::SortEntityVisitorPoint lVisit;
    if(  execVisitorOnCurrentLine( lVisit ) )
      {
	for( PointPtr lPtr : lVisit.cVectPoints )
	  {
	    lPtr->setId( cCurrentLineId++ );
	  }
      }
  }
  //------------------------------------------
  void CurrentInput::viewCurrentPoint( Point3d & pPt )
  {
    if( cCurrentPointObject == nullptr )
      {
	PointPtr lPt   = new Point( Point3d(0, 0, 0)); // never free
	PointPtr lPt2  = new Point( Point3d(0, 0, 0));
	LinePtr  lLine = new Line( lPt, lPt2 );
	lPt->setSelect( true );
	cCurrentPointObject = new ObjectPolylines( "Current", new Facet( lLine ));
      }

    cCurrentPointObject->getFacet()->getLine(0)->first()->set( Point3d( pPt.x(), 0, pPt.z() ));
    cCurrentPointObject->getFacet()->getLine(0)->second()->set( pPt );
    cCurrentPoint = cCurrentPointObject;
  }
  //------------------------------------------
  void CurrentInput::hideCurrentPoint( )
  {
    cCurrentPoint = nullptr;
  }
  //------------------------------------------
  void CurrentInput::addPointToCurrentLine(  Point3d pPt )
  {
    //		std::cout << "======== CurrentInput::addPointToCurrentLine" << std::endl;
    // FAIRE QUELQUE CHODE DE PLUS PROPRE !!!
    if( cCurrentLine == nullptr )
      {
	std::cout << "========= ObjectFacet"  << std::endl;

	cCurrentLine= new ObjectPolylines("Saisie", new Facet());
      }
		
    Facet* lFacet= cCurrentLine->getFacet();
		
    PointPtr lPoint= new Point( pPt );
    lPoint->setId( cCurrentLineId );
    testAndSelectCurrentLinePoint( cCurrentLineId++ );
    
		
    LinePtrVect& lLines = lFacet->getLines();
				
    //		std::cout << " lLines size: "  << lLines.size()  <<std::endl;	
    if( lLines.size() == 0 )
      {
	//				std::cout << " new line 0 "  << std::endl;	
	LinePtr lLine = new Line( lPoint, lPoint ); // Un point 
	lLines.push_back( lLine );
	//		std::cout << "2 lLines size: "  << lLines.size()  <<std::endl;
      }
    else if(lLines.size() == 1 && lLines[0]->isPoint() )  // Un  point !
      {
	if( lLines[0]->getFirst()->get() == pPt )
	  {
	    std::cerr << "*** ERROR : same point " << std::endl;
	    return ;
	  }
	//	std::cout << " is Point  "  << std::endl;	
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
	LinePtr	lLine = new Line(  lLines[lLines.size()-1]->getPoints().second, lPoint);
	lLines.push_back( lLine );
      }		
  }  //------------------------------------------
  void CurrentInput::delLastPointToCurrentLine( )
  {
    if( cCurrentLine == nullptr )
      return;
		
		
    Facet* lFacet= cCurrentLine->getFacet();
    LinePtrVect& lLines = lFacet->getLines();
    if( lLines.size() == 1)
      {
	LinePtr lLine = lFacet->getLines()[0];
	if( lLine->isPoint() )
	  {
	    delete lLine;
	    delete cCurrentLine;
	    resetCurrentLine();
	  }
	else {
	  delete lLine->second();
	  lLine->second() = lLine->first();
	}
      }
    else {
      LinePtr lTmp = lLines.back();
      lLines.pop_back();
      delete lTmp->second();
      delete lTmp;
    }
    renumberPoints();
  }
  //------------------------------------------
  GLuint CurrentInput::getNbCurrentPoints()
  {   
    PP3d::SortEntityVisitorPoint lVisit;
    if(  execVisitorOnCurrentLine( lVisit ) )           
      return (GLuint)lVisit.cVectPoints.size();
    return 0;
  }
  //------------------------------------------		
  Object* CurrentInput::validCurrentCreation( DataBase & iBase )
  {
    if( cCurrentCreation == nullptr )
      return nullptr;

    Object* lTmp = cCurrentCreation;
    iBase.addObject( lTmp );
	
    cCurrentCreation = nullptr;
    return lTmp;
  }
    //------------------------------------------
  // N'enleve pas les Entity de la base de données !
  // Attention aussi car l'Object est detruit quand on revient du visiteur dans execVisitor ! 
  struct VisitorDestroy : public EntityVisitorNode{
    DataBase & cDatabase;

    VisitorDestroy( DataBase & pDatabase )
      :cDatabase( pDatabase )
    {
    }
    //------------------------
    void execEndNode( Entity* pEntity, Entity* pOwner )
    {
      if(pOwner != nullptr )
	{
	  pEntity->removeOwner( pOwner );
	}
			
      // si l'entity n'a plus aucun proprietaire
      if( pEntity->howManyOwner()==0 )
	{
	  if( cDatabase.removeEntityIfNoOwner( pEntity ) ) //enleve de la base si aucun owner
	    delete pEntity;
	}
    }
  };
  //------------------------------------------
  ObjectLine* CurrentInput::convertCurrentLineToLine(DataBase & iBase)
  {
    PP3d::SortEntityVisitorPoint lVisit;
    if( execVisitorOnCurrentLine( lVisit ) == false ||  lVisit.cVectPoints.size() < 2 )
      return nullptr;


    VisitorTrace lVtrace( std::cout );
    cCurrentLine->execVisitor( lVtrace );
			
    ObjectLine* lLine = new ObjectLine( "Line",
					new Line( cCurrentLine->getFacet()->getLines()[0]->getFirst(),
						  cCurrentLine->getFacet()->getLines()[0]->getSecond()) );
    iBase.addObject( lLine );

    VisitorDestroy lVisitDestroy(iBase);
    cCurrentLine->execVisitor( lVisitDestroy );
			
    cCurrentLine= nullptr;
			
	
    return lLine;
  }
  //------------------------------------------		
  ObjectFacet* CurrentInput::convertCurrentLineToFacet(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;
			
    ObjectFacet* lFacet = new ObjectFacet( "Facet", cCurrentLine->giveFacet() );
    // il faut creer une nouvelle ligne pour fermer la facette
    lFacet->getFacet()->closeFacet();
    iBase.addObject( lFacet );			
			
    delete cCurrentLine;
    resetCurrentLine();
    
    return lFacet;
  }
  //------------------------------------------		
  ObjectPoly* CurrentInput::convertCurrentLineToFacetPoly(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    PolyPtr  lPoly   = iBase.getNewPoly();
    FacetPtr lFacet1 = cCurrentLine->giveFacet();
    lFacet1->closeFacet();
    lPoly->addFacet( lFacet1 );
	
    
    ObjectPoly* lObjPoly = new ObjectPoly( "Facet", lPoly );
    iBase.addObject( lObjPoly );			
			
    delete cCurrentLine;
    resetCurrentLine();
	
    return lObjPoly;
  }
  //------------------------------------------		
  ObjBSpline* CurrentInput::convertCurrentLineToBSpline(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    FacetPtr lFacet1 = cCurrentLine->giveFacet();
	
    
    ObjBSpline* lObjBSpline = new ObjBSpline( "BSpline", lFacet1 );
    iBase.addObject( lObjBSpline );			
			
    delete cCurrentLine;
    resetCurrentLine();
	
    return lObjBSpline;
  }
  //------------------------------------------		
  ObjectPoly* CurrentInput::convertCurrentLineToBiFacetPoly(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    PolyPtr  lPoly   = iBase.getNewPoly();
    FacetPtr lFacet1 = cCurrentLine->giveFacet();
    lFacet1->closeFacet();
    lPoly->addFacet( lFacet1 );
		    
    FacetPtr lFacet2 = lFacet1->duplicate( iBase);
    lFacet2->inverseLines();
    lPoly->addFacet( lFacet2 );

    
    ObjectPoly* lObjPoly = new ObjectPoly( "Facet", lPoly );
    iBase.addObject( lObjPoly );			
			
    delete cCurrentLine;
    resetCurrentLine();

    
    return lObjPoly;
  }
  //------------------------------------------
  // c'est deja un polyline, il suffit de le reommer et de l'ajouter a la base
  ObjectPolylines* CurrentInput::convertCurrentLineToPolylines(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    //====== Reset the id of the points =====
    PointPtrSet lPoints;
    VisitorGetPoints lVisit(lPoints);
		
    cCurrentLine->execVisitor( lVisit );
		
    for(  const PP3d::PointPtr lPoint : lPoints )
      {
	lPoint->razId();
      }
    //==========================================
    
    ObjectPolylines* lPoly = cCurrentLine;
    resetCurrentLine();
    
    lPoly->rename( "Polylines");
		
    iBase.addObject( lPoly );
						
	
    return lPoly;
  }
  //------------------------------------------
	
  void CurrentInput::cancelCurrentCreation( )
  {
    delete cCurrentCreation;
    cCurrentCreation = nullptr;
  }
  //------------------------------------------	
  void CurrentInput::swapCurrentCreation( Object* pCurrentCreation )
  {
    if( cCurrentCreation != pCurrentCreation )
      {
	if( cCurrentCreation != nullptr )
	  {
	    cCurrentCreation->deleteAllHieracrchy();
	  }
	
	cCurrentCreation = pCurrentCreation;
      }
  }
  //------------------------------------------		
  Object* CurrentInput::getCurrentCreation( )
  {		  
    return cCurrentCreation;			
  }
  //------------------------------------------		
  void CurrentInput::addToInput( EntityPtr pEntity, bool pFlagLink )
  {
    //   std::cout << "CurrentInput::addToInput" << std::endl;
   
    PP3d::SortEntityVisitorPoint  lVisit;
    pEntity->execVisitor( lVisit );
    for( PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
      {				
	std::cout << "CurrentInput::addToInput addPointToCurrentLine " <<  std::endl;
	addPointToCurrentLine( lPt->get() );					
      }
  }
  //------------------------------------------		
  void CurrentInput::drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType)
  {
   
    if( cCurrentPoint != nullptr
	&&  iSelectOrDrawMode == GLMode::Draw )
      {
	//		std::cout << "**************** draw currentline ****" << std::endl;
	cCurrentPoint->drawGL( iViewInputCursor );
      }	 
      
       if( cCurrentLine != nullptr )
      {
	if( iSelectOrDrawMode == GLMode::Draw )
	  {
	    //		std::cout << "**************** draw currentline ****" << std::endl;

	    // si un des points est selectionné
	    PointPtr lPt = cCurrentLine->getFacet()->getPoint(cCurrentLineSelectPoint);
	    if( lPt != nullptr )
	      {
		//	std::cout << "*** drawGL must draw select point : "  << lPt->get() << std::endl;
		//	Point lPt2( lPt->x(), lPt->y(), 0 );		
		Line lLine( lPt, lPt );
		Facet lFacet( &lLine );
		
		//	ObjectLine lLineObj( "TMP", &lLine );
		//FAIRE PLUS SIMPLE !!!
		ObjectPolylines lPoly( "TMP", &lFacet );
		iViewInputPoly.cPointSize += 3;
		ColorRGBA lMem = iViewInputPoly.cColorPoint;
		iViewInputPoly.cColorPoint.set( 1, 0, 0 ) ;
		lPoly.drawGL( iViewInputPoly ); //iViewInputCursor );
		iViewInputPoly.cPointSize -= 3;
		iViewInputPoly.cColorPoint = lMem;
		}
	    
	    cCurrentLine->drawGL( iViewInputPoly );
	  }
	else
	  {
	    //	    std::cout << "SelectGL currentLine" << std::endl; 
	    cCurrentLine->selectGL( iViewGen );
	  }	 
      }
  if(  iSelectOrDrawMode == GLMode::Draw  )
      {		
	if( cCurrentCreation != nullptr )
	  {				
	    cCurrentCreation->drawGL( iViewInputObject );						
	  }				
      }	
  }
  //************************************
} // end namespace
