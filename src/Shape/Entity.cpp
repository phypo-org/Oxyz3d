#include "Entity.h"
#include "SortVisitor.h"
#include "DataBase.h"

#include <cmath>



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
  void Entity::deleteAllHieracrchy()
  {    
    SortVisitorEntity lVisit(false);  //SortEntityVisitor
    execVisitor( lVisit );

    for( auto lEntity : lVisit.cSetAllEntity )
      delete lEntity;
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
    pVisit.execNode(  this, nullptr );
    pVisit.execNode(  getPoints().first, this );
    pVisit.execEndNode(  getPoints().first, this );
    pVisit.execNode(  getPoints().second, this );
    pVisit.execEndNode(  getPoints().second, this );
    pVisit.execEndNode(  this, nullptr );
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
  Facet* Facet::duplicate( DataBase & lBase )
  {
    FacetPtr lNew = lBase.getNewFacet();
    
    size_t i =0;
    for( size_t i=0; i< cLines.size(); i++ )
      {
	lNew->addLine( lBase.getNewLine( cLines[i]->first(), cLines[i]->second())); 
      }
     return lNew;
   }
  //-------------------------------------
  Point3d Facet::getCenter( )
  {		 	
    PP3d::Point3d  lCenter;
    
    for( LinePtr lLine : cLines )
      {
	lCenter += lLine->first()->get();
	lCenter += lLine->second()->get();
      }
    
   lCenter /= cLines.size()*2;
   
   return lCenter;
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
    pVisit.execNode(  this, nullptr );
    for( LinePtr lLine : getLines() )
      {
	pVisit.execNode( lLine, this );
	lLine->execVisitor( pVisit );
	pVisit.execEndNode( lLine, this );
      }		
    pVisit.execEndNode( this, nullptr );
  }
  //-------------------------------------
  void Facet::closeFacet()
  {
    LinePtr lLine = new Line( cLines[ getNbLines() -1 ]->getSecond(),
			      cLines[0]->getFirst() );
    cLines.push_back( lLine);
  }
  //-------------------------------------
  // il suffit d'un seul angle superieur ou egal a 180 pour que la facette soit concave
  /*
  bool Facet::computeConcave()
  {
  if( cLines.size() <= 3 )
      return false;

    std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;
    size_t i;
    for(  i = 0 ; i<  cLines.size()-1; i++ )
      {
	Point3d A = cLines[i]->second()->get()   - cLines[i]->first()->get();
	Point3d B = cLines[i+1]->second()->get() - cLines[i+1]->first()->get();

	
	std::cout <<  cLines[i]->second()->get() << " - "  cLines[i]->first()->get()
		  << " -> " << A << std::endl;
	
	std::cout <<  cLines[i+1]->second()->get() << " - "  cLines[i+1]->first()->get()
		  << " -> " << B << std::endl;

	double lResult =  Point3d::GetAngleRadBetween( A, B) ;
	
	std::cout << lResult << std::endl;
	if( lResult >  M_PIl)
	  {
	    std::cout << "Facet::computeConcave  CONCAVE !  sz line:" << cLines.size() << "  i:" << i << " : " << lResult << " >>>>> " << M_PIl << std::endl;
	    cIsConcave = true;
	    return true;
	  }
      }
    Point3d A = cLines[i]->second()->get() - cLines[i]->first()->get();
    Point3d B = cLines[0]->second()->get() - cLines[0]->first()->get();
    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
    if( lResult >  M_PIl)
      {
	std::cout << "Facet::computeConcave2   sz line:" << cLines.size() << "  i:" << i << " : " << lResult << " >>>>> " << M_PIl<< std::endl;
	cIsConcave = true;
	return true;
      }
    
    
    cIsConcave = false;
    return false;
  }
  */
  bool Facet::computeConcave()
  {
    if( cLines.size() <= 3 )
      return false;

    //   std::cout << "       Facet::computeConcave " << std::dec << cLines.size()  << std::endl;
    size_t i;
    for(  i = 0 ; i<  cLines.size()-1; i++ )
      {
	Point3d A = cLines[i]->second()->get()   - cLines[i]->first()->get();
	Point3d B = cLines[i+1]->second()->get() - cLines[i+1]->first()->get();

	
	std::cout <<  cLines[i]->second()->get() << " - "  <<cLines[i]->first()->get()
		  << " -> " << A << std::endl;
	
	std::cout <<  cLines[i+1]->second()->get() << " - "  <<cLines[i+1]->first()->get()
		  << " -> " << B << std::endl;

	if( Point3d::isNegAngle( A, B, cNorm )) // On prend la normale comme repere
	  {
	    std::cout << "Facet::computeConcave  CONCAVE ! " << std::endl;
	    cIsConcave = true;
	    return true;
	  }
      }
    Point3d A = cLines[i]->second()->get() - cLines[i]->first()->get();
    Point3d B = cLines[0]->second()->get() - cLines[0]->first()->get();
    double lResult =  Point3d::GetAngleRadBetween( A, B) ;
	if( Point3d::isNegAngle( A, B, cNorm )) // On prend la normale comme repere
	  {
	    std::cout << "Facet::computeConcave  CONCAVE ! " << std::endl;
	    cIsConcave = true;
	    return true;
	  }

    
    cIsConcave = false;
    return false;
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
    pVisit.execNode(  this, nullptr );
    for( FacetPtr lFacet : getFacets() )
      {
	pVisit.execNode( lFacet, this );
	lFacet->execVisitor( pVisit );
	pVisit.execEndNode( lFacet, this );
      }
    pVisit.execEndNode( this, nullptr );
  }
 
	
  //*********************************************
  //*********************************************
}
