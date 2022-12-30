#ifndef H__ObjectFacet__H
#define H__ObjectFacet__H

#include "Entity.h"
#include "Object.h"
#include "Calcul3d.h"


namespace PP3d {

//******************************

	class ObjectFacet  : public Object
	{
		Facet*  cShape;
		Point3d cNorm; //????

	public:
		ObjectFacet(  const char*pName, Facet* pFacet)
			: Object( pName )
			,cShape(pFacet)
		{;}
		
		ObjectFacet(  const std::string& pName, Facet* pFacet)
			: Object( pName )
			,cShape(pFacet)
		{;}
		EntityPtr getShape() override { return cShape; };
		FacetPtr	getFacet()          { return cShape; };
	  ObjectType getObjType() const override { return ObjectType::ObjFacet; }
		ShapeType  getSubType() const override { return ShapeType::Facet; };

          FacetPtr giveFacet()
          {            
            Facet *lTmp = cShape;
            lTmp->clearOwner(this );
            cShape = nullptr;
            return lTmp;
          };

	};
  //******************************

}

#endif
