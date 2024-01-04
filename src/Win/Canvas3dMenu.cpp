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
#include "Preference.h"

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
#define StrMenu_CreateShapeBSplineClosed    "BSpline closed"


#define StrMenu_Spline "Splines"
#define StrMenu_MuteBSplineToPolyline    "Mute BSpline to Polyline"
#define StrMenu_CreatPolylineFromBSpline "Generate Polyline from BSpline"

#define  StrMenu_ModifyShape "Modify shape"
  
#define  StrMenu_CreateShapeAddFacet "Add new Facet to shape"
#define  StrMenu_DeleteShapeFacet "Delete facet"


#define StrMenu_Revol     "New Revolution "
#define StrMenu_RevolX    "Revol X ..."
#define StrMenu_RevolY    "Revol Y ..."
#define StrMenu_RevolZ    "Revol Z ..."
#define StrMenu_RevolAxis "Revol current axis..."

  
#define StrMenu_Spiral_Input     "New Spiral from Input"
#define StrMenu_Spiral_SelObjects     "New Spiral select's objects"
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
#define StrMenu_ScaleNormal "Scale normal"
#define StrMenu_ScaleAxis  "# Scale around current axis"


#define StrMenu_Flip "Flip"
#define StrMenu_FlipX StrMenu_Flip " X"
#define StrMenu_FlipY StrMenu_Flip " Y"
#define StrMenu_FlipZ StrMenu_Flip " Z"
#define StrMenu_FlipPlane StrMenu_Flip " *** current plane *** "
  
  
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
#define StrMenu_ExtrudeTrans StrMenu_Extrude  " current axis"
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




#define StrMenu_CombineIntoGroup "Combine into group"
#define StrMenu_SeparateGroup    "Separate group"


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


#define LAMBDA [](Fl_Widget *w, void *pUserData) { BEGINCALL
#define ADBMAL }
  
 //-------------------------------------------
  static bool DuplicateSelection()
  {    
    PP3d::SortVisitorEntity  lVisit;    
    TheSelect.execVisitorOnlyOnObjects( lVisit );
    
    std::stringstream lDupStr;
    PP3d::MySav lSav( lDupStr );				
    bool lRet = lSav.save( *TheAppli.getDatabase(), false, nullptr, &lVisit.cSetAllEntity  );
    
    if( lRet )
      {
        std::vector<PP3d::EntityPtr> lNewObjs;
        PP3d::MyRead lRead( lDupStr, &lNewObjs );
        
       TheSelect.removeAll();
       
       if( (lRet = lRead.read( *TheAppli.getDatabase(), &TheSelect, false ))
            && lNewObjs.size() > 0 )
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
 
    // A OPTIMISER !!!!
  static void Align( PP3d::Point3d & lAxis )
  {
    
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
  static bool Extrude( M3d::Canvas3d* iCanvas )
  {	  		
    PP3d::SortEntityVisitorPointFacet lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    
    std::vector<PP3d::EntityPtr> lNewFacets;
    
    if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL, 0.0 ))
      {
        TheSelect.removeAll();
        TheSelect.addGoodEntityFor(lNewFacets);  
        TheAppli.redrawAll(PP3d::Compute::FacetAll);	
        iCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
        return true;
      }
    return false;
  }
 
   //-------------------------------------------
  static void CutLine( int iNbCut )
  {
    // On recupere les objects de la selection
    PP3d::SortEntityVisitorLine lVisit;
    TheSelect.execVisitorOnEntity( lVisit );
    TheSelect.removeAll();
    
    // On prend les lignes
    
    //     std::cout << "MyMenuCallbackSelect before CutLines : " << lNbCut << std::endl;
    //Attention au lignes inverses doubles des facettes !
    PP3d::Modif::CutLines( lVisit.cVectLines, iNbCut, TheAppli.getDatabase() );	
    PushHistory();
    TheAppli.redrawAll(PP3d::Compute::FacetAll);
  } 
  //-------------------------------------------
  static  void SplineToPolyline( bool iDelete)
  {
    bool lFlagMakeAnything = false;
    
    for( PP3d::EntityPtr lEntity : TheSelect.getSelectionVect() )
      {
        if( lEntity->getType() ==  PP3d::ShapeType::Object
            && (( PP3d::Object*)lEntity)->getObjType() ==   PP3d::ObjectType::ObjBSpline )
          {
            PP3d::Object* lObj = nullptr;
                 
            if( iDelete )
              {
                lObj = PP3d::ObjBSpline::CreatePolyline( (( PP3d::ObjBSpline*)lEntity), MyPref.cBSplineMaille);
                TheBase.deleteEntity( lEntity );
              }
            else
              {
                lObj = PP3d::ObjBSpline::CreatePolyline( (( PP3d::ObjBSpline*)lEntity), MyPref.cBSplineMaille);
              }
            
            if( lObj != nullptr )
              {
            TheBase.addObject(lObj  );
            lFlagMakeAnything = true;
              }
          }
      }
    
    if( lFlagMakeAnything )
      {
        PushHistory();
        TheAppli.redrawAll( PP3d::Compute::FacetAll);                
      }
    //-----------------
  }
 
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------
#define StrMenu_MagnetSize "Size"
#define StrMenu_MagnetAlgo "Algo"
#define StrMenu_Magnet " Magnet "
#define StrMenu_MagnetRepel " Repel "
#define StrMenu_MagnetAttract " Attract "

    void  Canvas3d::makeMenuMagnet(Fl_Menu_Button& pMenu)
  {
    pMenu.label( StrMenu_Magnet);
    if( getMagnet().getAction() == MagnetAction::MAGNET_ACTION_ATTRACK )
      {
        pMenu.add( StrMenu_MagnetRepel,"", LAMBDA
                   
                   lCanvas->getMagnet().setAction(MagnetAction::MAGNET_ACTION_REPEL);
                   ADBMAL, this, FL_MENU_DIVIDER);
      }
    else
      {
        pMenu.add( StrMenu_MagnetAttract, "", LAMBDA
               
                 lCanvas->getMagnet().setAction(MagnetAction::MAGNET_ACTION_ATTRACK);
               ADBMAL, this, FL_MENU_DIVIDER);
      }

    
    pMenu.add( StrMenu_MagnetAlgo "/" "Distance", "", LAMBDA               
               lCanvas->getMagnet().setAlgo(MagnetAlgo::MAGNET_ALGO_DIST);
               ADBMAL, this);
    
    pMenu.add( StrMenu_MagnetAlgo "/" "Square distance", "", LAMBDA               
               lCanvas->getMagnet().setAlgo(MagnetAlgo::MAGNET_ALGO_SQUARE_DIST);
               ADBMAL, this);
    
   pMenu.add( StrMenu_MagnetAlgo "/" "Square root distance", "", LAMBDA               
              lCanvas->getMagnet().setAlgo(MagnetAlgo::MAGNET_ALGO_SQUARE_ROOT);
               ADBMAL, this, FL_MENU_DIVIDER);

   
    
    pMenu.add( StrMenu_MagnetSize "/" " 10", "", LAMBDA               
               lCanvas->getMagnet().setSize(10);
               ADBMAL, this);
    
    pMenu.add( StrMenu_MagnetSize "/" " 50", "", LAMBDA               
               lCanvas->getMagnet().setSize(50);
               ADBMAL, this);
    
    pMenu.add( StrMenu_MagnetSize "/" " 100", "", LAMBDA               
               lCanvas->getMagnet().setSize(100);
               ADBMAL, this);
    
    pMenu.add( StrMenu_MagnetSize "/" " 200", "", LAMBDA               
               lCanvas->getMagnet().setSize(200);
               ADBMAL, this);    
    pMenu.add( StrMenu_MagnetSize "/" " 300", "", LAMBDA               
               lCanvas->getMagnet().setSize(300);
               ADBMAL, this);    
   
    TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
  }
  //-------------------------------------------
  //
  void  Canvas3d::makeMenuSelect(Fl_Menu_Button& pMenu)
  {
    int lMenuFlagActif=0;

    pMenu.label( " Modify selection ");
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveX, "", LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::MoveX);

               ADBMAL, this);


    
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveY, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::MoveY);

               ADBMAL, this);

    
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveZ, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::MoveZ);

               ADBMAL, this, FL_MENU_DIVIDER);

    
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveNormal, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::MoveNormal);

               ADBMAL, this, FL_MENU_DIVIDER);

    
    pMenu.add( StrMenu_Move  "/" StrMenu_MoveAxis, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::MoveAxis);

               ADBMAL, this, FL_MENU_DIVIDER);
    

    pMenu.add( StrMenu_Rot  "/" StrMenu_RotX, "",    LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotX);

               ADBMAL, this);

    
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotY, "",   LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotY);

               ADBMAL , this);
    
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotZ, "",   LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotZ);

               ADBMAL , this, FL_MENU_DIVIDER);
    
    
    pMenu.add( StrMenu_Rot  "/" StrMenu_CenterRotAxis, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotAxis);

               ADBMAL , this);
    
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotAxis, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::RotAxis);

               ADBMAL , this);

    
    pMenu.add( StrMenu_Rot  "/" StrMenu_RotFacetNorm, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotFacetNorm);

               ADBMAL , this, FL_MENU_DIVIDER);

    
    if(  TheSelect.getSelectType() == PP3d::SelectType::Object         
	 || TheSelect.getSelectType() ==  PP3d::SelectType::Facet)
      {	
	pMenu.add( StrMenu_Rot  "/" StrMenu_RotNorm, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::CenterRotNorm);

               ADBMAL, this, FL_MENU_DIVIDER);
        
        
	if( TheSelect.getNbSelected() > 0 )
	  {
	    pMenu.add( StrMenu_InvertNormal, "",  LAMBDA
                       PP3d::VisitorInvertNormal lVisitInv;                       
                       TheSelect.execVisitorOnEntity( lVisitInv );                       
                       PushHistory();	    
                       TheAppli.redrawAll(PP3d::Compute::FacetAll);
                       ADBMAL, this, FL_MENU_DIVIDER );
            
	  }
      }
		
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleU, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleUniform);

               ADBMAL , this, FL_MENU_DIVIDER);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleX, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleX);

               ADBMAL , this);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleY, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleY);

               ADBMAL , this);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleZ, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleZ);

               ADBMAL , this, FL_MENU_DIVIDER);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRX, "", LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleRX);

               ADBMAL  , this);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRY, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleRY);

               ADBMAL , this);
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleRZ, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleRZ);

               ADBMAL , this, FL_MENU_DIVIDER);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleNormal, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleNormal);

               ADBMAL , this, FL_MENU_DIVIDER);
    
    pMenu.add( StrMenu_Scale  "/" StrMenu_ScaleAxis, "",  LAMBDA
               
               lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
               Application::Instance().setCurrentTransformType(Transform::ScaleAxis);

               ADBMAL , this, FL_MENU_DIVIDER);

    if(  TheSelect.getSelectType() == PP3d::SelectType::Object )
      {

        pMenu.add( StrMenu_Flip "/" StrMenu_FlipX, "", LAMBDA
                   //:::::::::::::::::::::::::::::::::::::::::
                   PP3d::SortEntityVisitorPoint lVisit;    
                   TheSelect.execVisitorOnEntity( lVisit );
                   PP3d::Point3d lAvg;
                   for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lAvg += lPtr->get();
                     }
                   lAvg *=2;
                   lAvg /= lVisit.cVectPoints.size();
	
                   for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lPtr->get().x() = -lPtr->get().x()+ lAvg.x();
                     }
                   PushHistory();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);
                   //:::::::::::::::::::::::::::::::::::::::::
                   ADBMAL, this, FL_MENU_DIVIDER);

        
        pMenu.add( StrMenu_Flip "/" StrMenu_FlipY, "", LAMBDA
                   PP3d::SortEntityVisitorPoint lVisit;    
                   TheSelect.execVisitorOnEntity( lVisit );
                   PP3d::Point3d lAvg;
                   for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lAvg += lPtr->get();
                     }
                   lAvg *=2;
                   lAvg /= lVisit.cVectPoints.size();

                   for(  PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lPtr->get().y() = - lPtr->get().y()+ lAvg.y() ;
                     }
                   PushHistory();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);
                   ADBMAL, this);

        
        pMenu.add( StrMenu_Flip "/" StrMenu_FlipZ, "", LAMBDA
                    //:::::::::::::::::::::::::::::::::::::::::
                  PP3d::SortEntityVisitorPoint lVisit;    
                   TheSelect.execVisitorOnEntity( lVisit );
                   PP3d::Point3d lAvg;
                   for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lAvg += lPtr->get();
                     }
                   lAvg *=2;
                   lAvg /= lVisit.cVectPoints.size();
 
                   for( PP3d::PointPtr lPtr : lVisit.cVectPoints )
                     {
                       lPtr->get().z() =  -lPtr->get().z()+lAvg.z();
                     }
                   PushHistory();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	 	
                   //:::::::::::::::::::::::::::::::::::::::::
                   ADBMAL, this);

        
        pMenu.add( StrMenu_Flip "/not implemented - " StrMenu_FlipPlane, "",LAMBDA
                   std::cerr << "StrMenu_FlipPlane not implemented" << std::endl;
                   ADBMAL , this);


        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupInPlace, "", LAMBDA
                   DuplicateSelection();
                   PushHistory();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                  ADBMAL, this, FL_MENU_DIVIDER);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveX, "",  LAMBDA               
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::MoveX);
                   ADBMAL, this);

        pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveY, "",  LAMBDA             
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::MoveY);
                   ADBMAL, this);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupMoveZ, "",  LAMBDA              
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::MoveZ);
                   ADBMAL, this);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotX, "",   LAMBDA               
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::CenterRotX);
                   ADBMAL, this);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotY, "",   LAMBDA               
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::CenterRotY);
                   ADBMAL, this);
        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupRotZ, "",   LAMBDA               
                   DuplicateSelection();
                   TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                   lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                   Application::Instance().setCurrentTransformType(Transform::CenterRotZ);
                   ADBMAL , this, FL_MENU_DIVIDER);	
      }
    
    if(  TheSelect.getSelectType() == PP3d::SelectType::Point
	 && TheSelect.getNbSelected() > 1 )
      {
	pMenu.add( StrMenu_ConnectPoint, "", LAMBDA
                   //LLLLLLLLLLLLLLLLLLLLLLLLL
                   PP3d::SortEntityVisitorPoint lVisit;
                   TheSelect.execVisitorOnEntity( lVisit );
                   
                   if( PP3d::Modif::ConnectPoints( lVisit.cVectPoints, TheAppli.getDatabase() ))
                     {
                       PushHistory();
                       TheAppli.redrawAll(PP3d::Compute::FacetAll);
                     }
                   //LLLLLLLLLLLLLLLLLLLLLLLLL
                   ADBMAL, this);	
      }
    

     lMenuFlagActif=FL_MENU_INACTIVE;   
    cout << "Nb Selected:" << TheSelect.getNbSelected() << " type:" << PP3d::GetStrShapeType( TheSelect.getSelectType() ) << endl;

    if( TheSelect.getNbSelected() > 2 
	&& TheSelect.getSelectType() ==  PP3d::SelectType::Point )
      // && TheSelect.allHaveTheSameOwner())


      lMenuFlagActif = 0;
    pMenu.add( StrMenu_ModifyShape "/" StrMenu_CreateShapeAddFacet,      "", LAMBDA
               //LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

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
               //LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
               ADBMAL, this, FL_MENU_DIVIDER | lMenuFlagActif);

    
    
    lMenuFlagActif=FL_MENU_INACTIVE;
    if( TheSelect.getNbSelected()
	&& TheSelect.getSelectType() ==  PP3d::SelectType::Facet )
      lMenuFlagActif = 0;

    pMenu.add( StrMenu_ModifyShape "/" StrMenu_DeleteShapeFacet , "", LAMBDA
               //LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
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
               //LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
               ADBMAL, this, FL_MENU_DIVIDER | lMenuFlagActif);


    

    if( TheSelect.getSelectType() ==  PP3d::SelectType::Object
        && TheSelect.getNbSelected() > 0
        && TheSelect.isOnlyObject( PP3d::ObjectType::ObjBSpline ) )
      {
          
        pMenu.add( StrMenu_Spline "/" StrMenu_MuteBSplineToPolyline , "", LAMBDA
                   SplineToPolyline( true );
                   ADBMAL, this);
        pMenu.add( StrMenu_Spline "/" StrMenu_CreatPolylineFromBSpline , "", LAMBDA
                   SplineToPolyline( false );
                   ADBMAL, this);
      }




    // ======= SPIRAL ========
    if(  TheSelect.getNbSelected() > 0
         && ( TheSelect.getSelectType() == PP3d::SelectType::Object
              ||  TheSelect.getSelectType() == PP3d::SelectType::Group)  )
      {
        pMenu.add( StrMenu_Spiral_SelObjects "/" StrMenu_SpiralX, "^x", LAMBDA
                   CallDialogSpiral( TypeRevol::RevolX, TypeOfInput::INPUT_OBJECT  ); 
                   ADBMAL, this );
        
        pMenu.add( StrMenu_Spiral_SelObjects "/" StrMenu_SpiralY, "^y",LAMBDA
                   CallDialogSpiral( TypeRevol::RevolY, TypeOfInput::INPUT_OBJECT  );     
                   ADBMAL, this );
        pMenu.add( StrMenu_Spiral_SelObjects "/" StrMenu_SpiralZ, "^z",LAMBDA                   
                   CallDialogSpiral( TypeRevol::RevolZ, TypeOfInput::INPUT_OBJECT  );     
                   ADBMAL, this );    
       pMenu.add( StrMenu_Spiral_SelObjects "/" StrMenu_SpiralAxis, "",LAMBDA
                  CallDialogSpiral( TypeRevol::RevolAxis, TypeOfInput::INPUT_OBJECT  );   
                  ADBMAL, this, FL_MENU_DIVIDER );
      }
        // ======= SPIRAL ========


    //=========== GROUP ===========
    if( TheSelect.getSelectType() == PP3d::SelectType::Object )
      {
    
        if(  TheSelect.getNbSelected() > 1 )
          pMenu.add( StrMenu_CombineIntoGroup, "",
                     //:::::::::::::::::::::::::::::::::::
                     [](Fl_Widget *w, void *pUserData) {
                         std::cout <<"Callback lambda" << std::endl;
                         TheSelect.combineGroup(TheBase);
                       },
                     //:::::::::::::::::::::::::::::::::::
                     this, FL_MENU_DIVIDER );
        
        if( TheSelect.getNbSelected() > 0 )
          {
            pMenu.add( StrMenu_SeparateGroup, "",
                     //:::::::::::::::::::::::::::::::::::
                       [](Fl_Widget *w, void *pUserData) {
                         BEGINCALL
                               
                         std::cout <<"Callback lambda 2" << std::endl;
                         TheSelect.separateGroup(TheBase);                               
                       },
                     //:::::::::::::::::::::::::::::::::::
                       this
                       );
          }
      }
        
        //=========== GROUP ===========

    
    if(  TheSelect.getSelectType() != PP3d::SelectType::Point)
      {
	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut2, "",  LAMBDA CutLine(2);  ADBMAL, this);
  	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut3, "",  LAMBDA CutLine(3);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut4, "",  LAMBDA CutLine(4);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut5, "",  LAMBDA CutLine(5);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut6, "",  LAMBDA CutLine(6);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut7, "",  LAMBDA CutLine(7);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut8, "",  LAMBDA CutLine(8);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut9, "",  LAMBDA CutLine(9);  ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_Cut10, "", LAMBDA CutLine(10); ADBMAL, this);
   	pMenu.add( StrMenu_Cut "/"  StrMenu_CutN, "",  LAMBDA
                   int lNbCut = 20;
                   if( CallDialogInputInt( "Cut line", lNbCut ) )
                     CutLine( lNbCut );
                   ADBMAL, this);
      }

    if(  (TheSelect.getSelectType() != PP3d::SelectType::Point
	  && TheSelect.getSelectType() != PP3d::SelectType::Line )
	 && TheSelect.getNbSelected() > 0)
      {
	pMenu.add( StrMenu_Inset, "", LAMBDA
                    //:::::::::::::::::::::::::::::::::::::::::
                   PP3d::SortEntityVisitorPointFacet lVisit;
                   TheSelect.execVisitorOnEntity( lVisit );       
                   
                   std::vector<PP3d::EntityPtr> lNewFacets;
                   if( PP3d::Modif::SubDivAngle( TheAppli.getDatabase(), lVisit.cSetFacets, lVisit.cSetPoints, lNewFacets, PP3d::SubDivFacetType::ANGLE_FACET_MARGE, PP3d::SubDivSelectType::SELECT_CENTRAL))
                     {
                       //	std::cout << "MyMenuCallbackInset finish" << std::endl;
                       TheSelect.removeAll();
                       TheSelect.changeSelectType( PP3d::SelectType::Facet );       
                       TheSelect.addGoodEntityFor(lNewFacets);
                       PushHistory();
                       TheAppli.redrawAll(PP3d::Compute::FacetAll);	
                       lCanvas->changeUserMode( ModeUser::MODE_TRANSFORM );
                       Application::Instance().setCurrentTransformType(Transform::ScaleNormal); 	    
                     }
                   //:::::::::::::::::::::::::::::::::::::::::
                   ADBMAL, this);


        
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
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeX, "", LAMBDA
                     if( Extrude(lCanvas) )
                       Application::Instance().setCurrentTransformType(Transform::MoveX);
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeY, "",  LAMBDA
                     if( Extrude(lCanvas) )
                       Application::Instance().setCurrentTransformType(Transform::MoveY);
                       ADBMAL, this);
          
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeZ, "",  LAMBDA
                     if( Extrude(lCanvas) )
                       Application::Instance().setCurrentTransformType(Transform::MoveZ);
                       ADBMAL, this);
          
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeNorm, "",  LAMBDA
                     if( Extrude(lCanvas) )
                       Application::Instance().setCurrentTransformType(Transform::MoveNormal);
                       ADBMAL, this);
          
	  pMenu.add( StrMenu_Extrude "/" StrMenu_ExtrudeTrans, "",  LAMBDA
                     if( Extrude(lCanvas) )
                       Application::Instance().setCurrentTransformType(Transform::MoveAxis);
                     ADBMAL, this);


	  if(TheInput.getCurrentLine()  != nullptr && TheSelect.getNbSelected() > 0 )
	    {
              pMenu.add( StrMenu_Extrude "/++++++" StrMenu_ExtrudePath, "", LAMBDA
                         //:::::::::::::::::::::::::::::::::::::::::
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
                         //:::::::::::::::::::::::::::::::::::::::::
                         ADBMAL, this);

              
              pMenu.add( StrMenu_Extrude "/++++++" StrMenu_ExtrudePathNorm, "", LAMBDA                         
                          //:::::::::::::::::::::::::::::::::::::::::
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
                         //:::::::::::::::::::::::::::::::::::::::::
                         ADBMAL, this);
	    }

	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenX, "", LAMBDA
                     //:::::::::::::::::::::::::::::::::::::::::
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
                     //:::::::::::::::::::::::::::::::::::::::::
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenY, "", LAMBDA
                     //:::::::::::::::::::::::::::::::::::::::::
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
                      //:::::::::::::::::::::::::::::::::::::::::
                    ADBMAL , this);
          
	  pMenu.add( StrMenu_Flatten "/" StrMenu_FlattenZ, "",  LAMBDA
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
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Flatten "/ not implemented y" StrMenu_FlattenAxis, "", LAMBDA
                     std::cerr << "FlattenAxis not implmented" << std::endl;
                     ADBMAL , this);
          
	  pMenu.add( StrMenu_Flatten "/Bug " StrMenu_FlattenAvg, "",  LAMBDA
                     //:::::::::::::::::::::::::::::::::::::::::
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
                     //:::::::::::::::::::::::::::::::::::::::::
                     ADBMAL, this);
	  
	  if( TheSelect.getNbSelected() == 2 )
	    pMenu.add( StrMenu_PutOn, "", LAMBDA
                       //:::::::::::::::::::::::::::::::::::::::::
                       if( TheSelect.getSelectType() == PP3d::SelectType::Facet
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
                       //:::::::::::::::::::::::::::::::::::::::::
                       ADBMAL, this);

          
	  
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnX, "", LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis(1,0,0);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnY, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis(0,1,0);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnZ, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis( 0,0,1);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_X, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis( -1,0,0);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_Y, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis( 0,-1,0);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOn_Z, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis( 0,0,-1);
                     Align( lAxis ); 
                     ADBMAL, this);
          
	  pMenu.add( StrMenu_Align "/" StrMenu_AlignOnAxis, "",  LAMBDA
                     if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                     PP3d::Point3d lAxis( TheAppli.getCurrentAxis().getB());
                     Align( lAxis ); 
                     ADBMAL, this);
        
        pMenu.add( StrMenu_Align "/" StrMenu_AlignOnNorm, "",  LAMBDA
                   if( TheSelect.getSelectType() != PP3d::SelectType::Facet )  return;
                   if( TheSelect.getNbSelected() < 2)
                     {
                       fl_alert( "At least two facets needed");
                       return;
                     }
                   // first facet of selection only use for give normal
                   PP3d::Point3d lAxis( ((PP3d::FacetPtr)TheSelect.getSelectionVect()[0])->getNormal());
                   TheSelect.removeEntity( TheSelect.getSelectionVect()[0] );                     
                   Align( lAxis ); 
                   ADBMAL, this);
      }
	break;
        
      case PP3d::SelectType::Poly :

        
	pMenu.add( StrMenu_Dup "/" StrMenu_DupNormal, "", LAMBDA
                   std::cerr << "StrMenu_DupNormal not implemented" << std::endl;
                   ADBMAL, this);
	break;
      case PP3d::SelectType::All :
      default:;
      }
  }

  //-------------------------------------------
  // If no entity is selected
  void  Canvas3d::makeMenuPrimitiv(Fl_Menu_Button& pMenu)
  {
    pMenu.label( " Create ");
    
    int lMenuFlagActif=0;
    pMenu.add( StrMenu_Primitiv2D "/" StrMenu_CreateFacet, "^c",
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::FACET_N  );
               }, this);

    
    pMenu.add( StrMenu_Primitiv2D "/" StrMenu_CreatePlane,    "",  
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::PLANE  );
               }, this);

 

    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateCylinder, "y",  
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::CYLINDER );
               }, this);  
    
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateSphere,   "s",   
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::SPHERE  );
               }, this);  
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateCube ,    "^c",  
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::CUBE  );
               }, this);   
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateTetra,    "^t",  
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::TETRA );
               }, this);   
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreatePyramid,  "^p",  
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::PYRAMID  );
               }, this);   
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateOcto,     "^o",   
               [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::OCTO  );
              }, this);  
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateDodec,    "^d",   
              [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::DODEC  );
              }, this);  
    pMenu.add( StrMenu_Primitiv3D "/" StrMenu_CreateIcosahe,  "^i", 
              [](Fl_Widget *w, void *pUserData)
               {      
                 CallDialogPrimitiv(  PP3d::PrimitivFactory::Type::ICOSAHED  );
              }, this, FL_MENU_DIVIDER);

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
    
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeLine, "", LAMBDA
              //::::::::::::::::::::::::::::::::::::::
              if( TheInput.getNbCurrentPoints() == 2 )
                {
                  PP3d::Object* lShape =TheInput.convertCurrentLineToLine(TheBase);
                  if( lShape != nullptr )
                    {
                      PushHistory();
                      TheInput.delAllPoint();
                      TheAppli.redrawAll( PP3d::Compute::FacetAll);
                    }                }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL,this,lMenuFlagActif);
     
    
    lMenuFlagActif = FL_MENU_INACTIVE;
    if(TheInput.isCurrentPoints()
       &&TheInput.getNbCurrentPoints()  >= 3 )
      {
 	lMenuFlagActif=0;
      }
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacet, "",  LAMBDA
              //::::::::::::::::::::::::::::::::::::::
              if( TheInput.getNbCurrentPoints() >= 3 )
                {
                  PP3d::Object* lShape =TheInput.convertCurrentLineToFacet(TheBase);
                  if( lShape != nullptr )
                    {
                      PushHistory();
                      TheInput.delAllPoint();
                      TheAppli.redrawAll( PP3d::Compute::FacetAll);
                    }                  
                }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL , this,lMenuFlagActif);
    
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacetP, "",  LAMBDA
              //::::::::::::::::::::::::::::::::::::::
              if( TheInput.getNbCurrentPoints() >= 3 )
                {
                  PP3d::Object* lShape = TheInput.convertCurrentLineToFacetPoly(TheBase);
                
                  if( lShape != nullptr )
                    {
                      PushHistory();
                      TheInput.delAllPoint();
                      TheAppli.redrawAll( PP3d::Compute::FacetAll);
                    }
                }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL, this,lMenuFlagActif);
    
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeFacet2P, "",  LAMBDA
              //::::::::::::::::::::::::::::::::::::::
              if( strcmp( m->label(), StrMenu_CreateShapeFacet2P ) == 0)
                {
                  if( TheInput.getNbCurrentPoints() >= 3 )
                    {
                      PP3d::Object* lShape =TheInput.convertCurrentLineToBiFacetPoly(TheBase);
                      
                      if( lShape != nullptr )
                        {
                          PushHistory();
                          TheInput.delAllPoint();
                          TheAppli.redrawAll( PP3d::Compute::FacetAll);
                        }
                    }
                }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL, this,lMenuFlagActif);

    
    lMenuFlagActif=FL_MENU_INACTIVE;
    if(TheInput.isCurrentPoints()
       &&TheInput.getNbCurrentPoints() >= 2 )
      {
 	lMenuFlagActif=0;
      }
    pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapePolyline, "",  LAMBDA
              //::::::::::::::::::::::::::::::::::::::
              if(TheInput.getNbCurrentPoints() >= 2 )
                {
                  PP3d::Object* lShape =TheInput.convertCurrentLineToPolylines(TheBase);
                
                  if( lShape != nullptr )
                    {
                      PushHistory();
                      TheInput.delAllPoint();
                      TheAppli.redrawAll( PP3d::Compute::FacetAll);
                    }
                }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL,this,   lMenuFlagActif);
    
        pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeBSpline, "",  LAMBDA
               //::::::::::::::::::::::::::::::::::::::
                  if(TheInput.getNbCurrentPoints() >= 2 )
                    {
                      PP3d::Object* lShape = TheInput.convertCurrentLineToBSpline(TheBase,
                                                                                  MyPref.cBSplineMaille, false );
                    if( lShape != nullptr )
                    {
                      PushHistory();
                      TheInput.delAllPoint();
                      TheAppli.redrawAll( PP3d::Compute::FacetAll);
                    }
                  }
               //::::::::::::::::::::::::::::::::::::::
             ADBMAL,this, FL_MENU_DIVIDER | lMenuFlagActif);
        
          pMenu.add(StrMenu_CreateShape "/" StrMenu_CreateShapeBSplineClosed, "",  LAMBDA
              //::::::::::::::::::::::::::::::::::::::
                    if(TheInput.getNbCurrentPoints() >= 2 )
                      {
                        PP3d::Object* lShape =TheInput.convertCurrentLineToBSpline(TheBase,
                                                                                   MyPref.cBSplineMaille, true );
                        if( lShape != nullptr )
                          {
                            PushHistory();
                            TheInput.delAllPoint();
                            TheAppli.redrawAll( PP3d::Compute::FacetAll);
                          }
                       }
              //::::::::::::::::::::::::::::::::::::::
              ADBMAL,this, FL_MENU_DIVIDER | lMenuFlagActif);

    //===== Revol
    lMenuFlagActif = 0;
    if( TheInput.getNbCurrentPoints() < 1 ) lMenuFlagActif=FL_MENU_INACTIVE;

    pMenu.add( StrMenu_Revol "/" StrMenu_RevolX, "^x", LAMBDA
               CallDialogRevol( TypeRevol::RevolX  );               
               ADBMAL, this, lMenuFlagActif);
    
    pMenu.add( StrMenu_Revol "/" StrMenu_RevolY, "^y",  LAMBDA
               CallDialogRevol( TypeRevol::RevolY  );     
               ADBMAL, this, lMenuFlagActif);
    
    pMenu.add( StrMenu_Revol "/" StrMenu_RevolZ, "^z",  LAMBDA
               CallDialogRevol( TypeRevol::RevolZ  );
               ADBMAL, this, lMenuFlagActif);
 
    lMenuFlagActif = 0;
    pMenu.add( StrMenu_Revol "/" StrMenu_RevolAxis, "",  LAMBDA
               CallDialogRevol( TypeRevol::RevolAxis  );
               ADBMAL, this, lMenuFlagActif);


    //====== Spiral 
    lMenuFlagActif = 0;
    if( TheInput.getNbCurrentPoints() < 1 ) lMenuFlagActif=FL_MENU_INACTIVE;
      
    pMenu.add( StrMenu_Spiral_Input "/" StrMenu_SpiralX, "^x", LAMBDA
               CallDialogSpiral( TypeRevol::RevolX, TypeOfInput::INPUT_ENTRY );
               ADBMAL, this, lMenuFlagActif);
    
    pMenu.add( StrMenu_Spiral_Input "/" StrMenu_SpiralY, "^y", LAMBDA
               CallDialogSpiral( TypeRevol::RevolY, TypeOfInput::INPUT_ENTRY  );
               ADBMAL, this, lMenuFlagActif);
    
    pMenu.add( StrMenu_Spiral_Input "/" StrMenu_SpiralZ, "^z", LAMBDA
               CallDialogSpiral( TypeRevol::RevolZ, TypeOfInput::INPUT_ENTRY  );
               ADBMAL, this, lMenuFlagActif);
 
    pMenu.add( StrMenu_Spiral_Input "/" StrMenu_SpiralAxis, "", LAMBDA
               CallDialogSpiral( TypeRevol::RevolAxis, TypeOfInput::INPUT_ENTRY  );
               ADBMAL, this, FL_MENU_DIVIDER | lMenuFlagActif);


    //==============
    pMenu.add( StrMenu_CallDialoDiagSub, "", LAMBDA
                 CallDialogSubDiv( slFlagDialog, lCanvas );
               ADBMAL, this, FL_MENU_DIVIDER); 

  }
  //-------------------------------------------
  //-------------------------------------------
  //-------------------------------------------

  
   
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
  //----------------------------------------
  int Canvas3d::handleMenu(  int pEvent )
  {
    if( pEvent == FL_PUSH
        &&  Fl::event_button() == FL_RIGHT_MOUSE  )
      {
        cPopup->clear();						
												
        if( getUserMode() == ModeUser::MODE_BASE )
          {
           cPopup->type(Fl_Menu_Button::POPUP1);
           
           if( getGlobalMode() == GlobalMode::MAGNET )
             {
               makeMenuMagnet(   *cPopup);
             }
           else
             if( TheSelect.getNbSelected() > 0 )
               {										
                 makeMenuSelect(  *cPopup);
               }
             else
               {
                 makeMenuPrimitiv( *cPopup );
               }
           cPopup->position( Fl::event_x() , Fl::event_y());           
           cPopup->popup();
          }
       	
	return 1;
      }
    
    return 0;
  }

  //***************************************


} //namespace PP3d

