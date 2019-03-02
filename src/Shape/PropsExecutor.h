#ifndef H__PropsExecutor__H
#define H__PropsExecutor__H


#include "PP3dType.h"

namespace PP3d {
 //******************************

	class ViewProps;
	class ObjProps;
	class SubSelect;
	class SelEncode;

 	class PropsExecutor
	{
		bool cFlagMatriceExec=false;
		
	public:
		
		PropsExecutor( SelectType pTypeView, ViewProps& pViewProps, ObjProps& pObjProps, SubSelect& pSubSelect, SelEncode& pSelEncode );
		
		~PropsExecutor();
	};

  //******************************

};

#endif
