#include "Modif.h"

#include <vector>

using namespace PP3d;
using namespace std;


//--------------------------------------------------

bool Modif::LoftingFacFromPath(  DataBase * iBase, FacetPtr iFacOrigin, FacetPtr lPath,
                                 std::vector<FacetPtr> & oNewFacets, bool iFlagJoin )
{  
  std::cout << "LoftingFacFromPath " << oNewFacets.size()<< std::endl;

  FacetPtr lFacOld = iFacOrigin;
  Point3d  lPtOld = iFacOrigin->getCenter3d();

  PP3d::SortEntityVisitorPointFacet lVisit;
  lPath->execVisitor( lVisit );         
  GLuint lNbPt =  (GLuint)lVisit.cVectPoints.size();
  
  for( GLuint i = 0; i<lNbPt; i++)
    {           
      FacetPtr lFac = lFacOld->duplicate();  // duplicate the facet
      oNewFacets.push_back( lFac );

      Point3d lMove = lPath->getPoint(i)->get() - lPtOld;
      PointPtrSet lPoints;
      VisitorGetPoints lVisit(lPoints);
      lFac->execVisitor( lVisit ); // get all the points without duplicate
     
      for(  const PP3d::PointPtr lPoint : lPoints )
        {
          lPoint->get() += lMove ;  // move point
        }

      
      // IL FAUT DETUIRE LES FACETTES INTERMEDIAIRES : faire un tableau pour les mettres dedans

      if(iFlagJoin)
        {
          JoinTwoFacets( iBase, lFacOld, lFac, oNewFacets, true, false, false );
        }
            
      lFacOld = lFac;
      lPtOld  = lPath->getPoint(i)->get();

    }

  // iBase->getNewFacet();

  


  return oNewFacets.size() > 0 ;
}
//--------------------------------------------------
