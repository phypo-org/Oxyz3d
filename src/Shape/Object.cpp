
#include "Object.h"


#include "ViewProps.h"


#include "GLUtility.h"
#include "Selection.h"
#include "PP3dType.h"
#include "DataBase.h"

#include "VisitorPicking.h"

#include <algorithm>
#include <sstream> 

namespace PP3d{
  //********************************

  void GroupObject::addObject( ObjectPtr iObj)
  {    
    if(iObj->getGroup() != this )
      {
        cObjects.insertObj( iObj );
        iObj->setGroup( this );
      }
  }
  //---------------------------
  void GroupObject::removeObject(ObjectPtr iObj)
  {
    if(iObj->getGroup() == this )
      {
        cObjects.removeObj( iObj );
        iObj->internalSetGroup( nullptr );
      }
  }
  //---------------------------
  bool  GroupObject::exist( ObjectPtr iObj) const 
  {
    if( iObj->getGroup() == this )
      {
        return cObjects.existObj(iObj);
      }
    return false;
  }
  //---------------------------
  void GroupObject::clear()
  {
    for( ObjectPtr lObj : cObjects )
      {
        lObj->internalSetGroup( nullptr );
      }
    cObjects.clear();
  }
  //********************************
  void Object::setGroup( GroupPtr iGroup )
  {
    if( cGroup != nullptr )
      {
        cGroup->removeObject( this );
      }
    internalSetGroup( iGroup );
  }
  //---------------------------
  Object::Object(  const char*pName,  EntityPtr iEntity )
    :cName(pName)
    ,cShape( iEntity )
    ,cDateCreation( PPu::PPDate::GetCurrentDateTime70() )
  {
  }
  Object::Object(  const std::string& pName,  EntityPtr iEntity )
    :cName( pName )
    ,cShape( iEntity )
    ,cDateCreation( PPu::PPDate::GetCurrentDateTime70() )
 {
  }
  //---------------------------
  Object::~Object()
  {
  }
  //-------------------------------
  void Object::execVisitor( EntityVisitor& pVisit )
  {
    std::cout << "Object::execVisitor" << std::endl;
    pVisit.execBeginObject( this );
		
    getShape()->execVisitor( pVisit );
			
    pVisit.execEndObject( this );
  }
 
 //-------------------------------
  void Object::execVisitor( EntityVisitorNode& pVisit )
  {		
    pVisit.execNode( this, nullptr );	// Object n'a pas de owner !
    pVisit.execNode( getShape(), this );
		
    getShape()->execVisitor( pVisit );
			
    pVisit.execEndNode( getShape(), this );
    pVisit.execEndNode( this, nullptr );	// Object n'a pas de owner !
  }
  //------------------------------------------------------
  //------------------------------------------------------
  
  //---------------------------
  int Object::recomputeAll(ObjProps&pProps, Compute iCompute)
  {
    switch( iCompute ){
    case Compute::Nothing :
      break;
      
    case Compute::FacetNormal:
      {
	//    std::cout << "   Object::recomputeAll Normal" << std::endl;
    VisitorNormalFacet lVisitN;
	execVisitor( lVisitN );
      }
      break;
      
    case  Compute::FacetConcave:
      {
	VisitorConcaveFacet lVisitC;
	//    std::cout << "   Object::recomputeAll Cancav" << std::endl;
	execVisitor( lVisitC );
	return lVisitC.count();
      }
      break;
      
    case  Compute::FacetAll:
      {
	VisitorNormalFacet lVisitN;
	execVisitor( lVisitN );
	VisitorConcaveFacet lVisitC;
	//    std::cout << "   Object::recomputeAll All" << std::endl;
	execVisitor( lVisitC );
	return lVisitC.count();
      }
      break;
    }
    
    return 0;
  }
  //---------------------------
    std::string  Object::getStringInfo( )
  {
    std::ostringstream lStr;  
    lStr  << GetStrShapeType( getShapeType() )
         << ':'<<  getName()  ;
    return lStr.str(); // Va faire temporaire du string 
  }
  //---------------------------
  EntityPtr Object::setShape( EntityPtr iShape )
  {
    EntityPtr lTmp = iShape;
    
    DataBase::RemoveOwner( lTmp, this );
    
    cShape = iShape;      
    DataBase::AddOwner( cShape, this );
    
    return lTmp;
  }
  //---------------------------
  
  void Object::drawGL( ViewProps& pViewProps ) 
  {
    std::cout << ">>>>>>>>>>><>>>>>>>>> Object::drawGL" << std::endl;
   
    if( cShape) cShape->drawGL(pViewProps);
  }
  //---------------------------
  void Object::selectGL( ViewProps& pViewProps ) 
  {
    if( cShape)
      cShape->selectGL(pViewProps);
    };
  
 //********************************
}
