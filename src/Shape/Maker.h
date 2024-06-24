#ifndef H__Maker__H
#define H__Maker__H


#include "Entity.h"
#include "Matrice.h"
#include "DataBase.h"


namespace PP3d {
          enum class  CloseRevol  { Yes, No };  
          enum class  CloseSeg    { Yes, No };
          enum class  CloseSegEnd { Yes, No };
          enum class  CloseHight  { Yes, No };
          enum class  CloseLow    { Yes, No };
          enum class  WithGrid    { Yes, No };

	//****************************************
  
	class Maker{ 
		
	public:

	static constexpr long double  kEpsilon=1e-10;
	  
	  static PolyPtr CreatePoly4FromFacet( FacetPtr pFacet, PIndex pNbPas, Mat4& pMat,
					       CloseRevol  iFlagCloseRevol,
					       CloseSeg    iFlagCloseSeg,
					       CloseSegEnd iFlagCloseSegEnd,
					       CloseHight  iCloseHight,
					       CloseLow    iCloseLow,
                                               WithGrid    iFlagGrid = WithGrid::No,
                                               Point3d  *  iGearDecal=0,
                                               PIndex      iGearMod1  =0,
                                               PIndex      iGearMod2  =0,
					       long double iEpsilon = kEpsilon);
          
          static GLuint QuadrangulerConcave( Facet & iFacet, FacetPtrVect & oFacVect );
          
          static FacetPtrVect Trianguler( DataBase& pBase, FacetPtrVect& pFacet, bool lForce );		
	};
		
	//****************************************

}
#endif
