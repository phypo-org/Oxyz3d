
#include <FL/Fl.H>

#include "Creation.h"

#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "GuiFLTK/Preference.h"

#include "Dialogs.h"
#include "GuiFLTK/AgentAutoSav.h"


#include <iostream> 

using namespace M3d;
using namespace std;



//////// #define TEST_TEX



#ifdef TEST_TEX



#define STB_IMAGE_IMPLEMENTATION
#include "Gui/stb_image.h"






#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <GL/gl.h>
#include <GL/glu.h>
//---------------------------------------------------

//---------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Shared_Image.H>	// fl_register_images()
//
// OpenGL spinning cube with PNG image texturemap -- erco/ian 03/23/09
//
//     Assumes /tmp/texture-128.png exists, and x/y res is 64x64, 128x128, etc.
//     For an example image, see http://seriss.com/people/erco/fltk/texture-128.png
//
//     Builds with:
//     fltk-config --use-gl --use-images --compile foo.cxx
//
#define WIN_W 400       // window width
#define WIN_H 400       // window height
#define FPS (1.0/24.0)  // frames per second playback
//
// OpenGL class to show texturemapped cube
//
class MyGlWindow : public Fl_Gl_Window {
    double spin;
    GLuint TexID;
    const char *texturefilename;
    // TIMER CALLBACK
    //    Handles rotation the object
    //
    static void Timer_CB(void *userdata) {
        MyGlWindow *mygl = (MyGlWindow*)userdata;
        mygl->spin += 2.0;       // spin
        mygl->redraw();
        Fl::repeat_timeout(FPS, Timer_CB, userdata);
    }
public:
    // CTOR
    MyGlWindow(int x,int y,int w,int h,const char *l=0) : Fl_Gl_Window(x,y,w,h,l) {
        spin = 0.0;
        Fl::add_timeout(FPS, Timer_CB, (void*)this);       // 24fps timer
        texturefilename = "";
	end();
    }
    void TextureFilename(const char *val) {
        texturefilename = val;
    }
    const char *TextureFilename() const {
        return(texturefilename);
    }
    // PERSPECTIVE VIEW
    //    Same as gluPerspective()..
    //
    void Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
        GLdouble xmin, xmax, ymin, ymax;
        ymax = zNear * tan(fovy * M_PI / 360.0);
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;
        glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
    }
    // LOAD TEXTUREMAP USING FLTK'S SHARED IMAGE LIB
    void LoadTexture(const char *filename, GLuint &TexID) {
	Fl_Shared_Image *img = Fl_Shared_Image::get(filename);		// load texturemap
	if ( ! img ) {
	    fprintf(stderr, "%s: unknown image format\n", filename);
	    exit(1);
	}
	if ( img->d() != 3 ) {
	    fprintf(stderr, "%s: image depth is %d (expected 3)\n", filename, img->d());
	    exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w(), img->h(), 0,
	             GL_RGB, GL_UNSIGNED_BYTE, img->data()[0]);
	img->release();
    }
    void CreateCube() {
	// Materials
	GLfloat ambient[]   = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuse[]   = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat specular[]  = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess[] = { 0.9 * 128.0 };		// 1=dull, 127=shiny
	GLfloat emission[]  = { 0.0, 0.0, 0.0, 0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,  emission);
	static float cube_vertexes[6][4][4] = {		// cube and normals from SGI opengl example
	  { { -1.0, -1.0, -1.0, 1.0 },			// "scube.c" by Mark J. Kilgard
	    { -1.0, -1.0,  1.0, 1.0 },
	    { -1.0,  1.0,  1.0, 1.0 },
	    { -1.0,  1.0, -1.0, 1.0 } },
	  { {  1.0,  1.0,  1.0, 1.0 },
	    {  1.0, -1.0,  1.0, 1.0 },
	    {  1.0, -1.0, -1.0, 1.0 },
	    {  1.0,  1.0, -1.0, 1.0 } },
	  { { -1.0, -1.0, -1.0, 1.0 },
	    {  1.0, -1.0, -1.0, 1.0 },
	    {  1.0, -1.0,  1.0, 1.0 },
	    { -1.0, -1.0,  1.0, 1.0 } },
	  { {  1.0,  1.0,  1.0, 1.0 },
	    {  1.0,  1.0, -1.0, 1.0 },
	    { -1.0,  1.0, -1.0, 1.0 },
	    { -1.0,  1.0,  1.0, 1.0 } },
	  { { -1.0, -1.0, -1.0, 1.0 },
	    { -1.0,  1.0, -1.0, 1.0 },
	    {  1.0,  1.0, -1.0, 1.0 },
	    {  1.0, -1.0, -1.0, 1.0 } },
	  { {  1.0,  1.0,  1.0, 1.0 },
	    { -1.0,  1.0,  1.0, 1.0 },
	    { -1.0, -1.0,  1.0, 1.0 },
	    {  1.0, -1.0,  1.0, 1.0 }
	  }
	};
	static float cube_normals[6][4] = {
	  { -1.0,  0.0,  0.0, 0.0 },
	  {  1.0,  0.0,  0.0, 0.0 },
	  {  0.0, -1.0,  0.0, 0.0 },
	  {  0.0,  1.0,  0.0, 0.0 },
	  {  0.0,  0.0, -1.0, 0.0 },
	  {  0.0,  0.0,  1.0, 0.0 }
	};
	// Cube
	glColor3f(1.0, 1.0, 1.0);
	for ( int i=0; i<6; i++ ) {
  	    glNormal3fv(&cube_normals[i][0]);
	    glBegin(GL_POLYGON);
	        glTexCoord2f(1.0, 0.0); glVertex4fv(&cube_vertexes[i][0][0]);
	        glTexCoord2f(0.0, 0.0); glVertex4fv(&cube_vertexes[i][1][0]);
	        glTexCoord2f(0.0, 1.0); glVertex4fv(&cube_vertexes[i][2][0]);
	        glTexCoord2f(1.0, 1.0); glVertex4fv(&cube_vertexes[i][3][0]);
	    glEnd();
	}
    }
    void CreateLight() {
	GLfloat light_position[] = {  0.0,  0.0,  0.0, 1.0 };  // XYZW (W=0: xyz=direction, W!=0, xyz=position)
	GLfloat light_white[]    = {  1.0,  1.0,  1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_white);
    }
    // RESHAPED VIEWPORT
    //     OpenGL stuff to do whenever window changes size
    //
    void ReshapeViewport() {
        glViewport(0, 0, w(), h());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat ratio = (float(w()) / float(h()));
        Perspective(30.0, ratio, 1.0, 30.0);
        glTranslatef(0.0, 0.0, -8.0);
    }
    // OPENGL INITIALIZATION
    //    OpenGL stuff to do *only once* on startup.
    //
    void GlInit() {
	// Texture Map Init
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Load texture image, bind to TexID
	LoadTexture(TextureFilename(), TexID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	// Lighting Init
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	// Misc OpenGL Init
	glEnable(GL_DEPTH_TEST);	// hidden surface
	glDepthFunc(GL_LEQUAL);
    }
    // FLTK DRAW
    //    Called by FLTK to draw the scene.
    //
    void draw() {
        // Initialize/handle reshaped viewport
        if ( !valid() ) {
            valid(1);
	    // Make sure we only do GlInit() once
	    static int first_time = 1;
	    if ( first_time ) {
		first_time = 0;
		GlInit();
	    }
            ReshapeViewport();
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    // Model view
	    glMatrixMode(GL_MODELVIEW);
        }
        // Clear
        glClearColor(.5,.5,.5, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();		// give light its own matrix
	    // Light
	    glTranslatef(0.0,  2.0,  8.0);
	    CreateLight();
	glPopMatrix();
	glPushMatrix();		// give object its own matrix
	    // Object
	    glRotatef(spin, 0.5, 1.0, 0.0); // show all sides of cube
	    CreateCube();
	glPopMatrix();
        // DEBUG: CHECK FOR ERRORS
        GLenum err = glGetError();
        if ( err != GL_NO_ERROR ) {
            fprintf(stderr, "GLGETERROR=%d\n", (int)err);
        }
    }
};
int main(int argc, char *argv[]) {
    fl_register_images();	// fltk image lib init
    MyGlWindow* mygl = new MyGlWindow(10, 10, WIN_W-20, WIN_H-20, "Textured Image Test");
    if ( argc >= 2 ) {
        mygl->TextureFilename(argv[1]);
    } else {
        printf("Usage: %s <texture_filename.[png,jpg,gif,bmp]>\n"
               "Texture file should be 64x64, 128x128, etc.\n"
	       "Defaulting to use '%s'\n\n", argv[0], mygl->TextureFilename());
    }
    mygl->resizable(mygl);
    mygl->show();
    return(Fl::run());
}



#else









//-------------------------------------------

int main(int argc, char **argv) 
{
  fl_register_images();	// fltk image lib init

  

  MyPref.initDefaultPaths();
  
  PPu::PPErrLog::Instance().init( & std::cout, & std::cerr, nullptr );
  
  PPu::PPArgs lArgs( argc, argv );
  std::string lVal;
 
  if( lArgs.get(  "-HomePath", lVal ) )
    {      
      MyPref.initDefaultPaths( lVal );
      cout << "HomePath:" << lVal << endl;
    }
  
  
  //---------------- Preference ------------------
  std::string lName("Oxyz3d.ini"); 
  TheCreat.getConfig().setFilename( PPu::PPFile::JoinPathNames( MyPref.cConfigPath, lName));

  std::cout << "%%%%% PREFRENCE:" << PPu::PPFile::JoinPathNames( MyPref.cConfigPath, lName) << std::endl;
  
  if( TheCreat.getConfig().readFile() == false )
    {
      WARN_LOG( "Config file not found : " <<  TheCreat.getConfig().getFilename() );  
    }
  else
    INFO_LOG( "Read config file : " <<  TheCreat.getConfig().getFilename() );  

  std::cout << "Read config file : " <<  TheCreat.getConfig().getFilename()  << std::endl;
  
  MyPref.initFromIni( TheCreat.getConfig() );  
  MyPref.initFromArg( lArgs );
  
  //---------------- Preference ------------------
  MyPref.initFromArgs(lArgs );

  

  Fl::use_high_res_GL(1);
  
  M3d::Creation::Instance().createNewWin3d( 1000, 800 );
  //  M3d::Creation::Instance().redrawObjectTree();	


  if( lArgs.get(  "-load", lVal ) )
    {
      if( PP3d::MyRead::Read(lVal.c_str(),
			     *Creation::Instance().getDatabase(), &TheSelect ))
	{
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll );
          Creation::Instance().changeCurrentPath(lVal);
	}
    }
  
  
#ifdef USING_LUA
  if( lArgs.exist( "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
#endif
  
#ifdef USING_PYTHON
  if( lArgs.exist( "-python" ) != -1 )
    {
      CallConsolePython();     
    }
#endif  
  PP3d::UndoHistory::Instance().sav( *Creation::Instance().getDatabase(), &TheSelect );


  
  AgentAutoSav lAutoSav; 
  lAutoSav.run();


 
  
  
  if( lArgs.get(  "-img", lVal ) )
    {
      std::cout << "CALL TO : Creation::Instance().loadModelImage" <<std::endl;
      Creation::Instance().loadModelImage( lVal );	
    }
 
  
  return Fl::run();
}

#endif

//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
