#include "Modif.h"

#include <vector>

using namespace PP3d;
using namespace std;


//--------------------------------------------------

bool Modif::LoftingFacFromPath(  DataBase * iBase, FacetPtr iFacOrigin, FacetPtr lPath,
                                 std::vector<FacetPtr> & oNewFacets, bool iFlagJoin )
{  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;


  std::cout << std::endl;




  std::cout << "************ LoftingFacFromPath ************"
            << oNewFacets.size()<< std::endl;

  FacetPtr lFacOld = iFacOrigin->duplicate();
  Point3d  lPtOld  = lFacOld->getCenter3d();

  
  PP3d::SortEntityVisitorPointFacet lVisit;
  lPath->execVisitor( lVisit );         
  GLuint lNbPt =  (GLuint)lVisit.cVectPoints.size();
  {
    std::cout << " Points from PATH  " << std::endl;
    for(  const PP3d::PointPtr lPoint : lVisit.cVectPoints)
      {
        std::cout << "   Point:" << lPoint;
      }
  }
     
     std::cout << std::endl;
  std::cout << std::endl;



  

  { //::::::::::::
    std::cout << "Points from Origin  " << std::endl;
    PointPtrSet lPoints;
    VisitorGetPoints lVisit2(lPoints);

    //    PP3d::SortEntityVisitorPointFacet lVisit2;
    iFacOrigin->execVisitor( lVisit2 );         

    std::cout << "LoftingFacFromPath OriginalPoints:" << lPoints.size() << std::endl;
    for(  const PP3d::PointPtr lPoint : lPoints )
    {
      std::cout << " Point:" << lPoint;
    }
  } //::::::::::::
      std::cout << std::endl;
      std::cout << std::endl;



      
      /*
  { //::::::::::::
    std::cout << "  Points from copy  " << std::endl;

    PP3d::SortEntityVisitorPointFacet lVisit2;
    lFacOld->execVisitor( lVisit2 );         

    std::cout << "LoftingFacFromPath new Points:" << lVisit2.cVectPoints.size() << std::endl;
    for(  const PP3d::PointPtr lPoint : lVisit2.cVectPoints )
    {
      std::cout << "  Point:" << lPoint;
    }
  } //::::::::::::
        std::cout << std::endl;

      std::cout << std::endl;
      */

      // Origin was not insert into oNewFacet

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
      lPtOld  = lPath->getPoint(0)->get();
      
      
      
  for( GLuint i = 1; i<lNbPt; i++)
    {           
      FacetPtr lFac = lFacOld->duplicate();  // duplicate the old facet and store them
      //  oNewFacets.push_back( lFac );          

      Point3d lMove = lPath->getPoint(i)->get() - lPtOld; // for put the facet in the right place
      PointPtrSet lPoints;
      VisitorGetPoints lVisit(lPoints);
      lFac->execVisitor( lVisit ); // get all the points without duplicate for move 
     
      for(  const PP3d::PointPtr lPoint : lPoints )
        {
          lPoint->get() += lMove ;  // move point
        }

      
      // IL FAUT DETUIRE LES FACETTES INTERMEDIAIRES faire un tableau pour les mettres dedans (si l'objet est ferme il fait garder les deux extremités)

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
