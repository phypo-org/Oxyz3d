
#include <ostream>



#include "SubDiv.h"

#include <math.h>

namespace  PP3d {
  //**************************************

  SubDiv::SubParam::SubParam( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize )
  {
    reset( pDepth, pFact, pCentralPoint, pNormalize );
  }
  //------------------------------------------------
  SubDiv::SubParam::~SubParam()
  {
    for( MyFacet* lFacet: cFacets )
      {
	delete lFacet;
      }
  }
  //------------------------------------------------
  void SubDiv::SubParam::reset( int pDepth, float pFact, bool pCentralPoint, SubNormalizeType pNormalize)
  {
    cDepth = pDepth;
    cFact  = pFact;
    cCentralPoint = pCentralPoint;
    cNormalize = pNormalize;
  }
  //------------------------------------------------
  void SubDiv::SubParam::normEffectSub( Float3& pVal, int  pDepth)
  {
    //		std::cout << "normEffectSub " << pVal 
    switch( cNormalize )
      {
      case SubNormalizeType::NORMALIZE:
      case SubNormalizeType::NORMALIZE_ONLY_SUB:
      case SubNormalizeType::NORMALIZE_HALF_INIT:
      case SubNormalizeType::NORMALIZE_DEC_INIT:
      case SubNormalizeType::NORMALIZE_INC_INIT:
      case SubNormalizeType::NORMALIZE_MUL_INIT:
	pVal.normalize( cFact );
	break;
				
      case 	SubNormalizeType::NORMALIZE_DEC_SUB:
	pVal.normalize( cFact / (1+((float)pDepth)*cDepthGrowFactor) );
	break;
				
      case 	SubNormalizeType::NORMALIZE_INC_SUB:
	pVal.normalize( cFact * (1+((float)pDepth)*cDepthGrowFactor) );
	break;
      case 	SubNormalizeType::NORMALIZE_MUL_SUB:
	pVal.normalize( cFact *cDepthGrowFactor );
	break;
				
      case SubNormalizeType::NORMALIZE_ONLY_INIT:								
      case SubNormalizeType::NORMALIZE_NONE:
	pVal *= cFact ;
	break;
      }
  }	 
  //------------------------------------------------
  void SubDiv::SubParam::normEffectInit( Float3* iArray, int iLength )
  {						
    for( int i = 0; i< iLength; i++ )
      {
								
	switch( cNormalize )
	  {										
	  case SubNormalizeType::NORMALIZE:
	  case SubNormalizeType::NORMALIZE_DEC_SUB:
	  case SubNormalizeType::NORMALIZE_INC_SUB:
	  case SubNormalizeType::NORMALIZE_MUL_SUB:
	  case SubNormalizeType::NORMALIZE_ONLY_INIT:	
	    iArray[i].normalize( cFact );
	    break;
										
	  case SubNormalizeType::NORMALIZE_HALF_INIT:
	    if( i % 2 == 0  )
	      iArray[i].normalize( cFact );
	    else
	      iArray[i] *=  cFact ;
	    break;
										
	  case SubNormalizeType::NORMALIZE_DEC_INIT:
	    iArray[i].normalize( ((float)cFact) / (1+((float)i)*cInitGrowFactor) );
	    break;
										
	  case SubNormalizeType::NORMALIZE_INC_INIT:			
	    iArray[i].normalize(  ((float)cFact) * (1+((float)i)*cInitGrowFactor) );
	    break;

	  case	SubNormalizeType::NORMALIZE_MUL_INIT:								
	    iArray[i].normalize(  ((float)cFact) *cInitGrowFactor );
	    break;


	  case SubNormalizeType::NORMALIZE_NONE:										
	  case SubNormalizeType::NORMALIZE_ONLY_SUB:
	    iArray[i] *= cFact;
	    break;
	  }
      }	 
  }	
	
  //------------------------------------------------
  void SubDiv::Subdivide5( SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, Float3 v5, int  pDepth )
  {
		
    if( pDepth <= 0  )
      {	
	if( pParam.cHoleFacet <= 5 && pParam.cHoleDepth <= pParam.cDepth 
	    && pParam.cHoleDepth != -1 && pParam.cDepth != -1 )
	  return;

	
	pParam.addFacet( v1, v2, v3, v4, v5 );
	
	return;
      }

	
    pDepth--;
	
    if( pParam.cCentralPoint )
      {
	Float3 v0 = Float3::Middle( v1, v2, v3, v4, v5 );	
	pParam.normEffectSub( v0, pDepth  );
			
	Subdivide3( pParam, v1, v2, v0, pDepth );
	Subdivide3( pParam, v2, v3, v0, pDepth );
	Subdivide3( pParam, v3, v4, v0, pDepth );
	Subdivide3( pParam, v4, v5, v0, pDepth );
	Subdivide3( pParam, v5, v1, v0, pDepth );
			
      }
    else
      {
	Float3 v12 = Float3::Middle( v1, v2 );
	Float3 v23 = Float3::Middle( v2, v3 );
	Float3 v34 = Float3::Middle( v3, v4 );
	Float3 v45 = Float3::Middle( v4, v5 );
	Float3 v51 = Float3::Middle( v5, v1 );

	pParam.normEffectSub( v12, pDepth  );
	pParam.normEffectSub( v23, pDepth  );
	pParam.normEffectSub( v34, pDepth  );
	pParam.normEffectSub( v45, pDepth  );
	pParam.normEffectSub( v51, pDepth  );
						
	Subdivide3( pParam, v1, v12, v51, pDepth );
	Subdivide3( pParam, v2, v23, v12, pDepth);
	Subdivide3( pParam, v3, v34, v23, pDepth );
	Subdivide3( pParam, v4, v45, v34, pDepth );
	Subdivide3( pParam, v5, v51, v45, pDepth );

	Subdivide5( pParam, v12, v23, v34, v45, v51, pDepth );
      }
  }
  //------------------------------------------------
  void SubDiv::Subdivide4( SubDiv::SubParam& pParam, Float3 v1, Float3 v2, Float3 v3,  Float3 v4, int  pDepth)
  {
    if( pDepth <= 0  )
      {						 
	if( pParam.cHoleFacet <= 4  && pParam.cHoleDepth <= pParam.cDepth
	    && pParam.cHoleDepth != -1 && pParam.cDepth != -1)
	  return;

	pParam.addFacet(  v1, v2, v3, v4 );
	  
	return;
      }

    pDepth--;

    if( pParam.cCentralPoint )
      {
	Float3 v0 = Float3::Middle( v1, v2, v3, v4 );	
	pParam.normEffectSub( v0, pDepth  );
				
	Subdivide3( pParam, v1, v2, v0, pDepth );
	Subdivide3( pParam, v2, v3, v0, pDepth );
	Subdivide3( pParam, v3, v4, v0, pDepth );
	Subdivide3( pParam, v4, v1, v0, pDepth );
      }
    else
      {
	Float3 v12 = Float3::Middle( v1, v2 );
	Float3 v23 = Float3::Middle( v2, v3 );
	Float3 v34 = Float3::Middle( v3, v4 );
	Float3 v41 = Float3::Middle( v4, v1 );
						
	pParam.normEffectSub( v12, pDepth  );
	pParam.normEffectSub( v23, pDepth  );
	pParam.normEffectSub( v34, pDepth  );
	pParam.normEffectSub( v41, pDepth  );
						
	Subdivide3( pParam, v1, v12, v41, pDepth );
	Subdivide3( pParam, v2, v23, v12, pDepth);
	Subdivide3( pParam, v3, v34, v23, pDepth );
	Subdivide3( pParam, v4, v41, v34, pDepth);
	
	Subdivide4( pParam, v12, v23, v34, v41, pDepth );
      }
  }
  //------------------------------------------------
  void SubDiv::Subdivide3( SubDiv::SubParam& pParam, Float3 v1, Float3 v2, Float3 v3, int  pDepth )
  {
				
    if( pDepth <= 0 )
      {
	pParam.addFacet(  v1, v2, v3 );
	return;
      }

    pDepth--;

    if( pParam.cCentralPoint )
      {
	Float3 v0 = Float3::Middle( v1, v2, v3 );	

	pParam.normEffectSub( v0, pDepth  );

	Subdivide3( pParam, v1, v2, v0, pDepth );
	Subdivide3( pParam, v2, v3, v0, pDepth );
	Subdivide3( pParam, v3, v1, v0, pDepth );
      }
    else
      { 				 					
	Float3 v12 = Float3::Middle( v1, v2 );
	Float3 v23 = Float3::Middle( v2, v3 );
	Float3 v31 = Float3::Middle( v3, v1 );
						
	pParam.normEffectSub( v12, pDepth  );
	pParam.normEffectSub( v23, pDepth  );
	pParam.normEffectSub( v31, pDepth  );

	Subdivide3( pParam, v1, v12, v31,  pDepth);
	Subdivide3( pParam, v2, v23, v12,  pDepth );
	Subdivide3( pParam, v3, v31, v23,  pDepth);
	Subdivide3( pParam, v12, v23, v31, pDepth );
      }
  }
  //------------------------------------------------
  //------------------------------------------------
  //------------------------------------------------
  SubDiv::SubParam&  SubDiv::Create(  SubDiv::GeometryType iGtype, SubDiv::SubParam&  pParam )
  {
    SubParam & lParam = pParam ;
    std::cout << " SubDiv::Create" << (int)iGtype << std::endl;
    
    switch(iGtype )
      {
      case   GeometryType::TETRAHEDRON :   return Tetrahedron (  pParam );
      case   GeometryType::PYRAMID4 :      return Pyramid4    (  pParam, 0, 0, 0, 1, 1 );
      case   GeometryType::CUBE :          return Cube        (  pParam, 1 );
      case   GeometryType::OCTAHEDRON :    return Octahedron  (  pParam );
      case   GeometryType::DODECAHEDRON :  return Dodecahedron(  pParam );
      case   GeometryType::ICOSAHEDRON :   return Icosahedron (  pParam );

	
      case   GeometryType::OCTAHEDRON2:    return Octahedron2    (  pParam );	// ???
      case   GeometryType::ODRON   :   // ????
     default:
	return Odron       (  pParam );
      }

		
    return lParam;
  }

  //------------------------------------------------
  SubDiv::SubParam& SubDiv::Parallelepiped( SubDiv::SubParam& pParam,  float pSzX, float pSzY, float pSzZ )
  {				
    Float3 lA(  pSzX*0.5, -pSzY*0.5, -pSzZ*0.5 ); 
    Float3 lB(  pSzX*0.5,  pSzY*0.5, -pSzZ*0.5 );    
    Float3 lC(  -pSzX*0.5,  pSzY*0.5, -pSzZ*0.5 );    
    Float3 lD(  -pSzX*0.5,  -pSzY*0.5, -pSzZ*0.5 );    

    Float3 lE(  pSzX*0.5, -pSzY*0.5, pSzZ*0.5 ); 
    Float3 lF(  pSzX*0.5,  pSzY*0.5, pSzZ*0.5 );    
    Float3 lG(  -pSzX*0.5,  pSzY*0.5, pSzZ*0.5 );    
    Float3 lH(  -pSzX*0.5,  -pSzY*0.5, pSzZ*0.5 );  


    Float3 lData[] = { 
      lA, lB, lC, lD, lE, lF, lG, lH 
    };

    pParam.normEffectInit( lData, 8 );

    Subdivide4( pParam, lA, lB, lC, lD, pParam.cDepth );						
    Subdivide4( pParam, lE, lH, lG, lF, pParam.cDepth );					
    Subdivide4( pParam, lA, lE, lF, lB, pParam.cDepth );						
    Subdivide4( pParam, lD, lC, lG, lH, pParam.cDepth );						
    Subdivide4( pParam, lB, lF, lG, lC, pParam.cDepth );
    Subdivide4( pParam, lA, lD, lH, lE, pParam.cDepth );			

    return pParam;
  }
  //------------------------------------------------
  SubDiv::SubParam& SubDiv::Cube(  SubDiv::SubParam& pParam,  float pSz  )
  {
    return Parallelepiped( pParam, pSz, pSz, pSz);
  }
  //------------------------------------------------
  SubDiv::SubParam &	SubDiv::Pyramid4( SubDiv::SubParam& pParam, float pX, float pY, float pZ, float  pWidth, float pHeight  )
  {

    Float3 lTop( pX, pY + pHeight, pZ );
    Float3 lFrontLeft( pX - pWidth, pY - pHeight, pZ + pWidth);    // Front left point
    Float3 lFrontRight( pX + pWidth, pY - pHeight, pZ + pWidth);    // Front right point
    Float3 lBackRight( pX + pWidth, pY - pHeight, pZ - pWidth);    // Back right point
    Float3 lBackLeft(pX - pWidth, pY - pHeight, pZ - pWidth);    // Back left point

    Float3 lData[] = { lTop, lFrontLeft, lFrontRight, lBackRight, lBackLeft };
    pParam.normEffectInit( lData, 5 );

    // MyFacet lPyramid4[5];

    Subdivide3( pParam, lTop, lFrontRight, lFrontLeft, pParam.cDepth );
    Subdivide3( pParam, lTop, lFrontLeft, lBackLeft, pParam.cDepth );
    Subdivide3( pParam, lTop, lBackLeft, lBackRight, pParam.cDepth );
    Subdivide3( pParam, lTop, lBackRight, lFrontRight, pParam.cDepth );
    Subdivide4( pParam, lFrontRight, lFrontLeft, lBackLeft, lBackRight, pParam.cDepth );

    return pParam;
  }
  //------------------------------------------------
  static float T  = 1.73205080756887729f;
		
  SubDiv::SubParam& SubDiv::Odron(SubDiv::SubParam& pParam )
  {
		
    Float3 lData[4];
				
    lData[0] = Float3(  T,  T,  T );
    lData[1] = Float3(  T, -T, -T );
    lData[2] = Float3( -T,  T, -T );
    lData[3] = Float3( -T, -T,  T );

    pParam.normEffectInit( lData, 4 );
    
    					
    Subdivide3( pParam, lData[0], lData[1], lData[3], pParam.cDepth );
    Subdivide3( pParam, lData[2], lData[1], lData[0], pParam.cDepth );
    Subdivide3( pParam, lData[3], lData[2], lData[0], pParam.cDepth );
    Subdivide3( pParam, lData[1], lData[2], lData[3], pParam.cDepth );		
    return pParam;
  }

  //------------------------------------------------
  SubDiv::SubParam& SubDiv::Octahedron2( SubDiv::SubParam& pParam )
  {
    Float3 lData[6];

    
    lData[0] = Float3(  T,  0,  0 );
    lData[1] = Float3( -T,  0,  0 );
    lData[2] = Float3(  0,  T,  0 );
    lData[3] = Float3(  0, -T,  0 );
    lData[4] = Float3(  0,  0,  T );
    lData[5] = Float3(  0,  0, -T );

    pParam.normEffectInit( lData, 6 );

    Subdivide3( pParam, lData[0], lData[4], lData[2], pParam.cDepth );
    Subdivide3( pParam, lData[2], lData[4], lData[1], pParam.cDepth );
    Subdivide3( pParam, lData[1], lData[4], lData[3], pParam.cDepth );
    Subdivide3( pParam, lData[3], lData[4], lData[0], pParam.cDepth );
    Subdivide3( pParam, lData[0], lData[2], lData[5], pParam.cDepth );
    Subdivide3( pParam, lData[2], lData[1], lData[5], pParam.cDepth );
    Subdivide3( pParam, lData[1], lData[3], lData[5], pParam.cDepth );
    Subdivide3( pParam, lData[3], lData[0], lData[5], pParam.cDepth );
				
    return pParam;
  }
  //------------------------------------------------
	
	 
  //------------------------------------------------
  SubDiv::SubParam& SubDiv::Dodecahedron( SubDiv::SubParam& pParam  )
  {
    Float3 	lDodec[20];

    double alpha =(sqrt(2.0 / (3.0 + sqrt(5.0))));
    double beta =(1.0 + sqrt(6.0 / (3.0 + sqrt(5.0)) -
			     2.0 + 2.0 * sqrt(2.0 / (3.0 + sqrt(5.0)))));

              
    lDodec[0] = Float3( -alpha, 0, beta);
    lDodec[1] = Float3(  alpha, 0,  beta);
    lDodec[2] = Float3( -1,  -1, -1);
    lDodec[3] = Float3( -1,  -1,  1);
    lDodec[4] = Float3( -1,  1, -1);
    lDodec[5] = Float3( -1,  1,  1);
    lDodec[6] = Float3( 1,  -1,  -1);
    lDodec[7] = Float3( 1, -1,  1);
    lDodec[8] = Float3( 1,  1,  -1);
    lDodec[9] = Float3( 1,  1, 1);
    lDodec[10] = Float3( beta,  alpha,  0);
    lDodec[11] = Float3( beta,  -alpha, 0);
    lDodec[12] = Float3( -beta, alpha,  0);
    lDodec[13] = Float3( -beta,  -alpha,  0);
    lDodec[14] = Float3( -alpha,  0,  -beta);
    lDodec[15] = Float3( alpha, 0,  -beta);
    lDodec[16] = Float3( 0,  beta,  alpha);
    lDodec[17] = Float3( 0,  beta,  -alpha);
    lDodec[18] = Float3( 0,  -beta,  alpha);
    lDodec[19] = Float3( 0,  -beta,  -alpha);

    /*

      int  lDodecaIdx[][] = {
      {0, 1, 9, 16, 5},
      {1, 0, 3, 18, 7},
      {1, 7, 11, 10, 9},
      {11, 7, 18, 19, 6},
      {8, 17, 16, 9, 10},
      {2, 14, 15, 6, 19},
      {2, 13, 12, 4, 14},
      {2, 19, 18, 3, 13},
      {3, 0, 5, 12, 13},
      {6, 15, 8, 10, 11},
      {4, 17, 8, 15, 14},
      {4, 12, 5, 16, 17}
    */

    Subdivide5( pParam, lDodec[0], lDodec[ 1], lDodec[ 9], lDodec[ 16], lDodec[ 5], pParam.cDepth);
    Subdivide5( pParam, lDodec[1], lDodec[ 0], lDodec[ 3], lDodec[ 18], lDodec[ 7], pParam.cDepth);
    Subdivide5( pParam, lDodec[1], lDodec[ 7], lDodec[ 11], lDodec[ 10], lDodec[ 9], pParam.cDepth);
    Subdivide5( pParam, lDodec[11], lDodec[ 7], lDodec[ 18], lDodec[ 19], lDodec[ 6], pParam.cDepth);
    Subdivide5( pParam, lDodec[8], lDodec[ 17], lDodec[ 16], lDodec[ 9], lDodec[ 10], pParam.cDepth);
    Subdivide5( pParam, lDodec[2], lDodec[ 14], lDodec[ 15], lDodec[ 6], lDodec[ 19], pParam.cDepth);
    Subdivide5( pParam, lDodec[2], lDodec[ 13], lDodec[ 12], lDodec[ 4], lDodec[ 14], pParam.cDepth);
    Subdivide5( pParam, lDodec[2], lDodec[ 19], lDodec[ 18], lDodec[ 3], lDodec[ 13], pParam.cDepth);
    Subdivide5( pParam, lDodec[3], lDodec[ 0], lDodec[ 5], lDodec[ 12], lDodec[ 13], pParam.cDepth);
    Subdivide5( pParam, lDodec[6], lDodec[ 15], lDodec[ 8], lDodec[ 10], lDodec[ 11], pParam.cDepth);
    Subdivide5( pParam, lDodec[4], lDodec[ 17], lDodec[ 8], lDodec[ 15], lDodec[ 14], pParam.cDepth);
    Subdivide5( pParam, lDodec[4], lDodec[ 12], lDodec[ 5], lDodec[ 16], lDodec[ 17], pParam.cDepth);

    return pParam;													
  }
  //------------------------------------------------
  void SubDiv::Triangle( SubDiv::SubParam& pParam, int i, Float3 data[], int ndx[][3])
  {
    //		System.out.println( "  DrawTriangle "  + i);

    Float3 x0 = data[ ndx[i][0] ];
    Float3 x1 = data[ ndx[i][1] ];
    Float3 x2 = data[ ndx[i][2] ];

    SubDiv::Subdivide3( pParam, x0, x1, x2, pParam.cDepth );	
  }
  //------------------------------------------------
  void SubDiv::Triangle( SubDiv::SubParam& pParam,  Float3 x0, Float3 x1,  Float3 x2 )
  {
    SubDiv::Subdivide3( pParam, x0, x1, x2, pParam.cDepth );	
  }
  //------------------------------------------------
  /* octahedron data: The octahedron produced is centered at the
     origin and has radius 1.0 */

  static Float3 sOcta[] = {	
    Float3( 1.0, 0.0, 0.0),
    Float3(-1.0, 0.0, 0.0),
    Float3(0.0, 1.0, 0.0),
    Float3(0.0, -1.0, 0.0),
    Float3(0.0, 0.0, 1.0),
    Float3(0.0, 0.0, -1.0)				
  };
	
  static int  sOctaIdx[][3] = {
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
				
    pParam.normEffectInit( sOcta, 6 );
		
    for ( int i = 7; i >= 0; i--) {
      Triangle( pParam, i, sOcta, sOctaIdx  );
    }

    return pParam;
  }
  //------------------------------------------------	
  /* icosahedron data: These numbers are rigged to make an
     icosahedron of radius 1.0 */
		
  static  float sIicosaX =0.525731112119133606f;
  static  float sIicosaZ =0.850650808352039932f;
		
  static Float3 sIicosa[] = {
    Float3(-sIicosaX, 0, sIicosaZ),
    Float3(sIicosaX, 0, sIicosaZ),
    Float3(-sIicosaX, 0, -sIicosaZ),
    Float3(sIicosaX, 0, -sIicosaZ),
    Float3(0, sIicosaZ, sIicosaX),
    Float3(0, sIicosaZ, -sIicosaX),
    Float3(0, -sIicosaZ, sIicosaX),
    Float3(0, -sIicosaZ, -sIicosaX),
    Float3(sIicosaZ, sIicosaX, 0),
    Float3(-sIicosaZ, sIicosaX, 0),
    Float3(sIicosaZ, -sIicosaX, 0),
    Float3(-sIicosaZ, -sIicosaX, 0)
  };
		
  static int sIicosaIdx[][3]  =		{
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
    {7, 2, 11}
  };
  //-----------------------
  SubDiv::SubParam& 	SubDiv::Icosahedron( SubDiv::SubParam& pParam )
  {				
    pParam.normEffectInit( sIicosa, 12 );

    for ( int i = 19; i >= 0; i-- )
      {
	Triangle( pParam, i, sIicosa, sIicosaIdx );
      }
    return pParam;
  }
  //------------------------------------------------	
  // tetrahedron data: 

  static float  sTetraT =  1.73205080756887729f;
		
  Float3 sTetra[] =		{
    Float3( sTetraT, sTetraT, sTetraT),
    Float3( sTetraT, -sTetraT, -sTetraT),
    Float3(-sTetraT, sTetraT, -sTetraT),
    Float3(-sTetraT, -sTetraT, sTetraT)
  };
		
  int sTetraIdx[][3] = {
    {0, 1, 3},
    {2, 1, 0},
    {3, 2, 0},
    {1, 2, 3}
  };
  //-----------------------
  SubDiv::SubParam&  SubDiv::Tetrahedron( SubDiv::SubParam& pParam )
  {
    pParam.normEffectInit( sTetra, 4 );

    for ( int i = 3; i >= 0; i--)
      Triangle( pParam, i, sTetra, sTetraIdx);

    return pParam;
  }
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  Poly* SubDiv::SubParam::finish( float iScale )
  {

    std::cout << " ====================== SubDiv::finish " << cPoints.size() << " " <<  cFacets.size() << std::endl;

		
    Poly* lPoly = new Poly();           //mettre unique_ptr

    // on creer les points 
    std::vector<Point*> lPointsPtr( cPoints.size() );		
    for( size_t p =0; p< cPoints.size(); p++)
      {
	lPointsPtr[p]= new Point( cPoints[p] ); // points sans id
      }
				
			
    for( size_t f=0; f< cFacets.size() ; f++ )              // pour toutes les PrimFacet
      {
	PrimFacet& lPrimFacet  = *cFacets[f];
				
	Facet* lCurFacet = new Facet();                // nouvelle facette vide sans id
	uint i=0;
	for( ; i< lPrimFacet.size()-1; i++ ) // pour les points de la facette courante
	  {					 
	    Line* lLine = new Line( lPointsPtr[ lPrimFacet[i]] , lPointsPtr[ lPrimFacet[i+1]] );
	    lCurFacet->addLine( lLine );
	  }
	Line* lLine = new Line( lPointsPtr[ lPrimFacet[i]] , lPointsPtr[ lPrimFacet[0]] );
	lCurFacet->addLine( lLine );
			
	lPoly->addFacet( lCurFacet );
      }
		
    float lSz = iScale/cFact;
    Point3d lScale( lSz, lSz, lSz );

    
    //phipo 20190325 );
    lPoly->scale( lScale );
		
    return lPoly;
  }

  //**************************************

} // end namespace
