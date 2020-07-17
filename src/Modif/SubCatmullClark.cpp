#include "Modif.h"


using namespace PP3d;

//-----------------------------------------------

struct CCSubDiv {
  std::vector<Float3>   cEdgePoints;
  Float3                cMiddle;
  PP3d::FacetPtr        cOldFacet = nullptr;
  std::vector<Float3>   cPoints;  
};
//-----------------------------------------------

//-----------------------------------------------
// Creation des nlles facettes

bool Modif::SubCatmullClark( DataBase * iBase, std::set<FacetPtr>&  iOldFacets, std::set<PointPtr> &  iOldPoint, bool iChangeOld )
{
  std::vector<PointPtr> lNewPoints;
  std::vector<CCSubDiv> oSubDivs;
  std::vector<PointPtr> iBorderPoints;

  std::unordered_map<PointPtr, Point3d> lFinalPos;


  //==== Il faudra aussi deplacer les anciens points ====
  // Le calcul est fait içi car ensuite la structure des facettes change

  if( iChangeOld )
  for( PointPtr lPt : iOldPoint )
    {
      Point3d lAvgFacePts;
      Point3d lAvgMidPts;
	  
      int lNbLine=0;
      int lNbFac=0;
      for( EntityPtr lEntity : lPt->getOwners() )
	{
	  if( lEntity->getType() == ShapeType::Line )
	    {
	      // On calcule la moyennes des milieux des lignes
	      // si il y a une ligne reverse le point sera compté deux fois	
	      LinePtr lLine = (LinePtr)lEntity;
 	      Point3d lMiddle = (lLine->first()->get() +  lLine->second()->get())*0.5;
	      
	      lAvgMidPts += lMiddle;
	      lNbLine++;

	      LinePtr lRevLine = lLine->getReverseLine();
	      if( lRevLine == nullptr )
		{ // elle va compter pour deux
		  lAvgMidPts += lMiddle;
		  lNbLine++;			      
		}
	      
	      FacetPtr lFac = (FacetPtr)lLine->firstOwner();
	      if( lFac != nullptr && lFac->getType() == ShapeType::Facet )
		{
		  VisitorComputeMiddleFacet lVidMidFac;
		  lFac->execVisitor( lVidMidFac );
		  lAvgFacePts += lVidMidFac.getMiddle();
		  lNbFac++;
		  if( lRevLine == nullptr )
		    { // elle va compter pour deux ( mais le resultat n'est pas juste non plus)

		      lAvgFacePts += lVidMidFac.getMiddle();
		      lNbFac++;		      
		    }
		}		 
	    }	 
	}
     if( lNbLine && lNbFac )
       {	 
	 lAvgFacePts /=  lNbFac;
	 lAvgMidPts  /=  lNbLine;   
	 lAvgMidPts *= 2;
	 lNbLine /= 2;
	 
	 //	 std::cout << "lNbLine:" << lNbLine << " lNbFac:" << lNbFac << std::endl;
	 
	 int lN3 = lNbLine-3;
	 Point3d lPos = lPt->get()*lN3;

	 if( lN3 > 0 )  //?????????
	   lPos = lPt->get()*lN3;
	 
	 Point3d lNewPos = (lAvgFacePts + lAvgMidPts + lPos)/lNbLine;
	 //	 std::cout << lNewPos << std::endl;

	 lFinalPos.insert( {lPt, lNewPos} );
       }			    
    }
  //=======================================================

  for( PointPtr lPt : iOldPoint )
    {
      lPt->clearAllOwner();  // Il va etre reutilisé !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
  

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

	  
	  lEdge += cMiddle;
	  
	  // Il reste a trouver le milieu de l'autre facette
	  Line * lReverse = lLine->getReverseLine();
	  if( lReverse == nullptr )
	    {
	      // si on ne trouve pas de ligne reverse ! (bordure ?)
	      std::cerr << "Warning Reverse : no reverse " << std::endl;
		lEdge /= 3;
	      //	      cEdgePoints.push_back( lEdge );
	      continue;
	    }
	  FacetPtr lEdgeFacet = (FacetPtr)lReverse->firstOwner();
	  if( lEdgeFacet == nullptr )
	    {
	      std::cerr << "Warning Reverse : no facet" << std::endl;
	      // si la ligne reverse n'a pas de facette !
		lEdge /= 3;
	      //	      cEdgePoints.push_back( lEdge );
	      continue;
	    }
	  
	      // On a trouvé la deuxieme facette du point edge
	      VisitorComputeMiddleFacet lVidMidFac;
	      lEdgeFacet->execVisitor( lVidMidFac ); 
	      lEdge += lVidMidFac.getMiddle();
	      lEdge *= 0.25;

	  
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

 
      // Les points sont calculées pour les nlles facettes
      // il y a autant de nlles facettes que de edge-point et elles ont 4 points
      size_t lSz = cEdgePoints.size();
      size_t lN = lSz-1;

      
      // On cree les nouveaux points des nlles facettes (fusion possible via getPoint)
      PolyPtr lPoly = (PolyPtr) lOldFacet->firstOwner();
      if( lPoly )
	{
	  for( size_t i=0; i < lSz ; i++ )
	    {
	      FacetPtr lFacPtr = iBase->getNewFacet();
		
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
    } // fin facettes


  // on repositionne les anciens points avec les valeurs calcules auparavant
  if( iChangeOld )
  for( auto lPair : lFinalPos )
    {
      lPair.first->get() = lPair.second;
    }
  
  
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
