#ifndef H__EntityVisitor__H
#define H__EntityVisitor__H


#include <vector>
#include <set>

#include <unordered_set>
#include "PP3dType.h"



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
		SET &cPoints;  // le set elimne les doublons
		
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
	//*********************************************
	struct VisitorNormalFacet : public EntityVisitor	{
	
		virtual void execBeginFacet( Facet* pFacet )override;
	};
	//*********************************************
	//*********************************************
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
	//**********************************
	struct VisitorDrawPoints : public EntityVisitor {

		ViewProps& cViewProps;
		ObjProps & cObjProps;

		VisitorDrawPoints( ViewProps& pViewProps, ObjProps& pObjProps )
			:cViewProps( pViewProps)
			,cObjProps(pObjProps)
		{;}

		void execBeginObject( Object* pObj )   override;
		void execPoint( Point* pPt )   override;
		void execEndObject(  Object* pObj )   override;
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
			
		VisitorDrawFacet( ViewProps& pViewProps, ObjProps& pObjProps )
			:cViewProps( pViewProps)
			,cObjProps(pObjProps)
		{;}

		void execBeginFacet( Facet* pFacet )   override;
		void execBeginLine( Line* pLine )   override;
		void execEndFacet( Facet* pFacet )   override;
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
	//*********************************************
	//*********************************************
	//*********************************************
	struct VisitorDrawSelectPoints: public VisitorDrawPoints {

		VisitorDrawSelectPoints( ViewProps& pViewProps, ObjProps& pObjProps )
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
	struct  VisitorDrawObjectLine: public VisitorDrawLine{		
		VisitorDrawObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
			:VisitorDrawLine( pViewProps, pObjProps) {;}
		
		void execBeginObject( Object* pObject )  override;
		void execBeginLine( Line* pLine )   override;
	};
	//*********************************************
	struct  VisitorDrawSelectObjectLine: public VisitorDrawObjectLine{		
		VisitorDrawSelectObjectLine( ViewProps& pViewProps, ObjProps& pObjProps )
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
	struct VisitorDrawSelectFacet: public VisitorDrawFacet{
		
			VisitorDrawSelectFacet( ViewProps& pViewProps, ObjProps& pObjProps )
			:VisitorDrawFacet( pViewProps, pObjProps) {;}

		void execBeginFacet( Facet* pFacet )   override;
		void execEndFacet( Facet* pFacet )   override;
	};
	//*********************************************
	struct VisitorDrawSelectPoly: public VisitorDrawPoly{
		
			VisitorDrawSelectPoly( ViewProps& pViewProps, ObjProps& pObjProps )
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
};


#endif
