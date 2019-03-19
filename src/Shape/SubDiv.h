#ifndef H__SubDiv__H
#define H__SubDiv__H



#include "Point3d.h"

#include <vector>
	


using Float3 = PP3d::Point3d;


namespace  PP3d {

  //**************************************
  class SubDiv{

 
    //======================================
    class MyFacet {

    public:

      std::vector<Float3> cPoints;
			MyFacet();
      MyFacet( Float3 p0, Float3 p1, Float3 p2 ) {
				cPoints.push_back( p0);
				cPoints.push_back( p1);
				cPoints.push_back( p2);
			}

      MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3 ) {
				cPoints.push_back( p0);
				cPoints.push_back( p1);
				cPoints.push_back( p2);
				cPoints.push_back( p3);
			}
      MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4 ) {
				cPoints.push_back( p0);
				cPoints.push_back( p1);
				cPoints.push_back( p2);
				cPoints.push_back( p3);
				cPoints.push_back( p4);
			}
      MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4, Float3 p5 ) {
				cPoints.push_back( p0);
				cPoints.push_back( p1);
				cPoints.push_back( p2);
				cPoints.push_back( p3);
				cPoints.push_back( p4);
				cPoints.push_back( p5);
			}
    };

	
    //======================================

    enum  SubNormalizeType{
      NORMALIZE_NONE,  // DEVIENT TROP PETIT 
			NORMALIZE,
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
    //======================================
 	public:
   class SubParam {
			
    public:
      bool                 cUseTexture=false;
      int                  cDepth;
      float                cFact;
      SubNormalizeType     cNormalize;
      bool                 cCentralPoint;

      int                  cFlagStripFill;
      std::vector<MyFacet*> cFacets;

      float                cDepthGrowFactor = 1;
      float                cInitGrowFactor = 0.3;

      int                  cHoleFacet = -1;
      int                  cHoleDepth = -1;
    protected :

      SubParam( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize );
			virtual ~SubParam();
      void reset( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize);
		
    public:
      void normEffectSub ( Float3 pVal, int  pDepth);
			void normEffectInit( Float3* lArray, int iLength );
		};
    //				public Object3d getObject3d() {
    //						return null;
    //				}

    //**************************************
    // For direct drawing
    /*
      class SubParamDrawing:  extends SubParam {
			
      public :		
      SubParamDrawing( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize,  int pFlagStripFill );
      void set( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize,  int pFlagStripFill  );
      };
    */
    //================================================
    // For create an Object3d
    class SubParamObject3d : public SubParam {
								
    public:
      SubParamObject3d( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize );
      void set( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType  pNormalize );
    };
		
		//--------------------------------------

	public:
		static void      Subdivide5(  SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, Float3 v5, int  pDepth );
    static void      Subdivide (  SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, int  pDepth );
		static void      Subdivide (  SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  int  pDepth );
		
		static SubParam& Parallelepiped( SubDiv::SubParam&  pParam, float pSzX, float pSzY, float pSzZ );
		static SubParam& Cube          ( SubDiv::SubParam&  pParam, float pSz   );
		static SubParam& Pyramid4      ( SubDiv::SubParam&  pParam, float pX, float pY, float pZ, float  pWidth, float pHeight  );
		static SubParam& Odron         ( SubDiv::SubParam&  pParam );
		static SubParam& Octodron      ( SubDiv::SubParam&  pParam );
		static SubParam& Dodecahedron  ( SubDiv::SubParam&  pParam );
		static SubParam& Octahedron    ( SubDiv::SubParam&  pParam );
		static SubParam& Icosahedron   ( SubDiv::SubParam&  pParam );
		static SubParam& Tetrahedron   ( SubDiv::SubParam&  pParam );

		static void Triangle( SubDiv::SubParam& pParam, int i, Float3 data[], int nd[][3]);
		static void Triangle( SubDiv::SubParam& pParam,  Float3 x0, Float3 x1,  Float3 x2 );

  };
  //**************************************

} // end namespace

#endif
