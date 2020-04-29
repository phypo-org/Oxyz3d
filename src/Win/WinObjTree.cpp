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

#include "Application.h"


#include "MyFlWidget.h"


#include <string.h>

using namespace PP3d;

namespace M3d {

#define StrMenu_FoldAll   "Fold all"
#define StrMenu_UnfoldAll "Unfold all"
  
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
    virtual void execBeginLine ( Line* pLine) {
      
      std::ostringstream lOstr;      
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
      
      lOstr << pLine->first()->get();
      
      if( pLine->isPoint() == false )
      	lOstr << " --- " <<  pLine->second()->get() ;

      //      cStrLine =  lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( lOstr.str().c_str() );
      if( pLine->isTreeOpen() )
	cTree.open( lNode , 0 );
      else
	cTree.close( lNode , 0 );
      
      lNode->user_data( pLine );	    
    }    
    //-------------------------------------    
    virtual void execBeginFacet( Facet* pFacet )
    {      
      std::ostringstream lOstr;
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
      
      cStrFacet = lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( cStrFacet.c_str() );
      lNode->user_data( pFacet );
      if( pFacet->isTreeOpen() )
	cTree.open( lNode , 0 );
      else
	cTree.close( lNode , 0 );
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
      lOstr << "]";
 
      cStrPoly =lOstr.str();
      Fl_Tree_Item* lNode = cTree.add( cStrPoly.c_str() );
      
      //      cSav =cStrRoot;      

      //	lInput->value( lOstr.tr().c_str() );
      //      std::cout << "TreeVisitor " << "Facet " << lOstr.str().c_str() << std::endl;
      lNode->user_data( pPoly );
      
      if( pPoly->isTreeOpen() )
	cTree.open( lNode , 0 );
      else
	cTree.close( lNode , 0 );
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
    cMenubar = new Fl_Menu_Bar ( 0, 0, 10000, 30);
    int lX = 15;
    int lY = cMenubar->h();
    int lH = (int)(((float)cMenubar->h())*0.6f);
    int lW = 70;
    
    cMenubar->add( "&Fold/"    StrMenu_FoldAll,       "", MyMenuCallback, this);
    cMenubar->add( "&Fold/"    StrMenu_UnfoldAll,     "", MyMenuCallback, this);
    
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
  static void	SelectOneObject(Fl_Widget*w, void*pData)
  {
    MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
	
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

    //	std::cout << "Box:"  << (lCheck->value() != 0) << std::endl;
    if( PP3d::Selection::Instance().addToSelection( lObj->getShape() ) )
      {
	Application::Instance().redrawAllCanvas3d();
      }
  }
  //--------------------------------------------
  static void	ViewOneObject(Fl_Widget*w, void*pData)
  {
    MyCheckbutton* lCheck = reinterpret_cast<MyCheckbutton*>(pData);
		
    //		WinObjTree* lWinTree = reinterpret_cast<WinObjTree*>(lCheck->cUserData1);
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lCheck->cUserData2);

    lObj->setVisible( (lCheck->value() != 0) );
    Application::Instance().redrawAllCanvas3d();
  }
  //--------------------------------------------
  static void	RenameObject(Fl_Widget*w, void*pData)
  {
    MyInput* lInput = reinterpret_cast<MyInput*>(pData);

		
    PP3d::Object*lObj  = reinterpret_cast<PP3d::Object*>(lInput->cUserData2);
		
    std::cout << "Rename " << lInput->value() << std::endl;
    lObj->rename( (lInput->value()) );
    Application::Instance().redrawAllCanvas3d();
  }
  //--------------------------------------------
  void WinObjTree::draw()
  {
    rebuild();
  }		
  //--------------------------------------------
  void WinObjTree::rebuild()
  {
    std::cout << "+++++++ REBUILD TREE ++++++++++" << std::endl;
    cTree->clear();
    int lH = 18;
  
    cTree->begin();
    
    std::ostringstream lOs;
    lOs  << "Base" << " objets:" <<  Application::Instance().getDatabase()->getAllObject().size()
	 << " entities:" <<  Application::Instance().getDatabase()->getEntities().size() ;
    
    Fl_Tree_Item* lNode = cTree->add( lOs.str().c_str() );

    
 
    for( PP3d::Object* lObj : Application::Instance().getDatabase()->getAllObject() )
      {
	std::ostringstream lOstr;
				
	lOstr  << lObj->getStrType() << "/" << lObj->getId()  ;  
	Fl_Tree_Item* lNode = cTree->add( lOstr.str().c_str() );
	lNode->user_data( lObj );
		
	//      std::cout << " Add tree ******************************** >" <<  lOstr.str() << std::endl;
	
	if( lObj->isTreeOpen() )
	  cTree->open( lNode , 0 );
	else
	  cTree->close( lNode , 0 );

   
	//	TreeVisitor lTreeVisitor( *cTree, lOstr.str().c_str() );
	//	lObj->execVisitor( lTreeVisitor );
      
			
	Fl_Group* lGroup = new Fl_Group(100,100,140,18);
	lGroup->color( FL_WHITE );
	lGroup->begin();

      
	std::ostringstream lOstrId;
	lOstrId  << lObj->getId();
      
	int lX = lGroup->x();
	int lY = lGroup->y()+2;
	MyLabel *lLabel = new MyLabel( lX, lY, 60, lH , lOstrId.str() );

	lX += lLabel->w()+4;

   
	//------------				
	MyCheckbutton *lCheckSel = new MyCheckbutton( lX, lY, 30,15, "S", SelectOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
	lCheckSel->value( PP3d::Selection::Instance().isSelected( lObj->getShape() ));
	lX += lCheckSel->w()+4;


	//------------
	MyCheckbutton *lCheckView = new MyCheckbutton( lX, lY, 30,15, "V", ViewOneObject, this, lObj ); //lInput->x()+lInput->w()+4+40 ,
	lCheckView->value( lObj->isVisible());
	lX += lCheckView->w()+4;
				

	//------------
				
	MyInput* lInput = new MyInput( lX, lY, 100, lH, "", RenameObject, this, lObj );
	lInput->value( lObj->getName().c_str() );
			
	//------------

				
	lGroup->end();
	lGroup->resizable(lGroup);

               
	lGroup->show();
	lNode->widget(lGroup);

      }
    cTree->end();
 
    std::cout << "------- END REBUILD TREE ---------" << std::endl;

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
