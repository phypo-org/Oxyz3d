#include "Modif.h"

#include <vector>
#include "Utils/PPContainer.h"

using namespace PP3d;
using namespace std;

//--------------------------------------------------
void AlignFacetToNorm( FacetPtr ioFac, Point3d & ioMove, PointPtrSet & ioPoints )
{  
  // recalcul de la normal
  ioFac->computeNormal();
  PP3d::Vector3d lNormToMove = ioFac->getNormal();
  
  ioMove.normalize() ; // it's the direction we want to go
  
  PP3d::Mat4 lMatAlign;
  lMatAlign.rotateAlign( ioMove, lNormToMove  ); // for align the two vector
  
  PP3d::Point3d lCenterFac = ioFac->getCenter3d(); // for put at 0,0,0
  
          
  for( PP3d::PointPtr lPoint : ioPoints )
    { 
      lPoint->get() -= lCenterFac;    // put at 0,0,0 
      lPoint->get() *= lMatAlign;      // rotation for alignment
      lPoint->get() += lCenterFac;    // move to it's orignal position              
    }
}
//--------------------------------------------------

bool Modif::LoftingFacFromPath(  DataBase * iBase, FacetPtr iFacOrigin, FacetPtr lPath,
                                 std::vector<FacetPtr> & oNewFacets,  ParamLoftingFacPath & iParam)
{
  
  PPu::HashSetPtr<Facet> lFacToDel; //Facet that will be destroy at the end
  
    
  std::cout <<  std::endl << "************ LoftingFacFromPath ************"
            << oNewFacets.size()<< std::endl;
  

  FacetPtr lFacOld = iFacOrigin->duplicate();
  lFacOld->inverseLines();
  lFacToDel.insertObj(lFacOld);
  
 
  Point3d  lPtOld  = lFacOld->getCenter3d();

  
  // Get all the point of the facet
  PP3d::SortEntityVisitorPointFacet lVisit;
  lPath->execVisitor( lVisit );         
  GLuint lNbPt =  (GLuint)lVisit.cVectPoints.size();


  
  //====== Move the facet to the begining of path
  if( iParam.cLocalisation == LoftingFacPathLocalisation::BeginOfPath )
  {
    Point3d lMove = lPath->getPoint(0)->get() - lPtOld; // for put the facet in the right place
    PointPtrSet lPoints;
    VisitorGetPoints lVisit(lPoints);
    lFacOld->execVisitor( lVisit ); // get all the points without duplicate for move
    
    for(  const PP3d::PointPtr lPoint : lPoints )
      {
        lPoint->get() += lMove ;  // move point
      }
  }
  //======= end move
  

  
  lPtOld  = lPath->getPoint(0)->get();

  //==============================================
  for( GLuint i = 1; i<lNbPt; i++)  // Begining at index 1
    {           
      FacetPtr lFac = lFacOld->duplicate();   // duplicate the old facet and store them
     lFacToDel.insertObj(lFac);

      
      Point3d lMove = lPath->getPoint(i)->get() - lPtOld; // For the next position of the facet
     
      
      PointPtrSet lPoints;
      VisitorGetPoints lVisit(lPoints);
      lFac->execVisitor( lVisit ); // get all the points without duplicate for move 
     
      for(  PP3d::PointPtr lPoint : lPoints )
        {
          lPoint->get() += lMove ;  // move point
        }

      //====== Align the facet to the normal of current path ======
      if( iParam.cFlagAlign )
        {
          if( i == 100000000 ) // first time  ???? bug ???? 
            {
              PointPtrSet lOldPoints;
              VisitorGetPoints lOldVisit(lOldPoints);
              lFac->execVisitor( lOldVisit ); // get all the points without duplicate for move 
              
              AlignFacetToNorm( lFacOld, lMove, lOldPoints );
            }

          AlignFacetToNorm( lFac, lMove, lPoints );        
        }
      //====== end align ======

      

      //======== Join the facet ==========
      if(iParam.cFlagJoin )
        {
          std::cout << "========== JoinTwoFacets" <<std::endl;
          JoinTwoFacets( iBase, lFacOld, lFac, oNewFacets, true, false, false );
          
          if( iParam.cFlagClose )
            {
              if( i == 1 )
                {
                  oNewFacets.push_back( lFacOld );
                  lFacToDel.removeObj( lFacOld );                  
                }
              if( i== lNbPt-1 )
                {
                  oNewFacets.push_back( lFac );  
                  lFacToDel.removeObj( lFac);                  
                }
            }            
        }
      else
        {
          if( i == 1 )
            {
              oNewFacets.push_back( lFacOld );  
              lFacToDel.removeObj( lFacOld );                  
            }
          
          oNewFacets.push_back( lFac );           
          lFacToDel.removeObj( lFac);                  
        }
      //======== end join 
      

      
      lFacOld = lFac;
      lPtOld  = lPath->getPoint(i)->get();
    }
  //================== end of for ===============



  
  for( PIndex i=0; i< oNewFacets.size()- ((int)iParam.cFlagClose); i++)
    {
      oNewFacets[i]->inverseLines();  // Pas terrible !!!! 
    }

  
  
  lFacToDel.callDeleteForAll(); // empty the trash
  
  return oNewFacets.size() > 0 ;
}
//--------------------------------------------------
