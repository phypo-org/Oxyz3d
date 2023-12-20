#ifndef H__Maker__H
#define H__Maker__H


#include "Entity.h"
#include "Matrice.h"
#include "DataBase.h"


namespace PP3d {


	//****************************************
	class Maker{
		
	public:

	static constexpr long double  kEpsilon=1e-10;
	  
	  static PolyPtr CreatePoly4FromFacet( FacetPtr pFacet, PIndex pNbPas, Mat4& pMat,
					       bool iFlagClose,
					       bool iFlagCloseSeg,
					       bool iFlagCloseSegEnd,
					       bool iCloseHight,
					       bool iCloseLow,
                                               bool iFlagGrid = false,
					       long double  iEpsilon = kEpsilon);
          
          static GLuint QuadrangulerConcave( Facet & iFacet, FacetPtrVect & oFacVect );
          
          static FacetPtrVect Trianguler( DataBase& pBase, FacetPtrVect& pFacet, bool lForce );		
	};
		
	//****************************************

}
#endif
