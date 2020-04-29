
#ifndef H__UniqueId__H
#define H__UniqueId__H



namespace PP3d {
  //*********************************************
  template <typename TYPE, long VALUE=0>
  class TUniqueId {
	
    TYPE   cId=VALUE;
    
  public:

    TYPE getNewId()     { return cId++; }
    TYPE getCurrentId() { return cId; }
    void reset( TYPE iResetVal) { cId=iResetVal; }
  };
  //*********************************************	
}



#endif
