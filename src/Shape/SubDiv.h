#ifndef H__SubDiv__H
#define H__SubDiv__H

#include <vector>


namespace  M3d {
	
	typedef Point3d Float3;

	//**************************************
	class MyFacet {

	public:

		std::vector<Float3> cPoints;

		MyFacet( Float3 p0, Float3 p1, Float3 p2 );
		MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3 );
		MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4 );
		MyFacet( Float3 p0, Float3 p1, Float3 p2, Float3 p3, Float3 p4, Float3 p5 );
	};

	
	//**************************************
	enum class SubNormalizeType{
		NORMALIZE_NONE  // DEVIENT TROP PETIT 
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
	//**************************************
	class SubParam {

	public:
		bool                 cUseTexture=false;
		int                  cDepth;
		float                cFact;
		SubNormalizeType     cNormalize;
		bool                 cCentralPoint;

		int                  cFlagStripFill;
		std::vector<MyFacet> cFacet;

		float                cDepthGrowFactor = 1f;
		float                cInitGrowFactor = 0.3f;

		int                  cHoleFacet = -1;
		int                  cHoleDepth = -1;
	protected :

		SubParam( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize );
		void reset( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize);
		
	public:
		void normEffectSub( Float3 pVal, int  pDepth);

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
		class SubParamObject3d : extends SubParam {
								
		public:
			SubParamObject3d( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize );
			void set( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType  pNormalize );
		};
	//**************************************
		class SubDiv{

		public:
		};

		//**************************************

	} // end namespace

#endif
