#include "SavReadStl.h"

#include "DataBase.h"
#include "Selection.h"

#include "ObjectLine.h"
#include "ObjectFacet.h"
#include "ObjectPolylines.h"
#include "ObjectPoly.h"

/*
const char* TokPoint="Point:";
const char* TokLine="Line:";
const char* TokFacet="Facet:";
const char* TokPoly="Poly:";
const char* TokObject="Object:";

const char* TokSaisiePt="SaisiePt:";

const char* TokSelection="Selection:";
*/

static const char* TokSolid="solid";
static const char* TokEndSolid="endsolid";

static const char* TokFacet="facet";
static const char* TokEndFacet="endfacet";

static const char* TokNormal="normal";

static const char* TokOuter="outer";
static const char* TokLoop="loop";
static const char* TokEndLoop="endloop";
static const char* TokVertex="vertex";

static bool sDebugSav = false;

#define SAVCOUT if( sDebugSav ) std::cout

namespace PP3d {
  //*************************************

  //-----------------------------------------
  bool MySavStl::saveTxt( const char* iName, std::vector<FacetPtr>  cVectFacets )
  {
    cOut << TokSolid << ' ' << iName << std::endl;
    
    
    for( FacetPtr lFacet :  cVectFacets )
      {
	auto lNorm = lFacet->getNormal();
	
	cOut << TokFacet << ' ' << TokNormal 
	     << ' ' << lNorm.cX   
	     << ' ' << lNorm.cY	 
	     << ' ' << lNorm.cZ
	     << std::endl;
	
	
	cOut << TokOuter << ' ' << TokLoop << std::endl;
	
	for( auto lLine : lFacet->getLines() )
	  {
	    auto lPt = lLine->first();
	    cOut << TokVertex 
		 << ' ' << lPt->get().cX   
		 << ' ' << lPt->get().cY	 
		 << ' ' << lPt->get().cZ
		 << std::endl;
	  }      			       	   		
	cOut << TokEndLoop << std::endl;
	cOut << TokEndFacet << std::endl;
      }
    
    cOut << TokEndSolid << ' ' << iName << std::endl;
    
    return true;
  }
  //-----------------------------------------
   
  bool MySavStl::Save( const char* iFileIn, DataBase * iDatabase, Selection * iSelect, bool iTxt )
  {
    std::ofstream lFilSav;						
    lFilSav.open( iFileIn );
    if( lFilSav.good() )
      {	  
	MySavStl lSav( lFilSav );

	SortEntityVisitor lSort;

	if( iDatabase != nullptr )
	  {
	    iDatabase->execVisitorOnObject( lSort );
	  }
	else  if( iSelect != nullptr )
	  {
	    iSelect->execVisitorOnlyOnObjects( lSort );
	  }

	if( iTxt )
	  return lSav.saveTxt( "Oxyz3d", lSort.cVectFacets  );	  
      }
    return false;
  }
  //*************************************

#define ReadEndLine		std::getline(	cIn,  lEndOfLine, '\n' )

  //--------------------------------
  bool MyReadStl::read( DataBase& pData, Selection * ioSel, bool pConserveOldId  )
  {   			
    return false;
  }	
  //*************************************

}
