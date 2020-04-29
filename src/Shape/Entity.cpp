#include "Entity.h"



namespace PP3d {
  //*********************************************
  Entity::~Entity()
  {
    // Normalement les liens entre Entity lors de la destruction
    // seront gérés par un Visiteur, 
    // l'entité n'est detruite que quand sont elle n'a plus d'owner

    //    if( howManyOwner() != 0 )
    //      std::cout << "*** ERROR : Entity destroy when they are owner" << std::endl;
  }	
  //---------------------------
  void Entity::move ( Point3d p )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt += p;
      }
  }
  //---------------------------
  void Entity::scale ( Point3d p )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt *= p;
      }
  }
  //---------------------------
  void Entity::modify (  Mat4& pMat )
  {
    VisitorGetPoints<PointPtrSet> lVisit;
    execVisitor( lVisit );

    for( PointPtr lPt : lVisit.getPoints() )
      {
	lPt->cPt *= pMat;
      }
  }

  //---------------------------
  /*
    Object* Entity::FindMyObject( EntityPtr pCurrent )
    {
    while( pCurrent != nullptr )
    {
    if( lCurrent->getType() == ShapeType::Object )
    {
    return static_cast<Object*>(lCurrent);
    }
				
    if( lCurrent->cOwners.size() == 0 )
    lCurrent =  nullptr;
    else
    lCurrent = lCurrent->cOwner[0];
    }
    return nullptr;
    }
  */
  //*********************************************
  void Point::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execPoint( this );
  }
  //-------------------------------------
  // direct call 
  void Point::execVisitor( EntityVisitorNode& pVisit )
  {
  }
  //*********************************************
  void Line::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginLine( this );		
    pVisit.execPoint( getPoints().first );
    pVisit.execPoint( getPoints().second );
    pVisit.execEndLine( this );		
  }
  //-------------------------------------	
  void Line::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode(  getPoints().first, this );
    pVisit.execEndNode(  getPoints().first, this );
    pVisit.execNode(  getPoints().second, this );
    pVisit.execEndNode(  getPoints().second, this );
  }
  //--------------------------------
  LinePtr Line::getReverseLine() {
    
    //    std::cout << "Line::getReverseLine         " << cPoints.first  << " : " << cPoints.second << std::endl;
    
    PointPtrPair lReverse =   Reverse( cPoints);
    //    std::cout << "Line::getReverseLine Reverse " << lReverse.first  << " : " << lReverse.second << std::endl;
    
    
    FindPointPairVisitor lVisit( lReverse );

    
    if( getOwners().size() )
      {
	Entity *lEntity =  *getOwners().begin();
	ExecVisitor( lEntity->getOwners(), lVisit );
	if( lVisit.getLines().size() )
	  {
	    return *(lVisit.getLines().begin());
	  }
      }
    return nullptr;
  }		

  //*********************************************
  void Facet::inverseLines( )
  {		
    GLuint lNbDemi  = (GLuint)cLines.size()/2;
    GLuint lLastPos = (GLuint)cLines.size()-1;
    // si nombre impair, le milieu ne change pas !
    for( GLuint l=0; l< lNbDemi; l++)
      {
	LinePtr lTmp = cLines[l];
	cLines[l] = cLines[lLastPos-l];
	cLines[lLastPos-l] = lTmp;
      }
    // On inverse aussi les points dans les lignes !
    for( LinePtr lLine : cLines )
      {
	lLine->inversePoint();
      }		
  }
  //-------------------------------------
  void Facet::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginFacet( this );
	
    for( LinePtr lLine : getLines() )
      {
	lLine->execVisitor( pVisit );
      }
		
    pVisit.execEndFacet( this );
  }
  //-------------------------------------
  void Facet::execVisitor( EntityVisitorNode& pVisit )
  {		
    for( LinePtr lLine : getLines() )
      {
	pVisit.execNode( lLine, this );
	lLine->execVisitor( pVisit );
	pVisit.execEndNode( lLine, this );
      }		
  }
  //-------------------------------------
  void Facet::closeFacet()
  {
    LinePtr lLine = new Line( cLines[ getNbLines() -1 ]->getSecond(),
			      cLines[0]->getFirst() );
    cLines.push_back( lLine);
  }
  //*********************************************
  void Poly::execVisitor( EntityVisitor& pVisit )
  {
    pVisit.execBeginPoly( this );
		
    for( FacetPtr lFacet : getFacets() )
      {
	lFacet->execVisitor( pVisit );
      }
    pVisit.execEndPoly( this );
  }
  //-------------------------------------
  void Poly::execVisitor( EntityVisitorNode& pVisit )
  {
    for( FacetPtr lFacet : getFacets() )
      {
	pVisit.execNode( lFacet, this );
	lFacet->execVisitor( pVisit );
	pVisit.execEndNode( lFacet, this );
      }
  }
	
  //*********************************************
  //*********************************************
}
