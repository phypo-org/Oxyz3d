#ifndef H__MyFLCallback__H
#define H__MyFLCallback__H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>


namespace M3d {

  //---------------------------
  struct MyUserData{
    void* cUserData1;
    void* cUserData2;
    void* cUserData3;
    void* cUserData4;
    void* cUserData5;
    void* cUserData6;
    void* cUserData7;
    void* cUserData8;
    void* cUserData9;
    void* cUserData10;
		
    MyUserData( void* pUserData1, void* pUserData2, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr, void* pUserData9=nullptr, void* pUserData10=nullptr )
      :cUserData1(pUserData1)
      ,cUserData2(pUserData2)
      ,cUserData3(pUserData3)
      ,cUserData4(pUserData4)
      ,cUserData5(pUserData5)
      ,cUserData6(pUserData6)
      ,cUserData7(pUserData7)
      ,cUserData8(pUserData8)
      ,cUserData9(pUserData9)
      ,cUserData10(pUserData10)
    {
    }

    void setUserData( void* pUserData1, void* pUserData2, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr, void* pUserData9=nullptr, void* pUserData10=nullptr )
    {
      cUserData1=pUserData1;
      cUserData2=pUserData2;
      cUserData3=pUserData3;
      cUserData4=pUserData4;
      cUserData5=pUserData5;
      cUserData6=pUserData6;
      cUserData7=pUserData7;
      cUserData8=pUserData8;
      cUserData9=pUserData9;
      cUserData10=pUserData10;
    }
  };  
  //****************************************************
} // namespace

#endif
