#ifndef H__EntityHelper__H
#define H__EntityHelper__H

#include "Poly.h"



namespace PP3d {

  //*********************************************
  
  template <class MYCLASS>
  class Alloc{   
    static std::vector<MYCLASS> EntityHeap;
    
  public:
    void free( MYCLASS * iPtr, bool iFlagClearOwner = false )
    {
      if( iPtr == nullptr )
        return;
	
      if( iFlagClearOwner )
        {
          iPtr->getOwners().clear();
        }
      else
        {
          if( iPtr->getOwners().size() )
            return ;
        }	
    }
  };
  //*********************************************
  
  inline EntityPtr GetOwnerWithType( EntityPtr iShape, ShapeType iSearchType ) {
    
    EntityPtr lOwner = iShape;
    for( int i = 0; i< 8; i++ )
      {
        if( lOwner == nullptr )
          return nullptr;
	
        if(lOwner->getShapeType() == iSearchType )
          return lOwner;
	
        lOwner = lOwner->firstOwner();
      }
    return nullptr;
  }

  //------------------------------------------------------
  inline PolyPtr GetOwnerPolyFromFacet( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getShapeType() ==  ShapeType::Facet )
      {
        EntityPtr lOwner =  iShape->firstOwner();
        if( lOwner != nullptr && lOwner->getShapeType() == ShapeType::Poly ) 
          {
            return (PolyPtr) lOwner;
          }
      }
    return nullptr;
  }
 
  //------------------------------------------------------
  inline FacetPtr GetOwnerFacetFromLine( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getShapeType() ==  ShapeType::Line )
      {
        EntityPtr lOwner =  iShape->firstOwner();
        if( lOwner != nullptr && lOwner->getShapeType() == ShapeType::Facet ) 
          {
            return (FacetPtr) lOwner;
          }
      }
    return nullptr;
  }
  //------------------------------------------------------
  inline bool AsCommonPoint( LinePtr A, LinePtr B )
  {
    return A->first() == B->first()
      || A->first() == B->second()
      ||  A->second() == B->first()
      ||  A->second() == B->second();
  }


  template <class MYCLASS>
  inline void RemoveFromOwners( MYCLASS iVect) { for( EntityPtr lEntity : iVect ) lEntity->removeFromOwners();}

  
  //**************************************
  
}

#endif
