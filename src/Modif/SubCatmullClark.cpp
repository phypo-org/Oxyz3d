#include "Modif.h"



struct CCFacet {
  std::vector<Float3>   cEdgePoints;
  Float3                cMiddle;
  PP3d::FacetPtr        cOldFacet = nullptr;
  std::vector<Float3>   cPoints;
};


struct CCSubParam{
  
  std::vector<Float3>   cPoints;
  std::vector<MyFacet*> cFacets;

};

//-----------------------------------------------
class VisitorMiddleFacet : public EntityVisitor	{

  Point3d  cMiddle;
  size_t   cNbPt=0;

public:  
  void execPoint( Point* pPoint ) override
  {
    cMiddle += pPoint;
    cNbPt++;
  }
  Point3d getMiddle()
  {
    return cMiddle / cNbPt;
  }
};
//-----------------------------------------------
bool SubCatmullClark(  std::set<FacetPtr>&  iOldFacets, std::vector<PointPtr> & iBorderPoints, std::vector<CCFacet> & lNewFacets )
{  
  std::vector<Point3d> lOldPoints;  
  std::set<Point3d> lOldPointsUniq;  

  //===== Pour toutes les facettes à traiter ===== 
  for( FacetPtr lOldFacet : iOldFacets )
    {
      CCFacet lFacDummy;
            
      lFacets.push_back( lFacDummy );
      CCFacet & lFac = *NewlFacets[lFacets.size()-1];

      {
	VisitorMiddleFacet lVidMidFac;
	lOldFacet.execVisitor( lVidMidFac );
      
	// On creer le point du milieu pour chaque facette
	lFac.cMiddle   = lVidMidFac.getMiddle();
	lFac.cOldFacet = lOldFacet;
      }

      // On calcules les points edges pour chaque lignes
      // moyenne des deux middles et des deux points originels
      for( LinePtr lLine : lOldFacet.getLines() )
	{
	  Point3d lPtDummy;
	  lFac.cEdgePoints.push_back( lFacDummy );
	  Point3d & lEdge = *lFac.cEdgePoints[lFac.cEdgePoints.size()-1];
	  
	  lOldPoints.push_back( lLine->first().get() );
	  lOldPointsUniq.insert(  lLine->first().get() );
	  
	  lEdge = lLine->first().get();
	  lEdge += lLine->second().get();
	  lEdge += lFac.cMiddle;

	  Line * lReverse = lLine->getReverseLine();
	  if( lReverse == nullptr )
	    {
	      // si on ne trouve pas de ligne reverse ! (bordure ?)
	      lEdge /= 3;
	      lFac.cEdgeOldFacet.push_back( nullptr );
	      continue;
	    }
	  FacetPtr lEdgeFacet = lReverse->firstOwner();
	  if( lEdgeFacet == nullptr )
	    {
	      // si la ligne reverse n'a pas de facette !
	      lEdge /= 3;
	      lFac.cEdgeOldFacet.push_back( nullptr );
	      continue;
	    }

	  VisitorMiddleFacet lVidMidFac;
	  lEdgeFacet.execVisitor( lVidMidFac );
	  lEdge += lEdgeFacet.cMiddle;
	  lEdge /= 4;

	  if( iOldFacets.find( lVidMidFac ) == iOldFacets.end() )
	    {
	      // La facette ne sera pas transformé !
	      // il va falloir replacer lReverse par deux lignes !
	      std::cout << " Facette non transformé : division" << std::endl;
	      
	      PointPtr lA = lReverse->first();
	      PointPtr lB = lReverse->end();
	      
	      PointPtr lM = Entity.NewPoint( lEdge ) ;
	      lReverse->set( lA, lM );
	      LinePtr  lNewLine = Entity::NewLine(lM, lB);
	      lEdgeFacet->addLineAfter( lReverse, lNewLine );
	      iBorderPoints.push_back( lM ); // en esperant que la fusion marche !
	    }
	}

      // Il faut deplacer les anciens points
      for( PointPtr lPtr : lOldPointsUniq )
	{
	  A FAIRE
	}
 
      // Les points sont calculées pour la facettes   

      
      size_t lSz = lFac.cEdgePoints;
      size_t lN = lSz-1;

      // On cree les nouveaux points
      // Il seront normalement dedoublonnées plus tard !
      for( size_t i=0; i < lSz ; i++ )
	{
	  size_t j = (lN+i)%lSz;

	  lFac.cPoints.push_back( lOldPoints[i] );
	  lFac.cPoints.push_back( lFac.lEdge[j] );
	  lFac.cPoints.NewVect.push_back( lC );
	  lFac.cPoints.lNewVect.push_back( lFac.lEdge[i] );
	}	    
    }

 //  ici toutes les nlles facettes sont crees dans la structure de travail !

  
}
//--------------------------------------------------------


bool SubCatmullClark(  std::set<FacetPtr>&  iOldFacets, DataBase * iBase )
{
  std::vector<CCFacet> lNewFacets;

  // Il y aura peut etre des points en bordure qui seront crée
  std::vector<PointPtr> lNewPointsBorder;

  
  SubCatmullClark( iOldFacets, lNewPointsBorder, iBase );

  std::vector<PointPtr> & lOldPts   = lVisit.cVectPoints;
  std::vector<LinePtr>  & lOldLines = lVisit.cVectLines;
  //  ici toutes les nlles facettes sont crees dans la structure de travail !

  

  PP3d::SortEntityVisitor lVisit;
  for( FacetPtr lOldFacet : iOldFacets )
    {
      lOldFacet->execVisitor( lVisit );
    }	    
   // On va reutiliser les anciens points et les anciennes lignes des facettes et les facettes 	    
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
  for( size_t f=0; f< lNewFacets.size() ; f++ )
    {
      CCFacet & lNewFacet  = *(lNewFacets[f]); // juste pour simplifier le code
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
  

  for( size_t f=0; f< lNewFacets.size() ; f++ ) 
    {
      CCFacet & lNewFacet  = *(lNewFacets[f]); // juste pour simplifier le code


      //FAIRE UN ALLOCATEUR DE FACETTE ET LIGNE ET POLY ... DANS LA BASE
      // ET FAIRE ALLOC FREE  plutot que s'emmerder a chaque fois !
      // ne pas oublier les remove owner

      FacettePtr lFacPtr = iBaseNewFacette();
      
      //forcement 4 lignes
      lFacPtr->addLine( iBaseNewLine( lNewPoints[f*4], lNewPoints[f*4+1] ));
      lFacPtr->addLine( lLine = iBaseNewLine( lNewPoints[f*4+1], lNewPoints[f*4+1+2] ));
      lFacPtr->addLine( lLine = iBaseNewLine( lNewPoints[f*4+2], lNewPoints[f*4+1+3] ));
      lFacPtr->addLine( lLine = iBaseNewLine( lNewPoints[f*4+3], lNewPoints[f*4+1] ));

      lPoly->addFacette( lFacPtr );
    }
  // A DEBUGGER
}
