#ifndef H__SplineCalcul__H
#define H__SplineCalcul__H

#include "Shape/Double3.h"


namespace PP3d {
  //******************************
  class SplineCalcul {
  public:
    
    static void InitAll();
    static void InitYamaguchi( int iMaille, long ya1[], long ya2[]);
    static void Thomas ( const std::vector<PDouble> & iNodes,  std::vector<PDouble> & oPoles );
    static void CalculBSpline(int iMaille, VectDouble3 & iPts, VectDouble3 oResult )

#define   MAX_YAM_MAILLE 32

    static long Yam1[MAX_YAM_MAILLE+1], Yam2[MAX_YAM_MAILLE+1],  UYam1[MAX_YAM_MAILLE+1], VYam1[MAX_YAM_MAILLE+1], UYam2[MAX_YAM_MAILLE+1], VYam2[MAX_YAM_MAILLE+1];
    
    static int Maille   ;
    static int UMmaille ;
    static int VMaille  ;
  };
  //******************************

}


#endif
