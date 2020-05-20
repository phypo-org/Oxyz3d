#include "PrimitivFactory.h"

#include "PP3dType.h"
#include "Entity.h"


#include <memory>
#include <cmath>


namespace PP3d{
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
    
    long double lVal  = 2*M_PI/iParam->cNbU;
        
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
  void AddPoint( ObjectPolylines & lLines, Point3d & pPt )
  {
    //		std::cout << "======== DataBase::addPointToCurrentLine" << std::endl;
    // FAIRE QUELQUE CHODE DE PLUS PROPRE !!!
		
    Facet* lFacet= iLines.getFacet();
		
    PointPtr lPoint= new Point( pPt );
		
    LinePtrVect& lLines = lFacet->getLines();
				
    //		std::cout << " lLines size: "  << lLines.size()  <<std::endl;	
    if( lLines.size() == 0 )
      {
	//				std::cout << " new line 0 "  << std::endl;	
	LinePtr lLine = new Line( lPoint, lPoint ); // Un point 
	lLines.push_back( lLine );
	//		std::cout << "2 lLines size: "  << lLines.size()  <<std::endl;
      }
    else if(lLines.size() == 1 && lLines[0]->isPoint() )  // Un  point !
      {
	//	std::cout << " is Point  "  << std::endl;	
	lLines[0]->getPoints().second = lPoint;  // on change le second point
      }
    else
      {
	//	std::cout << " new line   "  << std::endl;	
	LinePtr	lLine = new Line(  lLines[lLines.size()-1]->getPoints().second, lPoint);
	lLines.push_back( lLine );
      }		
  }
  //************************
  void Delete( ObjectPolylines & lLines )
  {
  }
  //************************

  Poly*  PrimitivFactory::CreateCylinder(  PrimitivParam * iParam )
  {
    if( iParam == nullptr
	|| iParam->cNbU < 3 || iParam->cNbU > 4096
	|| iParam->cTop < sMinSz
	|| iParam->cBottom < sMinSz
	|| iParam->cHeight < sMinSz
	|| iParam->cTickness < sMinSz
	)
      {
	std::cout << "PrimitivFactory::CreateCylinder - Bad parameter" << std::endl;
	return nullptr;
      }
    
    std::cout << "PrimitivFactory::CreateCylinder " << std::endl;

    Point3d lTop   ( iParam->cTop,  iParam->cHeight/2, 0);
    Point3d lBottom( iParam->cTop, -iParam->cHeight/2, 0);

    
    ObjectPolylines   cPolyLines;
    AddPoint( cCurrentLine, lTop );
    AddPoint( cCurrentLine, lBottom );
    PP3d::Mat4 lMatIdent;
    Poly* lPoly = Maker::CreatePoly4FromFacet( cPolyLines.getFacets(), iParam->cNbU, lMat, true, false, false, true, true  );
    DeletePoly();
    return lPoly;
  }    
	
  //************************

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

  const char* PrimitivFactory::GetTypeName(PrimitivFactory::Type pType)
  {

    switch( pType )
      {
      case PrimitivFactory::Type::FACET_N : return "Facet";
      case PrimitivFactory::Type::CYLINDER: return "Cylinder";
      case PrimitivFactory::Type::CONE    : return "Cone";
      case PrimitivFactory::Type::SPHERE  : return "Sphere";
      case PrimitivFactory::Type::PLANE  : return "Sphere";
      case PrimitivFactory::Type::CUBE    : return "Cube";
      case PrimitivFactory::Type::TETRA   : return "Tetra";
      case PrimitivFactory::Type::PYRAMID : return "Pyramid";			 
      case PrimitivFactory::Type::OCTO    : return "Octoedre";			
      case PrimitivFactory::Type::DODEC   : return "Dodecaedre";
      case PrimitivFactory::Type::ICOSAHED: return "Icosedre";
      }
    return "unknown";
  }

  //************************	
  Poly* PrimitivFactory::Create( PrimitivFactory::Type pType, PrimitivParam * iParam )
  {
    Poly *lPoly= nullptr ;
    
    std::cout << "PrimitivFactory::Create" <<  GetTypeName(pType) << std::endl;

    switch( pType )
      {
      case PrimitivFactory::Type::FACET_N : lPoly = CreateFacet( iParam ); break;
      case PrimitivFactory::Type::CYLINDER: lPoly = CreateCylinder( iParam ); break;
      case PrimitivFactory::Type::CONE    :
      case PrimitivFactory::Type::SPHERE  :
      case PrimitivFactory::Type::PLANE   :  break;
      case PrimitivFactory::Type::CUBE    : lPoly = CreatePoly( sCubePt,  8, sCubeIdx,  6 );	 break;
      case PrimitivFactory::Type::TETRA   : lPoly = CreatePoly( sTetraPt, 4, sTetraIdx, 4 );	 break;	
      case PrimitivFactory::Type::PYRAMID : lPoly = CreatePoly( sPyraPt,  5, sPyraIdx,  5 );	 break;	
      case PrimitivFactory::Type::OCTO    : lPoly = CreatePoly( sOctoPt,  6, sOctoIdx,  8 );	 break;
      case PrimitivFactory::Type::DODEC   : lPoly = CreatePoly( sDodePt,  20, sDodeIdx, 12 );	 break;
      case PrimitivFactory::Type::ICOSAHED: lPoly = CreatePoly( sIcoPt,   12, sIcoIdx,  20 );	 break;

	//	case PrimitivFactory::Type::TETRA : lPoly = CreateTetra( );	 break;	
      default : return nullptr;
      }

    return lPoly;
  }
  //--------------------------------------------------


  //************************


}
