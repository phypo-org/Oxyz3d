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
		ObjectFacet(  const char*pName, Facet* pFacet, ClassType iClassType=ClassTypeObj)
			: Object( pName, iClassType )
			,cShape(pFacet)
                        
		{;}
		
		ObjectFacet(  const std::string& pName, Facet* pFacet, ClassType iClassType=ClassTypeObj)
			: Object( pName, iClassType )
			,cShape(pFacet)
                        
		{;}
          bool       isVoid()   const override { return cShape == nullptr; }
          void       removeShape() override  { cShape = nullptr; }
		EntityPtr getShape() override { return cShape; };
		FacetPtr	getFacet()          { return cShape; };
	  ObjectType getObjType() const override { return ObjectType::ObjFacet; }
		ShapeType  getSubType() const override { return ShapeType::Facet; };

          FacetPtr giveFacet()
          {            
            Facet *lTmp = cShape;
            lTmp->removeOwner(this );
            cShape = nullptr;
            return lTmp;
          };

	};
  //******************************

}

#endif
