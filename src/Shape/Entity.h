#ifndef H__Entity__H
#define H__Entity__H

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
	
        if(lOwner->getType() == iSearchType )
          return lOwner;
	
        lOwner = lOwner->firstOwner();
      }
    return nullptr;
  }

  //------------------------------------------------------
  inline PolyPtr GetOwnerPolyFromFacet( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getType() ==  ShapeType::Facet )
      {
        EntityPtr lOwner =  iShape->firstOwner();
        if( lOwner != nullptr && lOwner->getType() == ShapeType::Poly ) 
          {
            return (PolyPtr) lOwner;
          }
      }
    return nullptr;
  }
 
  //------------------------------------------------------
  inline FacetPtr GetOwnerFacetFromLine( EntityPtr iShape) {
    if( iShape != nullptr && iShape->getType() ==  ShapeType::Line )
      {
        EntityPtr lOwner =  iShape->firstOwner();
        if( lOwner != nullptr && lOwner->getType() == ShapeType::Facet ) 
          {
            return (FacetPtr) lOwner;
          }
      }
    return nullptr;
  }
  //------------------------------------------------------

  template <class MYCLASS>
  inline void RemoveFromOwners( MYCLASS iVect) { for( EntityPtr lEntity : iVect ) lEntity->removeFromOwners();}

  
}

#endif
