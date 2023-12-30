#include "Selection.h"

#include "DataBase.h"
#include "ViewProps.h"

#include "Matrice.h"

#include "Object.h"

#include "DebugVars.h"

#include "OwnerVisitor.h"
#include "Modif/Modif.h"


using namespace std;

namespace PP3d {

  //******************************************

  
  Selection::Selection()
  {
  }
  //--------------------------------
  void Selection::getSelectPoints( std::unordered_set<Point*>& pPoints )
  {
    GetPoints( cSelectObj, pPoints );
  }
  //--------------------------------
  void Selection::changeSelectType( SelectType pType )
  {          
    if( cSelectType == pType )
      { 
	return;
      }    
    //    std::cout <<  " Selection::changeSelectType " << pType
    //	      << " " << cSelectObjVect.size()
    //	      << " " << cSelectObj.size() << std::endl;

    OwnerEntityVisitor lVisit;
    
    SelectType lOldType = cSelectType;
    cSelectType = pType;
    
    if( pType != SelectType::Null )
      {
	if( lOldType > cSelectType )
	  {
	    //	    std::cout <<  " Selection::changeSelectType execVisitorOnEntity " << std::endl;
	    execVisitorOnEntity( lVisit );
	  }
	else
	  {
	    //	    std::cout <<  " Selection::changeSelectType addOwnersOf " << cSelectObjVect.size() << std::endl;
	    lVisit.addOwnersOf( cSelectObjVect );
	  }
      }
    removeAll();
    
    changeSelectType( pType, lVisit );
  }
  //--------------------------------
  void Selection::changeSelectType(  SelectType pType, SortEntityVisitor & lVisit )
  {            
    std::vector<EntityPtr> * lVectEntity = nullptr;

    switch( pType )
      {
      case SelectType::Point:
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectPoints;
	break;
	
      case SelectType::Line:  ;	
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectLines;
	break;
	
      case SelectType::Facet:  ;
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectFacets;
	break;
	
      case SelectType::Group:
      case SelectType::Object:;
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectObjects;
	break;
	
      case SelectType::Poly: ;
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectPolys;
	break;
	
      case SelectType::All: ;
	lVectEntity = (std::vector<EntityPtr>*) & lVisit.cVectAllEntity;
	break;

      case SelectType::Null: ; 	
  	break;
    }
    if( lVectEntity != nullptr )
      {
	//	std::cout <<  " Selection::changeSelectType vect : " << lVectEntity->size() <<  " " << pType << std::endl;
	
	for( EntityPtr lEntity : *lVectEntity )
	  {
	    addEntity( lEntity, true );
	    
	  }
      }
    else {
      //     std::cout <<  " Selection::changeSelectType vect is void" << std::endl;
    }
  }
  //--------------------------------
  void Selection::addGoodEntityFor( std::vector<EntityPtr> & iToSelect )
  {
   OwnerEntityVisitor lVisit;
    
    for( EntityPtr lEntity : iToSelect )
      {
	lVisit.addOwnersOf( lEntity );
      }
    changeSelectType( cSelectType, lVisit);
  }
  //--------------------------------
  void Selection::addEntity( EntityPtr ioEntity, bool iSelectAll)
  {
    //        std::cout << "Selection::addEntity " << ioEntity->getId() << ":" << ioEntity->getType() << std::endl;

    auto lPair = cSelectObj.emplace( ioEntity );
    if( lPair.second == false )
      {
	//	std::cout <<  "Selection::addEntity already select " <<  std::endl;
	return ; // already selected
      }

    // insertion ok in cSelectObj
    
    ioEntity->setSelect( true );
    cSelectObjVect.push_back( ioEntity );

 
    
    if( iSelectAll && ioEntity->getType() == ShapeType::Line )
      {
	LinePtr lLine =  dynamic_cast<LinePtr>(ioEntity)->getReverseLine();
	if( lLine )
	  {
	    addEntity( lLine, false );	    
	  }
      }
    else
      //:::::::::: GROUP :::::::::::::
      if( cSelectType == SelectType::Group && ioEntity->getType() == ShapeType::Object )
        //        if( isSelectGroup() && ioEntity->getType() == ShapeType::Object )
        {
          // Si on a un element d'un groupe il faut tous les selectionner !!!
          ObjectPtr lObj = dynamic_cast<ObjectPtr>(ioEntity);
          if( lObj != nullptr ) 
            {
              GroupPtr lGroup = lObj->getGroup();
              if( lGroup != nullptr )
                {
                  for( ObjectPtr lObjElem: lGroup->values() )
                    {
                      if( lObj != lObjElem )
                        addEntity( lObjElem ); // il y a le emplace au debut!!!
                    }
                }
            }
        }
      //:::::::::: GROUP :::::::::::::

    //    std::cout << "======================================="<< std::endl;
    //    std::cout << *this << std::endl;
    //    std::cout << "======================================="<< std::endl;
  }
  //--------------------------------
  void Selection::selectAll( DataBase & iBase )
  {
    std::vector<EntityPtr>  lTmpVect;

    // get all the unselect object into lTmpVect
    for( auto  lPair : iBase.getEntities() )
      {
	if( lPair.second->getType() != ConvertToShapeType( getSelectType() ))
	  {
	    continue;
	  }
	  
	lTmpVect.push_back(  lPair.second );
     }
    
    removeAll();

    for( EntityPtr lEntity : lTmpVect )
      {
	addEntity( lEntity, true );
      } 
  }
  //--------------------------------
  void Selection::invertSelection( DataBase & iBase )
  {
    std::vector<EntityPtr>  lTmpVect;

    // get all the unselect object into lTmpVect
    for( auto  lPair : iBase.getEntities() )
      {
	if( lPair.second->getType() != ConvertToShapeType( getSelectType() ))
	  {
	    continue;
	  }
	  
	auto lSearch = cSelectObj.emplace( lPair.second);
	if( lSearch.second == false )
	  {
	    continue; 
	  }
	lTmpVect.push_back(  lPair.second );
     }
    
    removeAll();

    for( EntityPtr lEntity : lTmpVect )
      {
	addEntity( lEntity, true );
      } 
  }
  //--------------------------------
  void Selection::removeEntity( EntityPtr ioEntity, bool iSelectAll )
  {
    if( ioEntity->isSelect() )
      {
	ioEntity->setSelect( false );
	cSelectObj.erase( ioEntity );
	for( size_t i = 0; i< cSelectObjVect.size(); i++ )
	  {
	    if( cSelectObjVect[i] == ioEntity )
	      {
		cSelectObjVect.erase( cSelectObjVect.begin() + i );
		break;
	      }            
	  }
	
	if( iSelectAll && ioEntity->getType() == ShapeType::Line )
	  {
	    LinePtr lLine =  dynamic_cast<LinePtr>(ioEntity)->getReverseLine();
	    if( lLine )
	      removeEntity( lLine, false );
	  }
        else
          //:::::::::: GROUP :::::::::::::
          if( cSelectType == SelectType::Group  && ioEntity->getType() == ShapeType::Object )
            //   if(  isSelectGroup() && ioEntity->getType() == ShapeType::Object )
            {
              // Si on a un element d'un groupe il faut tous les deselectionner !!!
              ObjectPtr lObj = dynamic_cast<ObjectPtr>(ioEntity);
              if( lObj != nullptr ) 
                {
                  GroupPtr lGroup = lObj->getGroup();
                  if( lGroup != nullptr )
                    {
                      for( ObjectPtr lObjElem: lGroup->values() )
                        {
                          if( lObj != lObjElem )
                            removeEntity( lObjElem, iSelectAll ); // il y a le emplace au debut!!!
                        }
                    }
                }
            }
        //:::::::::: GROUP :::::::::::::

      }
  }
  //--------------------------------
  void Selection::removeAll()
  {
    for( EntityPtr lEntity : cSelectObj )
      {
	lEntity->setSelect( false );
      }
		
    cSelectObj.clear();
    cSelectObjVect.clear();
  }

  //--------------------------------
  bool Selection::addToSelection( EntityPtr lEntity)
  {
    /*    if( isTransform() != lEntity->isTransform() )
      {
        std::cerr << "Selection Mistmach type" << std::endl;
        return false;
      }
    */
    DBG_SEL_NL( "addToSelection " << isSelected(lEntity) << " " << lEntity->isSelect() << " " );
    if( lEntity->isSelect() )
      {
	DBG_SEL_NN( " removeEntity " );
	removeEntity( lEntity );
      }
    else
      {
	DBG_SEL_NN( " addEntity " );
	addEntity( lEntity );
      }
		
    DBG_SEL_LN( " --> " << isSelected(lEntity) << " " << lEntity->isSelect() << " " );
	
    return true;
  }
  //--------------------------------

  Point3d Selection::getCenter( DataBase& pDatabase)
  {
    Point3d lCenter;
    std::unordered_set<Point*> cPoints;

    // comme il ne faut pas compter plusieurs fois un point, on
    // va les recuperer dans un set ( pas trés rapide mais l'autre solution serait de marquer les point comme deja fais qq part : bof )
    GetPoints( cSelectObj, cPoints );
		
    for( auto lIter = cPoints.begin(); lIter != cPoints.end(); ++lIter )
      {
	lCenter += (*lIter)->cPt;
      }
		
    if( cPoints.size() > 0 )
      lCenter /= (double) cPoints.size();

    return lCenter;
  }
  //--------------------------------
  bool MyCmp( PP3d::PickingHit & A, PP3d::PickingHit& B)
  {
    //		 cout << " sort A " << *A << endl;
    //	 cout << " sort B " << *B << endl;
		 
    if( A.cEntity->getType() == B.cEntity->getType() )
      return ( A.cZ1 > B.cZ1);

    return ( A.cEntity->getType() >  B.cEntity->getType());
  }
  //--------------------------------
	
  void insertionSort( std::vector<PP3d::PickingHit>& pVect ) 
  { 
    long int i,  j, size;
	
    size = (long)pVect.size();
		
    for (i = 1; i < size; i++) 
      { 
	PP3d::PickingHit key = pVect[i];
				
	j = i-1;  
	DBG_SEL( "i:" << i << " j:" << j << " size:" << size );
	while (j >= 0 && MyCmp( pVect[j], key )  )
	  {
	    pVect[j+1] = pVect[j]; 
						
	    j = j-1; 					
	  } 

	pVect[j+1] = key; 
      } 
  }  
  //--------------------------------
  void Selection::removeHightLightEntity( DataBase & cBase )
  {
   EntityPtr lLastHightEntity =  cBase.findEntity( cLastHightLightEntityId  );
    if( lLastHightEntity != nullptr ){
      lLastHightEntity->setHighlight( false );
    }
  }
  //--------------------------------
  bool Selection::selectPickingColor( EntityPtr iEntity, DataBase& cBase, SelectMode & pSelectMode, bool pFlagOnlyHightlight )
  {		  			
#if __GNUC__ > 6 
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

 
    removeHightLightEntity( cBase );
    
    if(pFlagOnlyHightlight)
      {	
	iEntity->setHighlight( true );
	cLastHightLightEntityId = iEntity->getId();
	return true;
      }

    
	
    if( isSelected( iEntity ) )
      {
	if ( iEntity->isSelect() == false )
	  {
	    cerr <<"Selection Error "<< iEntity->getId() <<" mismach"<< endl;
	    iEntity->setSelect( true );
	  }

	switch( pSelectMode )
	  {
	  case SelectMode::Undefine:
	    pSelectMode = SelectMode::Unselect;
	  case SelectMode::Inverse:
	  case SelectMode::Unselect:
	    removeEntity(iEntity);
	    break;
	  case SelectMode::Select:
	    DBG_SEL( " NO " );
	  }
	return true;
      }				
    else
      {
	switch( pSelectMode )
	  {
	  case SelectMode::Undefine:
	    pSelectMode = SelectMode::Select;
	  case SelectMode::Inverse:
	  case SelectMode::Select:
	    addEntity( iEntity );								
	    break;
	  case SelectMode::Unselect:
	    return false;
	  }
						
	return true;
      }			
#if __GNUC__ > 6 
#pragma GCC diagnostic pop
#endif		
		
    return false;
  }
  //--------------------------------
  const char* Selection::GetStrSelectType( SelectType pSelectType )
  {
    switch( pSelectType )
      {
      case SelectType::Null:   return "None";
      case SelectType::Point:  return "Point";
      case SelectType::Line:   return "Line";
      case SelectType::Facet:  return "Facet";
      case SelectType::Poly:   return "Poly";
      case SelectType::Object: return "Object";
      case SelectType::Group:  return "Group";
      case SelectType::All:    return "All";
      }
    return "SelectType::unknown";
  }
  //--------------------------------
  SelectType  Selection::GetSelectTypeFromStr( const char* pStr )
  {
    if( ::strcmp( pStr, "None" ) == 0 )
      return SelectType::Null;
    else                if( ::strcmp( pStr, "Point" ) == 0 )
      return SelectType::Point;
    else		if( ::strcmp( pStr, "Line" ) == 0 )
      return SelectType::Line;
    else		if( ::strcmp( pStr, "Facet" ) == 0 )
      return SelectType::Facet;
    else		if( ::strcmp( pStr, "Poly" ) == 0 )
      return SelectType::Poly;
    else		if( ::strcmp( pStr, "Object" ) == 0 )
      return SelectType::Object;
    else		if( ::strcmp( pStr, "Group" ) == 0 )
      return SelectType::Group;
    else		if( ::strcmp( pStr, "All" ) == 0 )
      return SelectType::All;
    
    return SelectType::Null;
  }
   //--------------------------------
   bool Selection::isOnlyObject( ObjectType iObjType )
   {
     for( EntityPtr lEntity :  cSelectObjVect)
       {
         if( lEntity->getType() != ShapeType::Object )
           return false;
       }
     return true;
   }
  //--------------------------------
  void Selection::deleteAllFromDatabase(DataBase& pDatabase )
  {
    if( cSelectType == SelectType::Object || cSelectType == SelectType::Group )
      {
	std::vector<EntityPtr> lDelList;
	for(  EntityPtr lEntity : cSelectObjVect )
	  {
	    if( pDatabase.deleteEntity( lEntity ) )
	      {
		lDelList.push_back( lEntity );
	      }
	  }
		
	for( EntityPtr lEntity : lDelList )
	  {
	    pDatabase.deleteEntity( lEntity );
	  }
      }
    else 
    if( cSelectType == SelectType::Point )
      {
	std::vector<PointPtr> lPoints;
	
	for(  EntityPtr lEntity : cSelectObjVect )
	  {
	    lPoints.push_back( dynamic_cast<PointPtr>( lEntity) );
	  }
	Modif::DeletePoints( &pDatabase, lPoints, true );
      }
	
    cSelectObj.clear();
    cSelectObjVect.clear();
  }
  //--------------------------------
  void Selection::addSelectionToInput( DataBase& pDatabase, bool pFlagLink )				
  {
    for(  EntityPtr lEntity : cSelectObjVect )
      {
	pDatabase.getInput().addToInput( lEntity, pFlagLink );				
      }	
  }
  //------------ GROUP -------------
  bool Selection::combineGroup( DataBase & iBase)
  {
    std::cout << "combineGroup 1 sz:" << cSelectObjVect.size()  << std::endl;
      
    if( ( cSelectType != SelectType::Object && cSelectType != SelectType::Group) 
        || cSelectObjVect.size() < 2 )
      return false;

    std::cout << "combineGroup 2" << std::endl;
    
    GroupPtr lNewGroup = nullptr;
    //=====================
    for(  EntityPtr lEntity : cSelectObjVect )
      {
        if( lEntity->getType()  != ShapeType::Object )
          continue;
        
        ObjectPtr lObj = dynamic_cast<ObjectPtr>(lEntity);
        if( lObj == nullptr ) 
          continue;
        
        GroupPtr lGroup = lObj->getGroup();
        if( lGroup != nullptr && lGroup == lNewGroup ) // ???
          continue;
        
        if( lGroup != nullptr )
          iBase.freeGroup( lGroup );
        
        if( lNewGroup == nullptr )
          lNewGroup = iBase.newGroup();

        lNewGroup->addObject( lObj ); 
      }
     //=====================
    
    //  removeAll(); ?
    return true;
  }
  //--------------------------------
  void Selection::separateGroup( DataBase & iBase)
  {
    if( cSelectType == SelectType::Object ||  cSelectType == SelectType::Group )
      {
        //=====================
        for(  EntityPtr lEntity : cSelectObjVect )
          {
            if( lEntity->getType()  != ShapeType::Object ) continue;
            
            ObjectPtr lObj = ((ObjectPtr)lEntity);
            
            GroupPtr lGroup = lObj->getGroup();
            if( lGroup != nullptr )
              {
                iBase.freeGroup( lGroup );
              }        
          }
        //=====================
      }    
  }
  //------------ GROUP -------------

  //******************************************
 
}
