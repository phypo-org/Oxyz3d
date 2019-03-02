#ifndef H__Maker__H
#define H__Maker__H


#include "Entity.h"
#include "Matrice.h"
#include "DataBase.h"


namespace PP3d {
	//****************************************
	class Maker{
		
	public:
		static PolyPtr CreatePoly4FromFacet( FacetPtr pFacet, PIndex pNbPas, Mat4& pMat, bool pFlagClose,  bool pFlagCloseSeg, bool pFlagCloseSegEnd, bool pCloseHight, bool pCloseLow );
		static FacetPtrVect Trianguler( DataBase& pBase, FacetPtrVect& pFacet, bool lForce );		
	};
		
	//****************************************

};
#endif
