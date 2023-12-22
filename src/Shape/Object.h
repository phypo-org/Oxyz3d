#ifndef H__OBJECT__H
#define H__OBJECT__H

#include "PP3dType.h"
#include "ObjProps.h"
#include "Utils/PPDate.h"


#include <string>
#include <memory>
#include <vector>

#include "Matrice.h"

#include "Entity.h"

#include "Utils/PPDate.h"
#include "Utils/PPContainer.h"


// Cet objet a pour role de chapeauter les Shape
// il va s'occuper de tout ce qui n'est pas vraiment du graphique 3D
// nom, id, couleur, texture ...
// il peut contenir une matrice pour l"utilisation dans une scene,
// avoir des enfants ...
// C'est lui qui est vu par l'interface plus haut

namespace PP3d {

  class ViewProps;
  class SubSelect;

  enum ClassType { ClassTypeVoid=0, ClassTypeObj=1, ClassTypeGeo=2, ClassTypeAll=0xFFFF };
  
		
  enum class ObjectType  {  ObjPoint, ObjLine, ObjFacet, ObjPolyline, ObjPoly, ObjBSpline, ObjNull};
  inline static const char* GetStrObjectType( ObjectType pType )
  {
    switch( pType )
      {
      case ObjectType::ObjPoint:  return "ObjPoint";
      case ObjectType::ObjLine:   return "ObjLine";
      case ObjectType::ObjFacet:  return "ObjFacet";
      case ObjectType::ObjPoly:   return "ObjPoly";
      case ObjectType::ObjPolyline: return "ObjPolyline";

      case ObjectType::ObjBSpline: return "ObjBSpline";
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
    else		if( strcmp( pStr, "ObjBSpline" ) == 0 )
      return ObjectType::ObjBSpline;
    return ObjectType::ObjNull;
  }
  
	
  inline std::ostream& operator << ( std::ostream& pOs, ObjectType pType )
  {
    pOs <<   GetStrObjectType( pType );
    return pOs;
  }

  class Object;
  class GroupObject;
  using ObjectPtr = Object*;

  using GroupPtr = GroupObject*;
   
  //*********************************
  class Object : public Entity {

  protected:
    ObjProps                     cMyProps;
    std::string                  cName;

    ClassType                    cClassType=ClassTypeObj;
    
    PPu::PPDateTime70            cDateCreation;
  public:

    Object(  const char*pName,  ClassType  iClassType );
    Object(  const std::string & pName, ClassType  iClassType);
    virtual ~Object();

    
    virtual ObjectType getObjType() const =0;
    ShapeType getType() const override { return ShapeType::Object;}
    virtual ShapeType getSubType() const =0;
    bool  isClassType( ClassType iClass ) { return ((ulong)cClassType & ((ulong)iClass))!=0; }
    bool  isClassGeo() { return isClassType( ClassTypeGeo );}
    
    PPu::PPDateTime70 getDateCreation() const { return cDateCreation; }
    virtual void execVisitor( EntityVisitor& pVisit ) override;
    virtual void execVisitor( EntityVisitorNode& pVisit ) override;

    //		virtual Entity& getMyEntity();
    //		virtual const Entity& getMyEntity() const; 

    //		PP3dId  getId() { return getMyEntity().getId(); }
    virtual EntityPtr getShape()=0;
		

    virtual int recomputeAll(ObjProps&pProps,
			      Compute iCompute);


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


    //=========== GROUP ==========
    GroupPtr cGroup = nullptr;

  private:
    void internalSetGroup(GroupPtr iGroup ) { cGroup=iGroup;}
  public:
    GroupPtr  getGroup() { return cGroup; }
    void      setGroup( GroupPtr iGroup );
   
    //=========== GROUP ==========

    
    //virtual void drawSelectionGL( ViewProps& pViewProps );
    const std::string&   getName()      const     { return cName; }
    void                 rename( const char* pName) { cName = pName; }

	
		
    void   setVisible( bool pVisible=true ) { cMyProps.cVisible = pVisible; } 
    bool   isVisible( bool pVisible=true ) { return cMyProps.cVisible;}

    Point3d getCenter() const { return cMyProps.cCenter; }



    
    friend class DataBase;
    friend class GroupObject;
	
  };
	
  //*********************************
  class GroupObject{
    EntityId cGroupId=0;
    PPu::HashSetPtr<Object> cObjects;
    
  protected:
       GroupObject(EntityId cId) :cGroupId(cId) {;}
  
  public:
    EntityId getGroupId() const { return cGroupId; }
        
    void addObject(ObjectPtr iObj);
    void removeObject(ObjectPtr iObj);
    bool exist( ObjectPtr iObj) const ;
    void clear();
    const PPu::HashSetPtr<Object>& values() const { return cObjects;}
  
    friend class DataBase;
  };

  
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
