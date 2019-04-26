#include "History.h"




//************************************

History::save( PP3d::DataBase& iData )
{
  static size_t sId=1; 
  std::unique_ptr<HistoryLink> luLink = new HistoryLink(sId++);

  
  PP3d::MySav lSav( luLink->cSavData );
  luLink->cSavId = iData->getCurrentId();
  if( lSav.save( iData ) )
    {
      cLinks.push_back( luLink.release() );
      cCurrentLink = -1;
      return true;
    }
  return false;
}
//--------------------------------------
PP3d::DataBase* iDataHistory::restore( size_t iId )
{
  if( iId >= cLinks.size )
    {
      return nullptr;
    }
  std::unique_ptr<PP3d::DataBase> luBase = new PP3d::DataBase();
  yy
  
  cCurrentLink = iId;
}
