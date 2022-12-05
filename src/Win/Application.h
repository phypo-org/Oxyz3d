#ifndef H__Application__H
#define H__Application__H

#include <vector>
#include <memory>

#include "Shape/PP3dType.h"
#include "Shape/DataBase.h"
#include "Shape/CurrentInput.h"
#include "Shape/Selection.h"

#include "WinObjTree.h"
#include "Shape/PP3dType.h"

#ifdef USING_LUA
#include "Lua/PLua.h"
#include "ShapeLua.h"
#endif

#include "Shape/UndoHistory.h"

#include "Utils/PPSingletonCrtp.h"
#include "Utils/PPConfig.h"


  
#define PushHistory() PP3d::UndoHistory::Instance().sav( *Application::Instance().getDatabase(), &TheSelect, Application::Instance().autoSave() )
#define TheAppli M3d::Application::Instance()
#define TheBase  (*TheAppli.getDatabase())
#define TheInput (M3d::Application::Instance().getInput())
#define TheSelect          TheAppli.cSelect
#define TheSelectTransform TheAppli.cSelectTransform
 


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
      CenterRotFacetNorm,
      RotAxis,

      ScaleUniform,
      ScaleX,
      ScaleY,
      ScaleZ,
      ScaleRX,
      ScaleRY,
      ScaleRZ,
      ScaleAxis,
      ScaleNormal,      
      };

  enum class TypeDeform {
    Nothing,
      DeformProportinal,

      };

  enum class InputPlaneType { X, Y, Z, Free };
  
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
    
#ifdef USING_LUA
    M3d::ShapeLua*                        cLua=nullptr;
#endif
    PPu::PPConfig  cMyConfig;
	
  private:			
    Application();
    virtual ~Application();
		
		
    Transform       cCurrentTransform;
    PP3d::Transf3d  cCurrentTransf;

    TypeDeform      cCurrentDeform = TypeDeform::Nothing;
    
    InputPlaneType cInputPlaneType   = InputPlaneType::Y;
    double         cInputPlaneHeight = 0;

    double         cRoundingInput =0; // no rounding if 0

  public:
    static const int sIconBigSize = 64;
    static const int sIconSize = 32;
    static const int sIconSmallSize = 16;

    PPu::PPConfig & getConfig() { return cMyConfig; }

    TypeDeform      getCurrentDeformType() { return cCurrentDeform ; }
    void           setCurrentDeformType( TypeDeform iCurrentDeform) { cCurrentDeform= iCurrentDeform;}
    
    
    void setCurrentTransformType( Transform lTrans)
    {	
      cCurrentTransf.raz();
      cCurrentTransform= lTrans;			
    }
    Transform getCurrentTransformType()   { return cCurrentTransform;}
    PP3d::Transf3d& currentTransform()    { return cCurrentTransf;}

    InputPlaneType getInputPlane()              { return cInputPlaneType; }
    void   setInputPlane(InputPlaneType iPt )   { cInputPlaneType = iPt; }
    double getInputPlaneHeight()                 { return cInputPlaneHeight; }
    void   setInputPlaneHeight( double iHeight ) { cInputPlaneHeight = iHeight; }
    
    double getRoundingInput()                        { return cRoundingInput; }
    void   setRoundingInput( double iRoundingInput ) { cRoundingInput = iRoundingInput; }
    void   roundInput( PP3d::Point3d & ioPt ) {
      if( cRoundingInput !=0 )
	ioPt.round( cRoundingInput );
    }
    
    
    int init( int argc, char* argv[] );

    std::vector< std::unique_ptr<Win3d> >& getWinVector()  { return cAllWin3d; };
		
    PP3d::CurrentInput& getInput() { return cuDatabase->getInput(); }
    PP3d::DataBase* getDatabase() { return cuDatabase.get(); }
    void setDatabase(std::unique_ptr<PP3d::DataBase>&iuBase, bool iRazSelect = true )
    {
      if( iRazSelect )
	cSelect.removeAll();
      
      cuDatabase = std::move(iuBase);
    }
    PP3d::DataBase* getDatabaseTransform() { return cuDatabaseTransform.get(); }

#ifdef USING_LUA
    
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
#endif
		
    Win3d & createNewWin3d( int pW, int pH );
    void    redrawAllCanvas( PP3d::Compute iCompute);
    Win3d*  findCanvas3d( int iId );
    void    changeAllSelectType( PP3d::SelectType pType );
		
    void    createObjectTree( );
    void    redrawObjectTree();
	  
    void redrawAll( PP3d::Compute iCompute )
    {
      redrawAllCanvas( iCompute   );
      redrawObjectTree();
    }
    
    void recomputeAll( PP3d::Compute iCompute ) {
      getDatabase()->recomputeAll( iCompute );
      getDatabaseTransform()->recomputeAll( iCompute );
    }
      
	
    void setCursorPosition( PP3d::Point3d& pPos);

    //---------------------------
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
    //---------------------------
    bool addAxis( PP3d::PointPtr  lCenter, PP3d::PointPtr lVect )
    {
      if(lCenter->get() != lVect->get() )
	{
	  cCurrentAxis = MakeObjectLine( "Axis", lCenter->get(), lVect->get() );
	  cuDatabaseTransform->addObject( cCurrentAxis );
	  redrawObjectTree();
	  return true;
	}      
      return false;
    
    }
    //---------------------------
    bool setCurrentAxis(PP3d::ObjectPtr iObj) {
      if( iObj->getObjType() == PP3d::ObjectType::ObjLine )
	{
	  cCurrentAxis = (PP3d::ObjectLine*) iObj;
	  return true;
	}
      return false;
    }
     //---------------------------
   PP3d::ObjectLine * getCurrentAxis() { return cCurrentAxis; }
    //---------------------------
    bool getAxis( PP3d::Point3d & oPtZero, PP3d::Point3d & oAxis  )
    {
      PP3d::ObjectLine  * lObjAxis = TheAppli.getCurrentAxis();
      if(lObjAxis != nullptr )
	{
	  oPtZero = lObjAxis->getVectorOrigin();
	  oAxis = lObjAxis->getAxis();
	  return true;
	}
      return false;
    }
    //---------------------------
   const char * autoSave();
    //---------------------------
    bool isSelectAxis() {

      //      std::cout << "isSelectAxis:" << TheSelectTransform.getSelectionVect()[0]->getType()  <<  " " << PP3d::ShapeType::Object
      //		<< "   "	<< TheSelectTransform.getSelectionVect()[0]->getType() << " " <<  PP3d::ShapeType::Object << std::endl;
      return (TheSelectTransform.getSelectType() != PP3d::SelectType::Null
	      && TheSelectTransform.getNbSelected() >= 1
	      && TheSelectTransform.getSelectionVect()[0]->getType() == PP3d::ShapeType::Object
	      && ((PP3d::ObjectPtr)TheSelectTransform.getSelectionVect()[0])->getObjType() == PP3d::ObjectType::ObjLine);
    }
    //---------------------------
    void info( const std::string & iStr );  
    
 };
  //************************************

}

   
#endif
