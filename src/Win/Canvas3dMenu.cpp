#include "Canvas3d.h"

  //#include <config.h>
#include <FL/Fl.H>

#include <FL/fl_ask.H>

#include <FL/gl.h>

#include <FL/names.h>
#include <FL/Fl_Menu_Button.H>


#include "Service.h"


#include <iostream>
#include <memory>
#include <sstream>


#include "Shape/GLUtility.h"
#include "Shape/Light.h"
#include "Shape/ObjectLine.h"
#include "Shape/ObjectPoly.h"
#include "Shape/OwnerVisitor.h"
#include "Shape/PrimitivFactory.h"
#include "Shape/SortVisitor.h"
#include "Shape/Selection.h"
#include "Shape/SubDiv.h"
#include "Shape/SavRead.h"
#include "Shape/PP3dUtils.h"
#include "Shape/ViewProps.h"
#include "Shape/DataBase.h"

#include "Modif/Modif.h"


#include "Application.h"
#include "MyFlWidget.h"

#include "Dialogs.h"

using namespace std;


namespace M3d {

#define StrMenu_Primitiv2D    "Primitive 2D      ..."
	
#define StrMenu_CreateFacet     "Facet      ..."
#define StrMenu_CreatePlane     "Plane      ..."
  
#define StrMenu_Primitiv3D    "Primitive 3D      ..."
#define StrMenu_CreateCylinder  "Cylinder   ..."
#define StrMenu_CreateSphere    "Sphere     ..."
#define StrMenu_CreateCube      "Cube       ..."
#define StrMenu_CreateTetra     "Tetraede   ..."
#define StrMenu_CreatePyramid   "Pyramide   ..."
#define StrMenu_CreateOcto      "Octoedre   ..."
#define StrMenu_CreateDodec     "Dodecaedre ..."
#define StrMenu_CreateIcosahe   "Icosaedre  ..."
	
#define StrMenu_CreateShape          "New Shape "
#define StrMenu_CreateShapeLine       "Line"
#define StrMenu_CreateShapePolyline   "Polyline"
#define StrMenu_CreateShapeFacet      "Facet"
#define StrMenu_CreateShapeFacetP     "FacetPoly"
#define StrMenu_CreateShapeFacet2P    "BiFacetPoly"
#define StrMenu_CreateShapeBSpline    "BSpline"

#define  StrMenu_ModifyShape "Modify shape"
#define  StrMenu_CreateShapeAddFacet "Add new Facet to shape"
#define  StrMenu_DeleteShapeFacet "Delete facet to shape"


#define StrMenu_Revol     "New Revolution "
#define StrMenu_RevolX    "Revol X ..."
#define StrMenu_RevolY    "Revol Y ..."
#define StrMenu_RevolZ    "Revol Z ..."
#define StrMenu_RevolAxis StrMenu_Revol "Revol current axis..."

  
#define StrMenu_Spiral     "New Spiral "
#define StrMenu_SpiralX     "Spiral X ..."
#define StrMenu_SpiralY     "Spiral Y ..."
#define StrMenu_SpiralZ     "Spiral Z ..."
#define StrMenu_SpiralAxis  "Spiral current axis..."

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
#define StrMenu_RotNorm  "Rotate normal"
#define StrMenu_CenterRotAxis  "Rotate around current axis (center)"
#define StrMenu_RotAxis  "Rotate around current axis"
#define StrMenu_RotFacetNorm  "Rotate facet normal"

#define StrMenu_Scale      "Scale"
#define StrMenu_ScaleU     "Scale uniform"
#define StrMenu_ScaleX     "Scale X"
#define StrMenu_ScaleY     "Scale Y"
#define StrMenu_ScaleZ     "Scale Z"
#define StrMenu_ScaleRX    "Scale radial X"
#define StrMenu_ScaleRY    "Scale radial Y"
#define StrMenu_ScaleRZ    "Scale radial Z"
  //#define StrMenu_ScaleRZ    "Scale normal"
#define StrMenu_ScaleAxis  "# Scale around current axis"
#define StrMenu_ScaleNormal  "Scale around facet normal"

#define StrMenu_Flatten "Flatten"
#define StrMenu_FlattenX StrMenu_Flatten " X"
#define StrMenu_FlattenY StrMenu_Flatten " Y"
#define StrMenu_FlattenZ StrMenu_Flatten " Z"
#define StrMenu_FlattenAxis StrMenu_Flatten "# BUG current axis"
#define StrMenu_FlattenAvg StrMenu_Flatten "# average of facet"

  
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
#define StrMenu_ExtrudePath StrMenu_Extrude  " path"
#define StrMenu_ExtrudePathNorm StrMenu_Extrude  " path norm"

  
#define StrMenu_PutOn     "Put facet 1 on facet 2"
  
#define StrMenu_Align     "Align"  
#define StrMenu_AlignOnX  "Align Facet Normal on X axe"
#define StrMenu_AlignOnY  "Align Facet Normal on Y axe"
#define StrMenu_AlignOnZ  "Align Facet Normal on Z axe"
#define StrMenu_AlignOn_X  "Align Facet Normal on -X axe"
#define StrMenu_AlignOn_Y  "Align Facet Normal on -Y axe"
#define StrMenu_AlignOn_Z  "Align Facet Normal on -Z axe"
#define StrMenu_AlignOnAxis  "BUG - Align Facet Normal on current axis"
#define StrMenu_AlignOnNorm  "Align Facet Normal on first Facet Normal"

#define StrMenu_InvertNormal   "Invert normal"

#define  StrMenu_Cut "Cut line"
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
#define StrMenu_Inset          "Inset" // " angles + central facette+ marges "
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
    int lMenuFlagActif=0;
   
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
      {
        pMenu.add( StrMenu_Rot  "/" StrMenu_CenterRotAxis, "",  MyMenuCallbackSelect, this);
        pMenu.add( StrMenu_Rot  "/" StrMenu_RotAxis, "",  MyMenuCallbackSelect, this);
      }
    
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotFacetNorm, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    if(  TheSelect.getSelectType() == PP3d::SelectType::Object
	 || TheSelect.getSelectType() ==  PP3d::SelectType::Facet)
      {	
	pMenu.add( StrMenu_Rot  "/" StrMenu_RotNorm, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
	if( TheSelect.getNbSelected() > 0 )
	  {
	    pMenu.add( StrMenu_InvertNormal, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER );
	  }
      }
		
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleU, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleX, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleY, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRX, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRY, "",  MyMenuCallbackSelect, this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRZ, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleNormal, "",  MyMenuCallbackSelect, this, FL_MENU_DIVIDER);
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
	pMenu.add( StrMenu_ConnectPoint, "", MyMenuCallbackConnectPoint, this);	
      }
      
    lMenuFlagActif = FL_MENU_INACTIVE;
    if( TheSelect.getNbSelected() > 2 
	&& TheSelect.getSelectType() ==  PP3d::SelectType::Point )
      // && TheSelect.allHaveTheSameOwner()) 
      lMenuFlagActif = 0;
    pMenu.add( StrMenu_ModifyShape "/" StrMenu_CreateShapeAddFacet,      "", MyMenuCallbackModifyShape, this, FL_MENU_DIVIDER | lMenuFlagActif);

    
    lMenuFlagActif=FL_MENU_INACTIVE;
    if( TheSelect.getNbSelected()
	&& TheSelect.getSelectType() ==  PP3d::SelectType::Facet )
      lMenuFlagActif = 0;

    pMenu.add( StrMenu_ModifyShape "/" StrMenu_DeleteShapeFacet , "", MyMenuCallbackModifyShape, this, FL_MENU_DIVIDER | lMenuFlagActif);

    
    
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
	pMenu.add( StrMenu_Inset, "", MyMenuCallbackInset, this);
	pMenu.add( StrMenu_SubdivideCatmullClark, "", MyMenuCallbackSubdiveCatmullClark, this);
	pMenu.add( StrMenu_Subdivide "/" StrMenu_Triangulate,  "", MyMenuCallbackSubdivide1, this);
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
	break;
      case PP3d::SelectType::Facet :	
	{
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeX, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeY, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeZ, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeNorm, "", MyMenuCallbackExtrude, this);
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeTrans, "", MyMenuCallbackExtrude, this);


	  if(TheInput.getCurrentLine()  != nullptr && TheSelect.getNbSelected() > 0 )
	    {
              pMenu.add( StrMenu_Extrude "/++++++" StrMenu_ExtrudePath, "", MyMenuCallbackExtrudePath, this);
              pMenu.add( StrMenu_Extrude "/++++++" StrMenu_ExtrudePathNorm, "", MyMenuCallbackExtrudePathNorm, this);
	    }

	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenX, "", MyMenuCallbackFlatten, this);
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenY, "", MyMenuCallbackFlatten, this);
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenZ, "", MyMenuCallbackFlatten, this);
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenAxis, "", MyMenuCallbackFlatten, this);
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenAvg, "", MyMenuCallbackFlatten, this);
	  
	  if( TheSelect.getNbSelected() == 2 )
	    pMenu.add( StrMenu_PutOn, "", MyMenuCallbackPutOn, this);
	  
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnX, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnY, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnZ, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_X, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_Y, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_Z, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnAxis, "", MyMenuCallbackAlign, this);
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnNorm, "", MyMenuCallbackAlign, this);
	}
	break;
      case PP3d::SelectType::Poly :
	pMenu.add( StrMenu_Dup "/" StrMenu_DupNormal, "", MyMenuCallbackSelect, this);
	break;
      case PP3d::SelectType::All :
      default:;
      }
  }
  //-------------------------------------------
  // If no entity is selected
  void  Canvas3d::makeMenuPrimitiv(Fl_Menu_Button& pMenu)
  {
    int lMenuFlagActif=0;
    pMenu.add( StrMenu_Primitiv2D "/" StrMenu_CreateFacet,    "^c",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv2D "/" StrMenu_CreatePlane,    "",   MyMenuCallbackPrimitiv,   this);
    
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateCylinder, "y",   MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/"  StrMenu_CreateSphere,   "s",   MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateCube ,    "^c",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateTetra,    "^t",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreatePyramid,  "^p",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateOcto,     "^o",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateDodec,    "^d",  MyMenuCallbackPrimitiv,   this);
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateIcosahe,  "^i",  MyMenuCallbackPrimitiv,   this, FL_MENU_DIVIDER);

    // Ajouter sphere (2 types differents)
    // Ajouter cone
    // Ajouter cylindre (revolution) (disque,  anneau si troue)
    // Ajouter nappe
    // Ajout anneau
    // Ajout helice 
    // ... Ajout script lua !!! plugin
    
    lMenuFlagActif = FL_MENU_INACTIVE;
    
    if(TheInput.isCurrentPoints() 
       &&TheInput.getNbCurrentPoints() == 2 )
      {
	lMenuFlagActif=0;
      }
    pMenu.add(StrMenu_ModifyShape "/" StrMenu_CreateShapeLine, "", MyMenuCallbackPrimitiv,this,lMenuFlagActif);
      
    
    lMenuFlagActif = FL_MENU_INACTIVE;
    if(TheInput.isCurrentPoints()
       &&TheInput.getNbCurrentPoints()  >= 3 )
      {
 	lMenuFlagActif=0;
      }
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacet, "", MyMenuCallbackShape , this,lMenuFlagActif);
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacetP, "", MyMenuCallbackShape, this,lMenuFlagActif);
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacet2P, "", MyMenuCallbackShape, this,lMenuFlagActif);
    
    
    lMenuFlagActif=FL_MENU_INACTIVE;
    if(TheInput.isCurrentPoints()
       &&TheInput.getNbCurrentPoints() >= 2 )
      {
 	lMenuFlagActif=0;
      }
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapePolyline, "", MyMenuCallbackShape,this,   lMenuFlagActif);
        pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeBSpline, "", MyMenuCallbackShape,this, FL_MENU_DIVIDER | lMenuFlagActif);
  
    //===== Revol
    lMenuFlagActif = 0;
    if( TheInput.getNbCurrentPoints() < 1 ) lMenuFlagActif=FL_MENU_INACTIVE;

    pMenu.add( StrMenu_Revol "/" StrMenu_RevolX, "^x", MyMenuCallbackRevol, this, lMenuFlagActif);
    pMenu.add( StrMenu_Revol "/" StrMenu_RevolY, "^y", MyMenuCallbackRevol, this, lMenuFlagActif);
    pMenu.add( StrMenu_Revol "/" StrMenu_RevolZ, "^z", MyMenuCallbackRevol, this, lMenuFlagActif);
 
    lMenuFlagActif = 0;
    if( TheAppli.isSelectAxis() ==false ) lMenuFlagActif=FL_MENU_INACTIVE; 
    pMenu.add( StrMenu_RevolAxis, "", MyMenuCallbackRevol, this, lMenuFlagActif);


    //====== Spiral 
    lMenuFlagActif = 0;
    if( TheInput.getNbCurrentPoints() < 1 ) lMenuFlagActif=FL_MENU_INACTIVE;
      
    pMenu.add( StrMenu_Spiral "/" StrMenu_SpiralX, "^x", MyMenuCallbackSpiral, this, lMenuFlagActif);
    pMenu.add( StrMenu_Spiral "/" StrMenu_SpiralY, "^y", MyMenuCallbackSpiral, this, lMenuFlagActif);
    pMenu.add( StrMenu_Spiral "/" StrMenu_SpiralZ, "^z", MyMenuCallbackSpiral, this, lMenuFlagActif);
 
    lMenuFlagActif = 0;
    if( TheAppli.isSelectAxis() ==false ) lMenuFlagActif=FL_MENU_INACTIVE; 
    pMenu.add( StrMenu_SpiralAxis, "", MyMenuCallbackSpiral, this, FL_MENU_DIVIDER | lMenuFlagActif);


    //==============
    pMenu.add( StrMenu_CallDialoDiagSub, "",MyMenuCallbackPrimitiv, this, FL_MENU_DIVIDER  );

  }
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
#define BEGINCALL							\
  static bool slFlagDialog=false;					\
									\
  Fl_Menu_* mw = (Fl_Menu_*)w;						\
  const Fl_Menu_Item* m = mw->mvalue();					\
  if (!m)								\
    {									\
      printf("NULL\n");							\
									\
      return ;								\
    }									\
									\
  printf("%s\n", m->label());						\
  M3d::Canvas3d* lCanvas = reinterpret_cast<M3d::Canvas3d*>(pUserData);	\
  
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackShape(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL
      
    PP3d::Object* lShape=nullptr;
    //-----------------
    if( strcmp( m->label(), StrMenu_CreateShapeFacet ) == 0)
      {
        if( TheInput.getNbCurrentPoints() >= 3 )
          {
            lShape =TheInput.convertCurrentLineToFacet(TheBase);
          }
      }
    
    if( strcmp( m->label(), StrMenu_CreateShapeFacetP ) == 0)
      {
        if( TheInput.getNbCurrentPoints() >= 3 )
          {
            lShape =TheInput.convertCurrentLineToFacetPoly(TheBase);
          }
      }
    
    if( strcmp( m->label(), StrMenu_CreateShapeFacet2P ) == 0)
      {
        if( TheInput.getNbCurrentPoints() >= 3 )
          {
            lShape =TheInput.convertCurrentLineToBiFacetPoly(TheBase);
          }
      }
    
    //-----------------
    else if( strcmp( m->label(), StrMenu_CreateShapePolyline ) == 0)
      {
        if(TheInput.getNbCurrentPoints() >= 2 )
          {
           lShape =TheInput.convertCurrentLineToPolylines(TheBase);
          }
      }
    else if( strcmp( m->label(), StrMenu_CreateShapeBSpline ) == 0)
      {
        if(TheInput.getNbCurrentPoints() >= 2 )
          {
           lShape =TheInput.convertCurrentLineToBSpline(TheBase);
          }
      }
    
    //-----------------
    else if( strcmp( m->label(), StrMenu_CreateShapeLine ) == 0)
      {
        if( TheInput.getNbCurrentPoints() == 2 )
          {
            lShape =TheInput.convertCurrentLineToLine(TheBase);
          }
      }
	
    if( lShape != nullptr )
      {
        //						lCanvas->Application::Instance().getDatabase()_>addObject( new PP3d::Object3d( lShape, PP3d::Object3d::GetNewObjecId(), lShape->getClassName() ));
        PushHistory();	    
        TheAppli.redrawAll( PP3d::Compute::FacetAll);
      }
  }
  //-------------------------------------------

  void Canvas3d::MyMenuCallbackPrimitiv(Fl_Widget* w, void* pUserData)
  {		
    BEGINCALL  
				
      if( strcmp( m->label(), StrMenu_CreateFacet ) == 0)
	{
	  std::cout << "MyMenuCallbackPrimitiv StrMenu_CreateFacet"<< std::endl;
    
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
    //========================================
    else if( strcmp( m->label(), StrMenu_CallDialoDiagSub ) == 0)
      {
        CallDialogSubDiv( slFlagDialog, lCanvas );
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackRevol(Fl_Widget* w, void* pUserData)
  {		
    BEGINCALL  
 
      if( strcmp( m->label(), StrMenu_RevolX )  == 0)
        {
          CallDialogRevol( TypeRevol::RevolX  );
        }
      else if( strcmp( m->label(), StrMenu_RevolY )  == 0)
        {
          CallDialogRevol( TypeRevol::RevolY  );
        }
      else if( strcmp( m->label(), StrMenu_RevolZ)  == 0)
        {
          CallDialogRevol( TypeRevol::RevolZ  );
        }									
      else if( strcmp( m->label(), StrMenu_RevolAxis)  == 0)
        {
          CallDialogRevol( TypeRevol::RevolAxis  );
        }									
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSpiral(Fl_Widget* w, void* pUserData)
  {		
    BEGINCALL  

      if( strcmp( m->label(), StrMenu_SpiralX )  == 0)
        {
          CallDialogSpiral( TypeRevol::RevolX  );
        }
      else if( strcmp( m->label(), StrMenu_SpiralY )  == 0)
        {
          CallDialogSpiral( TypeRevol::RevolY  );
        }
      else if( strcmp( m->label(), StrMenu_SpiralZ)  == 0)
        {
          CallDialogSpiral( TypeRevol::RevolZ  );
        }									
      else if( strcmp( m->label(), StrMenu_SpiralAxis)  == 0)
        {
          CallDialogSpiral( TypeRevol::RevolAxis  );
        }									           
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackExtrudePath(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL ;

    std::cout << "++++++++++ Canvas3d::MyMenuCallbackExtrudePath" << std::endl;
    PP3d::FacetPtr lFacet = TheInput.getCurrentLine();    

    if( lFacet != nullptr && TheSelect.getNbSelected() > 0 )
      {
        std::cout << "   +++++++ Canvas3d::MyMenuCallbackExtrudePath  1" << std::endl;
        PP3d::Point3d lPtZero;
        PP3d::Point3d lAxis(1,0,0);
        TheAppli.getAxis( lPtZero, lAxis );
        std::cout << "   +++++++ Canvas3d::MyMenuCallbackExtrudePath  2" << std::endl;

        PP3d::Modif::ExtrudePath( TheBase, TheSelect, lPtZero, lAxis, lFacet );

        PushHistory();	
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackExtrudePathNorm(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL ;

    std::cout << "++++++++++ Canvas3d::MyMenuCallbackExtrudePathNorm" << std::endl;
    PP3d::FacetPtr lFacet = TheInput.getCurrentLine();    

    if( lFacet != nullptr && TheSelect.getNbSelected() > 0 )
      {
        std::cout << "   +++++++ Canvas3d::MyMenuCallbackExtrudePathNorm  1" << std::endl;
        PP3d::Point3d lPtZero;
        PP3d::Point3d lAxis(1,0,0);
        TheAppli.getAxis( lPtZero, lAxis );
        std::cout << "   +++++++ Canvas3d::MyMenuCallbackExtrudePathNorm  2" << std::endl;

        PP3d::Modif::ExtrudePathNorm( TheBase, TheSelect, lPtZero, lAxis, lFacet );

        PushHistory();	
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackExtrude(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL ;
    PP3d::SortEntityVisitorPointFacet lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    
    std::vector<PP3d::EntityPtr> lNewFacets;
    
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL, 0.0 ))
      {
        TheSelect.removeAll();
        TheSelect.addGoodEntityFor(lNewFacets);  
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	
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
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackFlatten(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL ;
    

    if( strcmp( m->label(), StrMenu_FlattenX ) == 0)
      {
        PP3d::SortEntityVisitorPoint lVisit;    
        TheSelect.execVisitorOnEntity( lVisit );
        PP3d::Point3d lAvg;
        for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lAvg += lPtr->get();
          }
        lAvg /= lVisit.cVectPoints.size();
	
        for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lPtr->get().x() =  lAvg.x();
          }
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);

      }
    else if( strcmp( m->label(), StrMenu_FlattenY ) == 0)
      {
        PP3d::SortEntityVisitorPoint lVisit;    
        TheSelect.execVisitorOnEntity( lVisit );
        PP3d::Point3d lAvg;
        for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lAvg += lPtr->get();
          }
        lAvg /= lVisit.cVectPoints.size();
	
        for(  PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lPtr->get().y() =  lAvg.y();
          }
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);
	
      }
    else if( strcmp( m->label(), StrMenu_FlattenZ ) == 0)
      {
        PP3d::SortEntityVisitorPoint lVisit;    
        TheSelect.execVisitorOnEntity( lVisit );
        PP3d::Point3d lAvg;
        for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lAvg += lPtr->get();
          }
        lAvg /= lVisit.cVectPoints.size();
	
        for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
          {
            lPtr->get().z() =  lAvg.z();
          }
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
    else if( strcmp( m->label(), StrMenu_FlattenAvg ) == 0)
      {
        //	BUG !!!!!!!!!!!!!!!!!!
        // il faut trouver le centre et le vecteur moyen
        PP3d::SortEntityVisitorPointFacet lVisit;		    
        TheSelect.execVisitorOnEntity(lVisit);
	
        PP3d::Point3d lCenter;
        PP3d::Point3d lAvgNorm;
	
        if( PP3d::GetVectorFromFacets( lVisit, lAvgNorm, lCenter ))
          {		    
            PP3d::Mat4 lMatAlign;
            PP3d::Point3d u1(0,1,0);	    
            lMatAlign.rotateAlign( u1, lAvgNorm  );
	    
            PP3d::Mat4 lMatInv;
            lMatInv.rotateAlignInv( u1, lAvgNorm ); 
	    
	    
            for( PP3d::PointPtr lPtr :  lVisit.cVectPoints )
              {
                lPtr->get() -= lCenter;  // changement de repere sur lCenter		
                lPtr->get() *= lMatAlign; // on fait tourner pour mettre la normale sur l'axe des y
                lPtr->get().y() = 0;
                lPtr->get() *= lMatInv;  // on remet la normale a sa place
		  
                lPtr->get() += lCenter;  // on revient au repere central
                // On doit pouvoir combiner tout ca dans une seul matrice pour eviter des calculs inutiles
              }
          }
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
    else if( strcmp( m->label(), StrMenu_FlattenAxis ) == 0)
      {
        // A FAIRE
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackPutOn(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL ;

    if( strcmp( m->label(), StrMenu_PutOn ) == 0
        && TheSelect.getSelectType() == PP3d::SelectType::Facet
        && TheSelect.getNbSelected() == 2 )
      {
        PP3d::FacetPtr lFacetToMove = (PP3d::FacetPtr)TheSelect.getSelectionVect()[0];
        PP3d::FacetPtr lFacetDest   = (PP3d::FacetPtr)TheSelect.getSelectionVect()[1];

        PP3d::Vector3d lNormToMove =  -lFacetToMove->getNormal(); // l'inverse !!!
        PP3d::Vector3d lNormDest   =  lFacetDest->getNormal();
        PP3d::Mat4 lMatAlign;
        lMatAlign.rotateAlign( lNormDest, lNormToMove  ); 
	
        PP3d::Point3d lPtToMove = lFacetToMove->getCenter();
        PP3d::Point3d lPtDest   = lFacetDest->getCenter();

        PP3d::OwnerEntityVisitor lVisit;	
        lVisit.addOwnersOf( lFacetToMove );
	
        for( PP3d::PolyPtr lPoly: lVisit.cVectPolys )
          {
            lPoly->execVisitor( lVisit ) ; // je sais, c'est tres limite comme code !
          }

        for( PP3d::PointPtr lPtr :  lVisit.cVectPoints )
          {
            lPtr->get() -= lPtToMove;  // Deplacement en zero
            lPtr->get() *= lMatAlign;  // Rotation alignement
            lPtr->get() += lPtDest;    // positionnement sur l'autre facette   
          }
	
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackAlign(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL ;

    if( TheSelect.getSelectType() != PP3d::SelectType::Facet )
      return;
    
 		
    PP3d::Point3d lAxis(1,0,0);
    
    if( strcmp( m->label(), StrMenu_AlignOnX ) == 0 )
      lAxis.set( 1,0,0);
    else if( strcmp( m->label(), StrMenu_AlignOnY ) == 0 )
      lAxis.set( 0,1,0);
    else if( strcmp( m->label(), StrMenu_AlignOnZ ) == 0 )
      lAxis.set( 0,0,1);
    else if( strcmp( m->label(), StrMenu_AlignOn_X ) == 0 )
      lAxis.set( -1,0,0);
    else if( strcmp( m->label(), StrMenu_AlignOn_Y ) == 0 )
      lAxis.set( 0,-1,0);
    else if( strcmp( m->label(), StrMenu_AlignOn_Z ) == 0 )
      lAxis.set( 0,0,-1);
    else
      if( strcmp( m->label(), StrMenu_AlignOnAxis ) == 0 )
        {
          if( TheAppli.getCurrentAxis() == nullptr )
            {
              fl_alert( "No default axis");
              return;
            }
          lAxis =  TheAppli.getCurrentAxis()->getAxis();
        }
      else
        if( strcmp( m->label(), StrMenu_AlignOnNorm ) == 0 )
          {
            if( TheSelect.getNbSelected() < 2)
              {
                fl_alert( "At least two facets needed");
                return;
              }
            // first facet of selection only use for give normal
            lAxis =  ((PP3d::FacetPtr)TheSelect.getSelectionVect()[0])->getNormal();
            TheSelect.removeEntity( TheSelect.getSelectionVect()[0] );
          }
	  
    // A OPTIMISER !!!!

    PP3d::OwnerEntityVisitor lVisitAll;	
    lVisitAll.addOwnersOf( TheSelect.getSelectionVect() );

	
    PP3d::SortEntityVisitorFacet lVisitSelect;
    TheSelect.execVisitorOnEntity( lVisitSelect );
	
	
    for( PP3d::ObjectPtr lObj : lVisitAll.cVectObjects ) // pour tout les objets trouvés
      {	    
        PP3d::SortEntityVisitorPointFacet lVisitObj;		    
        lObj->execVisitor(lVisitObj); // pour les facettes de l'objet
	
        PP3d::Vector3d lAvgNorm;
        PP3d::Vector3d lAvgCenter;
        int lCpt=0;
        for( PP3d::FacetPtr lFacet : lVisitObj.cVectFacets )
          {		
            auto lIter = lVisitSelect.cSetFacets.find( lFacet );
            if( lIter == lVisitSelect.cSetFacets.end() )
              continue;
            // la facette appartient a la selection et a l'objets
		
            lCpt++;
            lAvgNorm += lFacet->getNormal();
            lAvgCenter += lFacet->getCenter();
          }
        if( lCpt )
          {
            lAvgNorm /= lCpt;
            lAvgCenter /= lCpt;

            PP3d::Vector3d lNormToMove =  lAvgNorm; // l'inverse !!!
            PP3d::Mat4 lMatAlign;
	    
            lMatAlign.rotateAlign( lAxis, lNormToMove  ); 
	
            for( PP3d::PointPtr lPtr :  lVisitObj.cVectPoints )
              {
                lPtr->get() -= lAvgCenter;  // Deplacement en zero
                lPtr->get() *= lMatAlign;  // Rotation alignement
                lPtr->get() += lAvgCenter;  // On revient a la position originelle  
              }
          }
      }
	
    PushHistory();
    TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
  }
  //-------------------------------------------
  static bool DuplicateSelection()
  {    
    PP3d::SortVisitorEntity  lVisit;    
    TheSelect.execVisitorOnlyOnObjects( lVisit );
    
    std::stringstream lDupStr;
    PP3d::MySav lSav( lDupStr );				
    bool lRet = lSav.save( *TheAppli.getDatabase(), nullptr, &lVisit.cSetAllEntity );
    
    if( lRet )
      {
        std::vector<PP3d::EntityPtr> lNewObjs;
        PP3d::MyRead lRead( lDupStr, &lNewObjs );
	
        TheSelect.removeAll();	
        lRet = lRead.read( *TheAppli.getDatabase(), &TheSelect, false );

        /*
          lRet = lRead.read( *TheAppli.getDatabase(), false );
          if( lRet && lNewObjs.size() )
          {	    
          TheSelect.removeAll();
          for( PP3d::EntityPtr lObj : lNewObjs )
          {
          TheSelect.addEntity( lObj );
          }
          }
        */
      }
    return lRet;
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSelect(Fl_Widget* w, void* pUserData)
  {	  		
    BEGINCALL  

      if( strcmp( m->label(), StrMenu_InvertNormal) == 0)
        {
          PP3d::VisitorInvertNormal lVisitInv;
	
          TheSelect.execVisitorOnEntity( lVisitInv );
	
          PushHistory();	    
          TheAppli.redrawAll(PP3d::Compute::FacetAll);
        }
      else
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
        else if( strcmp( m->label(), StrMenu_CenterRotAxis ) == 0)
          {
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotAxis );
          }
        else if( strcmp( m->label(), StrMenu_RotAxis ) == 0)
          {
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::RotAxis );
          }
        else if( strcmp( m->label(), StrMenu_RotNorm ) == 0)
          {
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotNorm );
          }
        else if( strcmp( m->label(), StrMenu_RotFacetNorm ) == 0)
          {
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotFacetNorm );
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
        else if( strcmp( m->label(), StrMenu_ScaleNormal ) == 0)
          {
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::ScaleNormal );
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
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
          }
        else if( strcmp( m->label(), StrMenu_DupMoveX ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::MoveX);
          }
        else if( strcmp( m->label(), StrMenu_DupMoveY ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::MoveY);
          }
        else if( strcmp( m->label(), StrMenu_DupMoveZ ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::MoveZ);
          }
        else if( strcmp( m->label(), StrMenu_DupRotX ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotX);
          }
        else if( strcmp( m->label(), StrMenu_DupRotY ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotY);
          }
        else if( strcmp( m->label(), StrMenu_DupRotZ ) == 0)
          {	
            DuplicateSelection();
            TheAppli.redrawAll(PP3d::Compute::FacetAll);	
            lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
            Application::Instance().setCurrentTransformType(Transform::CenterRotZ);
          }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackConnectPoint(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;

    PP3d::SortEntityVisitorPoint lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    
    if( PP3d::Modif::ConnectPoints( lVisit.cVectPoints, TheAppli.getDatabase() ))
      {
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);
      }
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdiveCatmullClark(Fl_Widget* w, void* pUserData )
  {
    BEGINCALL  ;
    PP3d::SortEntityVisitorPointFacet lVisit;
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
        TheAppli.redrawAll(PP3d::Compute::FacetAll
                           );
      }    
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackInset(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
    PP3d::SortEntityVisitorPointFacet lVisit;
    TheSelect.execVisitorOnEntity( lVisit );       
    
    std::vector<PP3d::EntityPtr> lNewFacets;
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL))
      {
        //	std::cout << "MyMenuCallbackInset finish" << std::endl;
        TheSelect.removeAll();
        TheSelectTransform.changeSelectType( PP3d::SelectType::Facet );       
        TheSelect.addGoodEntityFor(lNewFacets);
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	
        //	lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
        //	Application::Instance().setCurrentTransformType(Transform::ScaleNormal); 	    
      }  
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide1(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
    PP3d::SortEntityVisitorPointFacet lVisit;
    TheSelect.execVisitorOnEntity( lVisit );

    PP3d::SubDivFacetType lDivType = PP3d::SubDivFacetType::ANGLE_SIMPLE;
    
    std::vector<PP3d::EntityPtr> lNewFacets;
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, lDivType ))
      {	
        TheSelect.removeAll();
        TheSelect.addGoodEntityFor(lNewFacets);
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);       	    
      }  
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide2(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
    PP3d::SortEntityVisitorPointFacet lVisit;
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
        TheAppli.redrawAll(PP3d::Compute::FacetAll);       	    
      }  
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackSubdivide(Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
     
    PP3d::SortEntityVisitorPointFacet lVisit;
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
    TheAppli.redrawAll(PP3d::Compute::FacetAll);
  }
  //-------------------------------------------
  void Canvas3d::MyMenuCallbackModifyShape( Fl_Widget* w, void* pUserData)
  {
    BEGINCALL  ;
    if( strcmp( m->label(), StrMenu_DeleteShapeFacet ) == 0)
      {	
        PP3d::SortEntityVisitor lVisit;
        TheSelect.execVisitorOnEntity( lVisit );

        int lNbErr=0;
        for( PP3d::FacetPtr lFacet :  lVisit.cVectFacets )
          {
            PP3d::PolyPtr lPoly = PP3d::GetOwnerPolyFromFacet( lFacet );
            if( lPoly != nullptr )
              {
                lPoly->removeFacet( lFacet );
                TheAppli.getDatabase()->freeFacet( lFacet );
              }
            else
              {
                if(lNbErr == 0 )
                  {		    
                    fl_alert( "No polyedre found for select points ");
                  }
                lNbErr++;
              }
          }
        TheSelect.removeAll();	
        PushHistory();	    
        TheAppli.redrawAll(PP3d::Compute::FacetAll);
      }
    else
      if( strcmp( m->label(), StrMenu_CreateShapeAddFacet ) == 0)
        {
          if( TheSelect.getSelectType() ==  PP3d::SelectType::Point )
            {
              PP3d::OwnerEntityVisitor lVisit;
              TheSelect.execVisitorOnEntity( lVisit ); // on recupere les points du select

              std::cout << "CreateShapeAddFacet points:" << lVisit.cVectPoints.size() << std::endl;
	    
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
              std::vector<PP3d::EntityPtr> &lVisitPt = (std::vector<PP3d::EntityPtr>& ) lVisit.cVectPoints;
#pragma GCC diagnostic pop
	      
              lVisit.addOwnersOf( lVisitPt ); // on recupere les owners des points
              if( lVisit.cVectPolys.size() == 0)
                {
                  fl_alert( "No polyedre found for select points ");
                  return;
                }
	    
              if( lVisit.cSetPolys.size() > 1)
                {
                  fl_alert( "Selected points have not the same owner");
                  cout << "Selected points have not the same owner" << endl;
                  for( PP3d::EntityPtr lEntity : lVisit.cVectPolys )
                    {
                      cout << "Entity : " << *lEntity << endl;
                    }
                  return;
                }	    
	    
              PP3d::PolyPtr  lPoly = lVisit.cVectPolys[0];
	    
              PP3d::FacetPtr lFacet = TheBase.getNewFacet();
	    
              size_t i;
              for( i=0; i< lVisit.cVectPoints.size() -1 ; i++ )
                {
                  PP3d::LinePtr lLine = TheBase.getNewLine( lVisit.cVectPoints[i], lVisit.cVectPoints[i+1]);
                  lFacet->addLine( lLine );
                }
              PP3d::LinePtr lLine = TheBase.getNewLine(lVisit.cVectPoints[i],  lVisit.cVectPoints[0] );
              lFacet->addLine( lLine);
              TheBase.validEntity( lFacet, true );
		
              lPoly->addFacet( lFacet );
              TheSelect.removeAll();	
              PushHistory();	    
              TheAppli.redrawAll(PP3d::Compute::FacetAll);
            }
        }
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
        if( CallDialogInputInt( "Cut line", lVal ) )
          lNbCut = lVal;
      }

    std::cout << "MyMenuCallbackSelect : " << lNbCut << std::endl;
    
    if( lNbCut > 1 )
      {
        // On recupere les objects de la selection
        PP3d::SortEntityVisitorLine lVisit;
        TheSelect.execVisitorOnEntity( lVisit );
        TheSelect.removeAll();
	
        // On prend les lignes

        std::cout << "MyMenuCallbackSelect before CutLines : " << lNbCut << std::endl;
        //Attention au lignes inverses doubles des facettes !
        PP3d::Modif::CutLines( lVisit.cVectLines, lNbCut, TheAppli.getDatabase() );	
        PushHistory();
        TheAppli.redrawAll(PP3d::Compute::FacetAll);
      } 
  }
  
  //***************************************


} //namespace PP3d

