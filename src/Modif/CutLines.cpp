
#include "Modif.h"


#include <set>

namespace PP3d{

  //**************************************************
  bool Modif::CutLines( std::vector<LinePtr>&  iVect, int iNbCut, DataBase * iBase  )
  {
    std::set<LinePtr> lAlreadyDone;
  
    //==========================
    for( LinePtr lLine: iVect )
      {
	if( lLine->getOwners().size() == 0 )
	  {
	    //	    fl_alert( "Fatal Error - Modifs::CutLines - The line %ld have no owner - Abort !", lLine->getId() );
	    return false;
	  }
      }
    
    //==========================
    for( LinePtr lLine: iVect )
      {
	CutLine( lLine, iNbCut, iBase );
      }
    //==========================
    return true;
  }
  //-----------------------------------------------
  bool Modif::RemplaceLineIntoFacet( Facet & iFacet, LinePtr iLine,  std::vector<LinePtr> iVectLines )
  {
    // find the position of the lLine
    size_t lPos = 0;
    for( lPos =0; lPos< iFacet.getLines().size(); lPos++ )
      {
	if( iFacet.getLines()[lPos] == iLine )
	  {
	    break;
	  }
      }
    
    if(  lPos == iFacet.getLines().size()) // line not found;
      {
	return false;
      }

    iFacet.setLine( lPos, iVectLines[0]);
    for( size_t i=1; i < iVectLines.size(); i++)
      {
	iFacet.insertLine( lPos+i, iVectLines[i]);
      } 
  }
  //-----------------------------------------------
  bool Modif::CutLine(  LinePtr iLine, int iNbCut, DataBase * iBase )
  {
    if( iLine->getOwners().size() == 0 )
      {
	return false;
      }
    
    lOwner = lLine->getOwners()[0];
     
    if( lLine->isPoint() )
      return true;
	
 
    if( lOwner->getType() != ShapeType::Facet )
      return true;
    
    // normaly the only possibility is a facet
    
    FacetPtr lFacet = (FacetPtr*) lOwner;
 	
    std::vector<PointPtr> lPts;
    
    LinePtr lLineRev = lLine->getReverseLine(); //we kept the inverse of the line before change it !!!
      
    Point3d& lA = lLine->first()->get();	
    Point3d& lB = lLine->second()->get();
    
    Point3d lDiff = lB - lA ; // the increase betwen the two points

    std::vector<PointPtr> lPts; // we put in lPts all the points of the new line (olds and news)
    
    lPts.Push_back( &lA );  // first
        
    for( int i=1; i< iNbCut; i++ )
      {
	lPts.push_back( new Point( lA + ((lDiff*i)/iNbCut) ));
      }
    
    lPts.Push_back( &lB ); // old second
    
    // there is iNbCut + 1 point
    
    
    std::vector<lLinePtr> lLines; // All the new lines
    
    // we create iNbCut new lines with points
    for( int i=0; i< iNbCut; i++ )
      {
	lLines.push_back( new Line( lPts[i], lPts[i+1] ));
      }


    std::vector<lLinePtr> lLinesInv; // All the Inv new lines
  
    if( RemplaceLineIntoFacet( *lFacet, iLine, lLines ) )
      {
	// now make the same thing for the inv line
	if( lLineRev )
	  {
	    for( int i= iNbCut; i>0; i-- )
	      {
		lLinesInv.push_back( new Line( lPts[i], lPts[i-1] )); //in reverse order !
	      }
	    RemplaceLineIntoFacet( *lFacetInv, iLineInv, lLinesInv );
	  }
      }

    // Now the two facets are good

    if( iBase  != nullptr )
      {
	// we give an Id to new objets and put them into the base
	for( PointPtr lPt : lPts )
	  {
	    iBase->validOneEntityLevel( lPt );
	  }
	
	for( LinePtr lLine : lLines )
	  {
	    iBase->validOneEntityLevel( lLine );	
	  }
	
	for( LinePtr lLine : lLinesInv )
	  {
	    iBase->validOneEntityLevel( lLine );	
	  }
      }
    // delete &lA;
    // delete &lA;
   
    return true;
  }
    
  //**************************************************
} // end namespace
