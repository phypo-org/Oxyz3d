#ifndef H__EntityBase__H
#define H__EntityBase__H


#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <set>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <memory>
#include <utility>
#include <set>
#include <unordered_set>
#include <vector>
#include <sstream>


#include "Point3d.h"
#include "Calcul3d.h"
#include "Matrice.h"

#include "EntityVisitor.h"

namespace PP3d {

  
  enum ClassType { ClassTypeVoid=0, ClassTypeObj=1, ClassTypeGeo=2, ClassTypeAll=0xFFFF };
  
  inline static const char* GetStrClassType( ClassType pType )
  {
    switch( pType )
      {
      case ClassType::ClassTypeVoid:  return "ClassTypeVoid";
      case ClassType::ClassTypeObj:   return "ClassTypeObj";
      case ClassType::ClassTypeGeo:   return "ClassTypeGeo";
      case ClassType::ClassTypeAll:   return "ClassTypeAll";
      }
    return "ClassType::unknown";
  }
		
  enum class ObjectType  {  ObjPoint, ObjLine, ObjFacet, ObjPolyline, ObjPoly, ObjBSpline, ObjNull};
  inline static const char* GetStrObjectType( ObjectType pType )
  {
    switch( pType )
      {
      case ObjectType::ObjPoint:    return "ObjPoint";
      case ObjectType::ObjLine:     return "ObjLine";
      case ObjectType::ObjFacet:    return "ObjFacet";
      case ObjectType::ObjPoly:     return "ObjPoly";
      case ObjectType::ObjPolyline: return "ObjPolyline";

      case ObjectType::ObjBSpline: return "ObjBSpline";
      case ObjectType::ObjNull:    return "Unknown";
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

  
  enum class Compute{
    Nothing,
    FacetNormal,
    FacetConcave,
    FacetAll
  };
    
  

  class Entity;
  class Point;
  class Line;
  class Facet;
  class Poly;
  class Object;
  class Mat4;
  class DataBase;

  class EntityVisitorType;

	
  using EntityId      = GLuint; // au moins 32 bits
	
  using VectEntityPtr = std::vector<Entity*>;

  //*********************************************

  class Entity {
		
  protected:

    EntityId          cId=0;
		
    std::set<Entity*>       cOwners;
    
    bool              cIsMagnet=false;
    bool              cIsSelected=false;
    bool              cIsHighlight=false;
    bool              cTreeOpen=false;      // for interface 
    
  private:
    void     setId(EntityId iId) { cId = iId; }
  	         
  public:
    virtual ShapeType getType() const=0;
    virtual ObjectType getObjType()     const  { return ObjectType::ObjNull; }

    
    Entity()	 	      
    {
    }
    virtual ~Entity();
  
    void deleteAllHierarchy();

    
    EntityId getId()    { return cId; }
    bool     isIdVoid() { return cId==0;}
    void     razId()    { cId=0;}

    
    virtual bool isVoid() const =0;
  
    bool isTreeOpen()             { return cTreeOpen; }
    void setTreeOpen( bool iVal ) { cTreeOpen=iVal;}

    
    virtual Point3d getCenter3d() =0;
    virtual Point3d getNormal3d();

    
  public:
    bool isMagnet()                 { return   cIsMagnet;}
    void setMagnet( bool pFlag )    { cIsMagnet  = pFlag;}		
    bool isSelect()                 { return   cIsSelected;}
    void setSelect( bool pFlag )    { cIsSelected  = pFlag;}		
    bool isHighlight()              { return   cIsHighlight;}    
    void setHighlight( bool pFlag ) { cIsHighlight =  pFlag;}

    

    //--------  OWNER --------
    auto & getOwners    ( )  const         { return  cOwners; }
    size_t howManyOwner ( )  const         { return cOwners.size(); }
    
    void   addOwner     ( Entity* pOwner ) { cOwners.emplace( pOwner); }
    void   removeOwner  ( Entity* pOwner ) { cOwners.erase( pOwner); }
    void   clearAllOwner( )                { cOwners.clear(); }    
    
    Entity * firstOwner()
    {
      Entity* lTmp = nullptr;
      for( Entity* lOwner  :  cOwners ) { return lOwner;}
      return lTmp;
    }    
    

    void removeFromOwners() 
    {
      for( Entity* lOwner : getOwners() )
	{
	  lOwner->remove( this, false );
	}
      clearAllOwner();
    }
     //-------  OWNER --------
    
   
    virtual void remove( Entity* lEntity, bool iFlagRemoveOwner = true )
    {
      if(iFlagRemoveOwner) lEntity->removeOwner( this );
    }

    virtual bool clear() { clearAllOwner();  return true; }
		
    const char* getStrType() const { return GetStrShapeType( getType()) ; }
		
    friend std::ostream & operator << ( std::ostream & pOs, Entity& pEntity )
    {
      pOs << "Id:"      << pEntity.cId
	  << " Owners:" << pEntity.cOwners.size()
	  << " Type:"   << pEntity.getStrType() 
	  << " " << (void*)&pEntity ;
      return pOs;
    }
    virtual std::string  getStringInfo( ){
          std::ostringstream lStr;  
          lStr << getStrType();
          return lStr.str();
    }
		

    virtual void execVisitor( EntityVisitor& pVisit )     = 0;
    virtual void execVisitor( EntityVisitorNode& pVisit ) = 0;

			
    void move   ( Point3d  p );
    void scale  ( Point3d p ); // A n'utiliser que si l'objet est centre sur l'origine !
    void modify ( Mat4& pMat );

    static Object* FindMyObject();

    friend class DataBase;
    friend class CurrentInput;
    friend class MyRead;
    friend class Line;
    friend class Facet;
    friend class Poly;
  };
  //*********************************************
	
  using EntityPtr     = Entity*;
  using EntityPtrVect = std::vector<Entity*>;
  using EntityPtrSet  = std::set<Entity*>;
  using EntityPtrHash = std::unordered_set<Entity*>;

  using OwnerPtr     = Entity*;
  using OwnerPtrSet  = std::set<OwnerPtr>;

  
  template <class SET = std::set<Entity*> >
  static inline void ExecVisitor( SET & ioSet, EntityVisitor & pVisit )    
  {
    for( Entity * lEntity : ioSet )
      lEntity->execVisitor( pVisit );
  }
  
  template <class SET=std::set<Entity*>>
  static inline void ExecVisitorNode( SET  & ioSet, EntityVisitorNode & pVisit )    
  {
    for( Entity * lEntity : ioSet )
      lEntity->execVisitor( pVisit );
  }
  
  //*********************************************

  
}

#endif
