#ifndef H__UNDOHISTORY__H
#define H__UNDOHISTORY__H




#include <vector>
#include <string>

#include "Utils/PPSingletonCrtp.h"

#include "DataBase.h"


  //*************************************

// On pourrait avoir un arbre de modification.

namespace PP3d {
  
  class  UndoHistory : public PPSingletonCrtp<UndoHistory>{

    std::vector<std::string> cHistoSav;
    size_t                   cMaxHisto = 5; // 32 ;    
    int                      cCurrent =0;
  public:

     UndoHistory() {;}

    size_t getMaxHisto()            { return cMaxHisto;}
    void setMaxHisto( size_t iVal ) { cMaxHisto = iVal; }

    int getLast() { return ((int)cHistoSav.size())-1; }
    
    bool sav(  DataBase & iDb );
    bool read( DataBase & oDb, int iPos=-1 ); //-1 the last !
    bool readCurrent( DataBase & oDb );
    bool readNext( DataBase & oDb );
    void cancelLastSav();

    int    getCurrent() { return cCurrent; }
    size_t getSize()    { return cHistoSav.size(); }
  };	
  //*************************************

}


#endif
