#ifndef H__SORT_VISITOR__H
#define H__SORT_VISITOR__H


namespace PP3d{

  //*****************************************
  // Visitor qui permet de trier les differentes entity
  // en fonction de leur type
  //*****************************************
  class SortEntityVisitor : public EntityVisitor {

    bool cAcceptDoublon;
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

    std::set<Entity*>      cSetAllEntity; // contient tout
   
    //--------------------------------
    SortEntityVisitor( bool iAcceptDoublon = false)
      :cAcceptDoublon( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void execPoint( Point* pPoint )
    {
      std::cout << '.' << std::flush;
     if( cAcceptDoublon == false )
	{
	  if( cSetPoints.find( pPoint) != cSetPoints.end() )
	    return;
	}
      std::cout << ':' << std::flush;
      cVectPoints.push_back( pPoint );
      cSetPoints.insert( pPoint );
      cSetAllEntity.insert( pPoint );
    }
    //--------------------------------
    virtual void execBeginLine ( Line* pLine)
    {
      std::cout << '-' << std::flush;
     if( cAcceptDoublon == false )
	{
	  if( cSetLines.find( pLine) != cSetLines.end() )
	    return;
	}
      std::cout << '_' << std::flush;
      cVectLines.push_back( pLine );
      cSetLines.insert( pLine );
      cSetAllEntity.insert( pLine );
    }		
    //--------------------------------
    virtual void execBeginFacet( Facet* pFacet )
    {
      std::cout << 'o' << std::flush;
      if( cAcceptDoublon == false )
	{
	  if( cSetFacets.find( pFacet) != cSetFacets.end() )
	    return;
	}
      std::cout << 'O' << std::flush;
      cVectFacets.push_back( pFacet );
      cSetFacets.insert( pFacet );
      cSetAllEntity.insert(pFacet  );
    }		
    //--------------------------------
    virtual void execBeginPoly ( Poly* pPoly )
    {
       std::cout << 'p' << std::flush;
     if( cAcceptDoublon == false )
	{
	  if( cSetPolys.find( pPoly) != cSetPolys.end() )
	    return;
	}
      std::cout << 'P' << std::flush;
      cVectPolys.push_back( pPoly );
      cSetPolys.insert( pPoly );
      cSetAllEntity.insert(pPoly  );
    }		
    //--------------------------------
    virtual void execBeginObject( Object* pObject)
    {
      std::cout << " -obj- " << std::flush;
     if( cAcceptDoublon == false )
	{
	  if( cSetObjects.find( pObject) != cSetObjects.end() )
	    return;
	}
      std::cout << " -OBJ- " << std::flush;
      cVectObjects.push_back( pObject );
      cSetObjects.insert( pObject );
      cSetAllEntity.insert(pObject  );
    }    
  };
  //*****************************************

} // namespace

#endif
