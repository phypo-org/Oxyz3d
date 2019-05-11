#include "PropsExecutor.h"

#include "ViewProps.h"
#include "ObjProps.h"
#include "Selection.h"

namespace PP3d {


//******************************

	PropsExecutor::PropsExecutor(  SelectType pTypeView, ViewProps& pViewProps, ObjProps& pObjProps, SubSelect& pSubSelect, SelEncode& pSelEncode )
	{
		ComponentId lCompId=pSelEncode.getNextCodeGL();

		if( pSubSelect.isSelect( lCompId ))
			{

				if( pViewProps.cFlagDrag )
					{
						cFlagMatriceExec = true;
						glPushMatrix();

				//	cCurrentTransform.execGL();
						glLoadMatrixd( Selection::Instance().getCurrentMatrice().vectForGL() );
					}

				switch( pTypeView )
					{
					case SelectType::Point:
						pViewProps.pointSelectGL();
						break;
					case SelectType::Line:
						pViewProps.lineSelectGL();
						break;
					case SelectType::Facet:
						//			std::cout << " facetSelectGL ";
							pViewProps.facetSelectGL();
						break;
					case SelectType::Object:
						pViewProps.facetSelectGL();
					break;
					}
			}
		else
			{
				switch( pTypeView )
					{
					case SelectType::Point:
						pViewProps.pointGL();
						break;
					case SelectType::Line:
						pViewProps.lineGL();
						break;
					case SelectType::Facet:
							pViewProps.facetGL();
						break;
					case SelectType::Object:
						pViewProps.facetGL();
						break;
					}
			}
	}
	//-------------------------------------
	PropsExecutor::~PropsExecutor()
	{
		if( cFlagMatriceExec )
			{
				glPopMatrix();
			}
	}
//******************************


};
