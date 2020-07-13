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

struct CCSubParam{
  
  std::vector<Float3>   cPoints;
  std::vector<MyFacet*> cFacets;
};
//-----------------------------------------------
// Calcule les coordonnees du milieu d"une facette
class VisitorComputeMiddleFacet : public EntityVisitor	{

  Point3d  cMiddle;
  size_t   cNbPt=0;

public:  
  void execPoint( Point* pPoint ) override
  {
    cMiddle += pPoint->get();
    cNbPt++;
  }
  Point3d getMiddle()
  {
    return cMiddle / cNbPt;
  }
};
//-----------------------------------------------
// Retrouve le point par sa position, ou en cree un nouveau

PointPtr getPoint( DataBase * iBase, std::vector<PointPtr> &ioPts,  Point3d & iPt )
{
  PointPtr lSearchPt = Modif::FindPointByEpsilon( ioPts, iPt );
  
  if( lSearchPt == nullptr )
    {
      lSearchPt = iBase->getNewPoint(iPt);
      ioPts.push_back( lSearchPt );        // pour un recherche ulterieure
    }
  return lSearchPt;
}
//-----------------------------------------------
// Creation des nlles facettes

bool Modif::SubCatmullClark( DataBase * iBase, std::set<FacetPtr>&  iOldFacets, std::set<PointPtr> &  iOldPoint )
{
  std::vector<PointPtr> lNewPoints;
  std::vector<CCSubDiv> oSubDivs;
  std::vector<PointPtr> iBorderPoints;
  
  //===== Pour toutes les facettes à traiter ===== 
  for( FacetPtr lOldFacet : iOldFacets )
    {
      std::vector<PointPtr>    lOldFacetPoints;
      // std::set<Point3d>    lOldPointsUniq;
      

      // Nlle SubDivision
      CCSubDiv lSubDummy;            
      oSubDivs.push_back( lSubDummy );
      CCSubDiv & lNewSubDiv = oSubDivs[ oSubDivs.size()-1 ];

      
      { // Calcul du milieu de la facette (face point)
	VisitorComputeMiddleFacet lVidMidFac;
	lOldFacet->execVisitor( lVidMidFac );
      
	// On creer le point du milieu pour la facette courante
	lNewSubDiv.cMiddle   = lVidMidFac.getMiddle();
	lNewSubDiv.cOldFacet = lOldFacet;
      }

      // Pour chaque lignes de la facette on calcule les edge-point  pour chaque lignes
      // moyenne des deux face-point et des deux points originels
      for( LinePtr lLine : lOldFacet->getLines() )
	{
	  // Nouveau point edge
	  Point3d lPtDummy;
	  lNewSubDiv.cEdgePoints.push_back( lPtDummy );
	  Point3d & lEdge = lNewSubDiv.cEdgePoints[ lNewSubDiv.cEdgePoints.size()-1 ];

	  // On memorise le premiers points de la ligne
	  lOldFacetPoints.push_back( lLine->first() );
	  //	  lOldPointsUniq.insert(  lLine->first()->get() );	  
	  lEdge = lLine->first()->get();
	  lEdge += lLine->second()->get();
	  lEdge += lNewSubDiv.cMiddle;

	  // Il reste a trouver le milieu de l'autre facette
	  Line * lReverse = lLine->getReverseLine();
	  if( lReverse == nullptr )
	    {
	      // si on ne trouve pas de ligne reverse ! (bordure ?)
	      lEdge /= 3;
	      lNewSubDiv.cEdgePoints.push_back( lEdge );
	      continue;
	    }
	  FacetPtr lEdgeFacet = (FacetPtr)lReverse->firstOwner();
	  if( lEdgeFacet == nullptr )
	    {
	      // si la ligne reverse n'a pas de facette !
	      lEdge /= 3;
	      lNewSubDiv.cEdgePoints.push_back( lEdge );
	      continue;
	    }
	  
	  // On a trouvé la deuxieme facette du point edge
	  VisitorComputeMiddleFacet lVidMidFac;
	  lEdgeFacet->execVisitor( lVidMidFac ); 
	  lEdge += lVidMidFac.getMiddle();
	  lEdge /= 4;
	  lNewSubDiv.cEdgePoints.push_back( lEdge );


	  if( iOldFacets.find( lEdgeFacet ) == iOldFacets.end() )
	    {
	      // La facette reverse n'est pas dans la liste des facette  transformé ! (bordure ?)
	      // il va falloir replacer la ligne lReverse par deux lignes !
	      std::cout << " Facette non transformé : division" << std::endl;
	      
	      PointPtr lA = lReverse->first();
	      PointPtr lB = lReverse->second();
	      
	      PointPtr lM = getPoint( iBase, lNewPoints, lEdge ); // en esperant que la fusions des points marche bien !
	      
	      lReverse->set( lA, lM );
	      LinePtr  lNewLine = iBase->getNewLine(lM, lB);  
	      lEdgeFacet->addLineAfter( lReverse, lNewLine );
	      
	      iBorderPoints.push_back( lM );
	      
	      iBase->validEntity( lEdgeFacet ); // VALIDATION
	    }
	  // else : le Edge point reverse sera creer lors du traitement de la facette adjacente
	  
	} // end edge-point

 
      // Les points sont calculées pour les nlles facettes
      // il y a autant de nlles facettes que de edge-point et elles ont 4 points
      size_t lSz = lNewSubDiv.cEdgePoints.size();
      size_t lN = lSz-1;

      
      // On cree les nouveaux points des nlles facettes (fusion possible via getPoint)
      PolyPtr lPoly = (PolyPtr) lOldFacet->firstOwner();
      if( lPoly )
	{
	  for( size_t i=0; i < lSz ; i++ )
	    {
	      FacetPtr lFacPtr = iBase->getNewFacet();
	      
	      size_t j = (lN+i)%lSz;
	      
	      Point* Pt1 = lOldFacetPoints[i];     // sa position va changer
	      Point* Pt2 = getPoint( iBase, lNewPoints, lNewSubDiv.cEdgePoints[j] ); 
	      Point* Pt3 = getPoint( iBase, lNewPoints, lNewSubDiv.cMiddle); 
	      Point* Pt4 = getPoint( iBase, lNewPoints, lNewSubDiv.cEdgePoints[i] );
	      
	      
	      lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
	      lFacPtr->addLine( iBase->getNewLine( Pt2, Pt3)  );
	      lFacPtr->addLine( iBase->getNewLine( Pt3, Pt4)  );
	      lFacPtr->addLine( iBase->getNewLine( Pt4, Pt1)  );
	      
	      lPoly->addFacet( lFacPtr );
	      
	      iBase->validEntity( lFacPtr );// VALIDATION (a optimiser)	     
	    }
  	}
    } // fin facettes

  
      // Il faut aussi deplacer les anciens points
  
  for( PointPtr lPtr : iOldPoint )
    {
      // AFAIRE
    }

  // On elimine les anciennes facettes
  for( FacetPtr lOldFacet : iOldFacets )
    {
      PolyPtr lPoly = (PolyPtr) lOldFacet->firstOwner();
      if( lPoly )
	{
	  lPoly->removeFacet( lOldFacet );
	}
      iBase->freeFacet( lOldFacet );
    }
  
 //  ici toutes les nouveaux points des nlles facettes sont crees dans la structure de travail !
  return true;
}
//--------------------------------------------------------
/*
bool SubCatmullClark(  std::set<FacetPtr>&  iFacets, DataBase & iBase )
{
	    
  // On va reutiliser les anciens points et les anciennes lignes des facettes et les facettes
  PP3d::SortEntityVisitor lVisit;		    
  PP3d::ExecVisitor<std::set<FacetPtr>>( iFacets , lVisit );
  
  std::vector<PointPtr> & lOldPts   = lVisit.cVectPoints;
  std::vector<LinePtr>  & lOldLines = lVisit.cVectLines;


  std::vector<CCSubDiv> lNewSubDiv;

  // Il y aura peut etre des points en bordure qui seront crée
  std::vector<PointPtr> lNewPointsBorder;
  std::vector<CCSubDiv> lSubDivs;
  
  SubCatmullClark( iFacets, lNewPointsBorder, lSubDivs );

  //  ici toutes les nlles facettes sont crees dans la structure de travail !

  // On enleve des anciens points les liens vers les vielles lignes
  for( PointPtr lOldPt :  lOldPts)
    {
      for( LinePtr lOldLine : lOldLines )
	{
	  lOldPt->removeOwner( lOldLine );
	}
    }

  for( PointPtr lPtr : lNewPointsBorder )
    {
      lOldPt.push_back( lPtr ); 
    }

  // fusion : On essaye de faire reutiliser les points 
  std::vector<PointPtr> lNewPoints( iSubDiv->cPoints.size() );
  
  //=========================
  for( size_t f=0; f< lNewSubDiv.size() ; f++ )
    {
      CCSubDiv & lNewFacet  = *(lNewSubDiv[f]); // juste pour simplifier le code
      for( size_t p =0; p< lNewFacet->cPoints.size(); p++)
	{
	  PointPtr lSearchPt = Modif::FindPointByExactValues( lOldPts, lNewFacet->cPoints[p] );
	  if( lSearchPt != nullptr )
	    {
	      DBG_MODIF( "Find Old ==  " << p  );
	      lNewPoints[p] = lSearchPt; // xyz sont forcement bonnes!
	}
	  else
	    {
	      NE FAUDRAIT IL PAS AUSSI ESSAYER DE FUSIONNER LES NOUVEAUX ? BEN SI !
		
	      lNewPoints[p] = new Point( lNewFacet->cPoints[p] ); 
	      iBase->validOneEntityLevel(lNewPoints[p]);        // on donne un id et on stocke
	    }
	}
    }
  //=========================
  // il y a le bon nombre de d'elements dans lNewPoints et il sont classés
  // dans le bon ordre mais il peut y avoir plusieurs fois le meme
  // pointeur si c'est les meme coordonnees du coup
  

  for( size_t f=0; f< lNewSubDiv.size() ; f++ ) 
    {
      CCSubDiv & lNewFacet  = *(lNewSubDiv[f]); // juste pour simplifier le code


      //FAIRE UN ALLOCATEUR DE FACETTE ET LIGNE ET POLY ... DANS LA BASE
      // ET FAIRE ALLOC FREE  plutot que s'emmerder a chaque fois !
      // ne pas oublier les remove owner

      FacettePtr lFacPtr = iBaseNewFacette();
      
      //forcement 4 lignes
      lFacPtr->addLine( iBase->getNewLine( lNewPoints[f*4], lNewPoints[f*4+1] ));
      lFacPtr->addLine( iBase->getNewLine( lNewPoints[f*4+1], lNewPoints[f*4+1+2] ));
      lFacPtr->addLine( iBase->getNewLine( lNewPoints[f*4+2], lNewPoints[f*4+1+3] ));
      lFacPtr->addLine( iBase->getNewLine( lNewPoints[f*4+3], lNewPoints[f*4+1] ));

      lPoly->addFacette( lFacPtr );
    }
  // A DEBUGGER
}
*/
