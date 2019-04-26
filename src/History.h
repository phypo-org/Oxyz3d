#ifndef H__History__H
#define H__History__H




#include <std::vector>
#include <std::string>
#include <std::iostream>


namespace M3d{


  //************************************
  
  class History{
    
    //===================
    struct HistoryLink{

      size_t        cIdLink=0;
      std::string   cName="no name";
      double        cTime=0;  //seconde.nano
      
      std::iostream cSavData;
      EntityId      cSavId=0;     

      HistoryLink( size_t iIdLink )
	:cIdLink( iIdLink )
      {
      }
      // ajouter cCurrentCreation
      // aj outer cCurrentLine
    };
    //===================

    std::vector<HistoryLink*>      cLinks;
    int                            cCurrentLink = -1; 
 
    bool save   ( PP3d::DataBase& );
    bool restore( int iPos );
    
    size_t size() { return cLink.size();}
 	
  };
  //************************************

};

#endif
