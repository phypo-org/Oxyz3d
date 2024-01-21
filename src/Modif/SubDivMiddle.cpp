#include "Modif.h"


using namespace PP3d;


//-----------------------------------------------
// Creation des nlles facettes

bool Modif::SubDivMiddle( DataBase * iBase, std::set<FacetPtr>&  iOldFacets, std::set<PointPtr> &  iOldPoint, std::vector<EntityPtr> & oNewFacet, SubDivFacetType iSubDivType,
			     SubDivSelectType iSubDivSelect,
			     double iMargeFactor)
{
  std::vector<PointPtr> lNewPoints;
  std::vector<PointPtr> iBorderPoints;

  std::unordered_map<PointPtr, Point3d> lFinalPos;


  //==== Il faudra aussi deplacer les anciens points ====
  // Le calcul est fait içi car ensuite la structure des facettes change

  for( PointPtr lPt : iOldPoint )
    {
      lPt->clearAllOwner();  // Il va etre reutilisé !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  //=======================================================

  int lFacet=0;
  //===== Pour toutes les facettes à traiter ===== 
  for( FacetPtr lOldFacet : iOldFacets )
    {
      std::vector<PointPtr>    lOldFacetPoints;

      std::vector<Float3>   cEdgePoints;
      Float3                cMiddle;
      std::vector<Float3>   cPoints;
      
      
      { // Calcul du milieu de la facette (face point)
	VisitorComputeMiddleFacet lVidMidFac;
	lOldFacet->execVisitor( lVidMidFac );
      
	// On creer le point du milieu pour la facette courante
	cMiddle   = lVidMidFac.getMiddle();
      }

      int lLineNb=0;
      // Pour chaque lignes de la facette on calcule les edge-point  pour chaque lignes
      // moyenne des deux face-point et des deux points originels
      for( LinePtr lLine : lOldFacet->getLines() )
	{

	  //	  std::cout << "   lLine:" << lLineNb++ << " first:" << lLine->first() << std::endl;
	  
	  // Nouveau point edge
	  Point3d lPtDummy;
	  cEdgePoints.push_back( lPtDummy );
	  Point3d & lEdge = cEdgePoints[ cEdgePoints.size()-1 ];

	  // On memorise le premiers points de la ligne
	  lOldFacetPoints.push_back( lLine->first() );
	  //	  lOldPointsUniq.insert(  lLine->first()->get() );	  
	  lEdge = lLine->first()->get();
	  lEdge += lLine->second()->get();

	  lEdge *= 0.5;
	  
	  // Il reste a trouver le milieu de l'autre facette
	  Line * lReverse = lLine->getReverseLineByPoint();
	  if( lReverse == nullptr )
	    {
	      // si on ne trouve pas de ligne reverse ! (bordure ?)
	      std::cerr << "Warning Reverse : no reverse " << std::endl;
	      continue;
	    }
	  FacetPtr lEdgeFacet = (FacetPtr)lReverse->firstOwner();
	  if( lEdgeFacet == nullptr )
	    {
	      std::cerr << "Warning Reverse : no facet" << std::endl;
	      // si la ligne reverse n'a pas de facette !
	      continue;
	    }
	  
	  if( iOldFacets.find( lEdgeFacet ) == iOldFacets.end() )
	    {
	      // La facette reverse n'est pas dans la liste des facette  transformé ! (bordure ?)
	      // il va falloir replacer la ligne lReverse par deux lignes !
	      std::cerr << "***** Facette non transformé : division de la ligne ****" << std::endl;
	      
	      PointPtr lA = lReverse->first();
	      PointPtr lB = lReverse->second();
	      
	      PointPtr lM = getPoint( iBase, lNewPoints, lEdge ); // en esperant que la fusions des points marche bien !
	      
	      lReverse->set( lA, lM );
	      LinePtr  lNewLine = iBase->getNewLine(lM, lB);  
	      lEdgeFacet->addLineAfter( lReverse, lNewLine );
	      
	      iBorderPoints.push_back( lM );
	      
	      iBase->validEntity( lEdgeFacet, true ); // VALIDATION
	    }
	  // else : le Edge point reverse sera creer lors du traitement de la facette adjacente
	  
	} // end edge-point

 
      size_t lSz = cEdgePoints.size();
      size_t lN = lSz-1;

      PolyPtr lPoly = (PolyPtr) lOldFacet->firstOwner();
      if( lPoly )
	{      
	  //	  std::cout << "SubDivMiddle **************" << std::endl;
	      
	  // On cree les nouveaux points des nlles facettes (fusion possible via getPoint)
	  if( iSubDivType == SubDivFacetType::CENTRAL_POINT )
	    {
	      for( size_t i=0; i < lSz ; i++ )
		{
		  FacetPtr lFacPtr = iBase->getNewFacet();
		  if( iSubDivSelect != SubDivSelectType::SELECT_NONE )
		    oNewFacet.push_back( lFacPtr );
		  
		  size_t j = (lN+i)%lSz;
		  
		  //	      std::cout <<  "       Size:"<< lSz << " i:" << i << " j:" << j << std::endl;
		  
		  Point* Pt1 = lOldFacetPoints[i];     // sa position va changer
		  Point* Pt2 = getPoint( iBase, lNewPoints, cEdgePoints[i] ); 
		  Point* Pt3 = getPoint( iBase, lNewPoints, cMiddle); 
		  Point* Pt4 = getPoint( iBase, lNewPoints, cEdgePoints[j] );
		  
		  
		  lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
		  lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
		  lFacPtr->addLine( iBase->getNewLine( Pt3, Pt4)  );
		  lFacPtr->addLine( iBase->getNewLine( Pt4, Pt1)  );
		  
		  lPoly->addFacet( lFacPtr );
		  
		  iBase->validEntity( lFacPtr, true );// VALIDATION (a optimiser)	     
		}
	    }
	
	  else
	    if( iSubDivType == SubDivFacetType::CENTRAL_FACET)
	      {
		FacetPtr lCentralFacPtr = iBase->getNewFacet();
		if( ((int)iSubDivSelect) & ((int) SubDivSelectType::SELECT_CENTRAL))
		  oNewFacet.push_back( lCentralFacPtr );
		for( size_t i=0; i < lSz ; i++ )
		  {
		    FacetPtr lFacPtr = iBase->getNewFacet();
		    if( ((int)iSubDivSelect) & ((int)SubDivSelectType::SELECT_MARGE))
		      oNewFacet.push_back( lFacPtr );

		    size_t j = (lN+i)%lSz;
		  
		    //	      std::cout <<  "       Size:"<< lSz << " i:" << i << " j:" << j << std::endl;
		  
		    Point* Pt1 = lOldFacetPoints[i];     // sa position va changer
		    Point* Pt2 = getPoint( iBase, lNewPoints, cEdgePoints[i] ); 
		    Point* Pt3 = getPoint( iBase, lNewPoints, cEdgePoints[j] );
		  		  
		    lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
		    lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
		    lFacPtr->addLine( iBase->getNewLine( Pt3, Pt1)  );
		  
		    lPoly->addFacet( lFacPtr );
		  
		    iBase->validEntity( lFacPtr, true );// VALIDATION (a optimiser)

		    lCentralFacPtr->addLine(  iBase->getNewLine(Pt3, Pt2) );
		  }
	      
		lPoly->addFacet( lCentralFacPtr );
		iBase->validEntity( lCentralFacPtr, true );// VALIDATION (a optimiser)		  
	      }    
	    else
	      if( iSubDivType == SubDivFacetType::CENTRAL_FACET_MARGE)
		{		  
		  FacetPtr lCentralFacPtr = iBase->getNewFacet();
		  if( ((int)iSubDivSelect) & ((int) SubDivSelectType::SELECT_CENTRAL))
		    oNewFacet.push_back( lCentralFacPtr );
		  for( size_t i=0; i < lSz ; i++ )
		    {
		      FacetPtr lFacPtr = iBase->getNewFacet();
		      if( ((int)iSubDivSelect) & ((int)SubDivSelectType::SELECT_MARGE))
			  oNewFacet.push_back( lFacPtr );

		      size_t j = (lN+i)%lSz;
		  
		      //	      std::cout <<  "       Size:"<< lSz << " i:" << i << " j:" << j << std::endl;

		      Point* Pt1 = lOldFacetPoints[i];     // sa position va changer
		      Point* Pt2 = getPoint( iBase, lNewPoints, cEdgePoints[i] );

		      
		      // Point3d l3 = (cEdgePoints[i] + cMiddle) / 2;
		      
		      Point3d Dist3 = cMiddle - cEdgePoints[i];
		      Point3d l3 = cEdgePoints[i] + (Dist3 * iMargeFactor);
		      
		      Point* Pt3 =  getPoint( iBase, lNewPoints, l3 );
		      
		      Point3d Dist4 = cMiddle - cEdgePoints[j];
		      Point3d l4 = cEdgePoints[j]+ (Dist4 * iMargeFactor);
		      
		      //   Point3d l4 = (cEdgePoints[j] + cMiddle) /  2;

		      Point* Pt4 =  getPoint( iBase, lNewPoints, l4 );
		      
		      Point* Pt5 = getPoint( iBase, lNewPoints, cEdgePoints[j] );
		  
		  
		      lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
		      lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
		      lFacPtr->addLine( iBase->getNewLine( Pt3, Pt4)  );
		      lFacPtr->addLine( iBase->getNewLine( Pt4, Pt5)  );
		      lFacPtr->addLine( iBase->getNewLine( Pt5, Pt1)  );

		  
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
