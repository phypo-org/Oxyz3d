#ifndef H__UNDOHISTORY__H
#define H__UNDOHISTORY__H




#include <vector>
#include <string>

#include "Utils/PPSingletonCrtp.h"

#include "DataBase.h"


  //*************************************

// On pourrait avoir un arbre de modification.

namespace PP3d {

  class Selection;
  class  UndoHistory : public PPSingletonCrtp<UndoHistory>{

    
    std::vector<std::string> cHistoSav;
    size_t                   cMaxHisto = 16 ;    
    int                      cCurrent =0;
  public:

     UndoHistory() {;}

    size_t getMaxHisto()            { return cMaxHisto;}
    void setMaxHisto( size_t iVal );

    int getLast() { return ((int)cHistoSav.size())-1; }
    
    bool sav(  DataBase & iDb, Selection * iSel );
    bool readPrev( DataBase & oDb, Selection * ioSel );
    bool readNext( DataBase & oDb, Selection * ioSel );
    void cancelLastSav();

    int    getCurrent() { return cCurrent; }
    size_t getSize()    { return cHistoSav.size(); }

    bool isMax() { return cCurrent >= getLast(); }
    bool isMin() { return cCurrent <= 0; }
  };	
  //*************************************

}


#endif
