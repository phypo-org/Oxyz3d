
#include <ostream>



include "SubDiv.h"



namespace  M3d {
	//**************************************

	SubDiv::SubParam::SubParam( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize )
	{
		reset( pDepth, pFact, pCentralPoint, pNormalize );
	}
	//------------------------------------------------
	void SubDiv::SubParam::reset( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize)
	{
		cDepth = pDepth;
		cFact  = pFact;
		cCentralPoint = pCentralPoint;
		cNormalize = pNormalize;
	}
	//------------------------------------------------
	void SubDiv::SubParam::normEffectSub( Float3 pVal, int  pDepth)
	{
		switch( cNormalize ){
		case NORMALIZE:
		case NORMALIZE_ONLY_SUB:
		case NORMALIZE_HALF_INIT:
		case NORMALIZE_DEC_INIT:
		case NORMALIZE_INC_INIT:
		case NORMALIZE_MUL_INIT:
			pVal.normalize( cFact );
			break;
								
		case 	NORMALIZE_DEC_SUB:
			pVal.normalize( cFact / (1+pDepth*cDepthGrowFactor) );
			break;
								
		case 	NORMALIZE_INC_SUB:
			pVal.normalize( cFact * (1+pDepth*cDepthGrowFactor) );
			break;
		case 	NORMALIZE_MUL_SUB:
			pVal.normalize( cFact *cDepthGrowFactor );
			break;

		case NORMALIZE_ONLY_INIT:								
		case NORMALIZE_NONE:
			pVal.multiply( cFact );
			break;
		}
	}	 
	//------------------------------------------------
	void SubDiv::SubParam::normEffectInit( Float3 iArray [] )
	{
						
		for( int i = 0; i< iArray.length; i++ )
			{
								
				switch( cNormalize )
					{
										
					case NORMALIZE:
					case NORMALIZE_DEC_SUB:
					case NORMALIZE_INC_SUB:
					case NORMALIZE_MUL_SUB:
					case NORMALIZE_ONLY_INIT:	
						iArray[i].normalize( cFact );
						break;
										
					case NORMALIZE_HALF_INIT:
						if( i % 2 == 0  )
							iArray[i].normalize( cFact );
						else
							iArray[i].multiply( cFact );
						break;
										
					case NORMALIZE_DEC_INIT:
						iArray[i].normalize( cFact / (1+i*cInitGrowFactor) );
						break;
										
					case NORMALIZE_INC_INIT:			
						iArray[i].normalize( cFact * (1+i*cInitGrowFactor) );
						break;

					case	NORMALIZE_MUL_INIT:								
						iArray[i].normalize( cFact *cInitGrowFactor );
						break;


					case NORMALIZE_NONE:										
					case NORMALIZE_ONLY_SUB:
						iArray[i].multiply( cFact );
						break;
					}
			}	 
	}	
	
	//**************************************
	/*
	SubDiv::SubParamDrawing::SubParamDrawing( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize,  int pFlagStripFill )
		:SubParam( pDepth, pFact, pCentralPoint, pNormalize)
	{		
		cFlagStripFill =  pFlagStripFill;
	}
	//------------------------------------------------
	void SubParamDrawing::set( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize,  int pFlagStripFill  ){						
		reset( pDepth, pFact, pCentralPoint, pNormalize );
		cFlagStripFill =  pFlagStripFill;
	}
	//------------------------------------------------
	SubParamDrawing SubParamDrawing::getSubParamDrawing(  int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize, int pFlagStripFill )
	{
		return new SubParamDrawing(   pDepth, pFact, pCentralPoint, pNormalize, pFlagStripFill );
	}
	*/
	//**************************************
	SubDiv::SubParamObject3d::SubParamObject3d( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType pNormalize )
		:SubDiv::SubParam( pDepth, pFact, pCentralPoint, pNormalize)
	{		
		//	cFacet = new std::vector<MyFacet>();
	}	 
	//------------------------------------------------
	void SubParamObject3d::set( int pDepth, float pFact, boolean pCentralPoint, SubNormalizeType  pNormalize )
	{			
		reset( pDepth, pFact, pCentralPoint, pNormalize );		
		//	cFacet = new std::vector<MyFacet>();
	}
	//------------------------------------------------
	//------------------------------------------------
	//------------------------------------------------
	/*
	SubParamObject3d* GetSubParamObject3d(  int pDepth, float pFact, boolean pCentralPoint,  SubNormalizeType pNormalize )
	{
		return new SubParamObject3d( pDepth, pFact, pCentralPoint, pNormalize );
	}
	*/
	//------------------------------------------------
	/*		public Object3d getObject3d() {

				Facet [] lFacetArray = new Facet[ cFacet.size() ];
				Object3d lObject3d = new Object3d( cFacet.toArray( lFacetArray )  );
				lObject3d.setUseTexture( cUseTexture);

				cFacet = new ArrayList(); // reset for future use !
					
				return lObject3d; 
				}
	*/
	//------------------------------------------------
	void SubDiv::Subdivide5( Float3 v1, Float3 v2, Float3 v3,  Float3 v4, Float3 v5, int  pDepth, SubParam& pParam )
	{
		
		if( pDepth <= 0  )
			{	
				if( pParam.cHoleFacet <= 5 && pParam.cHoleDepth <= pParam.cDepth 
						&& pParam.cHoleDepth != -1 && pParam.cDepth != -1 )
					return;
				/*
					if( pParam.cFacet == null )
					{
						
					if( pParam.cFlagStripFill == GLUgl2.GLU_SILHOUETTE )
					DrawPentaStrip( Engine.sTheGl, v1, v2, v3, v4, v5 );
					else
					DrawPentaNorm( Engine.sTheGl, v1, v2, v3, v4, v5 );
					 
					}
					else
				*/
				{
					pParam.cFacet.add( new Facet( v1, v2, v3, v4, v5 ));
				}
				return;
			}

	
		pDepth--;
	
		if( pParam.cCentralPoint )
			{
				Float3 v0 = Float3.Middle( v1, v2, v3, v4, v5 );	
				pParam.normEffectSub( v0, pDepth  );
			
				Subdivide( v1, v2, v0, pDepth, pParam );
				Subdivide( v2, v3, v0, pDepth, pParam );
				Subdivide( v3, v4, v0, pDepth, pParam );
				Subdivide( v4, v5, v0, pDepth, pParam );
				Subdivide( v5, v1, v0, pDepth, pParam );
			
			}
		else
			{
				Float3 v12 = Float3.Middle( v1, v2 );
				Float3 v23 = Float3.Middle( v2, v3 );
				Float3 v34 = Float3.Middle( v3, v4 );
				Float3 v45 = Float3.Middle( v4, v5 );
				Float3 v51 = Float3.Middle( v5, v1 );

				pParam.normEffectSub( v12, pDepth  );
				pParam.normEffectSub( v23, pDepth  );
				pParam.normEffectSub( v34, pDepth  );
				pParam.normEffectSub( v45, pDepth  );
				pParam.normEffectSub( v51, pDepth  );
						
				Subdivide( v1, v12, v51, pDepth, pParam );
				Subdivide( v2, v23, v12, pDepth, pParam);
				Subdivide( v3, v34, v23, pDepth, pParam );
				Subdivide( v4, v45, v34, pDepth, pParam );
				Subdivide( v5, v51, v45, pDepth, pParam );

				Subdivide5( v12, v23, v34, v45, v51, pDepth, pParam );
			}
	}
	//------------------------------------------------
	void SubDiv::Subdivide( Float3 v1, Float3 v2, Float3 v3,  Float3 v4, int  pDepth,  SubDiv::SubParam& pParam )
	{
		if( pDepth <= 0  )
			{						 
				if( pParam.cHoleFacet <= 4  && pParam.cHoleDepth <= pParam.cDepth
						&& pParam.cHoleDepth != -1 && pParam.cDepth != -1)
					return;
				/*
					if( pParam.cFacet == null )
					{
						
					if( pParam.cFlagStripFill == GLUgl2.GLU_SILHOUETTE )
					{
					DrawSquareStrip( Engine.sTheGl, v1, v2, v3, v4 );
					}
					else
					{
					DrawSquareNorm( Engine.sTheGl, v1, v2, v3, v4 );
					}
					}
					else
				*/
				{
					pParam.cFacet.add( new Facet( v1, v2, v3, v4 ));
				}
				return;
			}

		pDepth--;

		if( pParam.cCentralPoint )
			{
				Float3 v0 = Float3.Middle( v1, v2, v3, v4 );	
				pParam.normEffectSub( v0, pDepth  );
				
				Subdivide( v1, v2, v0, pDepth, pParam );
				Subdivide( v2, v3, v0, pDepth, pParam );
				Subdivide( v3, v4, v0, pDepth, pParam );
				Subdivide( v4, v1, v0, pDepth, pParam );
				
			}
		else
			{
				Float3 v12 = Float3.Middle( v1, v2 );
				Float3 v23 = Float3.Middle( v2, v3 );
				Float3 v34 = Float3.Middle( v3, v4 );
				Float3 v41 = Float3.Middle( v4, v1 );
						
				pParam.normEffectSub( v12, pDepth  );
				pParam.normEffectSub( v23, pDepth  );
				pParam.normEffectSub( v34, pDepth  );
				pParam.normEffectSub( v41, pDepth  );
						
				Subdivide( v1, v12, v41, pDepth, pParam );
				Subdivide( v2, v23, v12, pDepth, pParam);
				Subdivide( v3, v34, v23, pDepth, pParam );
				Subdivide( v4, v41, v34, pDepth, pParam);
				Subdivide( v12, v23, v34, v41, pDepth, pParam );
			}
	}
	//------------------------------------------------
	void SubDiv::Subdivide( Float3 v1, Float3 v2, Float3 v3, int  pDepth,  SubDiv::SubParam& pParam )
	{
				
		if( pDepth <= 0 )
			{
				/*
					if( pParam.cFacet == null )
					{
					if( pParam.cFlagStripFill ==  GLUgl2.GLU_SILHOUETTE )
					DrawTriangleStrip( Engine.sTheGl, v1, v2, v3 );
					else
					DrawTriangleNorm( Engine.sTheGl, v1, v2, v3 );
					}
					else
				*/
				{
					pParam.cFacet.add( new Facet( v1, v2, v3 ) );
				}
				return;
			}

		pDepth--;

		if( pParam.cCentralPoint )
			{
				Float3 v0 = Float3.Middle( v1, v2, v3 );	

				pParam.normEffectSub( v0, pDepth  );

				Subdivide( v1, v2, v0, pDepth, pParam );
				Subdivide( v2, v3, v0, pDepth, pParam );
				Subdivide( v3, v1, v0, pDepth, pParam );
			}
		else
			{ 				 					
				Float3 v12 = Float3.Middle( v1, v2 );
				Float3 v23 = Float3.Middle( v2, v3 );
				Float3 v31 = Float3.Middle( v3, v1 );
						
				pParam.normEffectSub( v12, pDepth  );
				pParam.normEffectSub( v23, pDepth  );
				pParam.normEffectSub( v31, pDepth  );

				Subdivide( v1, v12, v31,  pDepth, pParam);
				Subdivide( v2, v23, v12,  pDepth, pParam );
				Subdivide( v3, v31, v23,  pDepth, pParam);
				Subdivide( v12, v23, v31, pDepth, pParam );
			}
	}
	//------------------------------------------------
	SubDiv::SubParam& SubDiv::Parallelepiped( float pSzX, float pSzY, float pSzZ, SubDiv::SubParam& pParam )
	{				
		Float3 lA = new Float3(  pSzX*0.5f, -pSzY*0.5f, -pSzZ*0.5f ); 
		Float3 lB = new Float3(  pSzX*0.5f,  pSzY*0.5f, -pSzZ*0.5f );    
		Float3 lC = new Float3(  -pSzX*0.5f,  pSzY*0.5f, -pSzZ*0.5f );    
		Float3 lD = new Float3(  -pSzX*0.5f,  -pSzY*0.5f, -pSzZ*0.5f );    

		Float3 lE = new Float3(  pSzX*0.5f, -pSzY*0.5f, pSzZ*0.5f ); 
		Float3 lF = new Float3(  pSzX*0.5f,  pSzY*0.5f, pSzZ*0.5f );    
		Float3 lG = new Float3(  -pSzX*0.5f,  pSzY*0.5f, pSzZ*0.5f );    
		Float3 lH = new Float3(  -pSzX*0.5f,  -pSzY*0.5f, pSzZ*0.5f );  


		Float3 lData[] = { 
			lA, lB, lC, lD, lE, lF, lG, lH 
		};

		pParam.normEffectInit( lData );

		Subdivide( lA, lB, lC, lD, pParam.cDepth, pParam );						
		Subdivide( lE, lH, lG, lF, pParam.cDepth, pParam );					
		Subdivide( lA, lE, lF, lB, pParam.cDepth, pParam );						
		Subdivide( lD, lC, lG, lH, pParam.cDepth, pParam );						
		Subdivide( lB, lF, lG, lC, pParam.cDepth, pParam );
		Subdivide( lA, lD, lH, lE, pParam.cDepth, pParam );			

		return pParam;
	}
	//------------------------------------------------
	SubDiv::SubParam& SubDiv::Cube(  float pSz,  SubDiv::SubParam& pParam )
	{
		return Parallelepiped( pSz, pSz, pSz, pParam);
	}																																			 
	//------------------------------------------------
	SubDiv::SubParam &	SubDiv::Pyramid4(  float pX, float pY, float pZ, float  pWidth, float pHeight, SubDiv::SubParam& pParam  )
	{

		Float3 lTop    = new Float3( pX, pY + pHeight, pZ );
		Float3 lFrontLeft  = new Float3( pX - pWidth, pY - pHeight, pZ + pWidth);    // Front left point
		Float3 lFrontRight = new Float3( pX + pWidth, pY - pHeight, pZ + pWidth);    // Front right point
		Float3 lBackRight  = new Float3( pX + pWidth, pY - pHeight, pZ - pWidth);    // Back right point
		Float3 lBackLeft   = new Float3 (pX - pWidth, pY - pHeight, pZ - pWidth);    // Back left point

		Float3 lData[] = { lTop, lFrontLeft, lFrontRight, lBackRight, lBackLeft };
		pParam.normEffectInit( lData );

				
		Facet lPyramid4[] = new Facet[5];

		Subdivide( lTop, lFrontRight, lFrontLeft, pParam.cDepth, pParam );
		Subdivide( lTop, lFrontLeft, lBackLeft, pParam.cDepth, pParam );
		Subdivide( lTop, lBackLeft, lBackRight, pParam.cDepth, pParam );
		Subdivide( lTop, lBackRight, lFrontRight, pParam.cDepth, pParam );
		Subdivide( lFrontRight, lFrontLeft, lBackLeft, lBackRight, pParam.cDepth, pParam );

		return pParam;
	}
	//------------------------------------------------
	static float T  = 1.73205080756887729f;
		
	SubDiv::SubParam& SubDiv::Odron(SubDiv::SubParam& pParam )
	{
		
		Float3 lData[] = new Float3[4];
				
		lData[0] = new Float3(  T,  T,  T );
		lData[1] = new Float3(  T, -T, -T );
		lData[2] = new Float3( -T,  T, -T );
		lData[3] = new Float3( -T, -T,  T );

		pParam.normEffectInit( lData );


		if( pParam.cUseTexture )
			{

				//				System.out.println( "********** Odron Text");
				// A MODIFIER  il faut modifier les coordonnees qui sont fauses

				Float2 lTexa[] = new Float2[ 3 ];
				lTexa[ 0 ] = new Float2( 0.5f, 1.0f ); //0.0						
				lTexa[ 1 ] = new Float2( 0.0f, 0.0f ); //1.0;						
				lTexa[ 2 ] = new Float2( 1.0f, 0.0f ); //1.0
	
						
				Subdivide3Tex( lData[0], lData[1], lData[3], lTexa[0], lTexa[1], lTexa[2], pParam.cDepth, pParam );
				Subdivide3Tex( lData[2], lData[1], lData[0], lTexa[0], lTexa[1], lTexa[2], pParam.cDepth, pParam );
				Subdivide3Tex( lData[3], lData[2], lData[0], lTexa[0], lTexa[1], lTexa[2], pParam.cDepth, pParam );
				Subdivide3Tex( lData[1], lData[2], lData[3], lTexa[0], lTexa[1], lTexa[2], pParam.cDepth, pParam );		
						
			}
		else
			{						
				Subdivide( lData[0], lData[1], lData[3], pParam.cDepth, pParam );
				Subdivide( lData[2], lData[1], lData[0], pParam.cDepth, pParam );
				Subdivide( lData[3], lData[2], lData[0], pParam.cDepth, pParam );
				Subdivide( lData[1], lData[2], lData[3], pParam.cDepth, pParam );		
			}
		return pParam;
	}

	//------------------------------------------------
	SubDiv::SubParam& SubDiv::Octodron( SubDiv::SubParam& pParam )
	{
		Float3 lData[] = new Float3[6];
				
		lData[0] = new Float3(  T,  0,  0 );
		lData[1] = new Float3( -T,  0,  0 );
		lData[2] = new Float3(  0,  T,  0 );
		lData[3] = new Float3(  0, -T,  0 );
		lData[4] = new Float3(  0,  0,  T );
		lData[5] = new Float3(  0,  0, -T );

		pParam.normEffectInit( lData );

		Subdivide( lData[0], lData[4], lData[2], pParam.cDepth, pParam );
		Subdivide( lData[2], lData[4], lData[1], pParam.cDepth, pParam );
		Subdivide( lData[1], lData[4], lData[3], pParam.cDepth, pParam );
		Subdivide( lData[3], lData[4], lData[0], pParam.cDepth, pParam );
		Subdivide( lData[0], lData[2], lData[5], pParam.cDepth, pParam );
		Subdivide( lData[2], lData[1], lData[5], pParam.cDepth, pParam );
		Subdivide( lData[1], lData[3], lData[5], pParam.cDepth, pParam );
		Subdivide( lData[3], lData[0], lData[5], pParam.cDepth, pParam );
				
		return pParam;
	}

		//------------------------------------------------
		SubDiv::SubParam& SubDiv::Dodecahedron( SubDiv::SubParam pParam  )
		{

				if( sDodec == null )InitDodecahedron();

				pParam.normEffectInit( sDodec );

				Subdivide5( sDodec[0], sDodec[ 1], sDodec[ 9], sDodec[ 16], sDodec[ 5], pParam.cDepth, pParam);
			  Subdivide5( sDodec[1], sDodec[ 0], sDodec[ 3], sDodec[ 18], sDodec[ 7], pParam.cDepth, pParam);
			  Subdivide5( sDodec[1], sDodec[ 7], sDodec[ 11], sDodec[ 10], sDodec[ 9], pParam.cDepth, pParam);
				Subdivide5( sDodec[11], sDodec[ 7], sDodec[ 18], sDodec[ 19], sDodec[ 6], pParam.cDepth, pParam);
				Subdivide5( sDodec[8], sDodec[ 17], sDodec[ 16], sDodec[ 9], sDodec[ 10], pParam.cDepth, pParam);
				Subdivide5( sDodec[2], sDodec[ 14], sDodec[ 15], sDodec[ 6], sDodec[ 19], pParam.cDepth, pParam);
				Subdivide5( sDodec[2], sDodec[ 13], sDodec[ 12], sDodec[ 4], sDodec[ 14], pParam.cDepth, pParam);
				Subdivide5( sDodec[2], sDodec[ 19], sDodec[ 18], sDodec[ 3], sDodec[ 13], pParam.cDepth, pParam);
				Subdivide5( sDodec[3], sDodec[ 0], sDodec[ 5], sDodec[ 12], sDodec[ 13], pParam.cDepth, pParam);
				Subdivide5( sDodec[6], sDodec[ 15], sDodec[ 8], sDodec[ 10], sDodec[ 11], pParam.cDepth, pParam);
				Subdivide5( sDodec[4], sDodec[ 17], sDodec[ 8], sDodec[ 15], sDodec[ 14], pParam.cDepth, pParam);
				Subdivide5( sDodec[4], sDodec[ 12], sDodec[ 5], sDodec[ 16], sDodec[ 17], pParam.cDepth, pParam);

				return pParam;													
		}
		//------------------------------------------------
		
		static void Triangle( int i, Float3 data[], int ndx[][], SubDiv::SubParam pParam)
		{

				//		System.out.println( "  DrawTriangle "  + i);

				Float3 x0 = data[ ndx[i][0] ];
				Float3 x1 = data[ ndx[i][1] ];
				Float3 x2 = data[ ndx[i][2] ];

			SubDiv::Subdivide( x0, x1, x2, pParam.cDepth, pParam );	
		}
		//------------------------------------------------
		/* octahedron data: The octahedron produced is centered at the
			 origin and has radius 1.0 */

		final static Float3 sOcta[] = {	
				new Float3( 1.0f, 0.0f, 0.0f),
				new Float3(-1.0f, 0.0f, 0.0f),
				new Float3(0.0f, 1.0f, 0.0f),
				new Float3(0.0f, -1.0f, 0.0f),
				new Float3(0.0f, 0.0f, 1.0f),
				new Float3(0.0f, 0.0f, -1.0f)				
		};
		final static int  sOctaIdx[][] = {
				{0, 4, 2},
				{1, 2, 4},
				{0, 3, 4},
				{1, 4, 3},
				{0, 2, 5},
				{1, 5, 2},
				{0, 5, 3},
				{1, 3, 5}
		};
		//------------------------------------------------
	SubDiv::SubParam& SubDiv::Octahedron( SubDiv::SubParam& pParam  )
	{
				
				pParam.normEffectInit( sOcta);
	
				for ( int i = 7; i >= 0; i--) {
						Triangle( i, sOcta, sOctaIdx,  pParam);
				}
				return pParam;
		}
		//------------------------------------------------	
		/* icosahedron data: These numbers are rigged to make an
			 icosahedron of radius 1.0 */
		
		static  float sIicosaX =0.525731112119133606f;
		static  float sIicosaZ =0.850650808352039932f;
		
		static Float3 sIicosa[] = {
				new Float3(-sIicosaX, 0, sIicosaZ),
				new Float3(sIicosaX, 0, sIicosaZ),
				new Float3(-sIicosaX, 0, -sIicosaZ),
				new Float3(sIicosaX, 0, -sIicosaZ),
				new Float3(0, sIicosaZ, sIicosaX),
				new Float3(0, sIicosaZ, -sIicosaX),
				new Float3(0, -sIicosaZ, sIicosaX),
				new Float3(0, -sIicosaZ, -sIicosaX),
				new Float3(sIicosaZ, sIicosaX, 0),
				new Float3(-sIicosaZ, sIicosaX, 0),
				new Float3(sIicosaZ, -sIicosaX, 0),
				new Float3(-sIicosaZ, -sIicosaX, 0)
		};
		
		static int sIicosaIdx[][] =		{
				{0, 4, 1},
				{0, 9, 4},
				{9, 5, 4},
				{4, 5, 8},
				{4, 8, 1},
				{8, 10, 1},
				{8, 3, 10},
				{5, 3, 8},
				{5, 2, 3},
				{2, 7, 3},
				{7, 10, 3},
				{7, 6, 10},
				{7, 11, 6},
				{11, 0, 6},
				{0, 1, 6},
				{6, 1, 10},
				{9, 0, 11},
				{9, 11, 2},
				{9, 2, 5},
				{7, 2, 11},
		};
	//-----------------------
	SubDiv::SubParam& 	SubDiv::Icosahedron( SubDiv::SubParam& pParam )
		{				
				pParam.normEffectInit( sIicosa );

				for ( int i = 19; i >= 0; i-- ) 
						Triangle(i, sIicosa, sIicosaIdx, pParam);
				return pParam;
		}
		//------------------------------------------------	
		/* tetrahedron data: */

	static float  sTetraT =  1.73205080756887729f;
		
	Float3 sTetra[] =		{
				new Float3( sTetraT, sTetraT, sTetraT),
				new Float3( sTetraT, -sTetraT, -sTetraT),
				new Float3(-sTetraT, sTetraT, -sTetraT),
				new Float3(-sTetraT, -sTetraT, sTetraT)
		};
		
	int sTetraIdx[][] = {
				{0, 1, 3},
				{2, 1, 0},
				{3, 2, 0},
				{1, 2, 3}
		};
			//-----------------------
		SubDiv::SubParam&  SubDiv::Tetrahedron( SubPara&m pParam )
		{
				pParam.normEffectInit( sTetra );

				for ( int i = 3; i >= 0; i--)
						Triangle(i, sTetra, sTetraIdx, pParam);

				return pParam;
		}

	//**************************************

}; // end namespace
