#ifndef H__DataBase__H
#define H__DataBase__H


#include <vector>
#include <stack>



#include "Object.h"

#include "PP3dType.h"


#include "Entity.h"

#include "UniqueId.h"
#include "Object.h"
#include "ObjectPolylines.h"
#include "ObjectLine.h"

#include "SubDiv.h"


#include "CurrentInput.h"





namespace PP3d {

  class Kamera;
  class ObjectFacet;
  class UndoHistory;
  class MyRead;

 
  //************************************
  class  DataBase{	 
    static TUniqueId<EntityId, 1>         sUniqueId;
    std::unordered_map<EntityId, Entity*> cEntities;        // contient tout sauf cCurrentLine et cCurrentCreation !
    std::set<Object*>                     cContainerObject; // ne contient que les object
  
    CurrentInput        cCurrent;


    //------- GROUP -------
  private:
    static TUniqueId<EntityId, 1>  sUniqueGroupId;
    PPu::HashSetPtr<GroupObject>   cGroups;
  public:
    void           freeGroup( GroupPtr iGroup );
    GroupPtr newGroup();
    const PPu::HashSetPtr<GroupObject> getGroups() { return cGroups; }
    //------- GROUP -------

    
  public:
    CurrentInput   &    getInput() { return cCurrent;}
    
   private:
    
    Point3d              cCursorPos;


  private:
    static void resetUniqueId(EntityId iResetVal=1) {
      sUniqueId.reset( iResetVal );
    }
	  
  public:
		 
    DataBase();
    virtual ~DataBase();

    void resetIdFromMax(); // for UndoHistory
    
    void demo1();
    void demo2();
    void makeDemo2( float iPos,  int iDepth, float iSize, bool iCentralPoint, SubDiv::SubNormalizeType iNormalize);
    

    //		void add( O3dObj* pObj );
    void addObject( Object* pObj);
    void drawGL(ViewProps& iViewGen , ViewProps& iViewInputCursor, ViewProps& iViewInputPoly, ViewProps& iViewInputObject,  GLMode iSelectOrDrawMode, SelectType iSelectType, ClassType iClassType);
    void recomputeAll( Compute iCompute );
    const std::set<Object*> & getAllObject()  const { return  cContainerObject;}
    const std::unordered_map<EntityId, Entity*>& getEntities() const { return  cEntities;}
		
    void     setCursorPosition( Point3d pPt ) { cCursorPos=pPt; }
    Point3d  getCursorPosition() { return cCursorPos; }
		
 
  private:
    void     addValidEntityForUndo(  Entity* pEntity );
  public:
    EntityId validOneEntityLevel(  Entity* pEntity );
    EntityId validEntity( Entity* lEntitiy, bool iTotalScan = false );
  

    Object* findObjectById( PP3dId pId ) const ;
    Object* findObjectByGLNameId( GLuint pNameId ) const;
		
    //================================
    bool deleteEntity( EntityPtr );
    bool deleteEntityIfVoid( EntityPtr pEntity);
    
    //================================
		
    //		EntityId   addEntity( Entity* pEntity );		
    bool       removeEntityIfNoOwner( EntityId pId );
    bool       removeEntityIfNoOwner( Entity * pEntity  ) { return removeEntityIfNoOwner( pEntity->getId()); }
    
    EntityPtr  findEntity( EntityId pId )
    {
      if( pId == 0 ) return nullptr;
	
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
    template <class CLASSPTR,  auto ENUM>
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

    friend class UndoHistory;
    friend class MyRead;

    void execVisitorOnObject( EntityVisitor& pVisit )
    {
      for( Object* lObj : cContainerObject )
	lObj->execVisitor( pVisit );
    }
    
    void execVisitorOnEntity( EntityVisitor& pVisit )
    {
     for( auto  lPair : cEntities )
	lPair.second->execVisitor( pVisit );
    }

    
    Entity * findHightLightEntity()
    {
     for( auto  lPair : cEntities )
	{
	  if( lPair.second->isHighlight() )
	    return lPair.second;
	}
     return nullptr;
    }
    //---------------------------------

  protected:
    std::stack<PointPtr> cFreePoints;     
    std::stack<LinePtr>  cFreeLines;     
    std::stack<FacetPtr> cFreeFacets;     
    std::stack<PolyPtr>  cFreePolys;     
   //    std::stack<PolyPtr>  cFreePolys;     

  public:
    PointPtr getNewPoint(const Point3d & p);
    LinePtr  getNewLine(PointPtr lA, PointPtr lB);
    FacetPtr getNewFacet();
    PolyPtr  getNewPoly();

    void freePoint( PointPtr ioPt );
    void freeLine( LinePtr   ioLine );
    void freeFacet( FacetPtr ioFacet);
    void freePoly( PolyPtr  ioPoly);
  };
  //************************************
}



#endif
