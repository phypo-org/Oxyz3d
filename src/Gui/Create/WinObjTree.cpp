#include "WinObjTree.h"

#include <FL/Fl_Group.H>


#include <FL/Fl_Widget.H>

#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>


#include "Shape/Entity.h"
#include "Shape/Selection.h"
#include "Shape/EntityVisitor.h"

#include "Creation.h"


#include "Gui/MyFlWidget.h"
#include "Gui/MyImage.h"


#include <string.h>

#include "Gui/Preference.h"

using namespace PP3d;

namespace M3d {

#define StrMenu_FoldAll   "Fold all"
#define StrMenu_UnfoldAll "Unfold all"


  //****************************************************
  static void DeployGraphCB( Fl_Widget *w, void*pData )
  {
    MyToggleButton * lToggle = reinterpret_cast<MyToggleButton*>( pData);
    WinObjTree     * lWin  = reinterpret_cast<WinObjTree*>( lToggle->cUserData1);

    lWin->cDeployDetail = ! lWin->cDeployDetail;
    Creation::Instance().redrawObjectTree();
  }

  //****************************************************
  class TreeVisitor : public EntityVisitor {
		
    Fl_Tree   & cTree;
    std::string cStrRoot;
    
    std::string cStrPoly;
    std::string cStrFacet;
    //   std::string cStrLine;

  public:
    TreeVisitor(  Fl_Tree   & iTree, const char* iRootStr )
      :cTree(iTree)
      ,cStrRoot(iRootStr)
    {
    }
    
    virtual void execPoint( Point* pPoint )
    {
      //     std::ostringstream lOstr;      
      //  lOstr << cStrLine << "/" <<  pPoint->get();
      // Fl_Tree_Item* lINode = cTree.add( lOstr.str().c_str() );
    }   
    //-------------------------------------
    void printOwner( std::ostringstream & lOstr, Entity & iEntity )
    {
      //===========
      //      if( iEntity.howManyOwner() == 0 )
      //	return ;
      
      
      if(  iEntity.howManyOwner() > 2 )
	{
	  lOstr << '<' << iEntity.howManyOwner() << ">";
	}
      /*
      for(  Entity * lOwner : iEntity.getOwners() )
	{
	      lOstr << *lOwner << " " ;
	}
      lOstr << '>';
      */
    }
    //-------------------------------------        
    virtual void execBeginLine ( Line* pLine) {
      
      std::ostringstream lOstr;      
      if( cStrFacet.size() == 0 )
	lOstr << cStrRoot;

      lOstr << cStrFacet << "/Line " << pLine->getId();

       if( pLine->isSelect() )
	{
	  lOstr << 's';
	} 

      lOstr << " [ "
	    << pLine->first()->getId();
      if( pLine->first()->isSelect() )
	{
	  lOstr << 's';
	}

      if( pLine->isPoint() == false )
	{
	  lOstr << ", " <<  pLine->second()->getId() ;
	  if( pLine->second()->isSelect() )
	    {
	      lOstr << 's';
	    }
	}
      lOstr << "]  ";

      //      lOstr << " o" << pLine->getOwners().size() ;

      printOwner( lOstr, *pLine);
	
      lOstr << pLine->first()->get();
      printOwner( lOstr, *(pLine->first()));

      if( pLine->isPoint() == false )
	{
	  lOstr << " --- " <<  pLine->second()->get() ;
	  printOwner( lOstr, *(pLine->second()));
	}

      //      cStrLine =  lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( lOstr.str().c_str() );
      if( lNode == nullptr )
        {
          std::cerr << "WinObjTree::execBeginLine  cTree.add for " <<  lOstr.str() << "return null" << std::endl;
          lNode = cTree.add( "no name" );         
        }
      if( lNode != nullptr )
        {
          if( pLine->isTreeOpen() )
            cTree.open( lNode , 0 );
          else
            cTree.close( lNode , 0 );
          
          lNode->user_data( pLine );
        }
    }    
    //-------------------------------------    
    virtual void execBeginFacet( Facet* pFacet )
    {      
      std::ostringstream lOstr;
      if( cStrPoly.size() == 0 )
	lOstr << cStrRoot;
      
      lOstr << cStrPoly << "/Facet " << pFacet->getId();
      if( pFacet->isSelect() )
	{
	  lOstr << 's';
	}

      lOstr  <<  " : " << pFacet->getNbLines() << "[ " ;
      
      for( Line* lLine: pFacet->getLines() )
	{
	  lOstr << lLine->getId() ;
	  if( lLine->isSelect() )
	    {
	      lOstr << 's';
	    }
	  lOstr << "  ";
	}
      lOstr << "]";
      
      printOwner( lOstr, *pFacet);
      
      cStrFacet = lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( cStrFacet.c_str() );
      if( lNode == nullptr )
        {
          std::cerr << "WinObjTree::execBeginFacet  cTree.add for " << cStrFacet << "return null" << std::endl;
          lNode = cTree.add( "no name" );         
        }
      if( lNode != nullptr )
        {
          lNode->user_data( pFacet );
        
          if( pFacet->isTreeOpen() )
            cTree.open( lNode , 0 );
          else
            cTree.close( lNode , 0 );
        }
    }    
    //-------------------------------------
    virtual void execBeginPoly ( Poly* pPoly )
    {
      std::ostringstream lOstr;
      lOstr << cStrRoot << "/Poly " <<  pPoly->getId() << " : " << pPoly->getFacets().size() << "[ ";
       	
      for( Facet* lFac: pPoly->getFacets() )
	{
	  lOstr << lFac->getId() ;
	  if( lFac->isSelect() )
	    {
	      lOstr << 's';
	    }
	  lOstr << ", ";
	}
      lOstr << "] ";

      printOwner( lOstr, *pPoly);

      
      cStrPoly =lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( cStrPoly.c_str() );
      
      //      cSav =cStrRoot;      

      //	lInput->value( lOstr.tr().c_str() );
      //      std::cout << "TreeVisitor " << "Facet " << lOstr.str().c_str() << std::endl;
      if( lNode == nullptr )
        {
          std::cerr << "WinObjTree::execBeginPolyt  cTree.add for " << cStrPoly << "return null" << std::endl;
          lNode = cTree.add( "no name" );         
        }
      if( lNode != nullptr )
        {
          lNode->user_data( pPoly );
              
          if( pPoly->isTreeOpen() )
            cTree.open( lNode , 0 );
          else
            cTree.close( lNode , 0 );
        }
    }

  };
  //****************************************************	

  WinObjTree* WinObjTree::sTheWinObjTree =nullptr;

  //****************************************************
  WinObjTree::WinObjTree( )
  {
    int lSzX=400;
    int lSzY =580;
    cWin     = new Fl_Double_Window( lSzX, lSzY, "Oxyd3d : Entity Tree");
    cMenubar = new Fl_Menu_Bar ( 0, 0, 200, 32);
    int lX = 15;
    int lY = cMenubar->h();
    int lH = (int)(((float)cMenubar->h())*0.6f);
    int lW = 32;
    
    cMenubar->add( "&Fold/"    StrMenu_FoldAll,       "", MyMenuCallback, this);
    cMenubar->add( "&Fold/"    StrMenu_UnfoldAll,     "", MyMenuCallback, this);

    lX = 200;
    //========================		
    Fl_Image* lPixDeploy = MyImage::LoadImage( ICON_PATH "/deploy_graph.png", Creation::sIconSmallSize);

    MyToggleButton*
      lButDeploy = new MyToggleButton( lX, 0, Creation::sIconSmallSize,
				       Creation::sIconSmallSize,  nullptr,
				       DeployGraphCB, this, (void*)0 );
    lButDeploy->value(cDeployDetail);
    lButDeploy->image( lPixDeploy );
    lX += lW;

    //========================		
 
    
    
    cTree    = new Fl_Tree( 0, lY, lSzX-10, lSzY-30, "");
		
    cTree->callback((Fl_Callback*)CallBackTree, (void*)(this));
    cWin->end();
    cWin->resizable(*cWin);
    cWin->show( 0, nullptr);
    //   Fl::run();
  }
  //--------------------------------------------
  void WinObjTree::show()
  {
    cWin->show();
  }

  //--------------------------------------------
  static void	CB_SelectOneObject(Fl_Widget*w, void*pData)
  {
    MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
	
    if( TheSelect.getSelectType() != PP3d::SelectType::Object )
      {
	lCheck->value( false );
	return ;
      }
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

    //	std::cout << "Box:"  << (lCheck->value() != 0) << std::endl;
    if( lObj != nullptr )
      if( TheSelect.addToSelection( lObj ))
      {
	Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);
      }
  }
  //--------------------------------------------
  static void	CB_ViewOneObject(Fl_Widget*w, void*pData)
  {
    MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
		
    //		WinObjTree* lWinTree = reinterpret_cast<WinObjTree*>(lCheck->cUserData1);
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

    lObj->setVisible( (lCheck->value() != 0) );
    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);
  }
  //--------------------------------------------
  static void	CB_RenameObject(Fl_Widget*w, void*pData)
  {
    MyInput* lInput = reinterpret_cast<MyInput*>(pData);

		
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lInput->cUserData2);
		
    DBG_TREE( "Rename " << lInput->value() );
    lObj->rename( (lInput->value()) );
    Creation::Instance().redrawAllCanvas3d(PP3d::Compute::Nothing);
  }
  //--------------------------------------------
  void WinObjTree::draw()
  {
    rebuild();
  }
  //--------------------------------------------
  inline static const char* GetTransformStr( ObjectType pType )
  {
    switch( pType )
      {
      case ObjectType::ObjLine:   return "Axis";
      case ObjectType::ObjFacet:  return "Plane";
      default: ;
      }
    return "Unknown";
  }
 //--------------------------------------------
  void WinObjTree::rebuildDatabase( const char* iRootname,
				    PP3d::DataBase & iDatabase,
				    bool iFlagAxe,
                                    PP3d::ClassType iClassTypeFilter)
  {       
    int lH = 18;

  
    if( iFlagAxe == false )
      {
       std::ostringstream lOs;
       lOs << "Group(" << iDatabase.getGroups().size() << "):";
       for( const GroupPtr lGroup: iDatabase.getGroups())
         lOs << ' ' << lGroup->getGroupId() << ':'<<lGroup->values().size();

       Fl_Tree_Item* lNode = cTree->add( lOs.str().c_str() );
      }
  
    if( iDatabase.getEntities().size() )
    {
      std::ostringstream lOs;
      lOs  << iRootname << " objects:" <<  iDatabase.getAllObject().size()
	   << " entities:" <<  iDatabase.getEntities().size()  ;
      
      Fl_Tree_Item* lNode = cTree->add( lOs.str().c_str() );
    }

    
 
    for( PP3d::Object* lObj : iDatabase.getAllObject() )
      {
        if( lObj->isClassType(iClassTypeFilter) == false )
          continue;
        
	std::ostringstream lOstr;

				
	lOstr   << iRootname
		<< ( iFlagAxe ? GetTransformStr(lObj->getObjType()) : PP3d::GetStrObjectType( lObj->getObjType())) << "/"
		<< lObj->getId() ;

        GroupPtr lGroupObj = lObj->getGroup();
        if( lGroupObj != nullptr )
          {
            lOstr << " grp:" << lGroupObj->getGroupId();
          }
	
	Fl_Tree_Item* lNode = cTree->add( lOstr.str().c_str() );
	
	lNode->user_data( lObj );
		
	//      std::cout << " Add tree ******************************** >" <<  lOstr.str() << std::endl;
	
	if( lObj->isTreeOpen() )
	  cTree->open( lNode , 0 );
	else
	  cTree->close( lNode , 0 );

	if( cDeployDetail )
	  {
	    TreeVisitor lTreeVisitor( *cTree, lOstr.str().c_str() );
	    lObj->execVisitor( lTreeVisitor );
	  }
			
	Fl_Group* lGroup = new Fl_Group(100,100,140,18);
	lGroup->color( FL_WHITE );
	lGroup->begin();

            
	int lX = lGroup->x();
	int lY = lGroup->y()+2;
	
	//------------				
	MyCheckbutton *lCheckSel  = new MyCheckbutton( lX, lY, 30,15, "S", CB_SelectOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
	//	lCheckSel->value( TheSelect.isSelected( lObj->getShape() ));
	lCheckSel->value( lObj->isSelect() || lObj->getShape()->isSelect() );
	if( TheSelect.getSelectType() != PP3d::SelectType::Object )
	  {
	    ////////  BUG : Cycle sans fin -   lCheckSel->deactivate();
	  }
	
	lX += lCheckSel->w()+4;

	//------------
	MyCheckbutton *lCheckView = new MyCheckbutton( lX, lY, 30,15, "V", CB_ViewOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
	lCheckView->value( lObj->isVisible());
	lX += lCheckView->w()+4;				

	//------------				
	MyInput* lInput = new MyInput( lX, lY, 100, lH, "", CB_RenameObject, this, lObj );
	lInput->value( lObj->getName().c_str() );			
	//------------
	lX += lInput->w()+4;				

	std::ostringstream lOstrId;
	lOstrId  << lObj->getId() << " - "
		 <<  PPu::PPDate::ConvertTimeToStr( lObj->getDateCreation(),  PPu::PPDateUTC::Local, true, true, '-' );

	MyLabel *lLabel = new MyLabel( lX, lY, 150, lH , lOstrId.str() );
	lX += lLabel->w()+4;
	
				
	lGroup->end();
	lGroup->resizable(lGroup);

               
	lGroup->show();
	lNode->widget(lGroup);
      }	
  }
 //--------------------------------------------
  void WinObjTree::rebuild()
  {
    DBG_TREE( "+++++++ REBUILD TREE ++++++++++" );
    cTree->clear();

    cTree->begin();    

    rebuildDatabase( "Geometries/",   *Creation::Instance().getDatabase(),    true,  PP3d::ClassTypeGeo );
    rebuildDatabase( "Objects/",      *Creation::Instance().getDatabase(),  false, PP3d::ClassTypeObj );

    cTree->end();
 
    DBG_TREE( "------- END REBUILD TREE ---------" );

    cTree->redraw();
  }

  //--------------------------------------------
  void WinObjTree::CallBackTree( Fl_Widget* pWidget, void* pUserData )
  {
    Fl_Tree*    lTree = dynamic_cast<Fl_Tree*>( pWidget );
    WinObjTree* lWoT  = (WinObjTree*)pUserData;
  
    Fl_Tree_Item *lItem = lTree->callback_item();
    if( lItem != nullptr )
      {
	Entity* lEntity = (Entity*)lItem->user_data();
	if( lEntity != nullptr )
	  {
	    switch( lTree->callback_reason() )
	      {
	      case FL_TREE_REASON_OPENED: lEntity->setTreeOpen(true); break;
	      case FL_TREE_REASON_CLOSED:  lEntity->setTreeOpen(false); break;
	      case FL_TREE_REASON_SELECTED: ;
	      case FL_TREE_REASON_DRAGGED: ;
	      case FL_TREE_REASON_DESELECTED: ;
	      case FL_TREE_REASON_NONE:;
	      }
	  }
      }
    //		WinObjTree* lWinTree = reinterpret_cast<WinObjTree*>( pUserData );
  }
  //-------------------------------------------
  void WinObjTree::MyMenuCallback(Fl_Widget* w, void* pUserData)
  {
    static bool slFlagDialog=false; // C'est moche !!!
    
    Fl_Menu_* mw = (Fl_Menu_*)w;
    const Fl_Menu_Item* m = mw->mvalue();		
    if (!m)
      {
	printf("NULL\n");
	return ;
      } 

    printf("%s\n", m->label());
    WinObjTree* lWTree = reinterpret_cast<WinObjTree*>(pUserData);
   
    if( strcmp( m->label(),StrMenu_FoldAll) == 0)
      {
	
      }
    else
      if( strcmp( m->label(),StrMenu_UnfoldAll) == 0)
	{
	  
	}
   
  }

  //****************************************************
	
}
