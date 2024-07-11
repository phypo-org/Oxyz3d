
#include <FL/Fl.H>

#include "Creation.h"

#include "Shape/SavRead.h"
#include "Shape/UndoHistory.h"

#include "Gui/Preference.h"

#include "Dialogs.h"
#include "Gui/AgentAutoSav.h"


#include <iostream>

using namespace M3d;
using namespace std;



// #define TEST_TEX



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

class ImageWindow : public Fl_Gl_Window {
public:
    ImageWindow(int X, int Y, int W, int H, const char *L = 0);
    ~ImageWindow();
    void loadImage(const char* filename);

protected:
    void draw() override;

private:
  //   Fl_PNG_Image* image;
    GLuint texture;
    bool imageLoaded;
};
//---------------------------------------------------

ImageWindow::ImageWindow(int X, int Y, int W, int H, const char *L)
  : Fl_Gl_Window(X, Y, W, H, L)
    //, image(nullptr)
  ,imageLoaded(false) {
    texture = 0;
      mode(FL_RGB | FL_DOUBLE);
}

ImageWindow::~ImageWindow() {
    fprintf(stdout, "~~~~~~~~~~~~~~~~~~~~~~~\n");

  
    if (texture) {
        glDeleteTextures(1, &texture);
    }
    //    delete image;
}
//---------------------------------------------------
void ImageWindow::loadImage(const char* filename) {
  
  int width, height, nrChannels;
  unsigned char *data = stbi_load( filename,  &width, &height, &nrChannels, 0);
  
  //    image = new Fl_PNG_Image(filename);
  //    if (image->w() > 0 && image->h() > 0) {
  if( data != nullptr)
    {
      fprintf(stdout, "XXXXXXXXXXX %d %d\n", width, height);
      glActiveTexture(GL_TEXTURE0);
      
        glGenTextures(1, &texture);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        //      glGenerateMipmap(GL_TEXTURE_2D);
        imageLoaded = true;
        stbi_image_free(data);
    } else {
        fprintf(stderr, "Failed to load image %s\n", filename);
        imageLoaded = false;
    }
}
//---------------------------------------------------

void ImageWindow::draw() {
    if (!imageLoaded) return;

    if (!valid()) {
        glLoadIdentity();
        gluOrtho2D(0, w(), 0, h());
        glViewport(0, 0, w(), h());
    }
  
    glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    glColor3f( 1, 0, 0 );

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    
   
    //   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(w(), 0);
    glTexCoord2f(1, 1); glVertex2f(w(), h());
    glTexCoord2f(0, 1); glVertex2f(0, h());
    glEnd();

    glDisable(GL_TEXTURE_2D);
    valid(1); 
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image file>\n", argv[0]);
        return 1;
    }
    fprintf(stdout, "OpenGL Image Display with FLTK : %s\n", argv[1]);

    Fl_Window window(800, 600, "OpenGL Image Display with FLTK");
    ImageWindow imgWin(10, 10, 780, 580);
    window.end();
    window.show(argc, argv);

    imgWin.loadImage(argv[1]);

   Fl::run();

   return 0;
}
//---------------------------------------------------




#else









//-------------------------------------------

int main(int argc, char **argv) 
{
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
	  Creation::Instance().redrawAll(PP3d::Compute::FacetAll
					    );
	}
    }
  
  
  if( lArgs.get(  "-img", lVal ) )
    {   
      Creation::Instance().loadModelImage( lVal );	
    }
  
#ifdef USING_LUA
  if( lArgs.exist( "-lua" ) != -1 )
    {
      CallConsoleLua();     
    }
#endif  
  PP3d::UndoHistory::Instance().sav( *Creation::Instance().getDatabase(), &TheSelect );


  
  AgentAutoSav lAutoSav; 
  lAutoSav.run();
  
  return Fl::run();
}

#endif

//
// End of "$Id: gl_overlay.cxx 10498 2014-12-20 07:19:23Z manolo $".
//
