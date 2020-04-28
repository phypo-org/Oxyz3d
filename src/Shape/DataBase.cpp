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

    //	  Shape3dBase* lShape = new Shape3dCube( 30 );
    /*
      Shape3dLine* lShape = new Shape3dLine();
      lShape->addPoint( Point3d( 35, 57, 59 ) ); 
      lShape->addPoint( Point3d( 135, 157, 159 ) ); 
      lShape->addPoint( Point3d( 200, 200, 300 ) ); 
      lShape->addPoint( Point3d( 300, 300, 400 ) ); 

      addShape( lShape );

      Shape3dBase* lTetra = PrimitivFactory::Create( PrimitivFactory::Type::TETRA, 50 );
      addShape( lTetra );

      Shape3dBase* lCube = PrimitivFactory::Create( PrimitivFactory::Type::CUBE, 150 );
      addShape( lCube );
      lCube->move( Point3d( 300, 300, 300 ));
	

      Shape3dBase* lPyra = PrimitivFactory::Create( PrimitivFactory::Type::PYRAMID, 100 );
      addShape( lPyra );
      lPyra->move( Point3d( -300, -300, -300 ));

    */
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::OCTO, 1 ); 
      Object*  lObj = new ObjectPoly( lShape->getStrType(), lShape );
      addObject( lObj );
      lObj->move( Point3d( -5, 5, -5 ));
      lObj->rename( "Octo test");
    }
		
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::DODEC, 1 );			
      Object*   lObj = new ObjectPoly( lShape->getStrType(), lShape 	);
      addObject( lObj );
      lObj->move( Point3d( -5, 5, 5 ));
      lObj->rename( "Dodec test");
    }
		
    {
      Poly* lShape = PrimitivFactory::Create( PrimitivFactory::Type::ICOSAHED, 1 );			
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

  }
  //------------------------------------------
  //	void DataBase::add( O3dObj* pObj )
  //	{ 
  //		cLiveObj.push_back( pObj );
  //	}
  //------------------------------------------
  bool DataBase::removeEntityIfNoOwner( EntityId pId )
  {
    auto lIter = cEntities.find( pId );
    if( lIter == cEntities.end() )
      {
	std::cout << "DataBase::removeEntityIfNoOwner not found" << std::endl;
	return true;
      }
		
    if( lIter->second->howManyOwner() == 0 )
      {				std::cout << "DataBase::removeEntityIfNoOwner erase " << std::endl;
	cEntities.erase( lIter);
	return true;
      }
    return false;
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
	//	std::cout << " is Point  "  << std::endl;	
	lLines[0]->getPoints().second = lPoint;  // on change le second point
      }
    else
      {
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
    if( cCurrentLine == nullptr ) return 0;
		
    return (GLuint)cCurrentLine->getPoints().size();
  }
  //------------------------------------------		
  Object* DataBase::validCurrentCreation()
  {
    if( cCurrentCreation == nullptr )
      return nullptr;
			
			
    //		cCurrentCreation->rename( );
		
    //			validEntity( cCurrentCreation->getShape() );
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
    if( cCurrentLine == nullptr || cCurrentLine->getPoints().size() != 2 )
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
	delete cCurrentCreation;
	cCurrentCreation = pCurrentCreation;
      }
  }
  //------------------------------------------		
  Object* DataBase::getCurrentCreation( )
  {		  
    return cCurrentCreation;			
  }
  //-------------------------------------------------------------
  void DataBase::drawGL(ViewProps& pViewProps, GLMode pSelectOrDrawMode )
  {
    pViewProps.cSelectType = Selection::Instance().getSelectType();
    pViewProps.cGLMode     = pSelectOrDrawMode;

    //		std::cout << "******************** DataBase::drawGL ********************** " << pViewProps.cSelectType << std::endl;
    //		if( pViewProps.cDebugView )
    {
      //						std::cout << "******************** DataBase::drawGL ********************** " << Selection::GetStrSelectType(	pViewProps.cSelectType) << std::endl;
    }
		
    for( Object* lObj : cContainerObject )
      {
	if(  pSelectOrDrawMode == GLMode::Draw  )
	  {
	    //			std::cout << "draw lObj:" << lObj->getName() << std::endl;
	    lObj->drawGL( pViewProps );
	  }
	else
	  {
	    lObj->selectGL( pViewProps );
	    //						std::cout << "select lObj:" << lObj->getName() << std::endl;
	  }
	//	std::cout << " fin lObj:" << lObj->getName() << std::endl;
      }
		
		
    if( cCurrentLine != nullptr )
      {
	if( pSelectOrDrawMode == GLMode::Draw )
	  {
	    //		std::cout << "**************** draw currentline ****" << std::endl;
	    cCurrentLine->drawGL( pViewProps );
	  }
	else
	  cCurrentLine->selectGL( pViewProps );
      }
		
    glLoadName(0);
		
    // on ne peut pas selectionner les saisies en cours !
    if(  pSelectOrDrawMode == GLMode::Draw  )
      {		
	if( cCurrentCreation != nullptr )
	  {				
	    cCurrentCreation->drawGL( pViewProps );						
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
    std::cout << "DataBase::validOneEntityLevel 1 "  << cEntities.size()  << "  - "<< *pEntity << std::endl;
    pEntity->cId =  sUniqueId.getNewId();

    std::cout << "DataBase::validOneEntityLevel 2 " << cEntities.size() << "  - " << *pEntity << std::endl;

		
    cEntities.insert( { pEntity->cId, pEntity } ); 
    return pEntity->cId;
  }
  //******************************************
  class VisitValid : public EntityVisitorNode
  {
    DataBase& cBase;
  public:
    VisitValid( DataBase& pBase )
      :cBase( pBase ) {;}
    //------------------------------------------		
    void execNode( Entity *pEntity, Entity *pOwner )
    {
      if( pEntity->isIdVoid() == false )
	return ; // deja fait !
			
      cBase.validOneEntityLevel( pEntity  );
    }
  };
  //******************************************
	
  EntityId DataBase::validEntity( Entity* lEntity )
  {
    VisitValid lValid( *this );

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
    std::cout << "DataBase::deleteEntity" << pEntity->getStrType()<<  std::endl;
    if( pEntity->getType() == ShapeType::Object)
      {
	std::cout << "DataBase::deleteEntity Object "
		  << pEntity->howManyOwner() << std::endl;
		
	return removeEntityIfNoOwner( pEntity );
      }
    // FAIRE TOUT LES AUTRES CAS !!!!
		
    return false;
  }
  //------------------------------------------
  void DataBase::addToInput( EntityPtr pEntity, bool pFlagLink )
  {
    std::cout << "DataBase::addToInput" << std::endl;
    // Pour le moment ....
    std::unordered_set<Point*> lVectPoints;
		
    VisitorGetPoints lVisit( lVectPoints );
    pEntity->execVisitor( lVisit );

    for( PointPtr lPt : lVectPoints )
      {				
	std::cout << "DataBase::addToInput addPointToCurrentLine " <<  std::endl;
	addPointToCurrentLine( lPt->get() );					
      }

		
    //		switch( pEntity->getType() )
    //			{
    //			}
  }


  //************************************

}
