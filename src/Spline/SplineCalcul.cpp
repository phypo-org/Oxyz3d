#include "SplineCalcul.h" 

namespace PP3d {
  //******************************

  int SplineCalcul::Maille   = 8;
  int SplineCalcul::UMmaille = 1;
  int SplineCalcul::VMaille  = 1;

  //--------------------------------------
  void
  SplineCalcul::InitAll()
  {
    InitYamaguchi( Maille,  Yam1, Yam2 );
    InitYamaguchi( UMaille, UYam1, UYam2);
    InitYamaguchi( VMaille ,VYam1, VYam2);
  }
  //--------------------------------------
  void
  SplineCalcul::InitYamaguchi (int iMaille, long ya1[], long ya2[])

  {
    int i;
    long int m, m3;

    M  = maille;
    M3 = M*M*M;
 
    for ( i=0; i< iMaille+1; i++ )
      {
        long I2 = i*i;
        ya1[i] = I2*i;
        ya2[i] =(3 * ( ( I2 * (-2) * M ) + ya1[i] ) + 4* M3);
      }
  }
  //--------------------------------------

  void 
  SplineCalcul::Thomas ( const std::vector<PDouble> & iNodes,  std::vector<PDouble> & oPoles )
  {
    static const long sHi[ 10 ] = {1l, 5l, 19l, 71l, 265l, 989l, 3691l, 13775l, 51409l, 191861l};
  
  
    size_t lNb = iNodes.size();
    PDouble *Xi = new PDouble[ lNb ];   
 
    size_t i;
    Xi[ 0 ] = (iNodes[ 0 ]*6) / 5;
  
    if ( lNb <= 9 )              /* P0, ..., PlNb-1 */
      {
        for (  i=1; i<lNb-1 ; i++ )
          Xi[ i ] = ((6 *iNodes[ i ]-Xi[ i - 1 ])*sHi[ i ]) / sHi[ i + 1 ];
      
        Xi[lNb-1] = ((6 * iNodes[lNb-1] - Xi[lNb-2]) * sHi[lNb-1]) / (sHi[lNb-1] + sHi[lNb]) ;
      }
    else
      {
      
        for (  i=1; i<9; i++ )
          Xi[ i ] = ((6 *iNodes[ i ]-Xi[ i - 1 ])*sHi[ i ]) / sHi[ i + 1 ];
      
        for (  i=9; i<lNb-1; i++ )
          Xi[ i ] = ((6 *iNodes[ i ]-Xi[ i - 1 ])*51409l) / 191861l;
      
        Xi[lNb-1] = ((6 * iNodes[lNb-1] - Xi[lNb-2]) * 51409l) /243270l ;
      }
  
  
    oPoles[ lNb - 1 ] = Xi[ lNb - 1 ];
  
    if ( lNb > 9 )
      {
        for (  i=lNb-1 ; i>7 ; i-- )
          oPoles[ i - 1 ] = Xi[ i - 1 ]-((oPoles[ i ]*51409l) / 191861l);
      
        for (  i=9 ; i>0 ; i-- )
          oPoles[ i - 1 ] = Xi[ i - 1 ]-((oPoles[ i ]*sHi[ i - 1 ]) / sHi[ i ]);
      
      }
    else
      for (  i=lNb-1 ; i>0 ; i-- )
        oPoles[i-1] = Xi[i-1] - ((oPoles[i] * sHi[i-1]) / sHi[i]);
  
    /*	oPoles[0] = oPoles[1] ;  */
  
    delete Xi;
  }
  //--------------------------------------

  void CalculBSpline(int iMaille, VectDouble3 & iPts, VectDouble3 oResult )
  {
    size_t lNb = iPts.size();;


    PDouble lDivis = 1/(6 * Yam1[iIMaille]);
  
    const std::vector<PDouble> & CX iPts.X();
    const std::vector<PDouble> & CY iPts.Y();
    const std::vector<PDouble> & CZ iPts.Z();

 

    /* calcul du premier point de la courbe    */
    PDouble x = ( CX[0]  +  4 * CX[1]  +  CX[2] ) / 6 ;
    PDouble y = ( CY[0]  +  4 * CY[1]  +  CY[2] ) / 6 ;
    PDouble z = ( CZ[0]  +  4 * CZ[1]  +  CZ[2] ) / 6 ;
 
    oResult.add( x, y, z );
 
    /* calcul des autres points et trace       */
    for (size_t int k=0; k< lNb-3; k++ )
      {
        PDouble x0 = CX[k];
	PDouble x1 = CX[k+1];
	PDouble x2 = CX[k+2];
	PDouble x3 = CX[k+3];
        
	PDouble y0 = CY[k];
	PDouble y1 = CY[k+1];
	PDouble y2 = CY[k+2];
	PDouble y3 = CY[k+3];
        
	PDouble z0 = CY[k];
	PDouble z1 = CY[k+1];
	PDouble z2 = CY[k+2];
	PDouble z3 = CY[k+3];

	for (size_t int j=1; j<iMaille+1; j++ )
          {
            jm = iMaille-j;
            x = 	((Yam1[jm] * x0 +
                          Yam2[j] * x1 +
                          Yam2[jm] * x2 +
                          Yam1[j] * x3     	 )  *  lDiv );
            y =  	((Yam1[jm] * y0 +
                          Yam2[j] * y1  +
                          Yam2[jm] * y2 +
                          Yam1[j] * y3     	 )  *  lDiv );
            z =  	((Yam1[jm] * z0 +
                          Yam2[j] * z1  +
                          Yam2[jm] * z2 +
                          Yam1[j] * z3     	 )  *  lDiv );
            oResult.add( x, y, z );
          }

      }
  }
  //******************************

}
