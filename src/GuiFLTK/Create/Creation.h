#ifndef H__Creation__H
#define H__Creation__H

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
#include "GuiFLTK/ShapeLua.h"
#endif

#include "Shape/UndoHistory.h"

#include "Utils/PPSingletonCrtp.h"
#include "Utils/PPConfig.h"

#include "TransformType.h"

#include "GuiFLTK/ImgModel.h"



#define PushHistory() PP3d::UndoHistory::Instance().sav( *Creation::Instance().getDatabase(), &TheSelect, Creation::Instance().autoSave() )
#define TheCreat M3d::Creation::Instance()
#define TheBase  (*TheCreat.getDatabase())
#define TheInput (M3d::Creation::Instance().getInput())
#define TheSelect          TheCreat.cSelect
 


namespace M3d{

  class Win3d;


  enum class TypeDeform {
    Nothing,
      DeformProportinal,
      };

  enum class InputPlaneType { XY, XZ, ZY, Free };
  
  //************************************
  // Creation est un singleton 

  class Creation : public virtual PPSingletonCrtp<Creation> {
    
    friend PPSingletonCrtp<Creation>;

    std::vector< std::unique_ptr<Win3d> > cAllWin3d;
    std::unique_ptr<PP3d::DataBase>       cuDatabase;
   public:
    PP3d::Selection                       cSelect;
  protected:    
      
  protected:    
    std::unique_ptr<PP3d::DataBase>       cuDatabaseTmp; // Objects temporaires !!!!!!!!!!!
   
  public:

    PP3d::Selection                       cSelectTransform;
  protected:    
    //  PP3d::ObjectLine *                    cCurrentAxis;
    PP3d::Vect3d                          cCurrentAxis;

    bool hasCurrentAxis();

#ifdef USING_LUA
    M3d::ShapeLua*                        cLua=nullptr;
#endif
    PPu::PPConfig  cMyConfig;
	
  private:			
    Creation();
    virtual ~Creation();
		
		
    Transform       cCurrentTransform;
    PP3d::Transf3d  cCurrentTransf;
    double          cCurrentValTransf; // pour Le dialogInput

    

    TypeDeform      cCurrentDeform = TypeDeform::Nothing;
    
    InputPlaneType cInputPlaneType   = InputPlaneType::XY;
    double         cInputPlaneHeight = 0;

    double         cRoundingInput =0; // no rounding if 0


  private:
    Magnet           cMagnet;
  public:
    Magnet & getMagnet()   { return cMagnet; }
   

  public:
    static const int sIconBigSize = 64;
    static const int sIconSize = 32;
    static const int sIconSmallSize = 16;

    PPu::PPConfig & getConfig() { return cMyConfig; }

    TypeDeform      getCurrentDeformType() { return cCurrentDeform ; }
    void           setCurrentDeformType( TypeDeform iCurrentDeform) { cCurrentDeform= iCurrentDeform;}
    
    
    void setCurrentTransformType( Transform lTrans)
    {	
      //      cCurrentTransf.raz();
      cCurrentTransform= lTrans;			
    }
    Transform getCurrentTransformType()   { return cCurrentTransform;}
    PP3d::Transf3d& currentTransform()    { return cCurrentTransf;}
    double&         currentValTransf()    { return cCurrentValTransf;}

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
    
    PP3d::DataBase* getDatabaseTmp() { return cuDatabaseTmp.get(); }
    void clearDatabaseTmp() { return cuDatabaseTmp.reset(); }
    void setDatabaseTmp(std::unique_ptr<PP3d::DataBase>&iuBase ) { cuDatabaseTmp = std::move(iuBase);}
    void validateDatabaseTmp();


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
    void    redrawAllCanvas3d( PP3d::Compute iCompute);
    void    redrawAllWin3d( PP3d::Compute iCompute);
    Win3d*  findCanvas3d( int iId );
    void    changeAllSelectType( PP3d::SelectType pType );
		
    void    createObjectTree( );
    void    redrawObjectTree();
	  
    void redrawAll( PP3d::Compute iCompute )
    {
      redrawAllWin3d( iCompute   );
      redrawObjectTree();
    }
    
    void recomputeAll( PP3d::Compute iCompute ) {
      getDatabase()->recomputeAll( iCompute );
      if( getDatabaseTmp() )
        getDatabaseTmp()->recomputeAll( iCompute );
    }
      
	
    void setCursorPosition( PP3d::Point3d& pPos);
    void changeCurrentPath( const std::string & iStr );

    //---------------------------
    bool addAxis( PP3d::Point3d & lA, PP3d::Point3d & lB )
    {
      if( lA != lB )
	{
          PP3d::ObjectLinePtr lAxis = MakeObjectLine( "Axis", lA, lB, PP3d::ClassTypeGeo );
          
	  cuDatabase->addObject( lAxis );
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
	  PP3d::ObjectLinePtr lAxis= MakeObjectLine( "Axis", lCenter->get(), lVect->get(), PP3d::ClassTypeGeo );
          
	  cuDatabase->addObject( lAxis);
	  redrawObjectTree();
	  return true;
	}      
      return false;
    
    }
    //---------------------------
    bool setCurrentAxis(PP3d::EntityPtr iObj);   
    //---------------------------
    bool setCurrentAxis(PP3d::ObjectLinePtr iObj) {
      return setCurrentAxis( iObj->getLine() );
    }
     //---------------------------
    bool setCurrentAxis(PP3d::LinePtr iLine) {
      if( iLine->isSamePointXYZ() )
        {
          return false;
        }
      cCurrentAxis.set( iLine->first()->get(), iLine->second()->get() );
      std::cout << "CurrentAxis:" << cCurrentAxis << std::endl;
      return true;
    }
     //---------------------------
   PP3d::Vect3d & getCurrentAxis() { return cCurrentAxis; }
    //---------------------------
    bool getAxis( PP3d::Point3d & oPtZero, PP3d::Point3d & oAxis  )
    {
      PP3d::Vect3d lAxis = TheCreat.getCurrentAxis();
      
      oPtZero =  lAxis.getA();
      oAxis = lAxis.getB();
      return true; 
   }
    //---------------------------
   const char * autoSave();

    //---------------------------
    void info( const std::string & iStr );


  protected:
     ImgModel * cModelImg = nullptr;
    
  public:
    
    ImgModel * getModelImg() { return cModelImg; }
    void loadModelImage( const std::string & iVal )
    {
      if( cModelImg != nullptr )
        {
          delete cModelImg;
          cModelImg = nullptr;
        }
      std::cout << "CALL TO : Creation loadImage:" <<iVal << std::endl;
 
      cModelImg = new ImgModel(iVal);
      //      cModelImg->loadTexture(iVal.c_str());
    }
 };
  //************************************

}

   
#endif
