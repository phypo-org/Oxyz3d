#include "PrimitivFactory.h"

#include "PP3dType.h"
#include "Entity.h"


#include <memory>
#include <cmath>

#include "Maker.h"


namespace PP3d{


 long double PrimitivFactory::sMinSz =1E-6;


  
  //********************* TETRAEDRE ************

  static float  sT =  1.73205080756887729f;
		
  static Point3d sTetraPt[] =		{
    Point3d( sT,  sT,  sT),
    Point3d( sT, -sT, -sT),
    Point3d(-sT,  sT, -sT),
    Point3d(-sT, -sT,  sT)
  };
		
  static PrimFacet sTetraIdx[] = {
    /*			
			PrimFacet(  0, 1, 3 ),
			PrimFacet(  2, 1, 0 ),
			PrimFacet(  3, 2, 0 ),
			PrimFacet(  1, 2, 3 )
    */

    PrimFacet(  0, 3, 1 ),
    PrimFacet(  2, 0, 1 ),
    PrimFacet(  3, 0, 2 ),
    PrimFacet(  1, 3, 2 )
  };

  //************************ CUBE ***************

  const int pSzX = 2;
  const int pSzY = 2;
  const int pSzZ = 2;

  static Point3d sCubePt[] = {
    Point3d( pSzX*0.5f, -pSzY*0.5f, -pSzZ*0.5f), 
    Point3d( pSzX*0.5f,  pSzY*0.5f, -pSzZ*0.5f),    
    Point3d( -pSzX*0.5f,  pSzY*0.5f, -pSzZ*0.5f),
    Point3d( -pSzX*0.5f,  -pSzY*0.5f, -pSzZ*0.5f),

    Point3d( pSzX*0.5f, -pSzY*0.5f, pSzZ*0.5f),
    Point3d( pSzX*0.5f,  pSzY*0.5f, pSzZ*0.5f),
    Point3d( -pSzX*0.5f,  pSzY*0.5f, pSzZ*0.5f ),
    Point3d( -pSzX*0.5f,  -pSzY*0.5f, pSzZ*0.5f)
  };

  const int lA =0;
  const int lB =1;
  const int lC =2;
  const int lD =3;
  const int lE =4;
  const int lF =5;
  const int lG =6;
  const int lH =7;

  static PrimFacet sCubeIdx[] = {
    PrimFacet( lA, lD, lC, lB ),			
    PrimFacet( lE, lF, lG, lH ),					
    PrimFacet( lA, lB, lF, lE ),						
    PrimFacet( lD, lH, lG, lC ),						
    PrimFacet( lB, lC, lG, lF ),
    PrimFacet( lA, lE, lH, lD )
    /*
      PrimFacet( lA, lB, lC, lD ),			
      PrimFacet( lE, lH, lG, lF ),					
      PrimFacet( lA, lE, lF, lB ),						
      PrimFacet( lD, lC, lG, lH ),						
      PrimFacet( lB, lF, lG, lC ),
      PrimFacet( lA, lD, lH, lE )
    */
  };


  //************************ PYRAMIDE **************

  const float sPyraX=0;
  const float sPyraY=0;
  const float sPyraZ=0;
  const float sPyraH=1;
  const float sPyraW=1;

  static Point3d sPyraPt[] = {
    Point3d( sPyraX,                sPyraY + sPyraH, sPyraZ      ),  // lA Top
    Point3d( sPyraX - sPyraW,   sPyraY - sPyraH, sPyraZ + sPyraW ),  // lB Front left point
    Point3d( sPyraX + sPyraW,   sPyraY - sPyraH, sPyraZ + sPyraW  ), // lC Front right point
    Point3d( sPyraX + sPyraW,   sPyraY - sPyraH, sPyraZ - sPyraW  ), // lD Back right point
    Point3d( sPyraX - sPyraW,   sPyraY - sPyraH, sPyraZ - sPyraW  )  // lE Back left point
  };

	
  static PrimFacet sPyraIdx[] = {
    /*			PrimFacet( lA, lC, lB ),
			PrimFacet( lA, lB, lE ),
			PrimFacet( lA, lE, lD ),
			PrimFacet( lA, lD, lC ),
			PrimFacet( lC, lB, lE, lD)
    */
    //	PrimFacet( lA, lB, lC ),
    //	PrimFacet( lA, lE, lB ),
    //		PrimFacet( lA, lD, lE ),
    //		PrimFacet( lA, lC, lD ),
    //		PrimFacet( lC, lD, lE, lB)
    //	PrimFacet( lC, lB, lE, lD)
		
    //			PrimFacet( lA, lC, lB ),
    /*
      PrimFacet( lA, lC, lB ),
      PrimFacet( lA, lB, lE ),
      PrimFacet( lA, lE, lD ),
      PrimFacet( lA, lD, lC ),
      PrimFacet( lC, lD, lE, lB)
    */
    PrimFacet( lA, lB, lC ),
    PrimFacet( lA, lE, lB ),
    PrimFacet( lA, lD, lE ),
    PrimFacet( lA, lC, lD ),
    PrimFacet( lC, lB, lE, lD)
  };
		
  //************************ OCTOEDRE ******************
		
  static Point3d sOctoPt[] = {
    Point3d(  sT,  0,  0 ),
    Point3d( -sT,  0,  0 ),
    Point3d(  0,  sT,  0 ),
    Point3d(  0, -sT,  0 ),
    Point3d(  0,  0,  sT ),
    Point3d(  0,  0, -sT )
  };

  static PrimFacet sOctoIdx[] = {
    PrimFacet( 0, 2, 4),
    PrimFacet( 2, 1, 4),
    PrimFacet( 1, 3, 4),
    PrimFacet( 3, 0, 4),
    PrimFacet( 0, 5, 2),
    PrimFacet( 2, 5, 1),
    PrimFacet( 1, 5, 3),
    PrimFacet( 3, 5, 0)
    /*
      PrimFacet( 0, 4, 2),
      PrimFacet( 2, 4, 1),
      PrimFacet( 1, 4, 3),
      PrimFacet( 3, 4, 0),
      PrimFacet( 0, 2, 5),
      PrimFacet( 2, 1, 5),
      PrimFacet( 1, 3, 5),
      PrimFacet( 3, 0, 5)
    */
  };

  //************************ DODECAODRON ********************
  namespace Detail
  {
    double constexpr sqrtNewtonRaphson(double x, double curr, double prev)
    {
      return curr == prev
	? curr
	: sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }
  }
  double constexpr sqrtconstexpr(double x)
  {
    return x >= 0 && x < std::numeric_limits<double>::infinity()
			 ? Detail::sqrtNewtonRaphson(x, x, 0)
			 : std::numeric_limits<double>::quiet_NaN();
  }
  constexpr double sAlpha = (sqrtconstexpr(2.0 / (3.0 + sqrtconstexpr(5.0))));
  constexpr double sBeta = (1.0 + sqrtconstexpr(6.0 / (3.0 + sqrtconstexpr(5.0)) -
						2.0 + 2.0 * sqrtconstexpr(2.0 / (3.0 + sqrtconstexpr(5.0)))));

  static Point3d sDodePt[] = {
    Point3d( -sAlpha, 0, sBeta),
    Point3d(  sAlpha, 0,  sBeta),
    Point3d( -1,  -1, -1),
    Point3d( -1,  -1,  1),
    Point3d( -1,  1, -1),
    Point3d( -1,  1,  1),
    Point3d( 1,  -1,  -1),
    Point3d( 1, -1,  1),
    Point3d( 1,  1,  -1),
    Point3d( 1,  1, 1),
    Point3d( sBeta,  sAlpha,  0),
    Point3d( sBeta,  -sAlpha, 0),
    Point3d( -sBeta, sAlpha,  0),
    Point3d( -sBeta,  -sAlpha,  0),
    Point3d( -sAlpha,  0,  -sBeta),
    Point3d( sAlpha, 0,  -sBeta),
    Point3d( 0,  sBeta,  sAlpha),
    Point3d( 0,  sBeta,  -sAlpha),
    Point3d( 0,  -sBeta,  sAlpha),
    Point3d( 0,  -sBeta,  -sAlpha)
  };

  static PrimFacet sDodeIdx[] = {
    PrimFacet(0, 1, 9, 16, 5),
    PrimFacet(1, 0, 3, 18, 7),
    PrimFacet(1, 7, 11, 10, 9),
    PrimFacet(11, 7, 18, 19, 6),
    PrimFacet(8, 17, 16, 9, 10),
    PrimFacet(2, 14, 15, 6, 19),
    PrimFacet(2, 13, 12, 4, 14),
    PrimFacet(2, 19, 18, 3, 13),
    PrimFacet(3, 0, 5, 12, 13),
    PrimFacet(6, 15, 8, 10, 11),
    PrimFacet(4, 17, 8, 15, 14),
    PrimFacet(4, 12, 5, 16, 17)
  };
  //************************ ICOSAHEDRON **********************
	
  const double sIicosaX =0.525731112119133606f;
  const double sIicosaZ =0.850650808352039932f;

  static Point3d sIcoPt[] = {			
    Point3d(-sIicosaX, 0, sIicosaZ),
    Point3d(sIicosaX, 0, sIicosaZ),
    Point3d(-sIicosaX, 0, -sIicosaZ),
    Point3d(sIicosaX, 0, -sIicosaZ),
    Point3d(0, sIicosaZ, sIicosaX),
    Point3d(0, sIicosaZ, -sIicosaX),
    Point3d(0, -sIicosaZ, sIicosaX),
    Point3d(0, -sIicosaZ, -sIicosaX),
    Point3d(sIicosaZ, sIicosaX, 0),
    Point3d(-sIicosaZ, sIicosaX, 0),
    Point3d(sIicosaZ, -sIicosaX, 0),
    Point3d(-sIicosaZ, -sIicosaX, 0)
  };

  static PrimFacet sIcoIdx[] = {
    PrimFacet(	0, 1, 4),
    PrimFacet(0, 4, 9),
    PrimFacet(9, 4, 5),
    PrimFacet(4, 8, 5),
    PrimFacet(4, 1, 8),
    PrimFacet(8, 1, 10),
    PrimFacet(8, 10, 3),
    PrimFacet(5, 8, 3),
    PrimFacet(5, 3, 2),
    PrimFacet(2, 3, 7),
    PrimFacet(7, 3, 10),
    PrimFacet(7, 10, 6),
    PrimFacet(7, 6, 11),
    PrimFacet(11, 6, 0),
    PrimFacet(0, 6, 1),
    PrimFacet(6, 10, 1),
    PrimFacet(9, 11, 0),
    PrimFacet(9, 2, 11),
    PrimFacet(9, 5, 2),
    PrimFacet(7, 11, 2)
    /*
      PrimFacet(	0, 4, 1),
      PrimFacet(0, 9, 4),
      PrimFacet(9, 5, 4),
      PrimFacet(4, 5, 8),
      PrimFacet(4, 8, 1),
      PrimFacet(8, 10, 1),
      PrimFacet(8, 3, 10),
      PrimFacet(5, 3, 8),
      PrimFacet(5, 2, 3),
      PrimFacet(2, 7, 3),
      PrimFacet(7, 10, 3),
      PrimFacet(7, 6, 10),
      PrimFacet(7, 11, 6), 
      PrimFacet(11, 0, 6),
      PrimFacet(0, 1, 6),
      PrimFacet(6, 1, 10),
      PrimFacet(9, 0, 11),
      PrimFacet(9, 11, 2),
      PrimFacet(9, 2, 5),
      PrimFacet(7, 2, 11)
    */
  };
	
  //************************
  Poly*  PrimitivFactory::CreateFacet(  PrimitivParam * iParam )
  {
    if( iParam == nullptr
	|| iParam->cNbU < 3 || iParam->cNbU > 4096 )
      {
	std::cout << "PrimitivFactory::CreateFacet - Bad parameter" << std::endl;
	return nullptr;
      }
    std::cout << "PrimitivFactory::CreateFacet " << std::endl;

    PrimFacet         lFacets[2];
    std::vector<Point3d> lPoints;
    
    long double lVal  = M_PIx2/iParam->cNbU;
        
    for( int i=0; i< iParam->cNbU; i++ )
      {
	long double lAngle = lVal*i ;
	Point3d lPt( cos( lAngle ), 0, sin( lAngle ) );
	lPoints.push_back( lPt );
	
	lFacets[0].push_back( i );
	if( iParam->cCheckInvRot || iParam->cCheckDouble )
	  lFacets[1].push_back( (iParam->cNbU-1) -i );
      }
    if( iParam->cCheckDouble )
      return  PrimitivFactory::CreatePoly( lPoints.data(), lPoints.size(), lFacets, 2 );
    if( iParam->cCheckInvRot )
      return  PrimitivFactory::CreatePoly( lPoints.data(), lPoints.size(), &lFacets[1], 1 );
    
    return  PrimitivFactory::CreatePoly( lPoints.data(), lPoints.size(), &lFacets[0], 1 );
  }    
  
  //************************
  void Delete( ObjectPolylines & lLines )
  {
  }
  //************************

  Poly*  PrimitivFactory::CreateCylinder(  PrimitivParam * iParam, std::string & iName )
  {
    if( iParam == nullptr
	|| iParam->cNbU < 3 || iParam->cNbU > 4096
	|| iParam->cThickness < sMinSz
	|| (iParam->cBottom ==0 && iParam->cHeight ==0)
	|| (iParam->cBottom ==0 && iParam->cCheckHole )
	|| (iParam->cTop ==0 && iParam->cCheckHole )
	)
      {
	std::cerr  << "*** Error : PrimitivFactory::CreateCylinder - Bad parameter : " << iParam <<std::endl;
        
        std::cerr  << "NbU:"<< iParam->cNbU
                   << " Thickness:" <<  iParam->cThickness
                   << " Bottom:" << iParam->cBottom
                   <<  " Height:" << iParam->cHeight
                   << " Top:" << iParam->cTop  <<  std::endl;
	return nullptr;
      }
    
    std::cout << "PrimitivFactory::CreateCylinder " << std::endl;

    Point lTop   ( Point3d( iParam->cTop,    iParam->cHeight/2, 0) );
    Point lBottom( Point3d( iParam->cBottom, -iParam->cHeight/2, 0) );    
    Line  lLineTB( &lTop, &lBottom );
    Facet  lFacet( &lLineTB );
    
    if( iParam->cCheckHole )
      {
	Point lTop2   ( Point3d( iParam->cTop - iParam->cThickness,    iParam->cHeight/2, 0) );
	Point lBottom2( Point3d( iParam->cBottom - iParam->cThickness, -iParam->cHeight/2, 0) );
	Line  lLineBB2( &lBottom, &lBottom2);
	lFacet.addLine( &lLineBB2 );
	  
	Line  lLineB2T2( &lBottom2, &lTop2);
	lFacet.addLine( &lLineB2T2 );
	iName = "Tube";
      }

    

    std::cout << "PrimitivFactory::CreateCylinder Gear:" << iParam->cCheckGear
              << " : " << iParam->cParam1
              << " : "<< iParam->cParam2
              << std::endl;
    
    PP3d::Mat4 lMatRot;
    lMatRot.initRotY( -M_PIx2 / iParam->cNbU );

    
    if( iParam->cBottom == 0 )
      {
	iName = "Cone";
	return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                            CloseRevol::Yes,
                                            CloseSeg::No,
                                            CloseSegEnd::No,
                                            CloseHight::Yes,
                                            CloseLow::No );       	
      }

    if( iParam->cTop == 0 )
      {
	iName = "Cone";
	return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                            CloseRevol::Yes,
                                            CloseSeg::No,
                                            CloseSegEnd::No,
                                            CloseHight::No,
                                            CloseLow::Yes );       	
      }
      
  
    if( iParam->cCheckHole == false )
      {
        if( iParam->cCheckGear == false )
          {
            return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                                CloseRevol::Yes,
                                                CloseSeg::No,
                                                CloseSegEnd::No,
                                                CloseHight::Yes,
                                                CloseLow::Yes);
          }
        else
          {
            PIndex lMod1= (PIndex) iParam->cParam1;      
            PIndex lMod2= (PIndex) iParam->cParam2;      
            Point3d lTmpPt( iParam->cParam3,  0, 0);
    
  
            return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                                CloseRevol::Yes,
                                                CloseSeg::No,
                                                CloseSegEnd::No,
                                                CloseHight::Yes,
                                                CloseLow::Yes,
                                                WithGrid::No,
                                                &lTmpPt, lMod1, lMod2 );
                                                
         }
      }
    
    if( iParam->cCheckGear == false )
      {        
        return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                            CloseRevol::Yes,
                                            CloseSeg::Yes,
                                            CloseSegEnd::No,
                                            CloseHight::No,
                                            CloseLow::No);
      }
    
      PIndex lMod1= (PIndex) iParam->cParam1;      
      PIndex lMod2= (PIndex) iParam->cParam2;      
      Point3d lTmpPt( iParam->cParam3,  0, 0);
    
      return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                          CloseRevol::Yes,
                                          CloseSeg::Yes,
                                          CloseSegEnd::No,
                                          CloseHight::No,
                                          CloseLow::No,
                                          WithGrid::No,
                                          &lTmpPt,lMod1, lMod2 );
  
       
  }
  //************************
  Poly*  PrimitivFactory::CreatePlane(  PrimitivParam * iParam, std::string & iName )
  {
    if( iParam == nullptr
	|| iParam->cNbU < 1 || iParam->cNbU > 4096
	|| iParam->cNbV < 1 || iParam->cNbV > 4096
 	)
      {
	std::cerr << "*** Error : PrimitivFactory::CreateCylinder - Bad parameter" << std::endl;
	return nullptr;
      }

    bool lHole =  iParam->cCheckHole;
    

    std::cout << "PrimitivFactory::CreatePlane" << std::endl;

    int lNbU = iParam->cNbU;
    int lNbV = iParam->cNbV;
    double lDU = iParam->cLength / lNbU;
    double lDV = iParam->cWidth  / lNbV;

    PrimFacet            lPrimFacet;
    std::vector<Point3d> lPoints;
  
    
    double lA = -iParam->cLength/2;
    int u = 0;
    for( ; u< lNbU+1; u++ )
      {
       Point3d lPt( lA, iParam->cHeight/2,  iParam->cWidth/2 );
       lPoints.push_back( lPt );
       lPrimFacet.push_back( u );

       lA += lDU;
      }
    
    lA =  + iParam->cLength/2;
    if( iParam->cHeight != 0 )
      {
        for(; u< (lNbU+1)*2; u++ )
          {
            Point3d lPt( lA, -iParam->cHeight/2,  iParam->cWidth/2 );
            lPoints.push_back( lPt );
            lPrimFacet.push_back( u );      
            lA -= lDU;
          }
      }
    
        
    Facet  lFacet;    
    SetFacet( lFacet, lPoints, lPrimFacet, false );

             
    PP3d::Mat4 lMat;
    lMat.initMove( 0, 0, -lDV );

    if( iParam->cHeight == 0 )
         {
           return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbV+1, lMat,
                                               CloseRevol::No,
                                               CloseSeg::No,
                                               CloseSegEnd::No,
                                               CloseHight::No,
                                               CloseLow::No );
         }

    return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbV+1, lMat,
                                        CloseRevol::No,
                                        CloseSeg::Yes,
                                        CloseSegEnd::Yes,
                                        CloseHight::No,
                                        CloseLow::No,
                                        WithGrid::Yes ); //pFlagCloseSeg, pFlagCloseSegEnd
    // phipo 20231219 Old false, true, true, true, false ); //pFlagCloseSeg, pFlagCloseSegEnd, pCloseHight

  }
 //************************

  Poly*  PrimitivFactory::CreateSphere(  PrimitivParam * iParam, std::string & iName )
  {
    if( iParam == nullptr
	|| iParam->cNbU < 3 || iParam->cNbU > 4096
	|| iParam->cNbV < 3 || iParam->cNbV > 4096
	|| (iParam->cHeight < sMinSz )
	|| (iParam->cWidth  < sMinSz )
	)
      {
	std::cerr << "*** Error : PrimitivFactory::CreateSphere - Bad parameter "
                  << " NbU:" << iParam->cNbU
                  << " NbV:" << iParam->cNbV
                  << " Height:" << iParam->cHeight
                  << " Width:" << iParam->cWidth
                  << std::endl;
	return nullptr;
      }
    
    std::cout << "PrimitivFactory:::CreateSphere " << std::endl;


    // On crée la facette
    Facet  lFacet;
    std::vector<Point> lPoints;
    std::vector<Line>  lLines;
    
    double lDecalTore = 0;
    long double lVal = M_PI/iParam->cNbV; // un demi cercle pour la sphere !;
    
    if( iParam->cCheckHole )
      {
	lVal = M_PIx2 / iParam->cNbV; //  Un cercle pour la tore !
	
	// Il faudra doubler iParam->cNbV pour la tore !
	lDecalTore = iParam->cWidth+iParam->cThickness ;
      }


    for( int i=0 ; i< iParam->cNbV+1; i++ )
      {	
	long double lAngle = M_PI_2 + lVal*i ;
	//	std::cout << "i:" << i <<  "  " << lAngle << std::endl;
	
	lPoints.push_back(  Point(Point3d( iParam->cWidth*cos( lAngle )+lDecalTore, iParam->cHeight*sin( lAngle ), 0 ) )); //copy point
      }
    
    for( size_t i=0 ; i< lPoints.size()-1; i++ )
      {
	lLines.push_back( Line( &lPoints[i], &lPoints[i+1] ));	
      }
	  
    for( Line &lLine : lLines )
      {	
	lFacet.addLine( &lLine );
      }

 
    PP3d::Mat4 lMatRot;
    lMatRot.initRotY( -M_PIx2 / iParam->cNbU );

 
    
    if( iParam->cCheckHole )
      {
	iName = "Tore";
	lFacet.inverseLines();
    
	return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                            CloseRevol::Yes,
                                            CloseSeg::No,
                                            CloseSegEnd::No,
                                            CloseHight::No,
                                            CloseLow::No );       	
       }
  
    return Maker::CreatePoly4FromFacet( &lFacet, iParam->cNbU, lMatRot,
                                        CloseRevol::Yes,
                                        CloseSeg::No,
                                        CloseSegEnd::No,
                                        CloseHight::No,
                                        CloseLow::No );       	
  }
  //************************
  // Code venant plus ou moins de ChatGPT et de Claude (mais ils font un peu n'importe quoi)
  // plus ou moins corrigé par moi 
  Poly * PrimitivFactory::CreateTrapezohedron(  PrimitivParam * iParam, std::string & iName )
  {
    std::vector<Point3d>  lPoints;
    std::vector<PrimFacet> lFacets;

    double height = iParam->cHeight;
    float radius  = iParam->cWidth/2.0f;  // Radius of the bottom pentagon

 
    const int sides = iParam->cNbU; // Pentagonal base

    
    float angleStep = 2.0f * M_PI / sides;    
    float halfStep = angleStep/2.0f;

    float PHI=1.0f;
    if(sides==5)
      {
        PHI = (1.0 + sqrt(5)) / 2.0f;  // Nombre d'or ; sides = 5
     }
    else
      {        
        PHI =  1.0 + 0.5 * cos(2.0f*M_PI/sides);  // Nombre d'or ; sides = 5
      }

    
    double h_pent =( height * (PHI - 1) / PHI) *0.3f;    // Hauteur des pentagones

    // ???

    //  height = radius * sqrt(PHI);           // Hauteur des pôles
    //  radius = radius / sqrt(PHI);      // Rayon des pentagones
    // ????
    

    if( iParam->cLength > 0 )
      h_pent =iParam->cLength;
    
    
    //    float height = radius * sqrt(PHI);     // Hauteur optimale
    // float height = radius * k;
    
    // Generate lPoints for the bottom and top bases
    for (int i = 0; i < sides; ++i)
      {
        float angle = i * angleStep;
        
        // Bottom base
        lPoints.push_back (Point3d( radius * cos(angle), h_pent,
                                    radius * sin(angle))); 
        // Top base
        lPoints.push_back(Point3d( radius * cos(angle + halfStep), -h_pent,
                                   radius * sin(angle + halfStep) ));
      }
    
    int mod = lPoints.size();
    
    // Add the top and bottom poles
    lPoints.push_back(Point3d(0.0f,height, 0.0f));    // Top pole
    int top = lPoints.size()-1; // Index of the bottom pole
    
    lPoints.push_back(Point3d(0.0f, -height,  0.0f));   // Bottom pole
    int bottom = lPoints.size()-1; // Index of the bottom pole
    
    // Generate faces (connect the bases)
    for (int i = 0; i < mod; i+=2)
      {        
        lFacets.push_back(PrimFacet( top,      i, i+1, (i+2)%mod ));
        lFacets.push_back(PrimFacet( bottom, i+1, i, (i+mod-1)%mod ));
      }
    
    return CreatePoly( lPoints, lFacets );
  }	

  //***************************************************************************************
  //***************************************************************************************
  //***************************************************************************************

  Poly * PrimitivFactory::CreatePoly( Point3d * pPoints, size_t pSzPt, PrimFacet * pFacets, size_t pSzFac )
  {

    std::cout << " ====================== PrimitivFactory::CreatePoly " << pSzPt << " " << pSzFac << std::endl;


		
    Poly* lPoly = new Poly();           //mettre unique_ptr


    // on creer les points 
    std::vector<Point*> lPointsPtr( pSzPt );		
    for( size_t p =0; p< pSzPt; p++)
      {
	lPointsPtr[p]= new Point( pPoints[p] ); // points sans id
      }
				
		
    for( size_t f=0; f< pSzFac ; f++ )              // pour toutes les PrimFacet
      {
	PrimFacet& lPrimFacet  = pFacets[f];
				
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
						
	 
    return lPoly;
  }
  //************************

  Poly * PrimitivFactory::CreatePoly( std::vector<Point3d> &iPoints, std::vector<PrimFacet> & iFacets)
  {
    size_t lSzPt  = iPoints.size();
    size_t lSzFac = iFacets.size();
    std::cout << " ====================== PrimitivFactory::CreatePoly " << lSzPt << " " << lSzFac << std::endl;

		
    Poly* lPoly = new Poly();           //mettre unique_ptr


    // on creer les points 
    std::vector<Point*> lPointsPtr( lSzPt );		
    for( size_t p =0; p< lSzPt; p++)
      {
	lPointsPtr[p]= new Point( iPoints[p] ); // points sans id
      }
				
		
    for( size_t f=0; f< lSzFac ; f++ )              // pour toutes les PrimFacet
      {
	PrimFacet& lPrimFacet  = iFacets[f];
				
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
						
	 
    return lPoly;
  }
  //************************

  Facet & PrimitivFactory::SetFacet( Facet & pCurFacet,  std::vector<Point3d> & pPoints,  PrimFacet & pPrimFacet, bool pClose )
  {

    std::cout << " ====================== PrimitivFactory::CreateFacet " << pPoints.size() << std::endl;

    // on creer les points 
    std::vector<Point*> lPointsPtr( pPoints.size() );		
    for( size_t p =0; p< pPoints.size();  p++)
      {
	lPointsPtr[p]= new Point( pPoints[p] ); // points sans id
      }
				
    
    uint i=0;
	
    for( ; i< pPrimFacet.size()-1; i++ ) // pour les points de la facette courante
      {					 
        Line* lLine = new Line( lPointsPtr[ pPrimFacet[i]] , lPointsPtr[ pPrimFacet[i+1]] );
        pCurFacet.addLine( lLine );
      }

    if( pClose )
      {
        Line* lLine = new Line( lPointsPtr[ pPrimFacet[i]] , lPointsPtr[ pPrimFacet[0]] );
        pCurFacet.addLine( lLine );
      }
							 
    return pCurFacet;
  }
  //************************

  Facet * PrimitivFactory::CreateFacet(  std::vector<Point3d> & pPoints, PrimFacet & pPrimFacet, bool pClose )
  {				    
    Facet* lCurFacet = new Facet();                // nouvelle facette vide sans id
    return & PrimitivFactory::SetFacet( *lCurFacet, pPoints,pPrimFacet, pClose);
  }
  

  //************************
#define CASE_STR(A) if( strcmp( #A, iName ) == 0 ) { return  PrimitivFactory::Type::A; } else

  PrimitivFactory::Type  PrimitivFactory::GetTypeFromName(const char* iName)
  {  
    CASE_STR(FACET_N )
    CASE_STR(CYLINDER)
    CASE_STR(SPHERE  ) 
    CASE_STR(PLANE  ) 
    CASE_STR(CUBE    )
    CASE_STR(TETRA   )
    CASE_STR(PYRAMID )		 
    CASE_STR(OCTO    )		
    CASE_STR(DODEC   ) 
    CASE_STR(ICOSAHED)
    CASE_STR(TRAPEZOHEDRON)
    return PrimitivFactory::Type::NONE;

    return PrimitivFactory::Type::NONE;
  }
  //-----------------------------------------
  const char* PrimitivFactory::GetTypeName(PrimitivFactory::Type pType)
  {
    switch( pType )
      {
      case PrimitivFactory::Type::NONE: return "None";
      case PrimitivFactory::Type::FACET_N : return "Facet";
      case PrimitivFactory::Type::CYLINDER: return "Cylinder";
      case PrimitivFactory::Type::SPHERE  : return "Sphere";
      case PrimitivFactory::Type::PLANE  : return  "Plane";
      case PrimitivFactory::Type::CUBE    : return "Cube";
      case PrimitivFactory::Type::TETRA   : return "Tetra";
      case PrimitivFactory::Type::PYRAMID : return "Pyramid";			 
      case PrimitivFactory::Type::OCTO    : return "Octoedre";			
      case PrimitivFactory::Type::DODEC   : return "Dodecaedre";
      case PrimitivFactory::Type::ICOSAHED: return "Icosedre";
      case PrimitivFactory::Type::TRAPEZOHEDRON: return "Trapezoedre pentagonal";
      }
    return "unknown";
  }
 
  //************************	
  Poly* PrimitivFactory::Create( PrimitivFactory::Type pType, std::string & iName, PrimitivParam * iParam )
  {
    Poly *lPoly= nullptr ;
    
    std::cout << "PrimitivFactory::Create" <<  GetTypeName(pType) << std::endl;


    iName = GetTypeName( pType );
    
    switch( pType )
      {
      case PrimitivFactory::Type::FACET_N : lPoly = CreateFacet( iParam ); break;
      case PrimitivFactory::Type::PLANE: lPoly = CreatePlane( iParam, iName ); break;
      case PrimitivFactory::Type::CYLINDER: lPoly = CreateCylinder( iParam, iName ); break;
      case PrimitivFactory::Type::SPHERE  : lPoly = CreateSphere( iParam, iName ); break;
      case PrimitivFactory::Type::CUBE    : lPoly = CreatePoly( sCubePt,  8, sCubeIdx,  6 );	 break;
      case PrimitivFactory::Type::TETRA   : lPoly = CreatePoly( sTetraPt, 4, sTetraIdx, 4 );	 break;	
      case PrimitivFactory::Type::PYRAMID : lPoly = CreatePoly( sPyraPt,  5, sPyraIdx,  5 );	 break;	
      case PrimitivFactory::Type::OCTO    : lPoly = CreatePoly( sOctoPt,  6, sOctoIdx,  8 );	 break;
      case PrimitivFactory::Type::DODEC   : lPoly = CreatePoly( sDodePt,  20, sDodeIdx, 12 );	 break;
      case PrimitivFactory::Type::ICOSAHED: lPoly = CreatePoly( sIcoPt,   12, sIcoIdx,  20 );	 break;
      case PrimitivFactory::Type::TRAPEZOHEDRON: lPoly = CreateTrapezohedron( iParam, iName );	 break;

	//	case PrimitivFactory::Type::TETRA : lPoly = CreateTetra( );	 break;	
      default : return nullptr;
      }

    return lPoly;
  }
  //--------------------------------------------------


  //************************


}
