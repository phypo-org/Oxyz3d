#ifndef H__EntityVisitorDraw__H
#define H__EntityVisitorDraw__H



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
  struct VisitorDrawPoints : public EntityVisitor {

    ViewProps& cViewProps;
    ObjProps & cObjProps;

    VisitorDrawPoints( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginObject( Object* pObj )   override;
    void execPoint( Point* pPt )           override;
    void execEndObject(  Object* pObj )    override;
  };
  //*********************************************
  struct VisitorDrawLine: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;
		
    VisitorDrawLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginObject( Object* pObj )   override;
    void execBeginLine( Line* pLine )      override;
    void execEndObject(  Object* pObj )    override;	
  };
  //*********************************************
  struct VisitorDrawPolyline: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;

    GLuint cNumLineEnd;
    GLuint cNumLine;
			
    VisitorDrawPolyline( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execBeginLine( Line* pLine )   override;
    void execEndFacet( Facet* pFacet )   override;
  };
  //*********************************************
  struct VisitorDrawFacet: public EntityVisitor{
		
    ViewProps& cViewProps;
    ObjProps & cObjProps;

    GLuint cNumLineEnd;
    GLuint cNumLine;
    bool cNoDraw = false;
			
    VisitorDrawFacet( ViewProps& pViewProps, ObjProps& pObjProps )
      :cViewProps( pViewProps)
      ,cObjProps(pObjProps)
    {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execBeginLine( Line* pLine )   override;
    void execEndFacet( Facet* pFacet )   override;

    virtual void execAfterBegin( Entity* pFacet) {;}

  };
  //*********************************************
  struct VisitorDrawPoly: public VisitorDrawFacet{
		
    VisitorDrawPoly( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawFacet(  pViewProps, pObjProps)
    {;}
    void execBeginPoly( Poly* pPoly )    override;
    void execEndPoly( Poly* pPoly )   override;
    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )   override;

  };
  //*********************************************
  struct VisitorDrawObject: public VisitorDrawPoly{
		
    VisitorDrawObject( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPoly(  pViewProps, pObjProps)
    {;}
    void execBeginObject( Object* pPoly )   override;
    void execEndObject( Object* pPoly )   override;
    void execBeginPoly( Poly* pPoly )   override;
    void execEndPoly( Poly* pPoly )   override;
  };

   //*********************************************
  struct  VisitorDrawObjectLine: public VisitorDrawLine{		
    VisitorDrawObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawLine( pViewProps, pObjProps) {;}
		
    void execBeginObject( Object* pObject )  override;
    void execBeginLine( Line* pLine )   override;
  };
  //*********************************************
}
#endif
