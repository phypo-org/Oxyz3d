#ifndef H__ObjectPoly__H
#define H__ObjectPoly__H

#include "Calcul3d.h"

#include "Object.h"


namespace PP3d {

	
  //******************************
  class Poly;
	
  class ObjectPoly  : public Object {

    Poly* cShape;
    
  public:
    ObjectPoly( const char* pName, Poly* pPoly, ClassType iClassType=ClassTypeObj)
      :Object( pName, iClassType  )
      ,cShape( pPoly)
    {
      pPoly->addOwner( this );
    }	//    ShapePoints( VectPoint3d& pPoints, std::vector<Facet>& pFacet;
    ObjectPoly( const std::string& pName, Poly* pPoly, ClassType iClassType=ClassTypeObj)
      :Object( pName, iClassType )
      ,cShape( pPoly )
    {
      pPoly->addOwner( this );
    }	//    ShapePoints( VectPoint3d& pPoints, std::vector<Facet>& pFacet;
		
    bool       isVoid()   const override { return cShape == nullptr; }
    EntityPtr  getShape() override { return cShape; }
    Poly*      getPoly()           { return cShape; }
    ObjectType getObjType() const override { return ObjectType::ObjPoly; }
    ShapeType  getSubType() const override { return ShapeType::Poly; };
    
    Poly* setPoly( Poly* pPoly ) {
      Poly * lTmp = cShape;
      if( lTmp != nullptr )
        {
          lTmp->removeOwner( this );
        }
      cShape = pPoly;
      if( cShape != nullptr) 
        cShape->addOwner( this );

      return lTmp;
    }

  };
  //******************************
}

#endif
