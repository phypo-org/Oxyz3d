#include "Modif.h"


#include <set>

namespace PP3d{

  //**************************************************
  bool Modif::SubdivideFacet(  std::vector<FacetPtr>&  iVect, DataBase * iBase, SubDiv::SubParam * iSubDiv )
  {  
    std::cout  << "Modif::SubdivideFacet Nb : " << iVect.size() <<  std::endl;
    
   for( FacetPtr lFac : iVect )
      {
	PolyPtr lPoly = (PolyPtr)lFac->firstOwner();
	if( lPoly == nullptr
	    || lPoly->getType() != ShapeType::Poly )
	  {
	    std::cerr << "Modif::SubdivideFacet not a polyedre" << std::endl;
	    continue;
	  }
	
	int lDepth = 1;
	SubDiv::SubParam lSubDivLocal( 1, 1, false, SubDiv::SubNormalizeType::NORMALIZE_NONE );
	
	if( iSubDiv == nullptr )
	  {
	    iSubDiv = & lSubDivLocal;	
	  }
    
	PP3d::SortEntityVisitor lVisit;
	lFac->execVisitor( lVisit );
	std::vector<PointPtr> & lPts = lVisit.cVectPoints;
	
	std::cout  << "Modif::SubdivideFacet Facet size  : " << lPts.size() <<  std::endl;
	
	switch( lPts.size() )
	  {
	  case 0:
	  case 1:
	  case 2:
	    break;
	  case 3:
	    SubDiv::Subdivide3( *iSubDiv,
				lPts[0]->get(),
				lPts[1]->get(),
				lPts[2]->get(),
				lDepth);
	    break;
	  case 4:
	    SubDiv::Subdivide4( *iSubDiv,
				lPts[0]->get(),
				lPts[1]->get(),
				lPts[2]->get(),
				lPts[3]->get(),
				lDepth);
	    break;
	  case 5:
	    SubDiv::Subdivide5( *iSubDiv,
				lPts[0]->get(),
				lPts[1]->get(),
				lPts[2]->get(),
				lPts[3]->get(),
				lPts[4]->get(),
				lDepth);
 	    break;
	    
	  default:
	    /* A FAIRE !
	  SubDiv::SubdivideN( *iSubDiv, lPts, lDepth);
	    */
 	    break;
	  }

	
	    std::cout  << "Modif::SubdivideFacet New Facet : "
		       <<  iSubDiv->cFacets.size()
		       << " Pts :" << iSubDiv->cPoints.size() <<  std::endl;
	    
	if(  iSubDiv->cPoints.size()  && iSubDiv->cFacets.size() > 1 )
	  {

	    std::vector<PointPtr> lNewPoints( iSubDiv->cPoints.size() );		
	    //=========================
	    for( size_t p =0; p< iSubDiv->cPoints.size(); p++)
	      {
		// AFAIRE Tester s'il exite déja ? et le remplacer par l'ancien point !
		lNewPoints[p] = new Point( iSubDiv->cPoints[p] ); // points sans id
		iBase->validOneEntityLevel(lNewPoints[p]);
	      }
	    
	    //=========================
	    bool lFirst = true;
	    for( size_t f=0; f< iSubDiv->cFacets.size() ; f++ ) 
	      {
		PrimFacet & lPrimFacet  = *(iSubDiv->cFacets[f]); // juste pour simplifier le code
		
		Facet* lCurFacet =nullptr;
		if( lFirst )
		  { // Pour la premiere facette on va recycler l'ancienne
		    lFirst = false;
		    lFac->getLines().clear();
		    lCurFacet = lFac;
		  }
		else
		  {
		    lCurFacet = new Facet();                // nouvelle facette vide sans id
		    iBase->validOneEntityLevel(lCurFacet);
		    lPoly->addFacet( lCurFacet );
		  }
				
		// AFAIRE recycler les N anciennes lignes de la facettes
		uint i=0;
		for( ; i< lPrimFacet.size()-1; i++ ) // pour les points de la facette courante
		  {					 
		    LinePtr lLine = new Line( lNewPoints[ lPrimFacet[i]] , lNewPoints[ lPrimFacet[i+1]] );
		    iBase->validOneEntityLevel(lLine);   
		    lCurFacet->addLine( lLine );
		  }
		LinePtr lLine = new Line( lNewPoints[ lPrimFacet[i] ] , lNewPoints[ lPrimFacet[0] ] );
		lCurFacet->addLine( lLine );
		iBase->validOneEntityLevel(lLine);		
	      }
	    //=========================
	  }
      }

    return true;
  } 
  //**************************************************
} // end namespace
