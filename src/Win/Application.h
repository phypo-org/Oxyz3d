#ifndef H__Application__H
#define H__Application__H

#include <vector>
#include <memory>

#include "Shape/PP3dType.h"
#include "Shape/DataBase.h"
#include "Shape/Selection.h"

#include "WinObjTree.h"
#include "Shape/PP3dType.h"
#include "Lua/PLua.h"
#include "ShapeLua.h"
#include "Shape/UndoHistory.h"

#include "Utils/PPSingletonCrtp.h"
#include "Utils/PPConfig.h"

#define TheSelect          TheAppli.cSelect
#define TheSelectTransform TheAppli.cSelectTransform
   
#define PushHistory() PP3d::UndoHistory::Instance().sav( *Application::Instance().getDatabase(), &TheSelect )
#define TheAppli M3d::Application::Instance()



namespace M3d{

  class Win3d;
	
  enum class Transform{
    Nothing,
      MoveX,
      MoveY,
      MoveZ,
      MoveNormal,
      MoveAxis,
            
      CenterRotX,
      CenterRotY,
      CenterRotZ,
      CenterRotNorm,
      CenterRotAxis,
      
      ScaleUniform,
      ScaleX,
      ScaleY,
      ScaleZ,
      ScaleRX,
      ScaleRY,
      ScaleRZ,
      ScaleAxis,
      ScaleNormal
      };

  //************************************
  // Application est un singleton 

  class Application : public virtual PPSingletonCrtp<Application> {
    
    friend PPSingletonCrtp<Application>;

    std::vector< std::unique_ptr<Win3d> > cAllWin3d;
    std::unique_ptr<PP3d::DataBase>       cuDatabase;
    

    
    std::unique_ptr<PP3d::DataBase>       cuDatabaseTransform; // les axes, plans etc
  public:
    PP3d::Selection                       cSelect;
    PP3d::Selection                       cSelectTransform;
  protected:    
     PP3d::ObjectLine *                    cCurrentAxis;
    
    M3d::ShapeLua*                        cLua=nullptr;

    PPu::PPConfig  cMyConfig;
	
  private:			
    Application();
    virtual ~Application();
		
		
    Transform       cCurrentTransform;
    PP3d::Transf3d  cCurrentTransf;
    

  public:
    static const int sIconBigSize = 64;
    static const int sIconSize = 32;
    static const int sIconSmallSize = 16;

    PPu::PPConfig & getConfig() { return cMyConfig; }
    
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
    void setDatabase(std::unique_ptr<PP3d::DataBase>&iuBase )
    {
      cSelect.removeAll();
      cuDatabase = std::move(iuBase);
    }
    PP3d::DataBase* getDatabaseTransform() { return cuDatabaseTransform.get(); }
    
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

    bool addAxis( PP3d::Point3d & lA, PP3d::Point3d & lB )
    {
      if( lA != lB )
	{
	  cCurrentAxis = MakeObjectLine( "Axis", lA, lB );
	  cuDatabaseTransform->addObject( cCurrentAxis );
	  redrawObjectTree();
	  return true;
	}
      return false;
    }
    bool addAxis( PP3d::PointPtr  lA, PP3d::PointPtr lB )
    {
      if( lA->get() != lB->get() )
	{
	  cCurrentAxis = MakeObjectLine( "Axis", lA->get(), lB->get() );
	  cuDatabaseTransform->addObject( cCurrentAxis );
	  redrawObjectTree();
	  return true;
	}      
      return false;
    
    }
    bool setCurrentAxis(PP3d::ObjectPtr iObj) {
      if( iObj->getObjType() == PP3d::ObjectType::ObjLine )
	{
	  cCurrentAxis = (PP3d::ObjectLine*) iObj;
	  return true;
	}
      return false;
    }
    PP3d::ObjectLine * getCurrentAxis() { return cCurrentAxis; }

    bool isSelectAxis() {
      return (TheSelectTransform.getSelectType() != PP3d::SelectType::Null
	      && TheSelectTransform.getNbSelected() == 1
	      && TheSelectTransform.getSelectionVect()[0]->getType() == PP3d::ShapeType::Object
	      && ((PP3d::ObjectPtr)TheSelectTransform.getSelectionVect()[0])->getObjType() == PP3d::ObjectType::ObjLine);
    }
 };
  //************************************

}

#endif
