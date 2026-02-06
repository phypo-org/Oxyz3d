#include "Entity.h"
#include "SortVisitor.h"
#include "Shape/DataBase.h"

#include <cmath>



namespace PP3d {

  
  //*********************************************
  Entity::~Entity()
  {
    // Normalement les liens entre Entity lors de la destruction
    // seront gérés par un Visiteur, 
    // l'entité n'est detruite que quand sont elle n'a plus d'owner

    //    if( howManyOwner() != 0 )
    //      std::cout << "*** ERROR : Entity destroy when they are owner" << std::endl;
  }
    //-------------------------------------
  Point3d Entity::getNormal3d()
  {
    Point3d lNorm;
    for( Entity* lOwner  :  cOwners )
    {
      lNorm += lOwner->getNormal3d();
    }
    if( cOwners.size() )
      lNorm /= cOwners.size();
    
    return lNorm;
  }
  //---------------------------
  void Entity::move ( Point3d p )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt += p;
      }
  }  
  //---------------------------
  void Entity::scale ( Point3d p )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt *= p;
      }
  }
  
  //---------------------------
  void Entity::modify (  Mat4& pMat )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt *= pMat;
      } 
  }
  //---------------------------
  void Entity::deleteAllHierarchy()
  {    
    SortVisitorEntity lVisit(false);  //SortEntityVisitor
    execVisitor( lVisit );

    //  std::cout << "@@@@@@@@@@@ deleteAllHierarchy:" << std::endl;
    for( auto lEntity : lVisit.cSetAllEntity )
      {
        std::cout << lEntity << "   " ;
        delete lEntity;
      }
    //    std::cout << "@@@@@@@@@@@ deleteAllHierarchy End" << std::endl;
  }
  //---------------------------
  /*
    Object* Entity::FindMyObject( EntityPtr pCurrent )
    {
    while( pCurrent != nullptr )
    {
    if( lCurrent->getType() == ShapeType::Object )
    {
    return static_cast<Object*>(lCurrent);
    }
				
    if( lCurrent->cOwners.size() == 0 )
    lCurrent =  nullptr;
    else
    lCurrent = lCurrent->cOwner[0];
    }
    return nullptr;
    }
  */
  //*********************************************
  void Point::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execPoint( this );
  }
  //-------------------------------------
  // direct call 
  void Point::execVisitor( EntityVisitorNode& pVisit )
  {
  }
  //*********************************************

  //*********************************************
//--------------------------------
  
  void FindPointPairVisitor::execBeginLine ( LinePtr pLine )
  {
    //  std::cout << "FindPointPairVisitor::execBeginLine "
    //	<<  cPtPair.first << ":" <<  pLine->first()
    //	<< "   =========== " 
    //	<<  cPtPair.second << ":" <<  pLine->second()
    //	<< std::endl;
    
    if( cPtPair.first == pLine->first()
        && cPtPair.second == pLine->second() )
      {
        //	  std::cout << "FindPointPairVisitor::execBeginLine OK OK OK OK OK OK "
        //		    <<  pLine->getId()<< std::endl;
        
        cSet.insert( pLine );
      }
  }
  
  FindPointPairVisitor::FindPointPairVisitor(const PointPtrPair & iPtPair , std::set<LinePtr> & ioSet )
    :cPtPair( iPtPair )
    ,cSet( ioSet )
  {
  }
	  
  FindPointPairVisitor::FindPointPairVisitor(const PointPtrPair & iPtPair )
    :cPtPair( iPtPair )
    ,cSet( cInternalSet )
  {
    //   std::cout << "FindPointPairVisitor::execBeginLine 2222222222222222222222222222222222222222222 "
    //		<< iPtPair.first << "  :  " << iPtPair.second
    //		<< cPtPair.first << "  :  " << cPtPair.second
    //		<<  std::endl
  }
  
  //*********************************************
}
