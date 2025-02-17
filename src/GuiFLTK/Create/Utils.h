#ifndef H__WinUtils__H
#define H__WinUtils__H


#include "Shape/Entity.h"
#include "Shape/SortVisitor.h"
#include "Shape/DataBase.h"
#include "Shape/SavRead.h"


namespace M3d{


  //************************************
  class Utils{

    // Inefficace !!!
   
  public:
    template <class SET_ENTITY>
    static bool DuplicateObjects(  PP3d::DataBase   & iSrcBase,
                                  SET_ENTITY & iInVectObj, //std::vector<PP3d::EntityPtr>
                                  PP3d::DataBase   & oDestBase )
    {
      PP3d::SortVisitorEntity  lVisit;    
      for(  PP3d::EntityPtr lEntity : iInVectObj )
	{
          lEntity->execVisitor( lVisit );
	}
      
      std::stringstream lDupStr;      
      PP3d::MySav lSav( lDupStr );      
      bool lRet = lSav.save( iSrcBase, false, nullptr, &lVisit.cSetAllEntity );
      
      if( lRet )
        {
          std::vector<PP3d::EntityPtr> lNewObjs;
          PP3d::MyRead lRead( lDupStr, &lNewObjs );
          
          lRet = lRead.read( oDestBase,  nullptr );
        }
    
    return lRet;
    }
    //------------------------------------------------------------
    static bool WriteObjectFromStream( std::istream & iStr,  PP3d::DataBase  & oDestBase , std::vector<PP3d::EntityPtr> & oNewObjs)
    {
      PP3d::MyRead lRead( iStr, &oNewObjs );
          
     return lRead.read( oDestBase, nullptr, false );
   }
    //------------------------------------------------------------
    template <class SET_ENTITY>
    static bool SaveObjectsInStream(  PP3d::DataBase   & iSrcBase, bool iFlagGroup,
                                      SET_ENTITY & iInVectObj, //std::vector<PP3d::EntityPtr>
                                      std::ostream & oDupStr )
    {
      PP3d::SortVisitorEntity  lVisit;    
      for(  PP3d::EntityPtr lEntity : iInVectObj )
	{
          lEntity->execVisitor( lVisit );
	}
      
      PP3d::MySav lSav( oDupStr );      
      bool lRet = lSav.save( iSrcBase, iFlagGroup, nullptr, &lVisit.cSetAllEntity );
      
    return lRet;   }
  };
  //************************************

}  // namespace

#endif
