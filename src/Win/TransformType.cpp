#include "TransformType.h"


namespace M3d{

  TransformHelper::TransformHelper( Transform iTransfType, PP3d::Transf3d & ioTransf )
    :cTransfType(iTransfType)
    ,cTransf( ioTransf )
  {
    cInitValue = value();  
  }
  //----------------------------------------------------
  double &      TransformHelper::value()
  {
    /*
    switch( cTransfType )
      {
      case  Transform::Nothing: throw std::out_of_range("TransformHelper::value Nothing");
      case  Transform::MoveX: return cTransf.position().x();
      case  Transform::MoveY: return cTransf.position().y();
      case  Transform::MoveZ: return cTransf.position().z();
      case  Transform:: MoveNormal:;
      case  Transform::MoveAxis:;
            
      case  Transform::CenterRotX:;
      case  Transform::CenterRotY:;
      case  Transform::CenterRotZ:;
      case  Transform::CenterRotNorm:;
      case  Transform::CenterRotAxis:;
      case  Transform::CenterRotFacetNorm:;
      case  Transform::RotAxis:;

      case  Transform::ScaleUniform:;
      case  Transform::ScaleX:;
      case  Transform::ScaleY:;
      case  Transform::ScaleZ:;
      case  Transform::ScaleRX:;
      case  Transform::ScaleRY:;
      case  Transform::ScaleRZ:;
      case  Transform::ScaleAxis:;
      case  Transform::ScaleNormal: ;
      }
    */
    return cInitValue;
  }
  //-----------------------------------------------------
  void  TransformHelper::change( double iDx )
  {
  }
    
  
} //namespace
