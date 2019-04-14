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



namespace M3d{

  class Win3d;
	
  enum class Transform{ Nothing, MoveX, MoveY, MoveZ, CenterRotX, CenterRotY, CenterRotZ };

  //************************************
  // Application est un singleton 

  class Application{ 
 
    std::vector< std::unique_ptr<Win3d> > cAllWin3d;
    std::unique_ptr<PP3d::DataBase>       cuDatabase;
		M3d::ShapeLua*                        cLua=nullptr;

	
  private:			
    static Application* sTheAppli;
    Application();
    virtual ~Application();
		

		
    Transform       cCurrentTransform;
    PP3d::Transf3d  cCurrentTransf;

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
    void setCurrentTransformType( Transform lTrans)
    {	
      cCurrentTransf.raz();
      cCurrentTransform= lTrans;
			
    }
    Transform getCurrentTransformType()   { return cCurrentTransform;}
    PP3d::Transf3d& currentTransform()    { return cCurrentTransf;}

		
    int init( int argc, char* argv[] );

    std::vector< std::unique_ptr<Win3d> >& getWinVector()  { return cAllWin3d; };
		
    PP3d::DataBase* getDatabase() { return cuDatabase.get(); }
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
    void    redrawAllCanvas3d();
    Win3d*  findCanvas3d( int iId );
    void    changeAllSelectType( PP3d::SelectType pType );
		
    void    createObjectTree( );
    void    redrawObjectTree();
	  
    void redrawAll()
    {
      redrawAllCanvas3d();
      redrawObjectTree();
    }
	
    void setCursorPosition( PP3d::Point3d& pPos);

  };
  //************************************

};

#endif
