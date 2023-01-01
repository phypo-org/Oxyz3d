#ifndef H__OWNER_VISITOR__H
#define H__OWNER_VISITOR__H

#include "SortVisitor.h"

namespace PP3d{

  //*****************************************
  // Visitor qui permet de trier les differentes entity
  // en fonction de leur type
  //*****************************************
  class OwnerEntityVisitor  : public SortEntityVisitor {

  public:
    //--------------------------------
    OwnerEntityVisitor( bool iAcceptDoublon = false)
      :SortEntityVisitor( iAcceptDoublon )
    {
    }
    //--------------------------------
    virtual void addOwnersOf( EntityPtr iEntity )
    {
      std::cout << "addOwnersOf 1 Entity ptr:" << (void*) iEntity  << std::endl;
     if( iEntity  )
	{
          std::cout << "  addOwnersOf 1 Entity ptr:" << (void*) iEntity << " Id:" << iEntity->getId() << " type:" <<  iEntity->getType() << std::endl;
	  switch( iEntity->getType() )
	    {
	    case ShapeType::Point:  execPoint( (Point*)iEntity);break;
	    case ShapeType::Line:   execBeginLine((Line*)iEntity);break;
	    case ShapeType::Facet:  execBeginFacet ((Facet*)iEntity);break;
	    case ShapeType::Poly:   execBeginPoly ((Poly*)iEntity);break;
	    case ShapeType::Object: execBeginObject((Object*)iEntity);break;
	    case ShapeType::Null: return;
	    }
	  addOwnersOf( iEntity->getOwners() );
	}
    }
    //--------------------------------  
   virtual void addOwnersOf( const std::set<Entity*> & iEntitys )
    {
      std::cout << "addOwnersOf 2 Owners:"<<  iEntitys.size() << std::endl; 
      for( EntityPtr lEntity : iEntitys )
	{
	  addOwnersOf( lEntity );
	}
    }
    //--------------------------------  
    virtual void addOwnersOf( const std::vector<Entity*> & iEntitys )
    {
      std::cout << "addOwnersOf 3 Owners:"<<  iEntitys.size() << std::endl;
      
      for( EntityPtr lEntity : iEntitys )
	{
	  if( lEntity  )
	    {
	      addOwnersOf( lEntity );
	    }
	}
    }
  };
  //*****************************************
  class RemoveOwnerVisitor  : public EntityVisitorNode {

    virtual void execNode    ( Entity* pEntity, Entity* pOwner )
    {
      if( pEntity!=nullptr ) 
        pEntity->removeOwner( pOwner );
    }
  };
   //*****************************************

} // namespace

#endif
