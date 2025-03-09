
#include "Object.h"


#include "ViewProps.h"


#include "GLUtility.h"
#include "Selection.h"
#include "PP3dType.h"
#include "DataBase.h"

#include "EntityVisitorPicking.h"

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
  Object::Object(  const char*pName, ObjectType iObjType, EntityPtr iEntity, ClassType iClassType)
    :cName(pName)
    ,cObjectType(iObjType) 
    ,cClassType( iClassType )
    ,cShape( iEntity )
    ,cDateCreation( PPu::PPDate::GetCurrentDateTime70() )
  {
  }
  Object::Object(  const std::string& pName, ObjectType iObjType, EntityPtr iEntity, ClassType iClassType)
    :cName( pName )
    ,cObjectType(iObjType) 
    ,cClassType( iClassType )
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
  //---------------------------
  void Object::drawPointGL(ViewProps& pViewProps )
  {
    VisitorDrawPoints	lVisitP( pViewProps, cMyProps);
    execVisitor( lVisitP );
  }
  //---------------------------
  void Object::drawLineGL(ViewProps& pViewProps )
  {
    VisitorDrawLine	lVisitL( pViewProps, cMyProps);
    execVisitor( lVisitL );
  }
  //---------------------------
  void Object::drawFacetGL(ViewProps& pViewProps )
  {
    VisitorDrawFacet	lVisitF( pViewProps, cMyProps);
    execVisitor( lVisitF );
  }		
  //---------------------------
  void Object::drawPolyGL(ViewProps& pViewProps )
  {
    VisitorDrawPoly	lVisitY( pViewProps, cMyProps);
    execVisitor( lVisitY );
  }
  //---------------------------
  void Object::drawObjectGL(ViewProps& pViewProps )
  {
    VisitorDrawObject	lVisitO( pViewProps, cMyProps);
    execVisitor( lVisitO );
  }
  //---------------------------
  void Object::drawGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }
    //		std::cout << "Object::drawGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;
	 
    if( pViewProps.cGLMode != GLMode::Select  )
      {
	//  CHANGER DE PLACE : a ne faire quand cas de changement de l'objet 
	///////	recomputeAll( cMyProps,  );
      }

    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps	);		
	}
	break;

      case SelectType::All:
      case SelectType::Point:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL )
	    drawFacetGL(pViewProps);
					
	  drawLineGL( pViewProps );
	    
	  drawPointGL( pViewProps);
				
	}
	break;
				
      case SelectType::Line:
	{
	  //	  std::cout << "Object::drawGL line cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawFacetGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
				
				
      case SelectType::Facet:
	{
	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawFacetGL( pViewProps);

	  //	  std::cout << "Object::drawGL facet  cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
   
	  drawLineGL( pViewProps );		
	}
	break;

      case SelectType::Poly:
	{
	  //	  std::cout << "Object::drawGL poly cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawPolyGL(pViewProps);
	  
	  drawLineGL( pViewProps );			
	}
	break;
				
      case SelectType::Group:
      case SelectType::Object:
	{
	  //	  std::cout << "Object::drawGL object cGLMode:" <<  (int)pViewProps.cGLMode << std::endl;
	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawObjectGL( pViewProps);
					
	  drawLineGL( pViewProps );			
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
 
  //------------------------------------------------------
  //------------------------------------------------------
  //------------------------------------------------------
  void Object::drawSelectPointGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectPoints	lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectLineGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectLine	lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectFacetGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectFacet lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectPolyGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectPoly lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::drawSelectObjectGL(ViewProps& pViewProps )
  {
    VisitorDrawSelectObject lVisitS( pViewProps, cMyProps);
    execVisitor( lVisitS );
  }
  //---------------------------
  void Object::selectGL(ViewProps& pViewProps )
  {
    if( cMyProps.cVisible == false )
      {
	return;
      }

    //    std::cout << "*** Object::selectGL " << Selection::GetStrSelectType( pViewProps.cSelectType) <<  std::endl;

    switch( pViewProps.cSelectType )
      {
      case SelectType::Null:
	break;
	
      case SelectType::Point:
	{
  	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawPolyGL( pViewProps);
	  
	  drawSelectPointGL( pViewProps );
	}
	break;
				
				
      case SelectType::Line:
	{
  	  if( pViewProps.cViewMode == ViewMode::FULL)						
	    drawPolyGL( pViewProps);
	  
	  drawSelectLineGL( pViewProps );
	}
	break;
								
      case SelectType::Facet:
	{
	  drawSelectFacetGL( pViewProps );
	}
	break;

      case SelectType::Poly:
	{
	  drawSelectPolyGL( pViewProps );
	}
	break;
				
      case SelectType::Group:
      case SelectType::Object:
	{
	  drawSelectObjectGL( pViewProps );
	}
	break;
			
     case SelectType::All:
	{
	  drawSelectFacetGL( pViewProps );					
	  drawSelectLineGL( pViewProps);					
	  drawSelectPointGL( pViewProps );
	}
	break;
      }
    //	drawInfoGL( pViewProps, cMyProps );
  }
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
    lStr  << GetStrObjectType( getObjType() )
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
 //********************************
}
