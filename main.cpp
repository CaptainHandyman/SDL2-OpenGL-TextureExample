#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <string>
using namespace std;

class Timer{
public:
    void start(){
        if(!isStarted)
            isStarted=true;
    }

    float inMilliseconds(){
        if(isStarted){
            if(!canRestart)
                canRestart=true;
            milliseconds=(float)(SDL_GetTicks()-rTime)/1000;
        }
        return milliseconds;
    }

    void restart(){
        if(canRestart){
            canRestart=false;
            rTime=SDL_GetTicks();
        }
    }

private:
    bool isStarted=false;

    bool canRestart=false;

    float milliseconds=0;

    float rTime=0;
};

Timer timer;

SDL_Window *window;
SDL_Event event;

GLuint *textures;

float x=0, y=0;

int mode=GL_RGB;

const Uint8 *key=SDL_GetKeyboardState(0);

int textureCount=0;

void movement(){
    if(key[SDL_SCANCODE_D])
        x+=0.04;
    if(key[SDL_SCANCODE_A])
        x-=0.04;
    if(key[SDL_SCANCODE_W])
        y-=0.04;
    if(key[SDL_SCANCODE_S])
        y+=0.04;
}

SDL_Surface *image;

void loadTexture(GLuint texture, string filename){
    image=IMG_Load(filename.c_str());

    if(image->format->BitsPerPixel>=4)
        mode=GL_RGBA;
    else
        mode=GL_RGB;

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, 
        image->h, 0, mode, GL_UNSIGNED_BYTE, image->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

int id=0;

void showTexture(int a){
    id=a;
}

void displayTexture(){
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, textures[id]);
    glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glTexCoord2f(0, 0); glVertex2i(0, 0);
        glTexCoord2f(1, 0); glVertex2i(image->w*4, 0);
        glTexCoord2f(1, 1); glVertex2i(image->w*4, image->h*4);
        glTexCoord2f(0, 1); glVertex2i(0, image->h*4);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

int main(){
    window=SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);
        
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GLContext glContext=SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(SDL_FALSE);

    textures = new GLuint[3]; 
    glGenTextures(3, textures);

    loadTexture(textures[0], "1.png");
    loadTexture(textures[1], "2.png");
    loadTexture(textures[2], "3.png");
    loadTexture(textures[3], "4.png");

    timer.start();

    while(window!=NULL){
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT)
                window=NULL;
            if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE)
                window=NULL;
        }

        movement();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 800, 0, -10, 10);

        if(timer.inMilliseconds()>=0.1)
            showTexture(0);
        if(timer.inMilliseconds()>=0.2)
            showTexture(1);
        if(timer.inMilliseconds()>=0.3)
            showTexture(2);
        if(timer.inMilliseconds()>=0.4){
            showTexture(3);
            timer.restart();
        }

        displayTexture();

        SDL_GL_SwapWindow(window);
    }
}

// g++ file.cpp -o file -lSDL2 -lGL -lSDL2_image
