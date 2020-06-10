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
      if( iEntity  )
	{
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
   virtual void addOwnersOf( const std::set<Entity*> & Entitys )
    {
      for( EntityPtr lEntity : Entitys )
	{
	  addOwnersOf( lEntity );
	}
    }
    //--------------------------------  
    virtual void addOwnersOf( const std::vector<Entity*> & Entitys )
    {
      for( EntityPtr lEntity : Entitys )
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
      pEntity->removeOwner( pOwner );
    }
  };
   //*****************************************

} // namespace

#endif
