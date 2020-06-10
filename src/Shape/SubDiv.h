#ifndef H__SubDiv__H
#define H__SubDiv__H



#include "Point3d.h"

#include <vector>
	
#include "PrimitivFactory.h"


using Float3 = PP3d::Point3d;

using MyFacet = PP3d::PrimFacet;

namespace  PP3d {

  //**************************************
  class SubDiv{

  public:
    enum class GeometryType {
      TETRAHEDRON  =0,
	PYRAMID     ,
	CUBE         ,
	OCTAHEDRON   ,
	DODECAHEDRON ,
	ICOSAHEDRON  ,
	
	OCTAHEDRON2    ,
	ODRON        
	};
    
    static const char * GeometryType2Str( GeometryType iType );
    static GeometryType Str2GeometryType( const char * iStr );
    
    //======================================
  public:
    enum  class SubNormalizeType{
      NORMALIZE_NONE=0,  // DEVIENT TROP PETIT 
	NORMALIZE=1,
	NORMALIZE_ONLY_INIT, // BELLE ETOILE QD DEPTH
	NORMALIZE_INC_INIT,  // FORME IRREGULIERE 
	NORMALIZE_DEC_INIT, // FORME IRREGULIERE avec trou
	NORMALIZE_HALF_INIT, //BOF
	NORMALIZE_ONLY_SUB,
	NORMALIZE_MUL_SUB,				
	NORMALIZE_DEC_SUB, //  BON
	NORMALIZE_INC_SUB, // TRES BON
	NORMALIZE_MUL_INIT // Structure avec trou ou pic celon cInitGrowFactor
	};
    static const char * SubNormalizeType2Str( SubNormalizeType iType );
    static SubNormalizeType Str2SubNormalizeType( const char * iStr );
    //======================================
  public:
    class SubParam {
			
    public:
      //    bool                 cUseTexture=false;
      int                  cDepth;
      float                cFact;
      SubNormalizeType     cNormalize;
      bool                 cCentralPoint=false;
      bool                 cCentralFacet=false;

      std::vector<Float3>   cPoints;
      std::vector<MyFacet*> cFacets;

      float                cDepthGrowFactor = 1;
      float                cInitGrowFactor = 0.3f;

      int                  cHoleFacet = -1;
      int                  cHoleDepth = -1;
    public:

      SubParam( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize );
      virtual ~SubParam();
      void reset( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize);
      void clear();
			
      
      Poly* finish( float lScale=1 );

      void normEffectSub ( Float3& pVal, int  pDepth);
      void normEffectInit( Float3* lArray, int iLength );

      PIndex addPoint( Float3& p )
      {
	for( size_t i=0; i< cPoints.size(); i++)
	  {
	    Float3& lPt = cPoints[i];
	    if( lPt == p )
	      {
		// already exist, return indice
		return i;
	      }
	  }
	// no find, create it
	cPoints.push_back( p );
				
	return cPoints.size()-1; // return indice
      }
	 
      void addFacet(  Float3 p0, Float3 p1, Float3 p2 )
      {     
	cFacets.push_back( new MyFacet( addPoint(p0), addPoint(p1), addPoint(p2) ));
      }
      void addFacet(Float3 p0, Float3 p1, Float3 p2, Float3 p3)
      {
	cFacets.push_back( new MyFacet( addPoint(p0), addPoint(p1), addPoint(p2), addPoint(p3))); 
      }
      void addFacet(  Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4)
      {
	cFacets.push_back( new MyFacet( addPoint(p0), addPoint(p1), addPoint(p2), addPoint(p3),  addPoint(p4))); 
      }
      void addFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4, Float3 p5 )
      {
	cFacets.push_back( new MyFacet(  addPoint(p0),  addPoint(p1),  addPoint(p2),  addPoint(p3),  addPoint(p4),  addPoint(p5 ))); 
      }
      void addFacet( std::vector<Float3> & iVect )
      {
	MyFacet * lFacet = new MyFacet();
	cFacets.push_back( lFacet );
	
	for( Float3 lFloat : iVect )
	  {
	    lFacet->push_back( addPoint( lFloat ));
	  }
      }
    };
		

    //================================================

  public:
    static void      SubdivideN( SubParam & pParam, std::vector<PointPtr> & iVect, int  pDepth );
    static void      SubdivideN( SubParam& pParam, std::vector<Float3>  iVect, int  pDepth );
    static void      Subdivide5( SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, Float3 v5, int  pDepth );
    static void      Subdivide4 (SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, int  pDepth );
    static void      Subdivide3 (SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  int  pDepth );
		
    static SubParam& Parallelepiped( SubDiv::SubParam&  pParam, float pSzX, float pSzY, float pSzZ );
    static SubParam& Cube          ( SubDiv::SubParam&  pParam, float pSz   );
    static SubParam& Pyramid4      ( SubDiv::SubParam&  pParam, float pX, float pY, float pZ, float  pWidth, float pHeight  );
    static SubParam& Octahedron2   ( SubDiv::SubParam&  pParam );
    static SubParam& Dodecahedron  ( SubDiv::SubParam&  pParam );
    static SubParam& Octahedron    ( SubDiv::SubParam&  pParam );
    static SubParam& Icosahedron   ( SubDiv::SubParam&  pParam );
    static SubParam& Tetrahedron   ( SubDiv::SubParam&  pParam );
    static SubParam& Odron         ( SubDiv::SubParam&  pParam );
    static SubParam& Create        ( GeometryType iGtype, SubDiv::SubParam&  pParam );

    static void Triangle( SubDiv::SubParam& pParam, int i, Float3 data[], int nd[][3]);
    static void Triangle( SubDiv::SubParam& pParam,  Float3 x0, Float3 x1,  Float3 x2 );
	
  };
  //**************************************

} // end namespace

#endif
