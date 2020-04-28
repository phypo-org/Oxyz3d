#ifndef __PPNONCOPYABLE__
#define __PPNONCOPYABLE__

  
//***************************************
class PPNonCopyable {  
    
protected:
  PPNonCopyable  ()  = default;
  ~PPNonCopyable ()  = default; 
public: 
  PPNonCopyable   (const PPNonCopyable &)            = delete;
  PPNonCopyable & operator = (const PPNonCopyable &) = delete;
};  
  
//***************************************
  
#endif 
