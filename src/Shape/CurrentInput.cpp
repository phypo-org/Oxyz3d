#include "DataBase.h"

#include "Object.h"
//#include "ObjectFacet.h"
//#include "ObjectPoly.h"
//#include "ObjectLine.h"


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
	delAllPoint();
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
    FacetPtr lNewFac = nullptr;

    if( cCurrentPointObject == nullptr )
      {
	PointPtr lPt   = new Point( Point3d(0, 0, 0)); // never free
	PointPtr lPt2  = new Point( Point3d(0, 0, 0));
	LinePtr  lLine = new Line( lPt, lPt2 );  // AFAIRE DETRUIRE LINE 
	lPt->setSelect( true );
        lNewFac = new Facet( lLine );
	cCurrentPointObject = new Object( "Current", ObjectType::ObjPolyline, lNewFac  );
      }
    else
      lNewFac = cCurrentPointObject->getFacet();

    lNewFac->getLine(0)->first()->set( Point3d( pPt.x(), 0, pPt.z() ));
    lNewFac->getLine(0)->second()->set( pPt );
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
        FacetPtr lNewFac = new Facet();

	cCurrentLine= new Object("Saisie", ObjectType::ObjPolyline, lNewFac);
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
  }
  //------------------------------------------
  void CurrentInput::delLastPoint( )
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
            delete lLine->first();
	    delete lLine;
            delete lFacet;
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
  void CurrentInput::delAllPoint( )
  {
    if( cCurrentLine == nullptr )
      return;
		    
    Facet* lFacet= cCurrentLine->getFacet();
    lFacet->deleteAll();        
    delete lFacet;
    
    delete cCurrentLine;
    resetCurrentLine();
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
    std::cout << "+++++ validCurrentCreation :" << cCurrentCreation << std::endl;
         
    	
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
  
  ObjectPtr CurrentInput::convertCurrentLineToLine(DataBase & iBase)
  {
    LinePtrVect& lLines =  cCurrentLine->getFacet()->getLines();     
    ObjectPtr lObjLine = nullptr;
    
    if( lLines.size() == 1 )
      {
         LinePtr lLineInput = lLines[0];
         if( lLineInput->isPoint() )
           {
             PointPtr lPt = new Point( lLineInput->getFirst()->get() );
             
             LinePtr lNewLine = new Line( lPt );
             lObjLine = new Object( "Line",  ObjectType::ObjLine, lNewLine);                                              
           }
         else
           {
             lObjLine = new Object( "Line",  ObjectType::ObjLine, new Line(  new Point( lLineInput->getFirst()->get()),
                                                                        new Point( lLineInput->getSecond()->get())));
           }
         
      }
    			 	
    return lObjLine;
  }
  //------------------------------------------		
  ObjectPtr CurrentInput::convertCurrentLineToFacet(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;
    
    FacetPtr lFacInput = cCurrentLine->getFacet();
    FacetPtr lFac    =  lFacInput->duplicate();    
    lFac->closeFacet();
 
    ObjectPtr lFacet = new Object( "Facet", ObjectType::ObjFacet, lFac );    			
  
    
    return lFacet;
  }
  //------------------------------------------		
  ObjectPtr CurrentInput::convertCurrentLineToFacetPoly(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;
    
    FacetPtr lFacInput = cCurrentLine->getFacet();
    FacetPtr lFac    =  lFacInput->duplicate();    
    lFac->closeFacet();
 
    PolyPtr  lPoly   = iBase.getNewPoly();
    lPoly->addFacet( lFac );	
    
    ObjectPtr lObjPoly = new Object( "Facet-Poly", ObjectType::ObjPoly, lPoly );  
	
    return lObjPoly;
  }
  //------------------------------------------		
  /*
    Obj2BSpline* CurrentInput::convertCurrentLineToBSpline(DataBase & iBase, size_t iMaille, bool iClosed )
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    FacetPtr lFacInput = cCurrentLine->getFacet();
    FacetPtr lFac      = lFacInput->duplicate();	
    
    Obj2BSpline* lObjBSpline = new Obj2BSpline( "BSpline", lFac, iClosed );
    lObjBSpline->makePtsFromPoles( iMaille );    
 
 //   delete cCurrentLine;
    //  resetCurrentLine();  // DESALLOUER LES OBJETS !!!!!!!
	
    return lObjBSpline;
  }
  */
  //------------------------------------------		
  ObjectPtr CurrentInput::convertCurrentLineToBiFacetPoly(DataBase & iBase)
  {
    if( cCurrentLine == nullptr  )
      return nullptr;

    
    FacetPtr lFacInput = cCurrentLine->getFacet();  
    FacetPtr lFac1 = lFacInput->duplicate();
    lFac1->closeFacet();
    
		    
    FacetPtr lFac2 = lFac1->duplicate();
    lFac2->inverseLines();
   
    PolyPtr  lPoly = iBase.getNewPoly();
    lPoly->addFacet( lFac1 );
    lPoly->addFacet( lFac2 );
     
    ObjectPtr lObjPoly = new Object( "Facet",  ObjectType::ObjPoly, lPoly );
			   
    
    return lObjPoly;
  }
  //------------------------------------------
  ObjectPtr CurrentInput::convertCurrentLineToPolylines(DataBase & iBase)
  {
    if( cCurrentLine == nullptr )
      return nullptr;

     FacetPtr lFacInput = cCurrentLine->getFacet();
     FacetPtr lFac = lFacInput->duplicate();	
     
     ObjectPtr lObjPoly = new Object( "Polylines", ObjectType::ObjPolyline, lFac );     
  
     return lObjPoly;
  }
  //------------------------------------------
	
  void CurrentInput::cancelCurrentCreation( )
  {
    delete cCurrentCreation;
    cCurrentCreation = nullptr;
  }
  //------------------------------------------	
  void CurrentInput::swapCurrentCreation( Object* pCurrentCreation, bool iFlagDelete )
  {
    if( cCurrentCreation != pCurrentCreation )
      {
        std::cout << "----- swapCurrentCreation old:" << cCurrentCreation<< " --> " <<  pCurrentCreation << std::endl;

	if( iFlagDelete && cCurrentCreation != nullptr )
	  {
            cCurrentCreation->deleteAllHierarchy();
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
		Object lPoly( "TMP",  ObjectType::ObjFacet, &lFacet );
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
