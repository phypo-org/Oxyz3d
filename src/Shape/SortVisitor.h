#ifndef H__SORT_VISITOR__H
#define H__SORT_VISITOR__H

#include "Object.h"

#include <vector>
#include <set>
#include <iostream>

namespace PP3d{

  //*****************************************
  // Visitor qui permet de trier les differentes entity
  // en fonction de leur type
  //*****************************************
  class SortEntityVisitor : public EntityVisitor {

    bool cAcceptDoublon = false;
  public:
    std::vector<PointPtr>  cVectPoints;
    std::set<PointPtr>     cSetPoints;
 
    std::vector<LinePtr>   cVectLines;
    std::set<LinePtr>      cSetLines;
    
    std::vector<FacetPtr>  cVectFacets;
    std::set<FacetPtr>     cSetFacets;
    
    std::vector<PolyPtr>   cVectPolys;
    std::set<PolyPtr>      cSetPolys;

    std::vector<ObjectPtr> cVectObjects;
    std::set<ObjectPtr>    cSetObjects;

    std::vector<Entity*>   cVectAllEntity; // contient tout
    std::set<Entity*>      cSetAllEntity; // contient tout

    void clear(){
      cVectPoints.clear();
      cSetPoints.clear();
      cVectLines.clear();
      cSetLines.clear();
      cVectFacets.clear();
      cSetFacets.clear();
      cVectPolys.clear();
      cSetPolys.clear();
      cVectObjects.clear();
      cSetObjects.clear();
      cVectAllEntity.clear();
      cSetAllEntity.clear();
    }
   
    //--------------------------------
    friend std::ostream & operator << (  std::ostream & iOs, SortEntityVisitor & iVisit )
    {
      return iVisit.trace( iOs );
    }
    //--------------------------------
    SortEntityVisitor( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      //std::cout << '.' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPoints.find( pPoint) != cSetPoints.end() )
	    return;
	}
      //std::cout << ':' << std::flush;
      cVectPoints.push_back( pPoint );
      cSetPoints.insert( pPoint );
      cVectAllEntity.push_back( pPoint );
      cSetAllEntity.insert( pPoint );
    }
    //--------------------------------
    virtual void execBeginLine ( Line* pLine)
    {
      //std::cout << '-' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetLines.find( pLine) != cSetLines.end() )
	    return;
	}
      //std::cout << '_' << std::flush;
      cVectLines.push_back( pLine );
      cSetLines.insert( pLine );
      cVectAllEntity.push_back( pLine );
      cSetAllEntity.insert( pLine );
    }		
    //--------------------------------
    virtual void execBeginFacet( Facet* pFacet )
    {
      //std::cout << 'o' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetFacets.find( pFacet) != cSetFacets.end() )
	    return;
	}
      //std::cout << 'O' << std::flush;
      cVectFacets.push_back( pFacet );
      cSetFacets.insert( pFacet );
      cVectAllEntity.push_back(pFacet  );
      cSetAllEntity.insert(pFacet  );
    }		
    //--------------------------------
    virtual void execBeginPoly ( Poly* pPoly )
    {
      //std::cout << 'p' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPolys.find( pPoly) != cSetPolys.end() )
	    return;
	}
      //std::cout << 'P' << std::flush;
      cVectPolys.push_back( pPoly );
      cSetPolys.insert( pPoly );
      cVectAllEntity.push_back(pPoly  );
      cSetAllEntity.insert(pPoly  );
    }		
    //--------------------------------
    virtual void execBeginObject( Object* pObject)
    {
      //std::cout << " -obj- " << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetObjects.find( pObject) != cSetObjects.end() )
	    return;
	}
      //std::cout << " -OBJ- " << std::flush;
      cVectObjects.push_back( pObject );
      cSetObjects.insert( pObject );
      cVectAllEntity.push_back( pObject  );
      cSetAllEntity.insert(pObject  );
    }

 
    std::ostream & trace( std::ostream & iOs ){
      iOs
	<< "Points\t:"  << cVectPoints.size()   << "\t" << cSetPoints.size()    << "\t" 
	<< "Lines\t:"   << cVectLines.size()    << "\t" << cSetLines.size()     << "\t"
	<< "Facets\t:"  << cVectFacets.size()   << "\t" << cSetFacets.size()    << "\t"
	<< " Polys\t:"   << cVectPolys.size()    << "\t" << cSetPolys.size()     << "\t"
	<< "Objects\t:" << cVectObjects.size()  << "\t" << cSetObjects.size()   << "\t"
	<< "Entity\t:"  << cVectAllEntity.size()<< "\t" << cSetAllEntity.size() << "\t"
	;
      return iOs;
    }
  };
  //*****************************************
  //*****************************************
  //*****************************************
  // meme chose mais Que pour les points
  class SortEntityVisitorPoint : public EntityVisitor {

    bool cAcceptDoublon;
  public:
    std::vector<PointPtr>  cVectPoints;
    std::set<PointPtr>     cSetPoints;
   
    //--------------------------------
    SortEntityVisitorPoint( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      //std::cout << '.' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPoints.find( pPoint) != cSetPoints.end() )
	    return;
	}
      //std::cout << ':' << std::flush;
      cVectPoints.push_back( pPoint );
      cSetPoints.insert( pPoint );
    }
  };
  //*****************************************
  //*****************************************
  //*****************************************
  // meme chose mais Que pour les points et lignes
  class SortEntityVisitorPointLine : public EntityVisitor {

    bool cAcceptDoublon;
  public:
    std::vector<PointPtr>  cVectPoints;
    std::set<PointPtr>     cSetPoints;
 
    std::vector<LinePtr>   cVectLines;
    std::set<LinePtr>      cSetLines;
       
    //--------------------------------
    SortEntityVisitorPointLine( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      //std::cout << '.' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPoints.find( pPoint) != cSetPoints.end() )
	    return;
	}
      //std::cout << ':' << std::flush;
      cVectPoints.push_back( pPoint );
      cSetPoints.insert( pPoint );
    }
    //--------------------------------
    virtual void execBeginLine ( Line* pLine)
    {
      //std::cout << '-' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetLines.find( pLine) != cSetLines.end() )
	    return;
	}
      //std::cout << '_' << std::flush;
      cVectLines.push_back( pLine );
      cSetLines.insert( pLine );
    }		
  };
  //*****************************************
  //*****************************************
  //*****************************************
  // meme chose mais Que pour les points et lignes
  class SortEntityVisitorLine : public EntityVisitor {

    bool cAcceptDoublon;
  public:
 
    std::vector<LinePtr>   cVectLines;
    std::set<LinePtr>      cSetLines;
       
    //--------------------------------
    SortEntityVisitorLine( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execBeginLine ( Line* pLine)
    {
      //std::cout << '-' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetLines.find( pLine) != cSetLines.end() )
	    return;
	}
      //std::cout << '_' << std::flush;
      cVectLines.push_back( pLine );
      cSetLines.insert( pLine );
    }		
  };
  //*****************************************
  //*****************************************
  //*****************************************
  class SortEntityVisitorPointFacet : public EntityVisitor {

    bool cAcceptDoublon;
  public:
    std::vector<PointPtr>  cVectPoints;
    std::set<PointPtr>     cSetPoints;
     
    std::vector<FacetPtr>  cVectFacets;
    std::set<FacetPtr>     cSetFacets;
    
    //--------------------------------
    SortEntityVisitorPointFacet( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      //std::cout << '.' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPoints.find( pPoint) != cSetPoints.end() )
	    return;
	}
      //std::cout << ':' << std::flush;
      cVectPoints.push_back( pPoint );
      cSetPoints.insert( pPoint );
    }
    //--------------------------------
    virtual void execBeginFacet( Facet* pFacet )
    {
      //std::cout << 'o' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetFacets.find( pFacet) != cSetFacets.end() )
	    return;
	}
      //std::cout << 'O' << std::flush;
      cVectFacets.push_back( pFacet );
      cSetFacets.insert( pFacet );
    }		
  };
  //*****************************************
  //*****************************************
  //*****************************************
  class SortEntityVisitorFacet : public EntityVisitor {

    bool cAcceptDoublon;
  public:
   std::vector<FacetPtr>  cVectFacets;
    std::set<FacetPtr>     cSetFacets;
    
    //--------------------------------
    SortEntityVisitorFacet( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execBeginFacet( Facet* pFacet )
    {
      //std::cout << 'o' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetFacets.find( pFacet) != cSetFacets.end() )
	    return;
	}
      //std::cout << 'O' << std::flush;
      cVectFacets.push_back( pFacet );
      cSetFacets.insert( pFacet );
    }		
  };
  //*****************************************
  class SortEntityVisitorPoly : public EntityVisitor {

    bool cAcceptDoublon;
  public:
    std::vector<PolyPtr>   cVectPolys;
    std::set<PolyPtr>      cSetPolys;
    
    //--------------------------------
    SortEntityVisitorPoly ( bool iAcceptDoublon = false )
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execBeginPoly ( Poly* pPoly )
    {
      //std::cout << 'p' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetPolys.find( pPoly) != cSetPolys.end() )
	    return;
	}
      //std::cout << 'P' << std::flush;
      cVectPolys.push_back( pPoly );
      cSetPolys.insert( pPoly );
    }				
  };
  //*****************************************  
  class SortEntityVisitorObject : public EntityVisitor {

    bool cAcceptDoublon;
  public:

    std::vector<ObjectPtr> cVectObjects;
    std::set<ObjectPtr>    cSetObjects;
   
    //--------------------------------
    SortEntityVisitorObject( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execBeginObject( Object* pObject)
    {
      //std::cout << " -obj- " << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetObjects.find( pObject) != cSetObjects.end() )
	    return;
	}
      //std::cout << " -OBJ- " << std::flush;
      cVectObjects.push_back( pObject );
      cSetObjects.insert( pObject );
    }    
  };
  //*****************************************
  //*****************************************
  //*****************************************
  class SortVisitorEntity : public EntityVisitor {

    bool cAcceptDoublon;
  public:

    std::vector<Entity*>   cVectAllEntity; // contient tout
    std::set<Entity*>      cSetAllEntity; // contient tout
   
    //--------------------------------
    SortVisitorEntity( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      //std::cout << '.' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetAllEntity.find( pPoint) != cSetAllEntity.end() )
	    return;
	}
      //std::cout << ':' << std::flush;
      cSetAllEntity.insert( pPoint );
    }
    //--------------------------------
    virtual void execBeginLine ( Line* pLine)
    {
      //std::cout << '-' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetAllEntity.find( pLine) != cSetAllEntity.end() )
	    return;
	}
      //std::cout << '_' << std::flush;
      cVectAllEntity.push_back( pLine );
      cSetAllEntity.insert( pLine );
    }		
    //--------------------------------
    virtual void execBeginFacet( Facet* pFacet )
    {
      //std::cout << 'o' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetAllEntity.find( pFacet) != cSetAllEntity.end() )
	    return;
	}
      //std::cout << 'O' << std::flush;
      cVectAllEntity.push_back(pFacet  );
      cSetAllEntity.insert(pFacet  );
    }		
    //--------------------------------
    virtual void execBeginPoly ( Poly* pPoly )
    {
      //std::cout << 'p' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetAllEntity.find( pPoly) != cSetAllEntity.end() )
	    return;
	}
      //std::cout << 'P' << std::flush;
      cVectAllEntity.push_back(pPoly  );
      cSetAllEntity.insert(pPoly  );
    }		
    //--------------------------------
    virtual void execBeginObject( Object* pObject)
    {
      //std::cout << " -obj- " << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetAllEntity.find(pObject) != cSetAllEntity.end() )
	    return;
	}
      //std::cout << " -OBJ- " << std::flush;
      cVectAllEntity.push_back( pObject  );
      cSetAllEntity.insert(pObject  );
    }    
  };
  //*****************************************

} // namespace

#endif
