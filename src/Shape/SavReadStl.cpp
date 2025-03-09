#include "SavReadStl.h"

#include "DataBase.h"
#include "Selection.h"

#include "Object.h"

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
   void MySavStl::writePoint( PP3d::Point3d & iPt ){
    
    cOut << ' ' << iPt.cX   
	 << ' ' << iPt.cY	 
	 << ' ' << iPt.cZ
	 << std::endl;
  }
  //-----------------------------------------
   void MySavStl::writeFacetBegin( PP3d::Point3d & iNorm){
    
    cOut << TokFacet << ' ' << TokNormal ;
    writePoint( iNorm );	
    cOut << TokOuter << ' ' << TokLoop << std::endl;
  }
  //-----------------------------------------
   void MySavStl::writeVertex( PP3d::Point3d & iPt){
    
    cOut << TokVertex ;
    writePoint( iPt);	
  }
  //-----------------------------------------
   void MySavStl::writeFacetEnd(){
    
    cOut << TokEndLoop << std::endl;
    cOut << TokEndFacet << std::endl;
  }  
  //-----------------------------------------
  bool MySavStl::saveTxt( const char* iName, std::vector<FacetPtr>  cVectFacets )
  {
    cOut << TokSolid << ' ' << iName << std::endl;
    
    //==================================
    for( FacetPtr lFacet :  cVectFacets )
      {
	if( lFacet->getLines().size() == 3 )
	  {
	    auto lNorm = lFacet->getNormal();
	    
	    writeFacetBegin( lNorm );	
	
	    for( auto lLine : lFacet->getLines() )
	      {
		auto lPt = lLine->first();
		writeVertex( lPt->get());
	      }
	    writeFacetEnd();
	  }
	else if( lFacet->getLines().size() > 3 )
	  {
	    if( lFacet->isConcave() == false )
	      {	  
		PP3d::SortEntityVisitorPoint lVisit;
		lFacet->execVisitor( lVisit );
	      
		Float3   lMiddle;           
		for( PointPtr lPt:  lVisit.cVectPoints )
		  {
		    lMiddle += lPt->get();
		  }
		lMiddle /= lVisit.cVectPoints.size();
		std::vector<PointPtr> & lPoints = lVisit.cVectPoints;

		size_t lSz = lPoints.size();
		size_t lN = lSz;
		for( size_t i=0; i < lSz; i++ )
		  {
		    PP3d::Point3d& Pt1 = lPoints[i]->get(); 
		    PP3d::Point3d& Pt2 = lPoints[(i+1)%lN]->get();		  				  
		    PP3d::Point3d lNorm;
		  
		    Calcul3d::Normal( Pt1, Pt2, lMiddle, lNorm);
		
		    writeFacetBegin( lNorm );		  		  
		    writeVertex( Pt1 );
		    writeVertex( Pt2 );
		    writeVertex( lMiddle );
		    writeFacetEnd();
		  }
	      }
	    else
	      { //
		
	      }
	  }	  
      }
    //==================================
    
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

	SortEntityVisitorFacet lSort;

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
