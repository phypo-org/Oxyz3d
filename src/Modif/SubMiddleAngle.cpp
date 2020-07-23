#include "Modif.h"


using namespace PP3d;

bool Modif::SubDivAngle( DataBase * iBase,
			 std::set<FacetPtr>&  iOldFacets,
			 std::set<PointPtr> &  iOldPoint,
			 std::vector<EntityPtr> & oNewFacet,
			 SubDivFacetType iSubDivType,
			 SubDivSelectType iSubDivSelect,
			 double iMargeFactor )
{
  std::vector<PointPtr>   lNewPoints;
  //	  std::cout << "SubDivAngle **************" << std::endl;	      
  
  //===== Pour toutes les facettes à traiter ===== 
  for( FacetPtr lOldFac : iOldFacets )
    {
      Float3                cMiddle;      

      PP3d::SortEntityVisitor lVisit;
      lOldFac->execVisitor( lVisit );
      
      for( PointPtr lPt:  lVisit.cVectPoints )
	{
	  cMiddle += lPt->get();
	}
      cMiddle /= lVisit.cVectPoints.size();
      std::vector<PointPtr> & lPoints = lVisit.cVectPoints;

      PolyPtr lPoly = (PolyPtr) lOldFac->firstOwner();
      if( lPoly )
	{      
	  size_t lSz = lPoints.size();
	  size_t lN = lSz;
	  
	  if( iSubDivType == SubDivFacetType::ANGLE_SIMPLE )
	    {
	      for( size_t i=0; i < lSz; i++ )
		{
		  FacetPtr lFacPtr = iBase->getNewFacet();
		  if( iSubDivSelect != SubDivSelectType::SELECT_NONE )
		    oNewFacet.push_back( lFacPtr );
		  
		  //	      std::cout <<  "       Size:"<< lSz << " i:" << i << " j:" << j << std::endl;  
		  PointPtr Pt1 = lPoints[i]; 
		  PointPtr Pt2 = lPoints[(i+1)%lN];		  
		  PointPtr Pt3 = getPoint( iBase, lNewPoints, cMiddle ); 
		  
		  lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
		  lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
		  lFacPtr->addLine( iBase->getNewLine( Pt3, Pt1)  );
		  
		  lPoly->addFacet( lFacPtr );
		  
		  iBase->validEntity( lFacPtr, true );// VALIDATION (a optimiser)	     
		}
	    }	
	  else
	    if( iSubDivType == SubDivFacetType::ANGLE_FACET_MARGE)
	      {
		FacetPtr lCentralFacPtr = iBase->getNewFacet();
		if( ((int)iSubDivSelect) & ((int) SubDivSelectType::SELECT_CENTRAL))
		  oNewFacet.push_back( lCentralFacPtr );
		
		for( size_t i=0; i < lSz ; i++ )
		  {
		    FacetPtr lFacPtr = iBase->getNewFacet();
		    if( ((int)iSubDivSelect) & ((int)SubDivSelectType::SELECT_MARGE))
		      oNewFacet.push_back( lFacPtr );

		    PointPtr Pt1 = lPoints[i];
		    PointPtr Pt2 = lPoints[(i+1)%lN];

		    Point3d Dist3 = cMiddle - (*Pt2);
		    Point3d l3 = (*Pt2)+(Dist3 * iMargeFactor );
		    PointPtr Pt3 = getPoint( iBase, lNewPoints, l3 );


		    Point3d Dist4 = cMiddle - (*Pt1);
		    Point3d l4    =  (*Pt1)+(Dist4 * iMargeFactor );
		    PointPtr Pt4 = getPoint( iBase, lNewPoints, l4 );
		  
		  
		    lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
		    lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
		    lFacPtr->addLine( iBase->getNewLine( Pt3, Pt4));
		    lFacPtr->addLine( iBase->getNewLine( Pt4, Pt1));

		  
		    lPoly->addFacet( lFacPtr );
		  
		    iBase->validEntity( lFacPtr, true );// VALIDATION (a optimiser)

		    lCentralFacPtr->addLine( iBase->getNewLine(Pt4, Pt3) );
		  }
	      
		lPoly->addFacet( lCentralFacPtr );
		iBase->validEntity( lCentralFacPtr, true );// VALIDATION (a optimiser)		  
	      }    
	}
    } // fin facettes
    

      // On elimine les anciennes facettes
  for( FacetPtr lOldFacet : iOldFacets )
    {
      PolyPtr lPoly = (PolyPtr) lOldFacet->firstOwner();
      if( lPoly )
	{
	  lPoly->removeFacet( lOldFacet );
	}
      
      for( LinePtr lLine : lOldFacet->getLines() )
	{
	  iBase->freeLine( lLine );	  
	}
      iBase->freeFacet( lOldFacet );
    }
  
  //  ici toutes les nouveaux points des nlles facettes sont crees dans la structure de travail !
  return true;
}
//--------------------------------------------------------
