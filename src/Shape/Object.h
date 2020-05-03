#ifndef H__OBJECT__H
#define H__OBJECT__H

#include "PP3dType.h"
#include "ObjProps.h"


#include <string>
#include <memory>
#include <vector>

#include "Matrice.h"

#include "Entity.h"

// Cet objet a pour role de chapeauter les Shape
// il va s'occuper de tout ce qui n'est pas vraiment du graphique 3D
// nom, id, couleur, texture ...
// il peut contenir une matrice pour l"utilisation dans une scene,
// avoir des enfants ...
// C'est lui qui est vu par l'interface plus haut

namespace PP3d {

  class ViewProps;
  class SubSelect;

		
  enum class ObjectType  {  ObjPoint, ObjLine, ObjFacet, ObjPolyline, ObjPoly, ObjNull};
  inline static const char* GetStrObjectType( ObjectType pType )
  {
    switch( pType )
      {
      case ObjectType::ObjPoint:  return "ObjPoint";
      case ObjectType::ObjLine:   return "ObjLine";
      case ObjectType::ObjFacet:  return "ObjFacet";
      case ObjectType::ObjPoly:   return "ObjPoly";
      case ObjectType::ObjPolyline: return "ObjPolyline";
      case ObjectType::ObjNull: return "Unknown";
      }
    return "ObjectType::unknown";
  }
	
  inline static ObjectType GetObjectTypeFromStr( const char* pStr )
  {
    if( strcmp( pStr, "ObjPoint" ) == 0 )
      return ObjectType::ObjPoint;
    else		if( strcmp( pStr, "ObjLine" ) == 0 )
      return ObjectType::ObjLine;
    else		if( strcmp( pStr, "ObjFacet" ) == 0 )
      return ObjectType::ObjFacet;
    else		if( strcmp( pStr, "ObjPoly" ) == 0 )
      return ObjectType::ObjPoly;
    else		if( strcmp( pStr, "ObjPolyline" ) == 0 )
      return ObjectType::ObjPolyline;
    return ObjectType::ObjNull;
  }
	
  inline std::ostream& operator << ( std::ostream& pOs, ObjectType pType )
  {
    pOs <<   GetStrObjectType( pType );
    return pOs;
  }

  //*********************************
  class Object : public Entity {

  protected:
    ObjProps                     cMyProps;
    std::string                  cName;
	
  public:

    Object(  const char*pName );
    Object(  const std::string & pName );
    virtual ~Object();
		
    virtual ObjectType getObjType() const =0;
    ShapeType getType() const override { return ShapeType::Object;}
    virtual ShapeType getSubType() const =0;

    virtual void execVisitor( EntityVisitor& pVisit ) override;
    virtual void execVisitor( EntityVisitorNode& pVisit ) override;

    //		virtual Entity& getMyEntity();
    //		virtual const Entity& getMyEntity() const; 

    //		PP3dId  getId() { return getMyEntity().getId(); }
    virtual EntityPtr getShape()=0;
		
    PointPtrHash getPoints();

    virtual void recomputeAll(ObjProps&pProps);


    virtual void drawGL( ViewProps& pViewProps );
    virtual void selectGL( ViewProps& pViewProps );

    virtual void drawPointGL( ViewProps& pViewProps );
    virtual void drawLineGL( ViewProps& pViewProps );
    virtual void drawFacetGL( ViewProps& pViewProps );
    virtual void drawPolyGL( ViewProps& pViewProps );
    virtual void drawObjectGL( ViewProps& pViewProps );
		
    virtual void drawSelectPointGL( ViewProps& pViewProps );
    virtual void drawSelectLineGL( ViewProps& pViewProps );
    virtual void drawSelectFacetGL( ViewProps& pViewProps );
    virtual void drawSelectPolyGL( ViewProps& pViewProps );
    virtual void drawSelectObjectGL( ViewProps& pViewProps );


    //		void drawInfoGL(  ViewProps& pViewProps, ObjProps& pObjProps ); 

		
    //virtual void drawSelectionGL( ViewProps& pViewProps );
    const std::string&   getName()      const     { return cName; }
    void                 rename( const char* pName) { cName = pName; }

	
		
    void   setVisible( bool pVisible=true ) { cMyProps.cVisible = pVisible; } 
    bool   isVisible( bool pVisible=true ) { return cMyProps.cVisible;}

    Point3d getCenter() const { return cMyProps.cCenter; }

    friend class DataBase;
	
  };
  using ObjectPtr = Object*;
	
  //*********************************************
  //	class  ShapeLines : public Object {
		
  //	};
  //*********************************************
  //	class  ShapeFacet : public Object {
		
  //	};
  //*********************************************
  //	class  ShapePoly : public  Object {
  // };	
  //*********************************************
	
}


#endif
