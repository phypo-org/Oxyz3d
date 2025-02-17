#ifndef H__MyFloatInput__H
#define H__MyFloatInput__H

#include <sstream>

#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_File_Input.H>

//******************************
class MyFloatInput : public Fl_Float_Input 
{

public :
  MyFloatInput( int pX, int pY, int pW, int pH, const char*pStr )
    :Fl_Float_Input( pX, pY, pW, pH, pStr )
  {;}
  MyFloatInput( int pX, int pY, int pW, int pH, const char*pStr, double iVal )
    :Fl_Float_Input( pX, pY, pW, pH, pStr )
  {
    setFloatValue( iVal );
  }

  void setFloatValue( double pVal )
  {
    std::stringstream lOs;
    lOs <<  pVal;
    value( lOs.str().c_str() );
  }
  void setValue( double pVal )
  {
    setFloatValue( pVal );
  }
};
//******************************
class MyIntInput : public Fl_Int_Input 
{

public :
  MyIntInput( int pX, int pY, int pW, int pH, const char*pStr )
    :Fl_Int_Input( pX, pY, pW, pH, pStr )
  {;}
  MyIntInput( int pX, int pY, int pW, int pH, const char*pStr, int iVal )
    :Fl_Int_Input( pX, pY, pW, pH, pStr )
  {
    setIntValue( iVal );
    align( FL_ALIGN_LEFT );
  }

  void setIntValue( int pVal )
  {
    std::stringstream lOs;
    lOs <<  pVal;
    value( lOs.str().c_str() );
  }
  void setValue( int pVal )
  {
    setIntValue( pVal );
  }
};
//******************************
class MyFileInput : public Fl_File_Input
{

public :
  MyFileInput( int pX, int pY, int pW, int pH, const char*iStr )
    :Fl_File_Input( pX, pY, pW, pH, iStr )
  {;}
  MyFileInput( int pX, int pY, int pW, int pH, const char*iStr, const char* iVal )
    :Fl_File_Input( pX, pY, pW, pH, iStr )
  {
    setValue( iVal );
    align( FL_ALIGN_LEFT );
  }
  MyFileInput( int pX, int pY, int pW, int pH, const char*iStr, const std::string & iVal  )
    :MyFileInput( pX, pY, pW, pH, iStr, iVal.c_str() )
  {    
  }

  void setValue( const std::string & iVal ) { setValue( iVal.c_str()); }
  void setValue( const char* iVal )
  {
  
    value( iVal );
  }
};

//******************************

#endif
