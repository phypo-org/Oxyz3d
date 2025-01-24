#ifndef H__Poly__H
#define H__Poly__H


#include "Facet.h"

namespace PP3d {
 //*********************************************
  class  Poly :  public Entity{
		
    FacetPtrVect cFacets;
  
  public:
    bool isVoid() const override { return getNbFacets() >0; }
    ShapeType getType() const override { return ShapeType::Poly;}

    Point3d getCenter3d() override {
      Point3d lRes;
      for( FacetPtr lFac :  cFacets) 
        {
          lRes += lFac->getCenter3d();
        }
      if( cFacets.size()) lRes /= (double) cFacets.size(); 
      return lRes;
    } ;

    Point3d  getNormal3d() override;

    void addFacet( FacetPtr pFacet)
    {
      cFacets.push_back( pFacet );
      pFacet->addOwner( this );
    }

    void addFacet( const FacetPtrVect & iVectFacet)
    {
      for( FacetPtr lFacet : iVectFacet )
        addFacet( lFacet );
    }
    
    void removeFacet(  FacetPtr pFacet)
    {
      for( size_t i=0;  cFacets.size(); i++) 
        {
          if( cFacets[i] == pFacet )
            {
              cFacets.erase( cFacets.begin() + i); 
              pFacet->removeOwner(this);
              break;
            }
        }
    }
        
 
    void removeAllFacet()
    {
      for( FacetPtr lFacet :  cFacets ) 
        {
          lFacet->removeOwner(this);
        }
      cFacets.clear();
      Entity::clear();
    }
  
    virtual void remove( Entity* lEntity, bool iFlagRmOwner ) override
    {
      if( lEntity->getType() ==  ShapeType::Facet )
        {
          removeFacet(  ((FacetPtr)lEntity) );
          Entity::remove( lEntity, iFlagRmOwner);
        }
    }

    void giveAllFacetsTo( Poly * ioPoly )
    {
      if( ioPoly )
        {
          ioPoly->addFacet( getFacets() );
          removeAllFacet();
        }
    }

    FacetPtrVect&  getFacets()  { return cFacets; }
    PIndex  getNbFacets() const { return cFacets.size(); }
    void execVisitor( EntityVisitor& pVisit )override;

    bool clear() override { cFacets.clear();  return Entity::clear(); }

  protected:
    void execVisitor( EntityVisitorNode& pVisit )override;
		
    friend class Object;
    friend class Maker;
  };


  using PolyPtr     = Poly*;
  using PolyPtrVect = std::vector<Poly*>;


//*********************************************

}

#endif
