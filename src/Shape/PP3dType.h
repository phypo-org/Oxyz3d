
#ifndef H__PP3dType__H
#define H__PP3dType__H

#if defined(_WIN32)
#include <GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#endif


#include <iostream>
#include <stdexcept>
#include <vector>


#include "string.h"

namespace PP3d {
	
  using   PDouble =  GLdouble;
  using   PIndex  =  size_t ;
  using   PP3dId  =   GLuint ;
	
  using   ComponentId = int;
	
	
  enum class SelectType {  Null=0, Point=1, Line=2, Facet=4, Poly=8, Object=16, All=32};
  inline static const char* GetStrShapeType( SelectType pType )
  {
    switch( pType )
      {
      case SelectType::Null:   return "None";
      case SelectType::Point:  return "Point";
      case SelectType::Line:   return "Line";
      case SelectType::Facet:  return "Facet";
      case SelectType::Poly:   return "Poly";
      case SelectType::Object: return "Object";
      case SelectType::All:    return "";
      }
    return "SelectType::unknown";
  }
  inline std::ostream& operator << ( std::ostream& pOs, SelectType pType )
  {
    pOs <<   GetStrShapeType( pType );
    return pOs;
  }


	
  enum class GLMode { Select, Draw };

  enum class SelectMode { Undefine, Select, Unselect, Inverse };
  inline std::ostream& operator << ( std::ostream& pOs, SelectMode pVal)
  {
    switch( pVal )
      {
      case SelectMode::Undefine: pOs <<"Undefine"; break;
      case SelectMode::Select: pOs <<"Select"; break;
      case SelectMode::Unselect: pOs <<"Unselect"; break;
      case SelectMode::Inverse: pOs <<"Inverse"; break;
      }
    return pOs;
  }

  enum class ShapeType  {  Point, Line, Facet, Poly, Object, Null};
  inline static const char* GetStrShapeType( ShapeType pType )
  {
    switch( pType )
      {
      case ShapeType::Point:  return "Point";
      case ShapeType::Line:   return "Line";
      case ShapeType::Facet:  return "Facet";
      case ShapeType::Poly:   return "Poly";
      case ShapeType::Object: return "Object";
      case ShapeType::Null: return "";
      }
    return "ShapeType::unknown";
  }
	
  inline static ShapeType GetShapeTypeFromStr( const char* pStr )
  {
    if( ::strcmp( pStr, "Point" ) == 0 )
      return ShapeType::Point;
    else		if( ::strcmp( pStr, "Line" ) == 0 )
      return ShapeType::Line;
    else		if( ::strcmp( pStr, "Facet" ) == 0 )
      return ShapeType::Facet;
    else		if( ::strcmp( pStr, "Poly" ) == 0 )
      return ShapeType::Poly;
    else		if( ::strcmp( pStr, "Object" ) == 0 )
      return ShapeType::Object;
    return ShapeType::Null;
  }
	
  inline std::ostream& operator << ( std::ostream& pOs, ShapeType pType )
  {
    pOs <<   GetStrShapeType( pType );
    return pOs;
  }

  //********************************************************

}



#endif
