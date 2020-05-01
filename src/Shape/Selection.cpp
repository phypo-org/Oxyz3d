#include "Selection.h"

#include "DataBase.h"
#include "ViewProps.h"

#include "Matrice.h"


#include "Object.h"

#include "DebugVars.h"

using namespace std;

namespace PP3d {

  //******************************************
  Selection * Selection::sTheSelection=nullptr;
	
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
    // Quand on passe d un type plus general a un plus bas
    // il faudra prendre tout les sous-type !
    // dans le sens contraire ???
		
    removeAll(); // en attendant !
    cSelectType = pType;	
  }
  //--------------------------------
  void Selection::addEntity( EntityPtr ioEntity, bool iSelectAll)
  {
    cSelectObj.emplace( ioEntity );
    ioEntity->setSelect( true );

    if( iSelectAll && ioEntity->getType() == ShapeType::Line )
      {
	LinePtr lLine =  dynamic_cast<LinePtr>(ioEntity)->getReverseLine();
	if( lLine )
	  addEntity( lLine, false );
      }
  }

  //--------------------------------
  void Selection::removeEntity( EntityPtr ioEntity, bool iSelectAll )
  {
    ioEntity->setSelect( false );
    cSelectObj.erase( ioEntity );

    if( iSelectAll && ioEntity->getType() == ShapeType::Line )
      {
	LinePtr lLine =  dynamic_cast<LinePtr>(ioEntity)->getReverseLine();
	if( lLine )
	  removeEntity( lLine, false );
      }
  }
  //--------------------------------
  void Selection::removeAll()
  {
    for( auto lIter = cSelectObj.begin(); lIter != cSelectObj.end(); ++lIter)
      {
	(*lIter)->setSelect( false );
      }
		
    cSelectObj.clear();
  }
  //--------------------------------
  bool Selection::addToSelection( EntityPtr lEntity)
  {
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
  bool Selection::selectPickingHit( std::vector< PP3d::PickingHit>& pHits, DataBase& cBase, SelectMode& pSelectMode, bool pFlagOnlyHightlight )
  {
    DBG_SEL_NL(" <Selection::selectPickingHit : " << pHits.size() <<  " SM:" << pSelectMode << " " );
		
		
    for( PP3d::PickingHit& pHit : pHits )
      {
	EntityPtr lEntity = cBase.findEntity( pHit.cName );				
	if( lEntity == nullptr )
	  {
	    DBG_SEL( "***************** Selection "<<pHit.cName<<" not found ! **********************");
	    return false;
	  }				
				
	pHit.cEntity = lEntity;
      }

    //    cout<<"**************** 22222 ***********************"<< endl;
		
    // On tri les plus petits objets en premier, entre meme type c'est le Z qui compte
    //	std::sort( pHits.begin(), pHits.end(), MyCmp );

    // Je n'utilise pas std::sort : il plante !!!
    insertionSort( pHits );

    for( PP3d::PickingHit& pHit : pHits )
      {
	DBG_SEL( "Hit :" << pHit );
			
      }
    //    cout<<"**************** 333 ***********************"<< endl;

						
#if __GNUC__ > 6 
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif
						
    for( PP3d::PickingHit& pHit : pHits )
      {				
	cout << "Hit:" << pHit << endl;
	EntityPtr lEntity = pHit.cEntity;
								
	if(pFlagOnlyHightlight)
	  {
	    lEntity->setHighlight( true );
	    return true;
	  }

	if( isSelected( lEntity ) )
	  {
	    if ( lEntity->isSelect() == false )
	      {
		cout<<"Selection Error "<< lEntity->getId() <<" mismach"<< endl;
	      }


	    switch( pSelectMode )
	      {
	      case SelectMode::Undefine:
		pSelectMode = SelectMode::Unselect;
	      case SelectMode::Inverse:
	      case SelectMode::Unselect:
		removeEntity(lEntity);
		break;
	      case SelectMode::Select:
		DBG_SEL( " NO " );
		//		return false;
	      }
	    //	    cout <<  " R ->SM:" << pSelectMode << endl;
	    return true;
	  }				
	else
	  {
	    //	    cout <<  " U " ;
	    switch( pSelectMode )
	      {
	      case SelectMode::Undefine:
		pSelectMode = SelectMode::Select;
	      case SelectMode::Inverse:
	      case SelectMode::Select:
		addEntity( lEntity );								
		break;
	      case SelectMode::Unselect:
		//		cout <<  " NO " << endl;
		return false;
	      }
						
	    //	    cout <<  " A ->SM:" << pSelectMode << endl;
	    return true;
	  }				
      }
#if __GNUC__ > 6 
#pragma GCC diagnostic pop
#endif		
		
    //   cout <<  " KO " << endl;
    return false;
  }
  //--------------------------------
  const char* Selection::GetStrSelectType( SelectType pSelectType )
  {
    switch( pSelectType )
      {
      case SelectType::Point:  return "Point";
      case SelectType::Line:   return "Line";
      case SelectType::Facet:  return "Facet";
      case SelectType::Poly: return "Poly";
      case SelectType::Object: return "Object";
      case SelectType::All: return "All";
      }
    return "SelectType::unknown";
  }
  //--------------------------------
  void Selection::deleteAllFromDatabase(DataBase& pDatabase )
  {
    std::vector<EntityPtr> lDelList;
		
    for(  EntityPtr lEntity : cSelectObj )
      {
	if( pDatabase.deleteEntity( lEntity ) )
	  {
	    lDelList.push_back( lEntity );
	  }
      }


    for( EntityPtr lEntity : lDelList )
      {
	cSelectObj.erase( lEntity );
      }
  }
  //--------------------------------
  void Selection::addSelectionToInput( DataBase& pDatabase, bool pFlagLink )				
  {
    for(  EntityPtr lEntity : cSelectObj )
      {
	pDatabase.addToInput( lEntity, pFlagLink );				
      }	
  }
  //--------------------------------
  /*
    void Selection::drawGL( DataBase& pDatabase, ViewProps& pViewProps )
    {
    if( cSelectType != SelectType::Poly )
    return;
    ViewProps lViewProps( pViewProps );
		
    lViewProps.cColorPoint = ColorRGBA( 1, 0.5, 0, 0 );
    lViewProps.cColorLine  = ColorRGBA( 1, 0.5, 0, 0 );
    lViewProps.cColorFacet = ColorRGBA( 1, 0.5, 0, 0 );

 
    glPushMatrix();
		
    //	cCurrentTransform.execGL();
    glLoadMatrixd( cCurrentMatrice.vectForGL() );
		
    for( auto lIter = cSelectObj.begin() ; lIter != cSelectObj.end(); ++lIter  )
    {
    //		std::cout << "        Selection::drawGL " << lSubSel.cObjId << std::endl;
    Entity* lObj =  (*lIter);
    if( lObj != nullptr )
    {
    std::cout << "<<<     call    drawSelectionGL" << std::endl;
    lObj->drawSelectionGL( lViewProps );
    }
    std::cout << ">>>     fin call    drawSelectionGL" << std::endl;

    }
    glPopMatrix();
    }
  */
  //******************************************

}
