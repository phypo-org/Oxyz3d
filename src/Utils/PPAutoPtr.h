#ifndef H__PPAutoPtr__H
#define H__PPAutoPtr__H

namespace PPu {

  // ********************************************************
  template <typename OBJ>
  class PPAutoPtr {

    OBJ* ptr;

  public :

    PPAutoPtr( OBJ* obj=NULL ) 
      :ptr(obj)
    {    
    }

    PPAutoPtr( PPAutoPtr<OBJ>& ap ) 
      :ptr(ap.ptr)
    {    
      ap.ptr = NULL;
    }

    ~PPAutoPtr()
    {
      delete ptr;
      ptr = NULL;
    }

    void drop()
    {
      delete ptr;
      ptr = NULL;
    }

    int operator == (void*p_ptr)
    {
      return ptr == p_ptr;
    }

    int operator == (OBJ* p_ptr)
    {
      return ptr == p_ptr;
    }

    void operator = (PPAutoPtr<OBJ>& ap)
    {
      delete ptr;
      ptr = ap.ptr;
      ap.ptr = NULL;
    }


    void operator = ( OBJ* obj )
    {
      delete ptr;
      ptr = obj;
    }


    OBJ* operator->() const 
    {
      return (OBJ*)ptr;
    }

    OBJ* operator()()
    {
      return ptr;
    }

    operator OBJ*()
    {
      return ptr;
    }

    OBJ* give()
    {
      OBJ* tmp = ptr;
      ptr = NULL;
      return tmp;
    }

    OBJ* getPtr()
    {
      return ptr;
    }
  };
  // ********************************************************

} //end namespace


#endif
