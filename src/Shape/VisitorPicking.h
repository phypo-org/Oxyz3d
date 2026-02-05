#ifndef H__EntityVisitorPicking__H
#define H__EntityVisitorPicking__H



#include <vector>
#include <set>

#include <unordered_set>
#include "PP3dType.h"
#include "MinMaxBox3d.h"


#include "EntityVisitor.h"


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
  

  //**********************************
  struct VisitorPickingPoints : public EntityVisitor {

    ViewProps& cViewProps;
    ObjProps & cObjProps;

    VisitorPickingPoints( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginObject( Object* pObj )   override;
    void execPoint( Point* pPt )           override;
    void execEndObject(  Object* pObj )    override;
  };
  //*********************************************
  struct VisitorPickingLine: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;
		
    VisitorPickingLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginObject( Object* pObj )   override;
    void execBeginLine( Line* pLine )      override;
    void execEndObject(  Object* pObj )    override;	
  };
  //*********************************************
  struct VisitorPickingPolyline: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;

    GLuint cNumLineEnd;
    GLuint cNumLine;
			
    VisitorPickingPolyline( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execBeginLine( Line* pLine )   override;
    void execEndFacet( Facet* pFacet )   override;
  };
  //*********************************************
  struct VisitorPickingFacet: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;

    GLuint cNumLineEnd;
    GLuint cNumLine;
    bool cNoDraw = false;
			
    VisitorPickingFacet( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execBeginLine( Line* pLine )      override;
    void execEndFacet( Facet* pFacet )     override;

    virtual void execAfterBegin( Entity* pFacet) {;}
   
    static void PickingConcavFacet( Facet* pFacet );
  };
  //*********************************************
  struct VisitorPickingPoly: public VisitorPickingFacet{
		
    VisitorPickingPoly( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorPickingFacet(  pViewProps, pObjProps)
    {;}
    void execBeginPoly( Poly* pPoly )      override;
    void execEndPoly( Poly* pPoly )        override;
    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )     override;

  };
  //*********************************************
  struct VisitorPickingObject: public VisitorPickingPoly{
		
    VisitorPickingObject( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorPickingPoly(  pViewProps, pObjProps)
    {;}
    void execBeginObject( Object* pPoly )   override;
    void execEndObject( Object* pPoly )     override;
    void execBeginPoly( Poly* pPoly )       override;
    void execEndPoly( Poly* pPoly )         override;
  };

   //*********************************************
  
  struct  VisitorPickingObjectLine: public VisitorPickingLine{		
    VisitorPickingObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorPickingLine( pViewProps, pObjProps) {;}
		
    void execBeginObject( Object* pObject )  override;
    void execBeginLine( Line* pLine )        override;
  };
  
  //*********************************************
}
#endif
