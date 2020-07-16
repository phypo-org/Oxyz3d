#include "Modif.h"

#include "Shape/OwnerVisitor.h"

namespace PP3d{
  //**************************************************
  void Modif::PrepareChangePointToNeighbourLineAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos, bool iMeToo )
  {
    Point3d lMiddle;
    for(  const PP3d::PointPtr pPoint : iVect )
      {
	lMiddle += pPoint->cPt;
      }
    lMiddle /= (double)iVect.size();

    for( PointPtr lPt : iVect )
      {
	if( lPt->getOwners().size() )
	  {
	    Point3d lAverage;
		
	    std::cout << "Lines:" << lPt->getOwners().size() << std::endl;
	    for( EntityPtr lEntity : lPt->getOwners() )
	      {
		LinePtr lLine = (LinePtr) lEntity;
		
		lAverage += lLine->first()->get() - lMiddle;	
		lAverage += lLine->second()->get()- lMiddle;
	      }
	    lAverage /= lPt->getOwners().size()*2;
	    iVectNewPos.push_back( lAverage+lMiddle );
	  }
	else
	  {
	    iVectNewPos.push_back( lPt->get() );
	  }
      }
  }
  //**************************************************
   void Modif::PrepareChangePointToNeighbourFacetAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos, bool iMeToo )
  {
    Point3d lMiddle;
  
    for(  const PP3d::PointPtr pPoint : iVect )
      {
	lMiddle += pPoint->cPt;
      }
    lMiddle /= (double)iVect.size();

    // pour tout les points
    for( PointPtr lPt : iVect )
      {
	// on recupere les owners pour chaque point
	PP3d::OwnerEntityVisitor lVisit;

	lVisit.addOwnersOf( lPt );

	for( FacetPtr lFacet : lVisit.cVectFacets )
	  {
	    // On recupere tout de ces owners
	    lFacet->execVisitor( lVisit ) ; // je sais c'est tres limite comme code !
	  }
	
	Point3d lAverage;
	std::cout << "Facets:" << lVisit.cVectFacets.size() << " Points:" << lVisit.cVectPoints.size() << std::endl;

	if( lVisit.cVectPoints.size() >= 2 )
	  {
	    for( PointPtr lOtherPt : lVisit.cVectPoints )
	      {
		if(  lOtherPt != lPt  || iMeToo == false )
		  {
		    lAverage += ((lOtherPt->get()-lMiddle)+(lPt->get()-lMiddle))/2;		   
		  }
	      }
	
	    if( iMeToo )
	      {
		lAverage /= lVisit.cVectPoints.size()*2;
	      }
	    else
	      {
		lAverage /= (lVisit.cVectPoints.size()-1)*2;
	      }
	    iVectNewPos.push_back( lAverage +lMiddle);
	    std::cout << iVectNewPos.size() << std::endl;
	  }
	else
	  {
	    iVectNewPos.push_back( lPt->get());
	  }
      }
  }
  //--------------------------------------------
   void Modif::FinalizeChangePointToNeighbourAverage(  std::vector<PointPtr> & iVect, std::vector<Point3d> & iVectNewPos )
  {
    int i = 0;
    for( PointPtr lPt : iVect )
      {
	lPt->get() = iVectNewPos[i++];
      }
  }
  //**************************************************

} // end namespace
