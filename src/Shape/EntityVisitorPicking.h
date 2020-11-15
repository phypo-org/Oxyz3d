#ifndef H__EntityVisitorPicking__H
#define H__EntityVisitorPicking__H


#include <vector>
#include <set>

#include <unordered_set>
#include "PP3dType.h"
#include "MinMaxBox3d.h"
#include "Entity.h"


#include "EntityVisitorDraw.h"

namespace PP3d{

  
  //*********************************************
  struct VisitorDrawSelectPoints: public VisitorDrawPoints {

    VisitorDrawSelectPoints( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPoints( pViewProps, pObjProps) {;}
		
    void execPoint( Point* pPt )   override;
  };
  //*********************************************
  struct VisitorDrawSelectColorPoints: public VisitorDrawPoints {

    VisitorDrawSelectColorPoints( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPoints( pViewProps, pObjProps) {;}
		
    void execPoint( Point* pPt )   override;
  };
  //*********************************************
  struct  VisitorDrawSelectLine: public VisitorDrawLine{		
    VisitorDrawSelectLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawLine( pViewProps, pObjProps) {;}
		
    void execBeginLine( Line* pLine ) override;
  };
  //*********************************************
  struct  VisitorDrawSelectColorLine: public VisitorDrawLine{		
    VisitorDrawSelectColorLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawLine( pViewProps, pObjProps) {;}
		
    void execBeginLine( Line* pLine ) override;
  };

  //*********************************************
  struct  VisitorDrawSelectObjectLine: public VisitorDrawObjectLine{		
    VisitorDrawSelectObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawObjectLine( pViewProps, pObjProps) {;}
		
    void execBeginObject( Object* pObject )  override;
  };
  //*********************************************
  struct  VisitorDrawSelectColorObjectLine: public VisitorDrawObjectLine{		
    VisitorDrawSelectColorObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawObjectLine( pViewProps, pObjProps) {;}
		
    void execBeginObject( Object* pObject )  override;
  };
  //*********************************************
  struct VisitorDrawSelectPolyline: public VisitorDrawPolyline{
		
    VisitorDrawSelectPolyline( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPolyline( pViewProps, pObjProps) {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )   override;
  };

  
  //*********************************************
  struct VisitorDrawSelectColorPolyline: public VisitorDrawPolyline{
		
    VisitorDrawSelectColorPolyline( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPolyline( pViewProps, pObjProps) {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )   override;
  };
  //*********************************************
  struct VisitorDrawSelectFacet: public VisitorDrawFacet{
		
    VisitorDrawSelectFacet( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawFacet( pViewProps, pObjProps) {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )   override;
  };
  //*********************************************
  struct VisitorDrawSelectColorFacet: public VisitorDrawFacet{
		
    VisitorDrawSelectColorFacet( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawFacet( pViewProps, pObjProps) {;}

    void execBeginFacet( Facet* pFacet )   override;
    void execEndFacet( Facet* pFacet )   override;
    void execAfterBegin( Entity* pFacet ) override;
  };
  //*********************************************
  struct VisitorDrawSelectPoly: public VisitorDrawPoly{
		
    VisitorDrawSelectPoly( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPoly( pViewProps, pObjProps) {;}

    void execBeginPoly( Poly* pPoly )   override;
    void execEndPoly( Poly* pPoly )   override;
  };
  //*********************************************
  struct VisitorDrawSelectColorPoly: public VisitorDrawPoly{
		
    VisitorDrawSelectColorPoly( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawPoly( pViewProps, pObjProps) {;}

    void execBeginPoly( Poly* pPoly )   override;
    void execEndPoly( Poly* pPoly )   override;
  };
  //*********************************************
  struct VisitorDrawSelectObject: public VisitorDrawObject{
		
    VisitorDrawSelectObject( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawObject( pViewProps, pObjProps) {;}

    void execBeginObject( Object* pObject )   override;
    void execEndObject( Object* pObject )   override;
  };
  //*********************************************
  struct VisitorDrawSelectColorObject: public VisitorDrawObject{

    EntityId cId=0;
    VisitorDrawSelectColorObject( ViewProps& pViewProps, ObjProps& pObjProps )
      :VisitorDrawObject( pViewProps, pObjProps) {;}

    void execBeginObject( Object* pObject )  override;
    void execEndObject( Object* pObject )   override;
    void execAfterBegin( Entity* pFacet )   override;
  };
  //*********************************************
   
}
#endif
