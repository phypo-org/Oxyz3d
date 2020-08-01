#include "GLUtility.h"

#include "PP3dType.h"
#include "MinMaxBox3d.h"
#include "ColorRGBA.h"

#include "string.h"


namespace PP3d {
	//--------------------------
	void GLUtility::DrawOrtho( float pSz, int pNeg )
	{
		glDisable(GL_LIGHTING);
		
		glLineWidth( 2 );

		ColorRGBA::Red();
		glBegin( GL_LINES );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( pSz, 0.0, 0.0 );
		glEnd();
				DrawX( pSz *1.1f, 0.0f, 0.0f, pSz / 20.0f );
		
		
		ColorRGBA::Green();
		glColor3f( 0.0, 1.0, 0.0 );
		glBegin( GL_LINES );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, pSz, 0.0 );
		glEnd();
				DrawY( 0.0f, pSz*1.1f, 0.0f, pSz/20.0f );
		
		
		ColorRGBA::LightBlue();	
		glBegin( GL_LINES );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( 0.0, 0.0, pSz );
		glEnd();
			DrawZ( 0.0, 0.0, pSz*1.1f, pSz/20.0f );
		
		
		
		if( pNeg == 1 )
			{
				glLineStipple( 1, 0x00FF );
				glEnable( GL_LINE_STIPPLE );
				
				ColorRGBA::Red();
				glBegin( GL_LINES );
				glVertex3f( 0.0, 0.0, 0.0 );
				glVertex3f( -pSz, 0.0, 0.0 );
				glEnd();
				
				ColorRGBA::Green();
				glBegin( GL_LINES );
				glVertex3f( 0.0, 0.0, 0.0 );
				glVertex3f( 0.0, -pSz, 0.0 );
				glEnd();
				
				ColorRGBA::LightBlue();	
				glBegin( GL_LINES );
				glVertex3f( 0.0, 0.0, 0.0 );
				glVertex3f( 0.0, 0.0, -pSz );
				glEnd();
				
				glDisable( GL_LINE_STIPPLE );
				
				ColorRGBA::Red();
				DrawMinus( pSz*-1.1f, 0.0, 0.0, pSz/20.0f );
							DrawX( pSz*-1.1f, 0.0, 0.0, pSz/20.0f );
				
				ColorRGBA::Green();
				DrawMinus( 0.0f, -pSz*1.1f, 0.0, pSz/20.0f );
							DrawY( 0.0f, -pSz*1.1f, 0.0f, pSz/20.0f );
				
				ColorRGBA::LightBlue();	
				DrawMinus( 0.0f, 0.0f, -pSz*1.1f, pSz/20.0f );
								DrawZ( 0.0f, 0.0f, -pSz *1.1f, pSz / 20.0f );
			}

    glEnable(GL_LIGHTING);
				
	}
	//--------------------------
	void GLUtility::DrawGrid( float pSz, int pNb, bool pNombre )
	{
		//		std::cout << "====================== DRAW GRID ===================="
		//				<< std::endl;
	
		//  ColorRGBA::Grey();
		glLineWidth( 1 );
			
		float lPas = (float)pSz / (float)pNb;
		
		if( pNombre == false )
		  lPas =(float) pNb;
			
		glDisable(GL_LIGHTING);
		
    // Draw a 1x1 grid along the X and Z axis'
    for(float i = -pSz; i <= pSz; i += lPas )
			{

        // Start drawing some lines
        glBegin(GL_LINES);

				// Do the horizontal lines (along the X)
				glVertex3f(-pSz, 0, i);
				glVertex3f(pSz, 0, i);

				// Do the vertical lines (along the Z)
				glVertex3f(i, 0, -pSz);
				glVertex3f(i, 0, pSz);

        // Stop drawing lines
        glEnd();
			}
    glEnable(GL_LIGHTING);

	}

	//--------------------------
	void
	GLUtility::DrawMinus( float pX, float pY, float pZ, float pSz )
	{
		glBegin( GL_LINES );
		glVertex3f( pX-2*pSz, pY, pZ );
		glVertex3f( pX-pSz, pY, pZ );
		glEnd();
	}
	//--------------------------
	void
	GLUtility::DrawCursor( Point3d pPt, float pSz )
	{	
		glDisable(GL_LIGHTING);
		glColor3f( 0, 1.0, 0 );

		glBegin( GL_LINES );
		glColor3f( 1, 1.0, 0 );
		glVertex3f( (float)pPt.cX, (float)pPt.cY, (float)pPt.cZ );
		glVertex3f( 0, 0 , 0 );
		glEnd();


		glBegin( GL_LINES );
		glColor3f( 1.0, 0, 0 );
		glVertex3f( (float)pPt.cX, (float)pPt.cY, (float)pPt.cZ );
		glVertex3f( 0, (float)pPt.cY , (float)pPt.cZ );

		glColor3f( 0.6f, 0.6f, 0.6f );
		glVertex3f( 0, (float)pPt.cY , (float)pPt.cZ );
		glVertex3f( 0, 0 , (float)pPt.cZ );
		glVertex3f( 0, (float)pPt.cY , (float)pPt.cZ );
		glVertex3f( 0, (float)pPt.cY , 0 );
		glEnd();

		glBegin( GL_LINES );
		glColor3f( 0, 1, 0 );
		glVertex3f( (float)pPt.cX, (float)pPt.cY, (float)pPt.cZ );
		glVertex3f( (float)pPt.cX, 0, (float)pPt.cZ );

		glColor3f( 0.6f, 0.6f, 0.6f );
		glVertex3f( (float)pPt.cX, 0,(float) pPt.cZ );
		glVertex3f( 0, 0, (float)pPt.cZ );
		glVertex3f( (float)pPt.cX, 0, (float)pPt.cZ );
		glVertex3f( (float)pPt.cX, 0, 0 );
		glEnd();


		glBegin( GL_LINES );
		glColor3f( 0, 0, 1 );
		glVertex3f( (float)pPt.cX, (float)pPt.cY,(float) pPt.cZ );
		glVertex3f( (float)pPt.cX, (float)pPt.cY, 0);
		glColor3f( 0.6f, 0.6f, 0.6f );
		glVertex3f( (float)pPt.cX, (float)pPt.cY, 0);
		glVertex3f( (float)pPt.cX, 0, 0);
		glVertex3f( (float)pPt.cX, (float)pPt.cY, 0);
		glVertex3f( 0,(float) pPt.cY, 0);
		glEnd();


		glColor3f( 1.0, 1.0, 1.0 );
		glBegin( GL_LINES );

		glVertex3f( (float)pPt.cX-pSz, (float)pPt.cY-pSz, (float)pPt.cZ );
		glVertex3f( (float)pPt.cX + pSz, (float)pPt.cY + pSz, (float)pPt.cZ );

		glVertex3f( (float)pPt.cX+pSz, (float)pPt.cY-pSz, (float)pPt.cZ  );
		glVertex3f( (float)pPt.cX - pSz, (float)pPt.cY + pSz, (float)pPt.cZ );

		glVertex3f( (float)pPt.cX-pSz, (float)pPt.cY,    (float)pPt.cZ-pSz );
		glVertex3f( (float)pPt.cX + pSz, (float)pPt.cY , (float)pPt.cZ+ pSz );

		glVertex3f( (float)pPt.cX+pSz, (float)pPt.cY, (float)pPt.cZ-pSz  );
		glVertex3f( (float)pPt.cX - pSz, (float)pPt.cY , (float)pPt.cZ + pSz);

		glVertex3f( (float)pPt.cX, (float)pPt.cY-pSz,    (float)pPt.cZ-pSz );
		glVertex3f( (float)pPt.cX, (float)pPt.cY + pSz , (float)pPt.cZ+ pSz );

		glVertex3f( (float)pPt.cX, (float)pPt.cY+pSz, (float)pPt.cZ-pSz  );
		glVertex3f( (float)pPt.cX, (float)pPt.cY - pSz , (float)pPt.cZ + pSz);

		glEnd();
    glEnable(GL_LIGHTING);

	}
	//--------------------------
	void
	GLUtility::DrawCursorCruz( Point3d pPt, float pSz )
	{	
		glDisable(GL_LIGHTING);
		glBegin( GL_LINES );

		glVertex3f( (float)pPt.cX-pSz, (float)pPt.cY-pSz, (float)pPt.cZ );
		glVertex3f( (float)pPt.cX + pSz, (float)pPt.cY + pSz, (float)pPt.cZ );

		glVertex3f( (float)pPt.cX+pSz, (float)pPt.cY-pSz, (float)pPt.cZ  );
		glVertex3f( (float)pPt.cX - pSz, (float)pPt.cY + pSz, (float)pPt.cZ );

		glVertex3f( (float)pPt.cX-pSz, (float)pPt.cY,    (float)pPt.cZ-pSz );
		glVertex3f( (float)pPt.cX + pSz,(float) pPt.cY , (float)pPt.cZ+ pSz );

		glVertex3f( (float)pPt.cX+pSz,(float) pPt.cY, (float)pPt.cZ-pSz  );
		glVertex3f( (float)pPt.cX - pSz, (float)pPt.cY , (float)pPt.cZ + pSz);

		glVertex3f( (float)pPt.cX, (float)pPt.cY-pSz,    (float)pPt.cZ-pSz );
		glVertex3f( (float)pPt.cX, (float)pPt.cY + pSz , (float)pPt.cZ+ pSz );

		glVertex3f( (float)pPt.cX, (float)pPt.cY+pSz, (float)pPt.cZ-pSz  );
		glVertex3f( (float)pPt.cX, (float)pPt.cY - pSz , (float)pPt.cZ + pSz);

		glEnd();
    glEnable(GL_LIGHTING);
	}
	//--------------------------
	void
	GLUtility::DrawX( float pX, float pY, float pZ, float pSz )
	{	
		glBegin( GL_LINES );
		//	glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( pX-pSz, pY-pSz, pZ );
		glVertex3f( pX + pSz, pY + pSz, pZ );

		glVertex3f( pX+pSz, pY-pSz, pZ  );
		glVertex3f( pX - pSz, pY + pSz, pZ );
		glEnd();

	}
	//--------------------------

	void
	GLUtility::DrawY( float pX, float pY, float pZ, float pSz )
	{
		glBegin( GL_LINE_STRIP );
		glVertex3f( pX-pSz, pY+pSz, pZ );
		glVertex3f( pX, pY, pZ );
		glVertex3f( pX, pY - pSz, pZ );
		glVertex3f( pX, pY, pZ );
		glVertex3f( pX+pSz, pY+pSz, pZ );
		glEnd();
	}
	//--------------------------

	void
	GLUtility::DrawZ( float pX, float pY, float pZ, float pSz )
	{
		glBegin( GL_LINE_STRIP );
		glVertex3f( pX-pSz, pY+pSz, pZ );
		glVertex3f( pX + pSz, pY + pSz, pZ );
		glVertex3f( pX-pSz, pY-pSz, pZ );
		glVertex3f( pX+pSz, pY- pSz, pZ );
		glEnd();

	}	
#define sTsz 3
	//--------------------------
	void GLUtility::DrawChar(  Point3d pPos, float pSz,  char pChar )
	{	
		float pX = (float)pPos.cX;
		float pY = (float)pPos.cY;
		float pZ = (float) pPos.cZ;


	Point3d P[ sTsz ][ sTsz ];

	float lPas = pSz / sTsz;

//	float x = -pSz;
	float x = 0;

	for( int i = 0; i < sTsz; i++  )
	{
		float y = pSz-lPas;
		for( int j = 0; j < sTsz; j++ )
		{
			Point3d &p =  P[ i ][ j ];
			p.cX = pX+x;
			p.cY = pY+y;
			p.cZ = pZ;
			
			y -= lPas;
		}
		x+= lPas;
	}

#define P1 			glVertex3dv( *P[0][0] );
#define P2			glVertex3dv( *P[1][0] );
#define P3			glVertex3dv( *P[2][0] );
#define P4		  glVertex3dv( *P[0][1] );
#define P5		  glVertex3dv( *P[1][1] );
#define P6			glVertex3dv( *P[2][1] );
#define P7			glVertex3dv( *P[0][2] );
#define P8			glVertex3dv( *P[1][2] );
#define P9			glVertex3dv( *P[2][2] );

	glBegin( GL_LINE_STRIP );
	switch( pChar )
	{
		case '0':
			P1; P3; P9; P7; P1;
			break;


		case '1':
			P2; P8;
			break;
		case '2':
			P1; P3; P6;
			P4; P7; P9
			break;
		case '3':
			P1; P3; P6; P4; P6; P9; P7;
			break;
		case '4':
			P1; P4; P6; P9;
			break;
		case '5':
			P3; P1; P4; P6; P9; P7;
			break;
		case '6':
			P3; P1; P7; P9; P6; P4;
			break;
		case '7':
			P1; P3; P8;
			break;
		case '8':
			P1; P3; P9; P7; P1;
			P4; P6;
			break;
		case '9':
			P6; P4; P1; P3; P9; P7;
			break;
		case '-':
			P5; P6;
			break;
		case '/':
			P3; P7;
			break;
		case '+':
			P4; P6; P5;P2; P8;
			break;

		case '.':
			glEnd();
			glBegin( GL_POINT );				
			P8;
			break;
		case '_':
			P7; P9;
			break;

		case '=':
			P4; P6;
			glEnd();			
			glBegin( GL_LINE_STRIP );
			P8;P9;
			break;

		case '|':
			P2; P8;
			break;

		case ',':
			P5; P8;
			break;
		case '*':
			P1; P9; P5; P3; P7; P5; P4; P6; P5; P2; P8;
			break;
		case '?':
			P4; P1; P3; P6; P5; P8;
			break;
		case '[':
			P2; P1; P7; P8;
			break;
		case ']':
			P2; P3; P9; P8;
			break;
		case 'E':
			P3; P1; P4; P5; P4; P7; P9;
			break;
		case 'e':
			P4; P6; P3; P1; P7; P9;
			break;
		case 'A':
			P7; P1; P3; P6; P4; P6; P9;
			break;
		case 'X':
			P1; P9; P5; P3; P7;
			break;
		case 'Y':
			P1; P5; P3; P5; P8;
			break;
		case 'Z':
			P1; P3; P7; P9;
			break;

		default:;
	}
					
	glEnd();
	}
//----------------------------------------
void GLUtility::DrawText(  bool pOrient, Point3d pPos, float pSz, const char* pStr,  int pReverse)
{
  float x = (float)pPos.cX;
  float y = (float)pPos.cY;
  float z = (float)pPos.cZ;

  int lNb = (int)::strlen( pStr);

//	DrawX( pX, pY, pZ, 10 );
	if( pReverse == 0 )
		for( int i = 0; i < lNb;  i++ )
		{
			DrawChar( Point3d(x, y, z), pSz, pStr[ i ] );

			if( pOrient == true )
			{
				x += pSz;
			}
			else
			{
				y -= pSz;
			}
		
		}
	else
		for( int i = lNb-1; i >= 0;  i--)
		{
			if( pOrient == true )
			{
				x -= pSz;
			}
			else
			{
				y += pSz;
			}

			DrawChar( Point3d( x, y, z ), pSz, pStr[ i ] );		
		}
}
	//-----------------------------------------
	// FAUX !!!!!!
	void GLUtility::DrawMinMaxBox(Rect3d&lMmb)
	{
		 
		glDisable( GL_LIGHTING );
				
		glBegin( GL_LINES );
		
		glVertex3dv( lMmb.getMin().vectForGL() );
		glVertex3dv( lMmb.getMax().vectForGL() );
		
		
		glVertex3dv( (-lMmb.getMin()).vectForGL() );
		glVertex3dv( lMmb.getMin().vectForGL() );
		
		glVertex3dv( (-lMmb.getMax()).vectForGL() );
		glVertex3dv( lMmb.getMax().vectForGL() );
		
		glEnd();

		glEnable(GL_LIGHTING);

	}
	//********************************
}
	 
