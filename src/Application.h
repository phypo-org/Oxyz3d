#ifndef H__Application__H
#define H__Application__H

#include <vector>
#include <memory>

#include "Shape/PP3dType.h"
#include "Shape/DataBase.h"

#include "WinObjTree.h"
#include "Shape/PP3dType.h"
#include "Lua/PLua.h"
#include "ShapeLua.h"
#include "History.h"


namespace M3d{

  class Win3d;
	
  enum class Transform{ Nothing, MoveX, MoveY, MoveZ, MoveNormal, CenterRotX, CenterRotY, CenterRotZ };

  //************************************
  // Application est un singleton 

  class Application{ 
 
    std::vector< std::unique_ptr<Win3d> > cAllWin3d;
    std::unique_ptr<PP3d::DataBase>       cuDatabase;
    std::unique_ptr<History>              cuHistory;

		
    M3d::ShapeLua*            cLua=nullptr;
    
    //	History 

	
  private:			
    static Application* sTheAppli;
    Application();
    virtual ~Application();
		

		
    Transform       cCurrentTransform;
    PP3d::Transf3d  cCurrentTransf;

  public:
    enum class DeferRedraw { DeferFalse=false, DeferTrue = true };
  protected:
    DeferRedraw  cDeferFlagRedraw = DeferRedraw::DeferFalse;

  public:
    static const int sIconSize = 32;
	
    static Application& Instance()
    {
      if( sTheAppli == nullptr )
	{
	  sTheAppli = new Application( );
	}
      return *sTheAppli; 
    }
    
    static void FlCheckCallback( void* );

    
    void setCurrentTransformType( Transform lTrans)
    {	
      cCurrentTransf.raz();
      cCurrentTransform= lTrans;
			
    }
    Transform getCurrentTransformType()   { return cCurrentTransform;}
    PP3d::Transf3d& currentTransform()    { return cCurrentTransf; }

		
    int init( int argc, char* argv[] );

    std::vector< std::unique_ptr<Win3d> >& getWinVector()  { return cAllWin3d; };
		
    PP3d::DataBase* getDatabase() { return cuDatabase.get(); }
    History* getHistory()    { return cuHistory.get(); }
    M3d::ShapeLua&  getLua() { return *cLua; }
    const char*     execLuaHisto(const std::string& iLuaCode, std::ostream& iOut )
    {			
      return getLua().doCode( iLuaCode.c_str(), &iOut );
    }				
    const char*    execLuaHisto( std::ostringstream& iIn, std::ostream& iOut )
    {
      return execLuaHisto( iIn.str(), iOut );
    }
    const char*    	execLua(const  std::string& iLuaCode, std::ostream& iOut )
    {			
      return getLua().doCode( iLuaCode.c_str(), &iOut );
    }				
    const char*    execLua( std::ostringstream& iIn, std::ostream& iOut )
    {
      return execLua( iIn.str(), iOut );
    }
		
    Win3d & createNewWin3d( int pW, int pH );
    Win3d*  findCanvas3d( int iId );
    void    changeAllSelectType( PP3d::SelectType pType );
		
    void    createObjectTree( );
    void    redrawObjectTree();
    void    createWinHisto( );
    void    redrawWinHisto( );


    void setCursorPosition( PP3d::Point3d& pPos);

    void    redrawAllCanvas3d();	  
    void    redrawAll()
    {
      redrawAllCanvas3d();
      redrawObjectTree();
    }
	
    PP3d::DataBase* swapBase( PP3d::DataBase* ioBase );

		
    void    validate( History::SaveMode iMode, DeferRedraw iFlagDeverRedraw );
    void    makeDefer();

  };
  //************************************

};

#endif
