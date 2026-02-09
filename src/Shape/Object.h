#ifndef H__OBJECT_BASE__H
#define H__OBJECT_BASE__H

#include "PP3dType.h"
#include "ObjProps.h"
#include "Utils/PPDate.h"


#include <string>
#include <memory>
#include <vector>

#include "Matrice.h"

#include "Poly.h"

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

  class Object;
  class GroupObject;
  using ObjectPtr = Object*;

  using GroupPtr = GroupObject*;
   
  //*********************************
  class Object : public Entity {

  protected:
    ObjProps                     cMyProps;
    std::string                  cName;

    //    ObjectType                   cObjectType = ObjectType::ObjNull;
    ClassType                    cClassType  = ClassTypeObj;
    EntityPtr                    cShape     = nullptr;
    
    PPu::PPDateTime70            cDateCreation;
  public:

    Object(  const char*pName,          EntityPtr iEntity );
    Object(  const std::string & pName, EntityPtr iEntity );
    virtual ~Object();
    
    Point3d getCenter3d() override {
      if( getShape() != nullptr ) return getShape()->getCenter3d();
      return Point3d();
    }

    Point3d getNormal3d() override {
      if( getShape() != nullptr ) return getShape()->getNormal3d();
      return Point3d();
    }
    
  public:    
    //    virtual ObjectType getObjType()        const override  { return cObjectType; }
    virtual ShapeType getShapeType()       const override  { return ShapeType::Object;}
    virtual ShapeType getSubType()         const           { if( cShape) return cShape->getShapeType(); return ShapeType::Null; }
    bool  isClassType( ClassType iClass )                  { return ((ulong)cClassType & ((ulong)iClass))!=0; }
    bool  isClassGeo()                                     { return isClassType( ClassTypeGeo );}

    bool is(  ShapeType  iShape ) { if( cShape) return cShape->getSubType() == iShape; return false; }
    //    bool is(  ObjectType iObj  )  { return iObj == cObjectType; }

    virtual bool isVoid()  const override  {  if( cShape) return cShape->isVoid(); return true; }

    
    PPu::PPDateTime70 getDateCreation() const { return cDateCreation; }
    virtual void execVisitor( EntityVisitor& pVisit )     override;
    virtual void execVisitor( EntityVisitorNode& pVisit ) override;

    //		virtual Entity& getMyEntity();
    //		virtual const Entity& getMyEntity() const; 

    //		PP3dId  getId() { return getMyEntity().getId(); }
    virtual EntityPtr getShape()      { return cShape; }
    virtual void      removeShape()   { cShape = nullptr; };
    
    bool removeShapeIsVoid(EntityPtr iEntity = nullptr){
      if(iEntity == nullptr || iEntity == getShape() )
        {
          removeShape();
        }
      return isVoid();
    }

    
    
    LinePtr    getLine()           {
      if( is(  ShapeType::Line) ) return dynamic_cast<LinePtr>( cShape);
      return nullptr;
    }

    FacetPtr	getFacet()          {
      if( is( ShapeType::Facet) ) return dynamic_cast<FacetPtr>( cShape);
      return nullptr;   
    };

    PolylinePtr   getPolyline()           {
      if( is( ShapeType::Polyline) ) return dynamic_cast<PolylinePtr>(cShape);
      return nullptr;
    }
    /*    FacetPtr   getBSpline()           {
      if( is( ShapeType::BSpline) ) return dynamic_cast<FacetPtr>(cShape);
      return nullptr;
      }*/

    Poly*      getPoly()           {
      if( is( ShapeType::Poly) ) return dynamic_cast<PolyPtr>(cShape);
      return nullptr;
    }


    EntityPtr setShape( EntityPtr iShape );
    
    EntityPtr setLine( LinePtr iShape ) {
      //     cObjectType =  ObjectType::ObjLine ;
      return setShape( iShape );
    }
    
    EntityPtr setFacet( FacetPtr iShape ) {
      //  cObjectType =  ObjectType::ObjFacet;
      return setShape( iShape );
    }

    EntityPtr setPolyline( PolylinePtr iShape ) {
      //   cObjectType =  ObjectType::ObjPolyline ;
      return setShape( iShape );
    }
       
    EntityPtr setPoly( PolyPtr iShape ) {
      //   cObjectType =  ObjectType::ObjPoly ;
      return setShape( iShape );
    }

    static ObjectPtr CreateNewLine( std::string iName, PP3d::Point3d & iA, PP3d::Point3d & iB, ClassType iClassType=ClassTypeObj)
    {
      return new PP3d::Object( "Axe", MakeLine( iA, iB ) ); 
    }
    

    virtual int recomputeAll(ObjProps&pProps,
			      Compute iCompute);


    virtual void drawGL( ViewProps& pViewProps, bool iForceSelect=false, bool iSelect=false ) override;
 
    virtual void selectGL( ViewProps& pViewProps, EntityId iForceId=0 ) override;
  


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
    virtual  std::string getStringInfo();
	
		
    void   setVisible( bool pVisible=true ) { cMyProps.cVisible = pVisible; } 
    bool   isVisible( bool pVisible=true ) { return cMyProps.cVisible;}

    Point3d getPropsCenter() const { return cMyProps.cCenter; }



    
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
