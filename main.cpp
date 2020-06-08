#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

SDL_Window *window;
SDL_Event event;

float x=0, y=0;

const Uint8 *key=SDL_GetKeyboardState(0);

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

int main(){
    window=SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GLContext context=SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(SDL_FALSE);

    GLuint textureID=0;
    int mode=0;

    // Texture
    SDL_Surface *texture=IMG_Load("1.png");

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if(texture->format->BytesPerPixel==4)
        mode=GL_RGBA;
    else
        mode=GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Texture

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

        glTranslatef(x, y, 0);

        glEnable(GL_TEXTURE_2D);
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2i(0, 0); 
        glTexCoord2f(1, 0); glVertex2i(texture->w*4, 0);
        glTexCoord2f(1, 1); glVertex2i(texture->w*4, texture->h*4);
        glTexCoord2f(0, 1); glVertex2i(0, texture->h*4);
        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        SDL_GL_SwapWindow(window);
    }
}

// g++ file.cpp -o file -lSDL2 -lGL -lSDL2_image
