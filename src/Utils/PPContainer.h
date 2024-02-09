#ifndef H__PP_CONTAINER__H
#define H__PP_CONTAINER__H


#include <unordered_map>

namespace PPu {

  
    //*******************************************************  
  template <class OBJECT>
  class VectPtrDel: public std::vector<OBJECT*>  {
  public:
    virtual ~VectPtrDel() { free(); }     
    void free() {
      for( OBJECT* lObj : *this ) { delete lObj; }
      this->clear();
    }    
  };
   //**************************************

  template <class OBJECT >
  class HashSetPtr : public std::unordered_set<OBJECT*>  {
  public:
  
    //--------------------------------------
    bool existObj( OBJECT * iObjPtr ) const
    {
      return this->contains( iObjPtr  );
    }
    //--------------------------------------
    void insertObj(  OBJECT * iObjPtr )
    {
      this->insert( iObjPtr );
    }
    //--------------------------------------
    void removeObj(  OBJECT * iObjPtr )
    {
      this->erase( iObjPtr );
    }
     //--------------------------------------
    template <class CONTAINER>
    void removeObjs(  CONTAINER iCont  )
    {
      for( OBJECT* lObjPtr: iCont )
        this->erase( lObjPtr );
    }
 };
 
  //**************************************

  template <class KEY, class OBJECT >
  class HashMapPtr : public std::unordered_map<KEY, OBJECT*>  {
  public:

    OBJECT * findObj( const KEY & iKey ) const
    {
      auto lTmp = this->find( iKey );
      if( lTmp == this->end() )
        return nullptr;
      return lTmp->second;
    }
    //--------------------------------------
    bool existObj( const KEY & iKey ) const
    {
      return findObj( iKey  ) != nullptr;
    }
    //--------------------------------------
    bool  insertObj( const KEY & iKey, OBJECT * iObjPtr )
    {
      return this->insert( { iKey, iObjPtr} ).second;
    }
    //--------------------------------------
    void removeObj( const KEY & iKey )
    {
      this->erase( iKey );
    }
  
  };
  //**************************************
  
} //end namespace


#endif
