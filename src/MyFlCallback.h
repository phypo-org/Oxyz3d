#ifndef H__MyFLCallback__H
#define H__MyFLCallback__H

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>


namespace M3d {
	
	//****************************************************
	//	using  FlCallback = void (*)(Fl_Widget*w, void*data) ;
	/*
	//--------------------------------------------
	struct CallBackTarget{
		void test( Fl_Widget*w, void*pUserData1,void*pUserData2,void*pUserData3 )
		{
		}
	};
	using CB_ClassTarget=void (CallBackTarget::*)( Fl_Widget*w, void*pUserData1,void*pUserData2,void*pUserData3 );
	//	typedef void (CallBackTarget::* CB_ClassTarget)( Fl_Widget*w, void*pUserData1,void*pUserData2,void*pUserData3 );

	//--------------------------------------------
	struct MyCallBackFtn {
		CallBackTarget* cTarget;
		CB_ClassTarget cCallback;
		void*          cUserData1;
		void*          cUserData2;
		void*          cUserData3;

		MyCallBackFtn( CallBackTarget* pTarget,
									 CB_ClassTarget pCallback,
									 void* pUserData1=nullptr,void* pUserData2=nullptr,void* pUserData3=nullptr  )
			:cTarget   ( pTarget)
			,cCallback ( pCallback)
			,cUserData1( pUserData1)
			,cUserData2( pUserData2)
			,cUserData3( pUserData3)
		{
		}
		static void CallBack(Fl_Widget*w, void*pUserData)
		{
			CallBackTarget toto;
			CB_ClassTarget totoCB= &toto.test;
			MyCallBackFtn titi( &toto, &toto.test );

			titi.*totoCB( w, nullptr, nullptr, nullptr );
			
			MyCallBackFtn*  pCb = reinterpret_cast<MyCallBackFtn*>( pUserData );
			CallBackTarget* cClass = pCb->cTarget;
			CB_ClassTarget  cCb= pCb->cCallback;
		
			cClass->*cCb(  w, pCb->cUserData1, pCb->cUserData2, pCb->cUserData3 );
			//	pCb->cTarget.*(pCb->cCallback)( w, pCb->cUserData1, pCb->cUserData2, pCb->cUserData3 );
		}
	};
	*/
	//---------------------------
	struct MyUserData{
		void* cUserData1;
		void* cUserData2;
		void* cUserData3;
		void* cUserData4;
		void* cUserData5;
		void* cUserData6;
		void* cUserData7;
		void* cUserData8;
		
		MyUserData( void* pUserData1, void* pUserData2, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr )
			:cUserData1(pUserData1)
			,cUserData2(pUserData2)
			,cUserData3(pUserData3)
			,cUserData4(pUserData4)
			,cUserData5(pUserData5)
			,cUserData6(pUserData6)
			,cUserData7(pUserData7)
			,cUserData8(pUserData8)
		{
		}

		void setUserData( void* pUserData1, void* pUserData2, void* pUserData3=nullptr, void* pUserData4=nullptr, void* pUserData5=nullptr, void* pUserData6=nullptr, void* pUserData7=nullptr, void* pUserData8=nullptr )
		{
			cUserData1=pUserData1;
			cUserData2=pUserData2;
			cUserData3=pUserData3;
			cUserData4=pUserData4;
			cUserData5=pUserData5;
			cUserData6=pUserData6;
			cUserData7=pUserData7;
			cUserData8=pUserData8;
		}
	};
		//****************************************************

};
#endif
