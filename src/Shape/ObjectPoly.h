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
		ObjectPoly( const char* pName, Poly* pPoly)
		 :Object( pName )
		,cShape( pPoly )
	{
	  pPoly->setOwner( this );
	}	//    ShapePoints( VectPoint3d& pPoints, std::vector<Facet>& pFacet;
		ObjectPoly( const std::string& pName, Poly* pPoly)
		 :Object( pName )
		,cShape( pPoly )
	{
	  pPoly->setOwner( this );
	}	//    ShapePoints( VectPoint3d& pPoints, std::vector<Facet>& pFacet;
		
		EntityPtr getShape() override { return cShape; }
		Poly*     getPoly()           { return cShape; }
	  ObjectType getObjType() const override { return ObjectType::ObjPoly; }
		ShapeType getSubType() const override { return ShapeType::Poly; };

  };
  //******************************
}

#endif
