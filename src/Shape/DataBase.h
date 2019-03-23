#ifndef H__DataBase__H
#define H__DataBase__H


#include <vector>



#include "Object.h"

#include "PP3dType.h"


#include "Entity.h"

#include "UniqueId.h"
#include "Object.h"
#include "ObjectPolylines.h"
#include "ObjectLine.h"

#include "SubDiv.h"

namespace PP3d {

	class Kamera;
	class ObjectFacet;
	
	//************************************
	class  DataBase{

		static TUniqueId<EntityId, 1>         sUniqueId;
		std::unordered_map<EntityId, Entity*> cEntities;
		std::set<Object*>                     cContainerObject;

		Object               *cCurrentCreation=nullptr; 
		ObjectPolylines      *cCurrentLine=nullptr;

		Point3d              cCursor;
		
		MinMaxBox3d          cRectSelect;

	public:
		 
		DataBase();
		virtual ~DataBase();

		void demo1();
		void demo2();
		void makeDemo2( float iPos,  int iDepth, float iSize, bool iCentralPoint, SubDiv::SubNormalizeType iNormalize);


		//		void add( O3dObj* pObj );
		void addObject( Object* pObj );
		void drawGL(ViewProps& pView,  GLMode pSelectOrDrawMode );
		void recomputeAll();
		const std::set<Object*> & getAllObject()  const { return  cContainerObject;}
		const std::unordered_map<EntityId, Entity*>& getEntities() const { return  cEntities;}

		Object* validCurrentCreation();
		void      cancelCurrentCreation( );
		void      swapCurrentCreation( Object* pCurrentCreation );

		
		void setCursorPosition( Point3d pPt ) { cCursor=pPt; }
		Point3d getCursorPosition() { return cCursor; }
		
		void setSelectionRectanglePosition(  Point3d A, Point3d B)
		{
			cRectSelect.set( A, B);			
		}
		MinMaxBox3d getSelectionRectanglePosition()
		{
			return cRectSelect;
		}

		EntityId validOneEntityLevel(  Entity* pEntity );
		EntityId validEntity( Entity* lEntitiy );

		
		void          addPointToCurrentLine( Point3d pPt );
		void          delPointToCurrentLine( );
		GLuint        getNbCurrentPoints();
		ObjectFacet*     convertCurrentLineToFacet();
		ObjectPolylines* convertCurrentLineToPolylines();
		ObjectLine*      convertCurrentLineToLine();

		bool isCurrentPoints()    { return cCurrentLine     != nullptr; }
		bool iscCurrentCreation() { return cCurrentCreation != nullptr; }
		FacetPtr getCurrentLine()
		{
			if( cCurrentLine != nullptr)
				return  cCurrentLine->getFacet();
			return nullptr;
		}


		Object* findObjectById( PP3dId pId ) const ;
		Object* findObjectByGLNameId( GLuint pNameId ) const;
		
		//================================
		bool deleteEntity( EntityPtr );
		void addToInput( EntityPtr, bool pFlagLink );				
		//================================
		
		//		EntityId   addEntity( Entity* pEntity );		
		bool       removeEntityIfNoOwner( EntityId pId );
		bool       removeEntityIfNoOwner( Entity * pEntity  ) { return removeEntityIfNoOwner( pEntity->getId()); }
		EntityPtr  findEntity( EntityId pId )
		{
			auto lIter = cEntities.find( pId );
			if( lIter == cEntities.end() )
				return nullptr;
			return lIter->second;
		}
		
		EntityId     addPolyAndComponents( Poly* pPoly );

		
		
		//================================
	
		PointPtr  findPoint( EntityId pId )
		{
			EntityPtr lPt = findEntity( pId );
			if( lPt != nullptr
					&& lPt->getType() ==  ShapeType::Point )
				{
					return static_cast<PointPtr>( lPt) ;
				}
			return nullptr;																							 				
		}
		//================================
		template <class CLASSPTR, auto ENUM>
		CLASSPTR  Tfind( EntityId pId )
		{
			EntityPtr lPt = findEntity( pId );
			if( lPt != nullptr
					&& lPt->getType() == ENUM )
				{
					return static_cast<CLASSPTR>( lPt);
				}
			return nullptr;																							 				
		}
	};
	//************************************
};



#endif
