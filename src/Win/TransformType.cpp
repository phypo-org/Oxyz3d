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
    
    switch( cTransfType )
      {
      case  Transform::Nothing: throw std::out_of_range("TransformHelper::value Nothing");
      case  Transform::MoveX:  return cTransf.position().x();
      case  Transform::MoveY:  return cTransf.position().y();
      case  Transform::MoveZ:  return cTransf.position().z();
        
      case  Transform:: MoveNormal:;
      case  Transform:: MoveAxis:;
            
      case  Transform::CenterRotX:;  return cTransf.angle().x();
      case  Transform::CenterRotY:   return cTransf.angle().y();
      case  Transform::CenterRotZ:   return cTransf.angle().z();

        /* on prend .angle().x(), sans raison precise */
      case  Transform::CenterRotNorm: return cTransf.angle().x();
      case  Transform::CenterRotAxis: return cTransf.angle().x();
      case  Transform::CenterRotFacetNorm: return cTransf.angle().x();
      case  Transform::RotAxis:       return cTransf.angle().x();

      case  Transform::ScaleUniform:;
      case  Transform::ScaleX:;
      case  Transform::ScaleY:;
      case  Transform::ScaleZ:;
      case  Transform::ScaleRX:;
      case  Transform::ScaleRY:;
      case  Transform::ScaleRZ:;
      case  Transform::ScaleAxis:;
      case  Transform::ScaleNormal: ;

      case Transform::MagnetMove: ;
      }
    
    return cInitValue;
  }
  //-----------------------------------------------------
  void  TransformHelper::change( double iDx )
  {
  }
    
  
} //namespace
