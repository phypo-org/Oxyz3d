#ifndef __PPSINGLETON_CRTP__H
#define __PPSINGLETON_CRTP__H

#include "PPNonCopyable.h"

// CRTP for implementation of Singleton 

template<class CLASS_CHILD>

class PPSingletonCrtp : public virtual PPNonCopyable{
  
  static inline CLASS_CHILD* sInstance = nullptr;
  
public:
  static inline CLASS_CHILD & Instance()
  {
    if( sInstance == nullptr )
      {
	sInstance = new CLASS_CHILD; 
      }
    return *sInstance;
  }
};
 
//***************************************


#endif


