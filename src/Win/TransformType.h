#ifndef H__TRANSFORM_TYPE__H
#define H__TRANSFORM_TYPE__H

#include "Shape/Transf3d.h"


namespace M3d{

	
  enum class Transform{
  
    Nothing,
      MoveX,
      MoveY,
      MoveZ,
      MoveNormal,
      MoveAxis,
            
      CenterRotX,
      CenterRotY,
      CenterRotZ,
      CenterRotNorm,
      CenterRotAxis,
      CenterRotFacetNorm,
      RotAxis,

      ScaleUniform,
      ScaleX,
      ScaleY,
      ScaleZ,
      ScaleRX,
      ScaleRY,
      ScaleRZ,
      ScaleAxis,
      ScaleNormal,
      
      };

  //*************************
  class TransformHelper{
  protected:
    Transform        cTransfType;
    PP3d::Transf3d & cTransf;
    double           cInitValue=0;
    
  public:
    TransformHelper( Transform iTransfType, PP3d::Transf3d & ioTransf );
    

    double &      value();
    double        value() const;
    void          change( double iDx );
    
  };  
   //*************************
 

} //namespace

  #endif
