#include "Modif.h"

#include "Shape/OwnerVisitor.h"

using namespace PP3d;

//-----------------------------------------------
bool Modif::DeletePointFromFacet( DataBase * iBase, PointPtr iPoint , FacetPtr ioFacet, std::vector<PointPtr> &oPoints )
{
  if( ioFacet->getNbLines() <= 3 )
    {
      
      for( LinePtr lLine : ioFacet->getLines())
	{
      // Les points ???
	  
	  lLine->removeFromOwners();
	  iBase->freeLine( lLine );
	}

      ioFacet->removeFromOwners();
      iBase->freeFacet( ioFacet );
      return true;
    }

  std::cout << "**** Modif::DeletePointFromFacet" << std::endl;

  for( GLuint i=0; i< ioFacet->getNbLines()+1; i++ )
    {
      LinePtr lFirstLine = ioFacet->getLineModulo( i );
    
      if( lFirstLine->getSecond() == iPoint )
	{
	  std::cout << "**** Modif::DeletePointFromFacet --> " << i << std::endl;

	  LinePtr lSecondLine = ioFacet->getLineModulo( i+1 );
	  lFirstLine->setSecond( lSecondLine->getSecond() );
	  ioFacet->removeLineModulo( i+1 );	  
	  if( iBase != nullptr )
	    iBase->freeLine( lSecondLine );

	  oPoints.push_back( lFirstLine->getFirst() );
	  break;
	}
    }
  return false;
}
//-----------------------------------------------
bool Modif::DeletePoint( DataBase * iBase, PointPtr iPoint, bool iCreateFacet )
{
  // Trouver ou il est utilisé !
  PP3d::OwnerEntityVisitor lVisitPtOwn;
  
  lVisitPtOwn.addOwnersOf( iPoint );

  PP3d::OwnerEntityVisitor lVisitLineOwner;
  std::vector<PointPtr> lPointsNewFacet;
  
  for( LinePtr lLine : lVisitPtOwn.cVectLines )
    {
    std::cout << "****  Modif::DeletePoints line " << std::endl;
      lVisitLineOwner.addOwnersOf( lLine );
    }
  PolyPtr lPoly = nullptr;
  for( FacetPtr lFacet : lVisitLineOwner.cVectFacets )
    {
      if( DeletePointFromFacet( iBase, iPoint, lFacet, lPointsNewFacet ) == true )
	{
	  // il faut detruire la facette !
	}
      lPoly = (PolyPtr) lFacet->firstOwner();
    }
  if( iCreateFacet
      && lPointsNewFacet.size() >= 3
      && lPoly && lPoly->getType() == ShapeType::Poly )
    {
      size_t lSz = lPointsNewFacet.size();
     
      FacetPtr lFacPtr = iBase->getNewFacet();
      for( size_t i=0; i < lSz-1 ; i++ )
	{	  
	  PointPtr Pt1 = lPointsNewFacet[i];    
	  PointPtr Pt2 = lPointsNewFacet[i+1];     
	  
	  lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
	}
      PointPtr Pt1 = lPointsNewFacet[lSz-1];    
      PointPtr Pt2 = lPointsNewFacet[0];           
      lFacPtr->addLine( iBase->getNewLine( Pt1, Pt2)  );
      lPoly->addFacet( lFacPtr );
				    
      iBase->validEntity( lFacPtr, true );// VALIDATION (a optimiser)	      
    }
  return true;
}
//-----------------------------------------------
bool Modif::DeletePoints( DataBase * iBase, std::vector<PointPtr>& iPoints, bool iCreateFacet )
{
  for( PointPtr lPoint : iPoints )
    {
    std::cout << "**** CALL Modif::DeletePoints" << std::endl;
    
    if( DeletePoint( iBase, lPoint, iCreateFacet) )
	{
	  if( iBase != nullptr )
	    iBase->freePoint( lPoint );
	}
    }

  return true;
}

//-----------------------------------------------
