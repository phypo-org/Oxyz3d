#include "Canvas3d.h"

//#include <config.h>
#include <FL/Fl.H>


#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>


#include "Service.h"


#include <iostream>
#include <memory>
#include <sstream>


#include "Shape/ObjectLine.h"
#include "Shape/ObjectPoly.h"
#include "Shape/GLUtility.h"
#include "Shape/PrimitivFactory.h"
#include "Shape/Light.h"
#include "Shape/SortVisitor.h"
#include "Shape/ViewProps.h"
#include "Shape/Selection.h"
#include "Shape/SubDiv.h"
#include "Shape/SavRead.h"

#include "Modif/Modif.h"


#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"

using namespace std;


namespace M3d {

	
#define StrMenu_CreateFacet     "Facet      ..."
#define StrMenu_CreateCylinder  "Cylinder   ..."
#define StrMenu_CreateSphere    "Sphere     ..."
#define StrMenu_CreatePlane     "Plane      ..."
#define StrMenu_CreateCube      "Cube       ..."
#define StrMenu_CreateTetra     "Tetraede   ..."
#define StrMenu_CreatePyramid   "Pyramide   ..."
#define StrMenu_CreateOcto      "Octoedre   ..."
#define StrMenu_CreateDodec     "Dodecaedre ..."
#define StrMenu_CreateIcosahe   "Icosaedre  ..."
	
#define StrMenu_CreateShape          "New Shape "
#define StrMenu_CreateShapeLine      StrMenu_CreateShape "Line"
#define StrMenu_CreateShapePolyline  StrMenu_CreateShape "Polyline"
#define StrMenu_CreateShapeFacet     StrMenu_CreateShape "Facet"


#define StrMenu_Revol     "New Revol "
#define StrMenu_RevolX    StrMenu_Revol "X ..."
#define StrMenu_RevolY    StrMenu_Revol "Y ..."
#define StrMenu_RevolZ    StrMenu_Revol "Z ..."
  	
#define StrMenu_CallDialoDiagSub     "New Subdivide ..."

#define StrMenu_Demo1            "Demo 1"
#define StrMenu_Demo2            "Demo 2"

  
#define StrMenu_Move     "Move"
#define StrMenu_MoveX    "Move X"
#define StrMenu_MoveY    "Move Y"
#define StrMenu_MoveZ    "Move Z"
#define StrMenu_MoveNormal "Move normal"
#define StrMenu_MoveAxis   "Move axis"

#define StrMenu_Rot      "Rotate"
#define StrMenu_RotX     "Rotate X"
#define StrMenu_RotY     "Rotate Y"
#define StrMenu_RotZ     "Rotate Z"
#define StrMenu_RotAxis  "Rotate around current axis"

#define StrMenu_Scale      "Scale"
#define StrMenu_ScaleU     "Scale uniform"
#define StrMenu_ScaleX     "Scale X"
#define StrMenu_ScaleY     "Scale Y"
#define StrMenu_ScaleZ     "Scale Z"
#define StrMenu_ScaleRX    "Scale radial X"
#define StrMenu_ScaleRY    "Scale radial Y"
#define StrMenu_ScaleRZ    "Scale radial Z"
#define StrMenu_ScaleAxis  "# Scale around current axis"

	
#define StrMenu_Dup        "Duplicate"	
#define StrMenu_DupInPlace  StrMenu_Dup  " in place"
#define StrMenu_DupMoveX    StrMenu_Dup  " and move X"
#define StrMenu_DupMoveY    StrMenu_Dup  " and move Y"
#define StrMenu_DupMoveZ    StrMenu_Dup  " and move Z"
#define StrMenu_DupNormal   StrMenu_Dup  " and move normal"

#define StrMenu_DupRotX     StrMenu_Dup  " and rotate X"
#define StrMenu_DupRotY     StrMenu_Dup  " and rotate Y"
#define StrMenu_DupRotZ     StrMenu_Dup  " and rotate Z"
#define StrMenu_DupRotAxis  StrMenu_Dup  " and rotate around current axis"

#define StrMenu_Extrude     "Extrude"
#define StrMenu_ExtrudeX    StrMenu_Extrude  " X"
#define StrMenu_ExtrudeY    StrMenu_Extrude  " Y"
#define StrMenu_ExtrudeZ    StrMenu_Extrude  " Z"
#define StrMenu_ExtrudeNorm StrMenu_Extrude  " normal"
#define StrMenu_ExtrudeTrans StrMenu_Extrude  " current transformation"

#define  StrMenu_Cut "Cut"
#define  StrMenu_Cut2 "  2"
#define  StrMenu_Cut3 "  3"
#define  StrMenu_Cut4 "  4"
#define  StrMenu_Cut5 "  5"
#define  StrMenu_Cut6 "  6"
#define  StrMenu_Cut7 "  7"
#define  StrMenu_Cut8 "  8"
#define  StrMenu_Cut9 "  9"
#define  StrMenu_Cut10 " 10"
#define  StrMenu_CutN "-----  N"


#define StrMenu_ConnectPoint "Connect Points"
#define StrMenu_Subdivide             "Subdivide"
 
#define StrMenu_SubdivideCatmullClark      "Smooth"
#define StrMenu_Triangulate           " angles "
#define StrMenu_Triangulate1          " angles + central facette+ marges "
#define StrMenu_Triangulate2          " segments"
#define StrMenu_Triangulate3          " segments + central facette"
#define StrMenu_Triangulate4          " segments + central facette + marges"
  
#define StrMenu_SubdivideSpike        "Spike"
#define StrMenu_SubdivideCatmullClarkFalse "Small spike"
  //#define StrMenu_SubdivideCentralSpike "Spike  central facet (Bug!)"
#define StrMenu_SubdivideFold        "Fold"
//#define StrMenu_SubdivideCentralFold "Fold  central facet (Bug!)"

  //-------------------------------------------
  void  Canvas3d::makeMenu(Fl_Menu_Button& pMenu)
  {
    if( TheSelect.getNbSelected() > 0 )
      {										
	makeMenuSelect(  *cPopup);
      }
    else
      {
	makeMenuPrimitiv( *cPopup );
      }
  }
  //-------------------------------------------
 
  //-------------------------------------------
  //
  void  Canvas3d::makeMenuSelect(Fl_Menu_Button& pMenu)
  {
    
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveX, "", MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveY, "", MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveZ, "", MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveNormal, "", MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    if( TheAppli.getCurrentAxis() )
      pMenu.add( StrMenu_Move  "/" StrMenu_MoveAxis, "", MyMenuCallbackSelect, this, FL_MENU_DIVIDER);

    pMenu.add( StrMenu_Rot  "/" StrMenu_RotX, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotY, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    if( TheAppli.getCurrentAxis() )
      pMenu.add( StrMenu_Rot  "/" StrMenu_RotAxis, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
		
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleU, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleX, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleY, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRX, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRY, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    if( TheAppli.getCurrentAxis() )
	pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleAxis, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);

    if(  TheSelect.getSelectType() == PP3d::SelectType::Object )
      {
	pMenu.add( StrMenu_Dup "/" StrMenu_DupInPlace, "", MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveX, "", MyMenuCallbackSelect, this);
	pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveY, "", MyMenuCallbackSelect, this);
	pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveZ, "", MyMenuCallbackSelect, this);
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotX, "",  MyMenuCallbackSelect, this);
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotY, "",  MyMenuCallbackSelect, this);
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);	
      }
    
    if(  TheSelect.getSelectType() == PP3d::SelectType::Point
	 && TheSelect.getNbSelected() > 1 )
      {
	pMenu.add( StrMenu_ConnectPoint, "c", MyMenuCallbackConnectPoint, this);	
      }
    
    if(  TheSelect.getSelectType() != PP3d::SelectType::Point)
      {
	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut2, "", MyMenuCallbackCutLine, this);
  	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut3, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut4, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut5, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut6, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut7, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut8, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut9, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut10, "", MyMenuCallbackCutLine, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_CutN, "", MyMenuCallbackCutLine, this);
      }

    if(  (TheSelect.getSelectType() != PP3d::SelectType::Point
	  && TheSelect.getSelectType() != PP3d::SelectType::Line )
	  && TheSelect.getNbSelected() > 0)
      {
	pMenu.add( StrMenu_SubdivideCatmullClark, "", MyMenuCallbackSubdiveCatmullClark, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate,  "", MyMenuCallbackSubdivide1, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate1, "", MyMenuCallbackSubdivide1, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate2, "", MyMenuCallbackSubdivide2, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate3, "", MyMenuCallbackSubdivide2, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate4, "", MyMenuCallbackSubdivide2, this, FL_MENU_DIVIDER);
	//	pMenu.add( StrMenu_TriangulateCentral, "", MyMenuCallbackSubdivide, this);
	
	pMenu.add( StrMenu_Subdivide "/" StrMenu_SubdivideCatmullClarkFalse, "", MyMenuCallbackSubdiveCatmullClark, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_SubdivideSpike, "", MyMenuCallbackSubdivide, this);
	//	pMenu.add( StrMenu_Subdivide "/" StrMenu_SubdivideCentralSpike, "", MyMenuCallbackSubdivide, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_SubdivideFold, "", MyMenuCallbackSubdivide, this);
	//	pMenu.add( StrMenu_Subdivide "/" StrMenu_SubdivideCentralFold, "", MyMenuCallbackSubdivide, this); 
      }
   
    switch( TheSelect.getSelectType() )
      {
      case PP3d::SelectType::Point :	
      case PP3d::SelectType::Line :
      case PP3d::SelectType::Object :
      case PP3d::SelectType::Poly :
	pMenu.add( StrMenu_Dup "/" StrMenu_DupNormal, "", MyMenuCallbackSelect, this);
          [[fallthrough]]; 
      case PP3d::SelectType::Facet :	
	{
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeX, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeY, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeZ, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeNorm, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeTrans, "", MyMenuCallbackExtrude, this);
	}
	break;
      case PP3d::SelectType::All :
      default:;
      }
  }
  //-------------------------------------------
  // If no entity is selected
  void  Canvas3d::makeMenuPrimitiv(Fl_Menu_Button& pMenu)
  {
    pMenu.add( StrMenu_CreateFacet,    "^c",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateCylinder, "y",   MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateSphere,   "s",   MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreatePlane,    "",   MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateCube ,    "^c",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateTetra,    "^t",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreatePyramid,  "^p",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateOcto,     "^o",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateDodec,    "^d",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_CreateIcosahe,  "^i",  MyMenuCallbackPrimitiv,   this, FL_MENU_DIVIDER);

    // Ajouter sphere (2 types differents)
    // Ajouter cone
    // Ajouter cylindre (revolution) (disque,  anneau si troue)
    // Ajouter nappe
    // Ajout anneau
    // Ajout helice 
    // ... Ajout script lua !!! plugin
    
    if(TheAppli.getDatabase()->isCurrentPoints()
	&&TheAppli.getDatabase()->getNbCurrentPoints() == 2 )
      {
	pMenu.add(StrMenu_CreateShapeLine, "", MyMenuCallbackPrimitiv,this);
      }
    
    if(TheAppli.getDatabase()->isCurrentPoints()
	&&TheAppli.getDatabase()->getNbCurrentPoints()  >= 3 )
      {
	pMenu.add(StrMenu_CreateShapeFacet, "", MyMenuCallbackPrimitiv, this);
      }
    
    if(TheAppli.getDatabase()->isCurrentPoints()
	&&TheAppli.getDatabase()->getNbCurrentPoints() >= 2 )
      {
	pMenu.add(StrMenu_CreateShapePolyline, "", MyMenuCallbackPrimitiv,this, FL_MENU_DIVIDER);
      }

    pMenu.add( StrMenu_RevolX, "^x", MyMenuCallbackPrimitiv, this);
    pMenu.add( StrMenu_RevolY, "^y", MyMenuCallbackPrimitiv, this);
    pMenu.add( StrMenu_RevolZ, "^z", MyMenuCallbackPrimitiv, this, FL_MENU_DIVIDER);

    pMenu.add( StrMenu_CallDialoDiagSub, "^b",MyMenuCallbackPrimitiv, this, FL_MENU_DIVIDER);

  }
  //-------------------------------------------
  //-------------------------------------------
 //-------------------------------------------
  #define BEGINCALL \
   static bool slFlagDialog=false; \
		\
    Fl_Menu_* mw = (Fl_Menu_*)w;\
    const Fl_Menu_Item* m = mw->mvalue();		\
    if (!m)\
      {\
	printf("NULL\n");\
			\
	return ;\
      }\
\
    printf("%s\n", m->label());\
    M3d::Canvas3d* lCanvas = reinterpret_cast<M3d::Canvas3d*>(pUserData);\
  

  //-------------------------------------------

  void Canvas3d::MyMenuCallbackPrimitiv(Fl_Widget* w, void* pUserData)
  {		
    BEGINCALL  
				
    if( strcmp( m->label(), StrMenu_CreateFacet ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::FACET_N  );
      }
    if( strcmp( m->label(), StrMenu_CreateCylinder ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::CYLINDER  );
      }
    if( strcmp( m->label(), StrMenu_CreateSphere ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::SPHERE  );
      }
    if( strcmp( m->label(), StrMenu_CreatePlane ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::PLANE  );
      }
    else if( strcmp( m->label(), StrMenu_CreateCube ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::CUBE  );
      }
    else if( strcmp( m->label(), StrMenu_CreateTetra ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::TETRA  );
      }
    else if( strcmp( m->label(), StrMenu_CreatePyramid ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::PYRAMID  );
      }
    else if( strcmp( m->label(), StrMenu_CreateOcto ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::OCTO  );
      }
    else if( strcmp( m->label(), StrMenu_CreateDodec ) == 0)
      {
	CallDialogPrimitiv( PP3d::PrimitivFactory::Type::DODEC  );
      }
    else if( strcmp( m->label(), StrMenu_CreateIcosahe ) == 0)
      {
	CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::ICOSAHED  );
      }
    //===================== Shape ==============
    else if( strncmp( m->label(), StrMenu_CreateShape, strlen(StrMenu_CreateShape)  ) == 0)
      {
	PP3d::Object* lShape=nullptr;
	//-----------------
	if( strcmp( m->label(), StrMenu_CreateShapeFacet ) == 0)
	  {
	    if( TheAppli.getDatabase()->getNbCurrentPoints() >= 3 )
	      {
		lShape =TheAppli.getDatabase()->convertCurrentLineToFacet();
	      }
	  }
	//-----------------
	else if( strcmp( m->label(), StrMenu_CreateShapePolyline ) == 0)
	  {
	    if(TheAppli.getDatabase()->getNbCurrentPoints() >= 2 )
	      {
		lShape =TheAppli.getDatabase()->convertCurrentLineToPolylines();
	      }
	  } 
	//-----------------
	else if( strcmp( m->label(), StrMenu_CreateShapeLine ) == 0)
	  {
	    if( TheAppli.getDatabase()->getNbCurrentPoints() == 2 )
	      {
		lShape =TheAppli.getDatabase()->convertCurrentLineToLine();
	      }
	  }
	
	if( lShape != nullptr )
	  {
	    //						lCanvas->Application::Instance().getDatabase()_>addObject( new PP3d::Object3d( lShape, PP3d::Object3d::GetNewObjecId(), lShape->getClassName() ));
	    PushHistory();	    
	    TheAppli.redrawAll();
	  }
      }
    //===================== Revolution ==============
    else if( strncmp( m->label(), StrMenu_Revol, strlen(StrMenu_Revol)  ) == 0)
      {
	if( strcmp( m->label(), StrMenu_RevolX )  == 0)
	  {
	    CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolX  );
	  }
	else if( strcmp( m->label(), StrMenu_RevolY )  == 0)
	  {
	    CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolY  );
	  }

	else if( strcmp( m->label(), StrMenu_RevolZ)  == 0)
	  {
	    CallDialogRevol( slFlagDialog, lCanvas, TypeRevol::RevolZ  );
	  }									
      }
    //========================================
    else if( strcmp( m->label(), StrMenu_CallDialoDiagSub ) == 0)
      {
	CallDialogSubDiv( slFlagDialog, lCanvas );
      }    
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackExtrude(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL ;
    PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    
    std::vector<PP3d::EntityPtr> lNewFacets;
    
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL, 0.1 ))
      {
	TheSelect.removeAll();
	TheSelect.addGoodEntityFor(lNewFacets);  
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	
	//============== TRANSFORMATION ====================
	if( strcmp( m->label(), StrMenu_ExtrudeX ) == 0)
	  {
	    Application::Instance().setCurrentTransformType(Transform::MoveX);
	  }
	else if( strcmp( m->label(), StrMenu_ExtrudeY ) == 0)
	  {
	    Application::Instance().setCurrentTransformType(Transform::MoveY);
	  }
	else if( strcmp( m->label(), StrMenu_ExtrudeZ ) == 0)
	  {
	    Application::Instance().setCurrentTransformType(Transform::MoveZ);
	  }
	else if( strcmp( m->label(), StrMenu_ExtrudeNorm) == 0)
	  {
	    Application::Instance().setCurrentTransformType(Transform::MoveNormal);
	  }
	else if( strcmp( m->label(), StrMenu_ExtrudeTrans ) == 0)
	  {
	    Application::Instance().setCurrentTransformType(Transform::MoveAxis);
	  }
      }
  }
  //-------------------------------------------
  static bool DuplicateSelection()
  {    
    PP3d::SortEntityVisitor  lVisit;    
    TheSelect.execVisitorOnlyOnObjects( lVisit );
    
    std::stringstream lDupStr;
    PP3d::MySav lSav( lDupStr );				
    bool lRet = lSav.save( *TheAppli.getDatabase(), &lVisit.cSetAllEntity );
    
    if( lRet )
      {
	std::vector<PP3d::EntityPtr> lNewObjs;
  	PP3d::MyRead lRead( lDupStr, &lNewObjs );
	
	    		
	lRet = lRead.read( *TheAppli.getDatabase(), false );
	if( lRet && lNewObjs.size() )
	  {	    
	    TheSelect.removeAll();
	    for( PP3d::EntityPtr lObj : lNewObjs )
	      {
		TheSelect.addEntity( lObj );
	      }
	  }
      }
    return lRet;
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSelect(Fl_Widget* w, void* pUserData)
  {	  		
     BEGINCALL  

		
    //============== TRANSFORMATION ====================
    // MOVE
    if( strcmp( m->label(), StrMenu_MoveX ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveX);
      }
    else if( strcmp( m->label(), StrMenu_MoveY ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveY);
      }
    else if( strcmp( m->label(), StrMenu_MoveZ ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveZ);
      }
    else if( strcmp( m->label(), StrMenu_MoveNormal ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveNormal);
      }
    else if( strcmp( m->label(), StrMenu_MoveAxis ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveAxis);
      }
     // ROTATE
    else if( strcmp( m->label(), StrMenu_RotX ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotX);
      }
    else if( strcmp( m->label(), StrMenu_RotY ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType( Transform::CenterRotY );
      }
    else if( strcmp( m->label(), StrMenu_RotZ ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotZ );
      }    
    else if( strcmp( m->label(), StrMenu_RotAxis ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotAxis );
      }
     // SCALE
    else if( strcmp( m->label(), StrMenu_ScaleU ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleUniform );
      }
     else if( strcmp( m->label(), StrMenu_ScaleX ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleX );
      }
    else if( strcmp( m->label(), StrMenu_ScaleY ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleY );
      }
    else if( strcmp( m->label(), StrMenu_ScaleZ ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleZ );
      }
     else if( strcmp( m->label(), StrMenu_ScaleRX ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleRX );
      }
    else if( strcmp( m->label(), StrMenu_ScaleRY ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleRY );
      }
    else if( strcmp( m->label(), StrMenu_ScaleRZ ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleRZ );
      }
    else if( strcmp( m->label(), StrMenu_ScaleAxis ) == 0)
      {
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::ScaleAxis );
      }
       // DUP
    else if( strcmp( m->label(), StrMenu_DupInPlace ) == 0)
      {	
	DuplicateSelection();
	PushHistory();
	TheAppli.redrawAll();	
      }
     else if( strcmp( m->label(), StrMenu_DupMoveX ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveX);
      }
     else if( strcmp( m->label(), StrMenu_DupMoveY ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveY);
      }
     else if( strcmp( m->label(), StrMenu_DupMoveZ ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::MoveZ);
      }
     else if( strcmp( m->label(), StrMenu_DupRotX ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotX);
      }
     else if( strcmp( m->label(), StrMenu_DupRotY ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotY);
      }
     else if( strcmp( m->label(), StrMenu_DupRotZ ) == 0)
      {	
	DuplicateSelection();
	TheAppli.redrawAll();	
	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
	Application::Instance().setCurrentTransformType(Transform::CenterRotZ);
      }
 }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackConnectPoint(Fl_Widget* w, void* pUserData)
  {
     BEGINCALL  ;

     PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    
    if( PP3d::Modif::ConnectPoints( lVisit.cVectPoints, TheAppli.getDatabase() ))
      {
	PushHistory();
	TheAppli.redrawAll();
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdiveCatmullClark(Fl_Widget* w, void* pUserData )
  {
    BEGINCALL  ;
    PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );

    bool lModifOldPts = true;
    
    if( strcmp( m->label(), StrMenu_SubdivideCatmullClarkFalse )==0 )
      {
	lModifOldPts = false;
      }
 
    std::vector<PP3d::EntityPtr> lNewFacets;
    if( PP3d::Modif::SubCatmullClark( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, lModifOldPts ))
      {	
	TheSelect.removeAll();
	TheSelect.addGoodEntityFor(lNewFacets);
	PushHistory();
	TheAppli.redrawAll();
      }    
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide1(Fl_Widget* w, void* pUserData)
  {
      BEGINCALL  ;
    PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );

    PP3d::SubDivFacetType lDivType = PP3d::SubDivFacetType::ANGLE_SIMPLE;
    
    if( strcmp( m->label(), StrMenu_Triangulate1 )==0 )
      {
	lDivType = PP3d::SubDivFacetType::ANGLE_FACET_MARGE;	
      }
 

 
    std::vector<PP3d::EntityPtr> lNewFacets;
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, lDivType ))
      {	
	TheSelect.removeAll();
	TheSelect.addGoodEntityFor(lNewFacets);
	PushHistory();
	TheAppli.redrawAll();       	    
      }  
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide2(Fl_Widget* w, void* pUserData)
  {
      BEGINCALL  ;
    PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );

    PP3d::SubDivFacetType lDivType = PP3d::SubDivFacetType::CENTRAL_POINT;
    
    if( strcmp( m->label(), StrMenu_Triangulate3 )==0 )
      {
	lDivType = PP3d::SubDivFacetType::CENTRAL_FACET;	
      }
    if( strcmp( m->label(), StrMenu_Triangulate4 )==0 )
      {
	lDivType = PP3d::SubDivFacetType::CENTRAL_FACET_MARGE ;	
      }
    

 
    std::vector<PP3d::EntityPtr> lNewFacets;
    if( PP3d::Modif::SubDivMiddle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, lDivType ))
      {	
	TheSelect.removeAll();
	TheSelect.addGoodEntityFor(lNewFacets);
	PushHistory();
	TheAppli.redrawAll();       	    
      }  
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
     
    PP3d::SortEntityVisitor lVisit;
    TheSelect.execVisitorOnEntity( lVisit );

    
    PP3d::SubDiv::SubParam lSubDivLocal( 1, 1, false, PP3d::SubDiv::SubNormalizeType::NORMALIZE_NONE );
    
    if( PP3d::Modif::SubdivideFacet(  lVisit.cVectFacets, TheAppli.getDatabase(), &lSubDivLocal))
      {
	std::vector<PP3d::Point3d> lVectNewPt;
	if( strcmp( m->label(), StrMenu_SubdivideSpike ) == 0 )
	  //	    || strcmp( m->label(), StrMenu_SubdivideCentralSpike ) == 0 )
	  {
	    PP3d::Modif::PrepareChangePointToNeighbourFacetAverage( lVisit.cVectPoints, lVectNewPt );
	    PP3d::Modif::FinalizeChangePointToNeighbourAverage( lVisit.cVectPoints, lVectNewPt );
	  }	    
	else
	  if( strcmp( m->label(), StrMenu_SubdivideFold ) == 0 )
	    //	      || strcmp( m->label(), StrMenu_SubdivideCentralFold ) == 0 )
	    {
	      PP3d::Modif::PrepareChangePointToNeighbourLineAverage( lVisit.cVectPoints, lVectNewPt );
	      PP3d::Modif::FinalizeChangePointToNeighbourAverage( lVisit.cVectPoints, lVectNewPt );
	    }
      }
    TheSelect.removeAll();
    //    TheSelect.addGoodEntityFor(lNewFacets);
    PushHistory();
    TheAppli.redrawAll();
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackCutLine(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;

    int lNbCut=0;
    //============== CUT  ====================
    if( strcmp( m->label(), StrMenu_Cut2 ) == 0)
      {
	lNbCut =2;
      }
    else  if( strcmp( m->label(), StrMenu_Cut3 ) == 0)
      {
 	lNbCut = 3;
     }
    else  if( strcmp( m->label(), StrMenu_Cut4 ) == 0)
      {
 	lNbCut = 4;
      }
    else  if( strcmp( m->label(), StrMenu_Cut5 ) == 0)
      {
 	lNbCut = 5;
      }
    else  if( strcmp( m->label(), StrMenu_Cut6 ) == 0)
      {
  	lNbCut =6 ;
     }
    else  if( strcmp( m->label(), StrMenu_Cut7 ) == 0)
      {
 	lNbCut = 7;
      }
    else  if( strcmp( m->label(), StrMenu_Cut8 ) == 0)
      {
 	lNbCut = 8;
      }
    else  if( strcmp( m->label(), StrMenu_Cut9 ) == 0)
      {
 	lNbCut = 9;
      }
    else  if( strcmp( m->label(), StrMenu_Cut10 ) == 0)
      {
 	lNbCut = 10;
      }
    else  if( strcmp( m->label(), StrMenu_CutN ) == 0)
      {
	int lVal = 20;
	if( CallDialogInputInt( slFlagDialog, lCanvas, "Cut line", lVal ) )
	  lNbCut = lVal;
      }

    std::cout << "MyMenuCallbackSelect : " << lNbCut << std::endl;
    
    if( lNbCut > 1 )
      {
	// On recupere les objects de la selection
	PP3d::SortEntityVisitor lVisit;
	TheSelect.execVisitorOnEntity( lVisit );
	TheSelect.removeAll();
	
	// On prend les lignes

	std::cout << "MyMenuCallbackSelect before CutLines : " << lNbCut << std::endl;
	//Attention au lignes inverses doubles des facettes !
	PP3d::Modif::CutLines( lVisit.cVectLines, lNbCut, TheAppli.getDatabase() );	
	PushHistory();
	TheAppli.redrawAll();
    } 
  }
  
  //***************************************


} //namespace PP3d

