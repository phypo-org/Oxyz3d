#ifdef EZRZARZERZER
#include "Modif.h"


namespace PP3d{

  //--------------------------------------------------------
  // Subdivide a concave facet in convexs facets
  
  // le principe est d'essayer de decouper la facette en essayant un
  // decoupage a partir de chacun des points du perimetre.
  // On decoupe dés q'une facette n'est pas concave (en essayant quand meme d'avoir la facette la plus grosse possible), ensuite on applique recursivement l'algo a la dexieme facette crees (la premiere est forcement connexe)
  // Est t'il possible que l'algorithme echoue s'il ne trouve aucune decoupe possible ????
  // (aprés il peut toujours exister des facettes mal formé, mais c'est une autre histoire ... )


  // Une classe temporaire juste pour notre traitement

  //*************************************
  struct LFacet : public std::vector<PointPtr> {
  
    LFacet( std::vector<PointPtr> cPoints ){
      for( PointPtr lPt : cPoints )
	add( lPt );
    }
    
    LFacet( PointPtr  P1,  PointPtr P2,  PointPtr  P3  ){
      	add( P1 );
	add( P2 );
	add( P3 );
    }

    //--------------------------------------------------------
    // non concave et pas plat non plus

    bool isGood( Point3d & iNorm  )
    {
      if(size() <= 3 ){ // bah oui ! 
	return true;
      }    
   
      //   std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;      
      for( int i = 0 ; i< size()-1; i++ )
	{
	  Point3d A = get(i)->get()   - get(i+1)->get();
	  Point3d B = get(i+1)->get() - get((i+2)%size())->get(); // % permet d'avoir le 0 pour le dernier i+2	

	  if( Point3d::signedAngle( A, B, iNorm ) <= 0 )  // Si negatif ou 0   (utiliser un epsilon)
	    {
	      return false;
	    }
	} 
    
      return true;
    }
  };
  //*************************************


  //--------------------------------------------------------
  bool Modif::SubDivConcav( Point3d& iNorm, LocalFacet & iOldFac, std::vector<lFacet> & iNewFacet)
  {
    int lNbPt =  LocalFacet.size();
  
    if( lNbPt <= 3 ) 
      return true;

    if( iOldFac->computeConcave() == false  )
      {
	return true;
      }
   
    for( int f = 0; f < lNbPt -2 ;  )
      {	      
	LFacet lFacet(iOldFac.getPt( f ), iOldFac.getPt( f+1), iOldFac.getPt( f+2) );
      
	if( lFacet.isGood( iNorm ) )
	  {	    
	    // is next segments is good ?
	    int f2=f+1;

	    for(  ; f2 < lNbPt-2 ; f2++ )
	      {
		LFacet lFacet2( iOldFac.getPt( f2 ), iOldFac.getPt( f2+1 ), iOldFac.getPt( f2+2)  ); 
		if( lFacet.isGood( iNorm ) == false ){
		// next segment fail, stopping here
		  break;		
		}
		// we can add the point to facet
		lFacet.add( iOldFac.getPt( f2+2) );
	      }
	    
	    // We have the biggest good facet possible in lFacet	  
	    iNewFacet.add( lFacet );
	    // Remove point from the original facet execpt the first and the last of the newest facet
	    // f+1 to f+(lFacet.size()-2)
	    iOldFac.erase ( myvector.begin()+f+1, myvector.begin()+f+(lFacet.size()-2));
	    
	    // we continue the loop with the same f !
	  }
	else
	  { // Bad luck, we try the next point	    
	    f++;
	  }
      }
    return false; // on n' a pas resussi ????
  }
  //--------------------------------------------------------

  bool Modif::SubDivConcav( DataBase * iBase, PolyPtr iPolyOwner, FacetPtr iOldFac )
  {
    if( iOldFac->computeConcave() == false ){ // Already good !
      return true;
    }
      
    // First, we keep the points of facet 
    PP3d::SortEntityVisitorPoint lVisit;
    iOldFac->execVisitor( lVisit );  
    std::vector<PointPtr> & lPoints = lVisit.cVectPoints;

    // Create a working facet with the points
    LFacet lFacet(  lVisit.cVectPoints );

    // The vector using for stocking the facets resulting of sudivision
    std::vector<lFacet> lNewFacet;


    // The Subdivision Elself !
    SubDivConcav( iOldFac->getNormal(), iFac, lNewFacet );

    // Remove the old facet from poly and create the new facets
    // TODO
    if( lNewFacet.size() > 1 ){
      
    }
  }
}
#endif
