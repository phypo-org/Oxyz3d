#ifndef H__EntityVisitor__H
#define H__EntityVisitor__H


#include <vector>
#include <set>

#include <unordered_set>
#include "PP3dType.h"
#include "MinMaxBox3d.h"



namespace PP3d{
	
  class Entity;
  class Point;
  class Line;
  class Facet;
  class Poly;
  class Object;
  class ViewProps;
  struct ObjProps;
  class Object;

  // Faire enventuellemnt d'autre type de visiteurs
  //**********************************
  struct EntityVisitorNode{
    virtual void execNode    ( Entity* pEntity, Entity* pOwner ){;}
    virtual void execEndNode ( Entity* pEntity, Entity* pOwner ){;}
  };								 
  //**********************************
  struct EntityVisitor{
			
    virtual void execPoint( Point* pPoint ){;}

    virtual void execBeginLine ( Line* pLine){;}
    virtual void execEndLine ( Line* pLine){;}
		
    virtual void execBeginFacet( Facet* pFacet ){;}
    virtual void execEndFacet( Facet* pFacet ){;}
		
    virtual void execBeginPoly ( Poly* pPoly ){;}
    virtual void execEndPoly ( Poly* pPoly ){;}
		
    virtual void execBeginObject( Object* ) {;}
    virtual void execEndObject( Object* ) {;}
  };								 
							 
  //**********************************
  template <class SET>
  class VisitorGetPoints : public EntityVisitor	{

    SET cInternalSet;
    SET &cPoints;  // le set elimne les doublons mais ils ne sont plus dans l'ordre !
		
  public:
    SET &getPoints() { return cPoints;}

    VisitorGetPoints( SET &pPoints)
      :cPoints( pPoints ) {;}
		
    VisitorGetPoints()
      :cPoints( cInternalSet ) {;}		 

    // FAIRE PLUS MALIN ! notament pour les facettes
    void execPoint( Point* pPoint ) override
    {
      cPoints.insert( pPoint );
    }
  };

  template <class SET_ENTITY, class SET_PT>
  void GetPoints( const SET_ENTITY & pEntities, SET_PT & pPoints)
  {
    VisitorGetPoints<SET_PT> lVisit( pPoints );
    for( auto lIter = pEntities.cbegin(); lIter != pEntities.cend(); ++lIter) 
      {
	(*lIter)->execVisitor( lVisit );
      }
  }
  //**********************************
	
  class VisitorTracePoints : public EntityVisitor	{

  public:
    // FAIRE PLUS MALIN ! notament pour les facettes
    void execPoint( Point* pPoint ) override
    {
      std::cout << (void*) pPoint << std::endl;
    }
  }; 
  //**********************************
	
  class VisitorMinMax : public EntityVisitor, public MinMaxBox3d {
  public:    
    void execPoint( Point* pPoint ) override;
  }; 
  //*********************************************
  // calcule et stocke la normale pour chaque facette
  struct VisitorNormalFacet : public EntityVisitor	{
	  
    virtual void execBeginFacet( Facet* pFacet )override;
  };
  //*********************************************
  // calcule et stocke la normale pour chaque facette
  struct VisitorNoNormFacet : public EntityVisitor	{
	  
    virtual void execBeginFacet( Facet* pFacet )override;
  };
  //*********************************************
  // calcule et stocke la normale pour chaque facette
  struct VisitorInvertNormal : public EntityVisitor	{
	  
    virtual void execBeginLine( Line* pLine ) override;
  };
  //*********************************************
  // calcule et stocke la normale pour chaque facette
  struct VisitorConcaveFacet : public EntityVisitor	{
  protected:
    int cCpt=0;
  public:
    virtual void execBeginFacet( Facet* pFacet )override;
    int count() { return cCpt; }
  };
  //*********************************************
  struct VisitorTrace : public EntityVisitor{

    std::ostream & cOs;
		
    VisitorTrace( std::ostream & pOs ) :cOs(pOs) {;}
		
    void execPoint( Point* pPoint )  override;

    void execBeginLine ( Line* pLine)  override;
		
    void execBeginFacet( Facet* pFacet )  override;
		
    void execBeginPoly ( Poly* pPoly )  override;
		
    void execBeginObject( Object* )  override;
  };								 
  //*********************************************
  
}
#endif
