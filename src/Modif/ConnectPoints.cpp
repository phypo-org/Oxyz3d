#include "Modif.h"


#include <map>
#include <algorithm>


namespace PP3d{

  //**************************************************
  int Modif::GetFacetFirstLineIndex( FacetPtr iFacet, PointPtr lPt)
  {
    
    for( int i=0 ; i< (int)iFacet->getLines().size(); i++ )
      {
	if( iFacet->getLines()[(size_t)i]->first() == lPt )
	  return i;
      }
    return -1;
  }
  //--------------------------------------------
  FacetPtr CutFacetInTwo( FacetPtr iFacet, PointPtr iPtA, PointPtr iPtB, DataBase * iBase )
  {
    std::cout << "   CutFacetInTwo" << std::endl;
     
   if( iFacet->getLines().size() < 3 
	|| iPtA == iPtB )
      {
	return nullptr;
      }
    std::cout << "    CutFacetInTwo 2" << std::endl;

    
    int lInd1 = Modif::GetFacetFirstLineIndex( iFacet, iPtA );
    int lInd2 = Modif::GetFacetFirstLineIndex( iFacet, iPtB );
    if( lInd1 == -1 || lInd2 == -1 )
      return nullptr;
    std::cout << "    CutFacetInTwo 3" << std::endl;

    if( lInd1 > lInd2 )
      {
	std::swap<PointPtr>( iPtA, iPtB );
	std::cout << "    CutFacetInTwo SWAP" << std::endl;
     }
      
    
    std::vector<LinePtr> lLinesForFac1;
    std::vector<LinePtr> lLinesForFac2;


    std::vector<LinePtr>* lCurrentFac = &lLinesForFac1;
    
    LinePtr lNewLineAB=nullptr;
    LinePtr lNewLineBA=nullptr;

    // On partage les lignes entre les deux facettes + les deux nouvelles
    bool lFirst = true;
    for( LinePtr lLine : iFacet->getLines() )
      {
	if( lFirst )
	  {
	    lFirst = false;
	    if( lLine->first() == iPtA )
	      {
		lCurrentFac = &lLinesForFac2 ;
	      }
	  }
	
	lLine->removeOwner( iFacet ); // surtout ne pas oublier	
	lCurrentFac->push_back( lLine );

	
	if( lLine->second() == iPtA )
	  {
	    lLinesForFac1.push_back( lNewLineAB = new Line( iPtA, iPtB ) );

	    lCurrentFac = &lLinesForFac2 ;
	  }

	if( lLine->second() == iPtB )
	    {
	      lLinesForFac2.push_back( lNewLineBA = new Line( iPtB, iPtA ) );
	      lCurrentFac = &lLinesForFac1 ;
	    }	
      }

    // We re-make the iFacet (same id)
    
    iFacet->getLines().clear();
    for( LinePtr lLine : lLinesForFac1 )
      {
	iFacet->addLine( lLine );
      }

    FacetPtr lNewFacet = new Facet();
    for( LinePtr lLine : lLinesForFac2 )
      {
	lNewFacet->addLine( lLine );
      }

    if( iBase )
      {
    std::cout << "    CutFacetInTwo valid to BASE" << std::endl;
	iBase->validOneEntityLevel( lNewLineAB );
	iBase->validOneEntityLevel( lNewLineBA );
	iBase->validOneEntityLevel( lNewFacet );
      }

    return lNewFacet;
  }			 
  //**************************************************  
  void GetFacetsOfPoint( PointPtr iPt, LinePtr iLine,  FacetsPoints & oFacPts )
  {
    if( iLine == nullptr ) return ;
        
    for(  EntityPtr lFac : iLine->getOwners() )
      {
	if( lFac->getType() == ShapeType::Facet )
	  {
	    //	    std::pair<FacetPtr,UniquePoints> &
	      UniquePoints & lUniq  = oFacPts[ (FacetPtr) lFac ]; // get/create
	    
	    lUniq.add( iPt ); // store the point		    
	  }
      }
  }
 //------------------------------------------------
  void GetFacetsOfPoint( PointPtr iPt,  FacetsPoints & oFacPts, bool iAlsoReverse )
  {
    if( iPt == nullptr ) return;
    
    // get the lines who ownes the point
    for( EntityPtr lLine : iPt->getOwners() )
      {
	if( lLine->getType() == ShapeType::Line )
	  {
	    GetFacetsOfPoint( iPt, (LinePtr)lLine, oFacPts );
	    
	    if( iAlsoReverse )
	      {
		GetFacetsOfPoint( iPt, ((LinePtr)lLine)->getReverseLine(), oFacPts );
	      }
	  }
      }
  }
  //------------------------------------------------
  void RemovePointFromVect(  std::vector<PointPtr> & iVectPt, PointPtr lPt )
  {
    for( size_t i = 0; i < iVectPt.size() ;  i++ )
      {
	if( iVectPt[i] == lPt)
	  {
	    iVectPt.erase( iVectPt.begin() + i );
	    break;
	  }
      }		      
  }
  //------------------------------------------------
  // A REFAIRE : le probleme c'est que quand on cree decoupe 
  // on va perdre le classement par facette 
  
  bool Modif::ConnectPoints( std::vector<PointPtr> &  iVectPt, DataBase * iBase )
  {
    std::cout << "Modif::ConnectPoints nb point : " << iVectPt.size() << std::endl;
    int k=0;
    while( iVectPt.size() >= 2 && k++ < 100 ) // le k  c'est une securite !
      {
	std::cout << "Modif::ConnectPoints  nb point : " << iVectPt.size() <<  "  k : " << k <<  std::endl;
	
	FacetsPoints lFacsPts;
	
	// get the facets of the points and order the points by facet
	for( PointPtr lPt : iVectPt )
	  {
	    GetFacetsOfPoint( lPt, lFacsPts, true );
	  }
	
	for( auto lPair : lFacsPts )
	  {
	    std::cout << "Modif::ConnectPoints for( auto lPair  "<<  std::endl;
	    
	    FacetPtr lFac = lPair.first;
	    PolyPtr lPoly = (PolyPtr)lFac->firstOwner();

	    if( lPoly == nullptr
		|| lPoly->getType() != ShapeType::Poly )
	      {
		std::cerr << "Modif::ConnectPoints owner not a polyedre" << std::endl;
		for( PointPtr lPtr : lPair.second.cVectPt )
		  RemovePointFromVect( iVectPt, lPtr );
		
		continue;
	      }
	    
	    std::vector<PointPtr> & lVectPoints = lPair.second.cVectPt;
	    std::cout << "Modif::ConnectPoints lVectPoints.size() = "<<  lVectPoints.size() << std::endl;
	    
	    
	    if( lVectPoints.size() == 0 )
	      { // abnormal
		std::cout << "Modif::ConnectPoints lVectPoints.size() == 0 )  "<<  std::endl;
		continue;
	      }

	    if( lVectPoints.size() >1 )
	      {
		// LE TRAITEMENT
		FacetPtr lNewFacet = CutFacetInTwo( lFac, lVectPoints[0], lVectPoints[1], iBase );

		// Moche !
		RemovePointFromVect( iVectPt, lVectPoints[1] );	      
	      }
	    RemovePointFromVect( iVectPt, lVectPoints[0] );
	    // S'il y a plus de deux points il ne sont plus forcement dans la facette courante
	    // il faudrait reanalyser pour voir si c'est toujours viable et sinon refaire le
	    // traitement (pour le moment je fais les deux premiers
	    // et le tour suivant fera le travail pour ces points la (on peut toujours y croire, hein !)
	    continue;// while  - facette suivante
	  }
      }
    
    return true;
  }
 
  //**************************************************
} // end namespace
