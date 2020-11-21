#ifndef H__PrimitivFactory__H
#define H__PrimitivFactory__H

#define _USE_MATH_DEFINES // for C++
#define ___USE_GNU
#include <cmath>

#include "ObjectPoly.h"
#include "PP3dType.h"

namespace PP3d{


	
  struct PrimFacet : public std::vector<PIndex>{
    PrimFacet( size_t p1, size_t p2, size_t p3 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet( size_t p1, size_t p2, size_t p3, size_t p4 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      push_back( p4);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet( size_t p1, size_t p2, size_t p3, size_t p4, size_t p5 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      push_back( p4);
      push_back( p5);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet( size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      push_back( p4);
      push_back( p5);
      push_back( p6);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet( size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      push_back( p4);
      push_back( p5);
      push_back( p6);
      push_back( p7);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet( size_t p1, size_t p2, size_t p3, size_t p4, size_t p5, size_t p6, size_t p7,  size_t p8 )
    {
      push_back( p1);
      push_back( p2);
      push_back( p3);
      push_back( p4);
      push_back( p5);
      push_back( p6);
      push_back( p7);
      push_back( p8);
      // Il faudra appeler le RecomputeNormal
    }
    PrimFacet() {;}

  };
  //************************
  struct PrimitivParam{

    int cNbU=1;
    int cNbV=1;
    int cNbW=1;

    bool cCheckInvRot=false;
    bool cCheckDouble=false;
    bool cCheckHole=false;

    long double cWidth=0;
    long double cHeight=0;
    long double cTop=0;
    long double cBottom=0;
    long double cThickness=0;

    

    long double cParam1=0;
    long double cParam2=0;
    long double cParam3=0;
  };


  //************************
  class PrimitivFactory {
    
  public:
    enum class Type{
      FACET_N,
	CYLINDER,
	SPHERE,
	PLANE,
	TETRA,
	CUBE,
	PYRAMID,
	OCTO,
	DODEC,
	ICOSAHED,
      //			CYLINDER, CONE, DISK, PARTIAL_DISK, SPHERE, TORUS, WIRETORUS
	};

    //    static constexpr long double M_PIx2 = 3.14159265358979323846*2;
    static constexpr long double M_PIx2 = M_PIl*2;

    
    static long double sMinSz;
    static const char* GetTypeName(PrimitivFactory::Type pType);
		
    static Poly * Create( PrimitivFactory::Type pType, std::string & iName, PrimitivParam * iParam=nullptr  );
    static Poly * CreatePoly( Point3d * pPoints, size_t pSzPt, PrimFacet * pFacets, size_t pSzFac );

    static Poly * CreateFacet(     PrimitivParam * iParam );
    static Poly * CreateCylinder(  PrimitivParam * iParam, std::string & iName );
    static Poly * CreateSphere(  PrimitivParam * iParam, std::string & iName );
  };
  //************************
}


#endif
