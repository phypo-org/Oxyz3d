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


namespace PP3d {
  //************************************

  TUniqueId<EntityId, 1> DataBase::sUniqueId;


	
  DataBase::DataBase()
    :cCurrentCreation(nullptr)
    ,cCurrentLine(nullptr)
  {
  
  }
  //------------------------------------------
  void 	DataBase::demo1()
  {
    std::string lName;
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::OCTO, lName); 
      Object*  lObj = new ObjectPoly( lShape->getStrType(), lShape );
      addObject( lObj );
      lObj->move( Point3d( -5, 5, -5 ));
      lObj->rename( "Octo test");
    }
		
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::DODEC, lName );			
      Object*   lObj = new ObjectPoly( lShape->getStrType(), lShape 	);
      addObject( lObj );
      lObj->move( Point3d( -5, 5, 5 ));
      lObj->rename( "Dodec test");
    }
		
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::ICOSAHED, lName);			
      Object*   lObj = new PP3d::ObjectPoly( lShape->getStrType(), lShape );
      addObject( lObj );
      lObj->move( Point3d( -5, -5, 5 ));
      lObj->rename( "Ico test");
    }

    //	  lShape.turnY( 45 );
    //	  lShape.scale( 2 );

  }
  //------------------------------------------
  void 	DataBase::makeDemo2( float iPos, int iDepth, float iSize, bool iCentralPoint, SubDiv::SubNormalizeType  iNormalize)
  {
    {
      SubDiv::SubParam lParam( iDepth, iSize, iCentralPoint, iNormalize);
      SubDiv::Icosahedron( lParam );
			
      Poly* lShape =lParam.finish(1);
			
      Object*  lObj = new ObjectPoly( lShape->getStrType(), lShape );
      addObject( lObj );
      lObj->move( Point3d( iPos, iPos, iPos ));
      lObj->rename( "Icosahedron 2 0.35 ");
    }

    {
      SubDiv::SubParam lParam( iDepth, iSize, iCentralPoint, iNormalize);
      SubDiv::Octahedron( lParam );
			
      Poly* lShape =lParam.finish(iPos);
			
      Object*  lObj = new ObjectPoly( lShape->getStrType(), lShape );
      addObject( lObj );
      lObj->move( Point3d( iPos, iPos, -iPos ));
      lObj->rename( "Octahedron 2 0.35 ");
    }

		
  }
  //------------------------------------------
  void 	DataBase::demo2()
  {
    makeDemo2( 2,   2, 1, 0,  SubDiv::SubNormalizeType::NORMALIZE);
    makeDemo2( 4,   1, 1, 0,  SubDiv::SubNormalizeType::NORMALIZE_DEC_INIT);
  }
  //------------------------------------------
  DataBase::~DataBase()
  {
    for( auto  lPair : cEntities )
      {
	delete lPair.second;       
      }

    while( cCurrentLine != nullptr )
      {
	delPointToCurrentLine();
      }    
    //    delete cCurrentCreation;  // normalement a nullptr qd on detruit !
  }
  //------------------------------------------
  //	void DataBase::add( O3dObj* pObj )
  //	{ 
  //		cLiveObj.push_back( pObj );
  //	}
  //------------------------------------------

  bool DataBase::removeEntityIfNoOwner(  EntityId pId )
  {
    auto lIter = cEntities.find( pId );
    if( lIter == cEntities.end() )
      {
	DBG_BAZ( "DataBase::removeEntityIfNoOwner not found" );
	return true;
      }
		
    if( lIter->second->howManyOwner() == 0 )
      {
	DBG_BAZ( "DataBase::removeEntityIfNoOwner erase ");
	cEntities.erase( lIter);
	return true;
      }
    else{
      DBG_BAZ( " removeEntityIfNoOwner failed : too many owner" );
    }
    return false;
  }
  //------------------------------------------
  void DataBase::viewCurrentPoint( Point3d & pPt )
  {
    if( cCurrentPointObject == nullptr )
      {
	PointPtr lPt   = new Point( Point3d(0, 0, 0));
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
  void DataBase::hideCurrentPoint( )
  {
    cCurrentPoint = nullptr;
  }
  //------------------------------------------
  void DataBase::addPointToCurrentLine( Point3d pPt )
  {
    //		std::cout << "======== DataBase::addPointToCurrentLine" << std::endl;
    // FAIRE QUELQUE CHODE DE PLUS PROPRE !!!
    if( cCurrentLine == nullptr )
      {
	std::cout << "========= ObjectFacet"  << std::endl;

	cCurrentLine= new ObjectPolylines("Saisie", new Facet());
      }
		
    Facet* lFacet= cCurrentLine->getFacet();
		
    PointPtr lPoint= new Point( pPt );
		
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
  void DataBase::delPointToCurrentLine( )
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
	    cCurrentLine = nullptr;
	  }
	else {
	  lLine->second() = lLine->first();
	}
      }
    else {
      delete lLines.back();
      lLines.pop_back();
    }
  }
  //------------------------------------------
  GLuint DataBase::getNbCurrentPoints()
  {   
    PP3d::SortEntityVisitorPoint lVisit;
    if(  execVisitorOnCurrentLine( lVisit ) )           
      return (GLuint)lVisit.cVectPoints.size();
    return 0;
  }
  //------------------------------------------		
  Object* DataBase::validCurrentCreation()
  {
    if( cCurrentCreation == nullptr )
      return nullptr;

    Object* lTmp = cCurrentCreation;
    addObject( lTmp );
	
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
  ObjectLine* DataBase::convertCurrentLineToLine()
  {
    PP3d::SortEntityVisitorPoint lVisit;
    if( execVisitorOnCurrentLine( lVisit ) == false ||  lVisit.cVectPoints.size() < 2 )
      return nullptr;


    VisitorTrace lVtrace( std::cout );
    cCurrentLine->execVisitor( lVtrace );
			
    ObjectLine* lLine = new ObjectLine( "Line",
					new Line( cCurrentLine->getFacet()->getLines()[0]->getFirst(),
						  cCurrentLine->getFacet()->getLines()[0]->getSecond()) );
    addObject( lLine );

    VisitorDestroy lVisitDestroy(*this);
    cCurrentLine->execVisitor( lVisitDestroy );
			
    cCurrentLine= nullptr;
			
	
    return lLine;
  }
  //------------------------------------------		
  ObjectFacet* DataBase::convertCurrentLineToFacet()
  {
    if( cCurrentLine == nullptr )
      return nullptr;
			
    ObjectFacet* lFacet = new ObjectFacet( "Facet", cCurrentLine->giveFacet() );
    // il faut creer une nouvelle ligne pour fermer la facette
    lFacet->getFacet()->closeFacet();
    addObject( lFacet );			
			
    delete cCurrentLine;
    cCurrentLine= nullptr;
	
    return lFacet;
  }
  //------------------------------------------		
  ObjectPoly* DataBase::convertCurrentLineToFacetPoly()
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    PolyPtr  lPoly   = getNewPoly();
    FacetPtr lFacet1 = cCurrentLine->giveFacet();
    lFacet1->closeFacet();
    lPoly->addFacet( lFacet1 );
	
    
    ObjectPoly* lObjPoly = new ObjectPoly( "Facet", lPoly );
    addObject( lObjPoly );			
			
    delete cCurrentLine;
    cCurrentLine= nullptr;
	
    return lObjPoly;
  }
  //------------------------------------------		
  ObjectPoly* DataBase::convertCurrentLineToBiFacetPoly()
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    PolyPtr  lPoly   = getNewPoly();
    FacetPtr lFacet1 = cCurrentLine->giveFacet();
    lFacet1->closeFacet();
    lPoly->addFacet( lFacet1 );
		    
    FacetPtr lFacet2 = lFacet1->duplicate( *this);
    lFacet2->inverseLines();
    lPoly->addFacet( lFacet2 );

    
    ObjectPoly* lObjPoly = new ObjectPoly( "Facet", lPoly );
    addObject( lObjPoly );			
			
    delete cCurrentLine;
    cCurrentLine= nullptr;

    
    return lObjPoly;
  }
  //------------------------------------------
  // c'est deja un polyline, il suffit de le reommer et de l'ajouter a la base
  ObjectPolylines* DataBase::convertCurrentLineToPolylines()
  {
    if( cCurrentLine == nullptr )
      return nullptr;

    ObjectPolylines* lPoly = cCurrentLine;
    cCurrentLine = nullptr;
			
    lPoly->rename( "Polylines");
		
    addObject( lPoly );
						
	
    return lPoly;
  }
  //------------------------------------------
	
  void DataBase::cancelCurrentCreation( )
  {
    delete cCurrentCreation;
    cCurrentCreation = nullptr;
  }
  //------------------------------------------	
  void DataBase::swapCurrentCreation( Object* pCurrentCreation )
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
  Object* DataBase::getCurrentCreation( )
  {		  
    return cCurrentCreation;			
  }
  //-------------------------------------------------------------
  void DataBase::drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType)
  {
    iViewGen.cSelectType = iViewInputCursor.cSelectType = iViewInputPoly.cSelectType = iViewInputObject.cSelectType = iSelectType;
    iViewGen.cGLMode = iViewInputCursor.cGLMode = iViewInputPoly.cGLMode = iViewInputObject.cGLMode = iSelectOrDrawMode;
    
    //		std::cout << "******************** DataBase::drawGL ********************** " << iViewGen.cSelectType << std::endl;
    //		if( iViewGen.cDebugView )
    {
      //						std::cout << "******************** DataBase::drawGL ********************** " << Selection::GetStrSelectType(	iViewGen.cSelectType) << std::endl;
    }
    //======================================			
    for( Object* lObj : cContainerObject )
      {
	if(  iSelectOrDrawMode == GLMode::Draw  )
	  {
	    //			std::cout << "draw lObj:" << lObj->getName() << std::endl;
	    lObj->drawGL( iViewGen );
	  }
	else
	  {
	    lObj->selectGL( iViewGen );
	    //						std::cout << "select lObj:" << lObj->getName() << std::endl;
	  }
	//	std::cout << " fin lObj:" << lObj->getName() << std::endl;
      }
		
    //======================================
 
    if( cCurrentPoint != nullptr
	&&  iSelectOrDrawMode == GLMode::Draw )
      {
	//		std::cout << "**************** draw currentline ****" << std::endl;
	cCurrentPoint->drawGL( iViewInputCursor );
      }	 
      
    //======================================	
    ColorRGBA l3( 0.7, 0.4, 0.1, 0.7 );
    ColorRGBA l4( 0.6, 0.5, 0.2, 0.7 );
    
    ViewProps lV2( iViewGen );

    lV2.cColorPoint = l3 ;
    lV2.cColorLine  = l4;

    if( cCurrentLine != nullptr )
      {
	if( iSelectOrDrawMode == GLMode::Draw )
	  {
	    //		std::cout << "**************** draw currentline ****" << std::endl;
	    cCurrentLine->drawGL( iViewInputPoly );
	  }
	else
	  {
	    cCurrentLine->selectGL( iViewGen );
	  }	 
      }
		
    //======================================	
    ColorRGBA l5( 0.4, 1, 0.4, 0.7 );
    ColorRGBA l6( 0.4, 0.8, 0.3, 0.6 );
    ColorRGBA l7( 0.2, 0.4, 0.2, 0.5 );

    ViewProps lV3( iViewGen );

    lV3.cColorPoint = l5 ;
    lV3.cColorLine  = l6;
    lV3.cColorFacet = l7;
	
    // on ne peut pas selectionner les saisies en cours !
    if(  iSelectOrDrawMode == GLMode::Draw  )
      {		
	if( cCurrentCreation != nullptr )
	  {				
	    cCurrentCreation->drawGL( iViewInputObject );						
	  }				
      }		
    //	std::cout << "******************** end DataBase::drawGL ********************** "  << std::endl;
  }
  //------------------------------------------
  void DataBase::recomputeAll( )
  {
    //		std::cout << "******************** RecomputeAll **********************" << std::endl;
    for( auto iter = cContainerObject.begin(); iter != cContainerObject.end(); ++iter )
      {
	(*iter)->recomputeAll( (*iter)->cMyProps );
      }
  }
  //------------------------------------------
  Object* DataBase::findObjectById( PP3dId pId ) const
  {
    for( Object* lObj : cContainerObject )
      {
	if( lObj->getId() == pId )
	  return lObj;
      }

    return nullptr;
  }
  //------------------------------------------
  Object* DataBase::findObjectByGLNameId( GLuint pNameId ) const
  {
    for( Object* lObj : cContainerObject )
      {
	if( lObj->cMyProps.cGLNameId == pNameId )
	  {
	    std::cout << "findObjectByGLNameId " << pNameId << " " << lObj->cMyProps.cGLNameId << std::endl;
	    return lObj;
	  }
      }

    return nullptr;
  }
  //------------------------------------------
  //------------------------------------------
  //------------------------------------------
  EntityId DataBase::validOneEntityLevel(  Entity* pEntity )
  {
    //    std::cout << "DataBase::validOneEntityLevel 1 "  << cEntities.size()  << "  - "<< *pEntity << std::endl;
    if( pEntity->isIdVoid())
      {
	pEntity->cId =  sUniqueId.getNewId();

    //    std::cout << "DataBase::validOneEntityLevel 2 " << cEntities.size() << "  - " << *pEntity << std::endl;
		
	cEntities.insert( { pEntity->cId, pEntity } );
      }
    return pEntity->cId;
  }
  //------------------------------------------
  void DataBase::addValidEntityForUndo(  Entity* pEntity )
  {   	
    cEntities.insert( { pEntity->cId, pEntity } ); 
  }
  //******************************************
  class VisitValid : public EntityVisitorNode
  {
    DataBase& cBase;
    bool cTotalScan;
  public:
    VisitValid( DataBase& pBase, bool iTotalScan = false )
      :cBase( pBase )
      ,cTotalScan( iTotalScan) {;}
    //------------------------------------------		
    void execNode( Entity *pEntity, Entity *pOwner )
    {
      //      std::cout << "execNode:" << pEntity->getId() << std::endl;
      
      if( pEntity->isIdVoid() == false
	  && cTotalScan == false )
	return ; // deja fait !
      
      //      std::cout << "execNode isVoidId" << std::endl;
			
      cBase.validOneEntityLevel( pEntity  );
    }
  };
  //******************************************
	
  EntityId DataBase::validEntity( Entity* lEntity, bool iTotalScan )
  {
    VisitValid lValid( *this, iTotalScan );

    lEntity->execVisitor( lValid );
		
    return lEntity->getId();
  }	
  //------------------------------------------
  void DataBase::addObject( Object* pObj )
  {
    validEntity( pObj );
    cContainerObject.emplace( pObj );			
  }
  //------------------------------------------
  bool DataBase::deleteEntity( EntityPtr pEntity)
  {
    DBG_BAZ( "DataBase::deleteEntity " << pEntity->getStrType() )
    
    if( pEntity->getType() == ShapeType::Object)
      {
	DBG_BAZ(  "DataBase::deleteEntity Object "
		  << pEntity->howManyOwner() );
	
	if( removeEntityIfNoOwner( pEntity ) == true )
	  {
	    cContainerObject.erase( dynamic_cast<Object*>(pEntity ));
	    return true;
	  }
	return false;
      }
    // FAIRE TOUT LES AUTRES CAS !!!!
		
    return false;
  }
  //------------------------------------------
  // Add All the points of the entity to the courant input
  
  void DataBase::addToInput( EntityPtr pEntity, bool pFlagLink )
  {
    //   std::cout << "DataBase::addToInput" << std::endl;
   
    PP3d::SortEntityVisitorPoint  lVisit;
    pEntity->execVisitor( lVisit );
    for( PointPtr lPt : lVisit.cVectPoints) // les points sont uniques
      {				
	std::cout << "DataBase::addToInput addPointToCurrentLine " <<  std::endl;
	addPointToCurrentLine( lPt->get() );					
      }
  }
  //---------------------------------------------------------
  void DataBase::resetIdFromMax()
  {
    EntityId lMaxId = 0;
    
    for( auto lPair : cEntities )
      {
	if( lPair.second->getId() > lMaxId )
	  {
	    lMaxId =  lPair.second->getId();
	  }
      }
    
    resetUniqueId( lMaxId + 1 );
  }
  //---------------------------------------------------------
  PointPtr DataBase::getNewPoint(const Point3d & iPt)
  {
    if( cFreePoints.empty() )
      return new Point(iPt);

    PointPtr lTmp = cFreePoints.top();
    cFreePoints.pop();
    lTmp->set( iPt);
    
    return lTmp;
  }
  //---------------------------------------------------------
  LinePtr  DataBase::getNewLine(PointPtr lA, PointPtr lB)
  {
    if( cFreeLines.empty() )
      return new Line( lA, lB );

    LinePtr lTmp = cFreeLines.top();
    cFreeLines.pop();
    lTmp->set( lA, lB );
    return lTmp;
  }
  //---------------------------------------------------------
  FacetPtr DataBase::getNewFacet()
  {
    if( cFreeFacets.empty() )
      return new Facet();
    
    FacetPtr lTmp = cFreeFacets.top();
    cFreeFacets.pop();
    return lTmp;
  }  
  //---------------------------------------------------------
  PolyPtr DataBase::getNewPoly()
  {
    if( cFreePolys.empty() )
      return new Poly();
    
    PolyPtr lTmp = cFreePolys.top();
    cFreePolys.pop();
    return lTmp;
  }  
  //---------------------------------------------------------
  //---------------------------------------------------------
  //---------------------------------------------------------
  // Les Free ne sont pas trés au point !!!
  
  void DataBase::freePoint( PointPtr ioPt )
  {
    //   clearAllOwner();
    if( removeEntityIfNoOwner( ioPt ) )
      {
	ioPt->razId();
	ioPt->clear();
	//	cFreePoints.push( ioPt );
      }
  }
  //---------------------------------------------------------
  void DataBase::freeLine( LinePtr ioLine )
  {
    if(ioLine == nullptr ) return;
      
    //  clearAllOwner();
    if( removeEntityIfNoOwner( ioLine ))
      {
	PointPtr lPt = ioLine->getFirst();
	  {
	    lPt->removeOwner( ioLine);
	    if( lPt->howManyOwner() == 0 )
	      freePoint( lPt );
	  }
	  lPt = ioLine->getSecond();
	  {
	    lPt->removeOwner( ioLine);
	    if( lPt->howManyOwner() == 0 )
	      freePoint( lPt );
	  }

	  
	ioLine->razId();
	ioLine->clear();
	//	cFreeLines.push( ioLine );
      }
  }
  //---------------------------------------------------------
  void DataBase::freeFacet( FacetPtr ioFacet)
  {
    if(ioFacet == nullptr ) return;
    
    //    clearAllOwner();
    if( removeEntityIfNoOwner( ioFacet ) )
      {
	// les lignes appartiennet a la facette donc on peut les detruire aussi
	for( LinePtr lLine : ioFacet->getLines() )
	  {
	    lLine->removeOwner( ioFacet );
	    freeLine( lLine );
	  }
	
	ioFacet->razId();
	// faire un free des lignes 
	ioFacet->clear();
	//	cFreeFacets.push( ioFacet );
      }
  }
  //---------------------------------------------------------
  void DataBase::freePoly( PolyPtr ioPoly)
  {
    if(ioPoly == nullptr ) return;
    //    clearAllOwner();
    if( removeEntityIfNoOwner( ioPoly ) )
      {
	for( FacetPtr lFacet : ioPoly->getFacets() )
	  {
	    lFacet->removeOwner( ioPoly );
	    freeFacet( lFacet );
	  }

	ioPoly->razId();
	// faire un free des facettes 
	ioPoly->clear();
	//	cFreePolys.push( ioPoly );
      }
  }
  //************************************
} // end namespace
