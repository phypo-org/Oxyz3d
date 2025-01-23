#include "Modif.h"

#include <vector>
#include "Utils/PPContainer.h"

using namespace PP3d;
using namespace std;

//--------------------------------------------------
void AlignFacetToNorm( bool iFlagAlign, FacetPtr ioFac, Point3d  iDir, Point3d  iMove, PointPtrSet & ioPoints,
                       double iGrow, double iSpin)
{
  PP3d::Mat4 lMatMov;
  lMatMov.initMove( iMove);
  
  // recalcul de la normal
  ioFac->computeNormal();
  PP3d::Vector3d lNormToMove = ioFac->getNormal();  
  iDir.normalize() ; // it's the direction we want to go  
  PP3d::Mat4 lMatAlign;
  lMatAlign.rotateAlign( iDir, lNormToMove  ); // for align the two vector
  
  PP3d::Point3d lCenter = ioFac->getCenter3d(); // for put at 0,0,0

  PP3d::Mat4 lMatRecenter;
  lMatRecenter.initMove( lCenter );

  PP3d::Point3d lNCenter = -lCenter;					
  PP3d::Mat4 lMatZero;
  lMatZero.initMove( lNCenter ); //on revient a la postion originale en zero;

  
  PP3d::Mat4 lMatGrow;
  lMatGrow.initScale( iGrow );

  PP3d::Mat4 lMatSpin;
  lMatSpin.identity();
   
  PP3d::Mat4 lMatTran;
  if( iFlagAlign )
    {
       lMatSpin.initRotAxis( iDir, iSpin );
       lMatTran = lMatRecenter * lMatMov * lMatSpin * lMatAlign  * lMatGrow * lMatZero;
    }
  else
    {
      lMatSpin.initRotAxis( lNormToMove, iSpin );
      lMatTran = lMatRecenter * lMatMov * lMatSpin  * lMatGrow * lMatZero;
    }

                
  for( PP3d::PointPtr lPoint : ioPoints )
    { 
      //      lPoint->get() -= lCenter;    // put at 0,0,0 
      lPoint->get() *= lMatTran;      // rotation for alignment

      
      //      lPoint->get() += lCenter;    // move to it's orignal position              
    }
}
//--------------------------------------------------

bool Modif::LoftingFacFromPath(  DataBase * iBase, FacetPtr iFacOrigin, FacetPtr lPath,
                                 std::vector<FacetPtr> & oNewFacets,  ParamLoftingFacPath & iParam)
{
  
  PPu::HashSetPtr<Facet> lFacToDel; //Facet that will be destroy at the end
  
    
  std::cout <<  std::endl << "************ LoftingFacFromPath ************"
            << oNewFacets.size()<< std::endl;
  

  FacetPtr lFacOrg = iFacOrigin->duplicate();
  lFacOrg->inverseLines();
  
 
  Point3d  lPtOrg  ;  //= lFacOrg->getCenter3d();

  
  
  FacetPtr lFacLast = lFacOrg->duplicate();
  lFacToDel.insertObj(lFacLast);
  
   // Get all the point of the Path
  PP3d::SortEntityVisitorPointFacet lVisit;
  lPath->execVisitor( lVisit );         
  GLuint lNbPtPath =  (GLuint)lVisit.cVectPoints.size();

   
  
  //====== Move the facet to the begining of path
  if( iParam.cLocalisation == LoftingFacPathLocalisation::BeginOfPath )
  {
    lPtOrg =  lFacOrg->getCenter3d();
  }
  else
    {
      lPtOrg = lPath->getPoint(0)->get();
    }
  
  //======= end move
  
  // lPtOrg  = lPath->getPoint(0)->get();

  double lGrow = iParam.cGrow / lNbPtPath;
  double lSpin = iParam.cSpin;

  //==============================================
  for( GLuint i = 0; i<lNbPtPath; i++)  // Begining at index 1
    {           
      FacetPtr lFac = lFacOrg->duplicate();   // duplicate the old facet and store them     
      Point3d lMove = lPath->getPoint(i)->get() - lPtOrg; // For the next position of the facet
      
      
      PointPtrSet lPoints;
      VisitorGetPoints lVisit(lPoints);
      lFac->execVisitor( lVisit ); // get all the points without duplicate for move 

      if( i== 0 )
        {
          Point3d lRien;
          AlignFacetToNorm( false, lFac, lRien, lMove, lPoints, 1+lGrow*i, lSpin*i );
        }
      else
        {
         
          AlignFacetToNorm( iParam.cFlagAlign, lFac,
                            lPath->getPoint(i-1)->get() - lPath->getPoint(i)->get(),
                            lMove, lPoints, 1+lGrow*i, lSpin*i );
        }
     

      //======== Join the facet ==========
      if(iParam.cFlagJoin )
        {
          lFacToDel.insertObj(lFac); // delete facet

          if( i != 0 )
            JoinTwoFacets( iBase, lFacLast, lFac, oNewFacets, true, false, false );
          
          
          if( iParam.cFlagClose ) // We must keep the fist and last facet
            {
              if( i == 1 )
                {   // TESTER LA LOCALISATION, SI COLLER FUSIONNER !!!! AFAIRE 
                  oNewFacets.push_back( lFacLast );
                  lFacToDel.removeObj( lFacLast );               
                }
              
              if( i== lNbPtPath-1 )
                {
                  oNewFacets.push_back( lFac );
                  lFac->inverseLines();
                  lFacToDel.removeObj( lFac);                  
                }
            }            
        }
      else
        {
          if( i == 1 )
            {
              oNewFacets.push_back( lFacLast );  
              lFacToDel.removeObj( lFacLast );                  
            }
          
          oNewFacets.push_back( lFac ); // keep the facet       
        }
      //======== end join 
      

      
      lFacLast = lFac;
      // lPtOld  = lPath->getPoint(i)->get();
    }
  //================== end of for ===============


  /*
  
  for( PIndex i=0; i< oNewFacets.size()- ((int)iParam.cFlagClose); i++)
    {
      oNewFacets[i]->inverseLines();  // Pas terrible !!!! 
    }
  */
  
  
  lFacToDel.callDeleteForAll(); // empty the trash
  
  return oNewFacets.size() > 0 ;
}
//--------------------------------------------------
