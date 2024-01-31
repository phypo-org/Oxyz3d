#ifndef H__UNDOHISTORY__H
#define H__UNDOHISTORY__H




#include <vector>
#include <string>
#include <mutex>

#include "Utils/PPSingletonCrtp.h"
#include "Utils/PPDate.h"

#include "DataBase.h"


  //*************************************

// On pourrait avoir un arbre de modification.

namespace PP3d {

  class Selection;
  class  UndoHistory : public PPSingletonCrtp<UndoHistory>{

    
    std::vector<std::string> cHistoSav;
    size_t                   cMaxHisto = 16 ;    
    int                      cCurrent =0;

    PPu::PPDateTimeFloat70   cLastTimeSav=0;
    mutable std::mutex       cMyMutex;   // for autosave, lock access to cHistoSav and cCurrent

  public:

     UndoHistory() {;}

    size_t getMaxHisto()            { return cMaxHisto;}
    void   setMaxHisto( size_t iVal );

    int  getLast() { return ((int)cHistoSav.size())-1; }
    
    bool sav(  DataBase & iDb, Selection * iSel, const char * iSavFile = nullptr);
    bool readPrev( DataBase & oDb, Selection * ioSel );
    bool readNext( DataBase & oDb, Selection * ioSel );
    void cancelLastSav();

    int    getCurrent() const { return cCurrent; }
    size_t getSize()    const { return cHistoSav.size(); }

    bool isMax() { return cCurrent >= getLast(); }
    bool isMin() { return cCurrent <= 0; }

    // fonctions that's must be use by no main thread (autosave)
    bool externalThread_getSav( PPu::PPDateTimeFloat70 iLastSav, std::string & oResult );
  };	
  //*************************************

}


#endif
