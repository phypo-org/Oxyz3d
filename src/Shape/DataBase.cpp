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
  TUniqueId<EntityId, 1> DataBase::sUniqueGroupId;


	
  DataBase::DataBase()
    :cCurrent()
  //    :cCurrentCreation(nullptr)
  //    ,cCurrentLine(nullptr)
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

  //-------------------------------------------------------------
  void DataBase::drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType, ClassType iClassType )
  {
    iViewGen.cSelectType = iViewInputCursor.cSelectType = iViewInputPoly.cSelectType = iViewInputObject.cSelectType = iSelectType;
    iViewGen.cGLMode = iViewInputCursor.cGLMode = iViewInputPoly.cGLMode = iViewInputObject.cGLMode = iSelectOrDrawMode;
    
    //		std::cout << "******************** DataBase::drawGL ********************** " << iViewGen.cSelectType << std::endl;
    //		if( iViewGen.cDebugView )
    {
      //						std::cout << "******************** DataBase::drawGL ********************** " << Selection::GetStrSelectType(	iViewGen.cSelectType) << std::endl;
    }
    cCurrent.drawGL( iViewGen, iViewInputCursor, iViewInputPoly, iViewInputObject, iSelectOrDrawMode, iSelectType);
    
    //======================================			
    for( Object* lObj : cContainerObject )
      {
        if( lObj->isClassType( iClassType ))
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
          }
	//	std::cout << " fin lObj:" << lObj->getName() << std::endl;
      }
		
  
  	
    //	std::cout << "******************** end DataBase::drawGL ********************** "  << std::endl;
  }
  //------------------------------------------
  void DataBase::recomputeAll( Compute iCompute )
  {
    long lCpt=0;
    //    std::cout << "******************** RecomputeAll ********************** " << (int)iCompute << std::endl;
    for( auto iter = cContainerObject.begin(); iter != cContainerObject.end(); ++iter )
      {
	lCpt += (*iter)->recomputeAll( (*iter)->cMyProps, iCompute);
      }

    if( iCompute != Compute::Nothing && lCpt > 0 )
      std::cout << "DataBase::recomputeAll Concav count:" << lCpt << " " << (int)iCompute << std::endl;
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
    DataBase & cBase;
    bool cTotalScan = false;
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
  void DataBase::addObject( Object* pObj)
  {
    validEntity( pObj );
    cContainerObject.emplace( pObj );			
  }
  //------------------------------------------
  bool DataBase::deleteEntityIfVoid( EntityPtr pEntity)
  {
    if( pEntity->isVoid() )
      {
        return deleteEntity( pEntity );
      }
    return false;
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
  /*
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
  */
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
    // if( cFreePoints.empty() )
      return new Point(iPt);

      // PointPtr lTmp = cFreePoints.top();
      // cFreePoints.pop();
    // lTmp->set( iPt);
    
    //return lTmp;
  }
  //---------------------------------------------------------
  LinePtr  DataBase::getNewLine(PointPtr lA, PointPtr lB)
  {
    //  if( cFreeLines.empty() )
      return new Line( lA, lB );

      // LinePtr lTmp = cFreeLines.top();
      //cFreeLines.pop();
      //lTmp->set( lA, lB );
      //return lTmp;
  }
  //---------------------------------------------------------
  FacetPtr DataBase::getNewFacet()
  {
    // if( cFreeFacets.empty() )
      return new Facet();
    
      //  FacetPtr lTmp = cFreeFacets.top();
    //  cFreeFacets.pop();
      //  return lTmp;
  }  
  //---------------------------------------------------------
  PolyPtr DataBase::getNewPoly()
  {
    //   if( cFreePolys.empty() )
      return new Poly();
    
    // PolyPtr lTmp = cFreePolys.top();
    // cFreePolys.pop();
    //  return lTmp;
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
  //---------------------- GROUP -----------------------------
     void DataBase::freeGroup( GroupPtr iGroup )
    {
      if( iGroup == nullptr )
        return;
      
      iGroup->clear();
      cGroups.removeObj(iGroup );
      
      delete iGroup;
    }
  //-----------------------------
    GroupPtr DataBase::newGroup()
    {
      GroupPtr lGroup = new GroupObject(sUniqueGroupId.getNewId());
      cGroups.insertObj( lGroup );
      return lGroup;
    } 
  //---------------------- GROUP -----------------------------

  
  //************************************
} // end namespace
