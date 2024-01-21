
#include "Modif.h"


#include <set>

namespace PP3d{

  //**************************************************
  bool Modif::CutLines( std::vector<LinePtr>&  iVect, int iNbCut, DataBase * iBase  )
  {
    std::cout << "Modif::CutLines : " << iNbCut << " lines : " << iVect.size() << std::endl;

    //==========================
    for( LinePtr lLine: iVect )
      {
	if( lLine->getOwners().size() == 0 )
	  {
	    //	    fl_alert( "Fatal Error - Modifs::CutLines - The line %ld have no owner - Abort !", lLine->getId() );
	    std::cout << "Fatal Error - Modifs::CutLines - The line %ld have no owner - Abort !"
		      << lLine->getId() << std::endl;
	    return false;
	  }
      }
    //==========================

    // Certain line have reversed line, CutLine treat them,
    // then we must prevent a re-treat of them if there are in iVect,
    // this is why we use lAlreadyDone !
     std::unordered_set<LinePtr> lAlreadyDone;
    
    //==========================
    for( LinePtr lLine: iVect )
      {	
	if( lAlreadyDone.find( lLine ) != lAlreadyDone.end())
	  {
	    continue;
	  }
	   
	   
	LinePtr lReverseLine = nullptr; // filled by CutLine if it find a reverse line 

	CutLine( lLine, iNbCut, iBase, &lReverseLine );

	
	lAlreadyDone.emplace( lLine );
	if( lReverseLine )
	  {
	    lAlreadyDone.emplace( lReverseLine );
	  }
      }
    //==========================
    iVect.clear(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      
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
    return true;
  }
  //-----------------------------------------------
  // If we want to work in reverse line we must provide an pointer for ioReverseLine 
  bool Modif::CutLine(  LinePtr iLine, int iNbCut, DataBase * iBase, LinePtr * ioReverseLine )
  {
    std::cout << "Modif::CutLine " << iNbCut << std::endl;
    
    EntityPtr lOwner = iLine->firstOwner();
    
    if( lOwner == nullptr )
      {
	return false;
      }

    
    if( iLine->isPoint() )
      return true;
	
 
    if( lOwner->getType() != ShapeType::Facet )
      {
	std::cout << "Error - Modif::CutLine - The line %ld owner is not a facet - ignore : "
		  << iLine->getId() << std::endl;
	return true;
      }
   
    // normaly the only possibility is a facet
    
    FacetPtr lFacet = (FacetPtr) lOwner;
 	
    
    if( ioReverseLine ) // if we 
      {
	*ioReverseLine = iLine->getReverseLineByPoint(); //we kept the inverse of the line before change it !!!
      }

    
    Point3d& lA = iLine->first()->get();	
    Point3d& lB = iLine->second()->get();
    
    Point3d lDiff = lB - lA ; // the increase betwen the two points

    std::vector<PointPtr> lPts; // we put in lPts all the points of the new line (olds and news)
    
    lPts.push_back( iLine->first() );  // first
        
    for( int i=1; i< iNbCut; i++ )
      {
	std::cout << "New point" << std::endl;
	
	lPts.push_back( new Point( lA + ((lDiff*i)/iNbCut) ));
      }
    
    lPts.push_back( iLine->second() ); // old second
    
    // there is iNbCut + 1 point
    
    
    std::vector<LinePtr> lLines; // All the new lines
    
    // we create iNbCut new lines with points
    for( int i=0; i< iNbCut; i++ )
      {
	std::cout << "New line " << i << " " <<  i+1 << std::endl;
	if( i == 0 )
	  { // recycle the line
	    iLine->set( lPts[i], lPts[i+1] );
	    lLines.push_back( iLine );
	  }
	else 
	  lLines.push_back( new Line( lPts[i], lPts[i+1] ));
      }


    std::vector<LinePtr> lLinesRev; // All the Inv new lines
 
    if( RemplaceLineIntoFacet( *lFacet, iLine, lLines ) )
      {
	// now make the same thing for the inv line
	if( ioReverseLine && *ioReverseLine
	    && (*ioReverseLine)->firstOwner()
	    && (*ioReverseLine)->firstOwner()->getType() == ShapeType::Facet)
	  {
	    for( int i= iNbCut; i>0; i-- )
	      {
		std::cout << "New line Rev" << i << " " <<  i-1 << std::endl;
		if( i == 0 )
		  { // recycle the line
		    (*ioReverseLine)->set( lPts[i], lPts[i-1] );
		    lLinesRev.push_back( *ioReverseLine );
		  }
		else 
		  lLinesRev.push_back( new Line( lPts[i], lPts[i-1] )); //in reverse order !
	      }
	    RemplaceLineIntoFacet( *((FacetPtr)(*ioReverseLine)->firstOwner()), *ioReverseLine, lLinesRev );
	  }
      }

    // Now the two facets are good

    if( iBase  != nullptr )
      {
	// we give an Id to new objets and put them into the base
	for( PointPtr lPt : lPts )
	  {
	    if( lPt->isIdVoid() )  // car il y aussi les deux anciens point !
		iBase->validOneEntityLevel( lPt );
	  }
	
	for( LinePtr lLine : lLines )
	  {
	    if( lLine != iLine ) // attentioin il ne faut pas reenregistrer les anciennes lignes
	      iBase->validOneEntityLevel( lLine );	
	  }
	
	for( LinePtr lLine : lLinesRev )
	  {
	    if( lLine !=  (*ioReverseLine) )// attentioin il ne faut pas reenregistrer les anciennes lignes	    
	      iBase->validOneEntityLevel( lLine );	
	  }
      }

    return true;
  }
    
  //**************************************************
} // end namespace
