// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
//#include <GLFW/glfw3.h>
// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
//#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
//void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
//void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset );
//void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera  camera(glm::vec3( 0.0f, 0.0f, 3.0f ) );
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
//bool keys[1024];
const Uint8 * keys;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
//int main( )
int main(int argc, char ** argv) // SDL needs this form
{
    // Init GLFW
    //glfwInit( );
    SDL_Init(SDL_INIT_EVERYTHING);
    // Init SDL_image with JPG and PNG
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    //glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    //glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // Enable doublebuffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    //GLFWwindow* window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr ); // Windowed
    SDL_Window * window = SDL_CreateWindow("LearnOpenGL",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL); 
    
    if ( nullptr == window )
    {
        //std::cout << "Failed to create GLFW window" << std::endl;
        std::cerr << "Failed to create SDL window" << std::endl;
        //glfwTerminate( );
        IMG_Quit();
        SDL_Quit();
        
        return EXIT_FAILURE;
    }
    
    //glfwMakeContextCurrent( window );
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if ( nullptr == context )
    {
        std::cerr << "Failed to create SDL context" << std::endl;
        //glfwTerminate( );
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        
        return EXIT_FAILURE;
    }
    
    //glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    SDL_GL_GetDrawableSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    
    // Set the required callback functions
    //glfwSetKeyCallback( window, KeyCallback );
    //glfwSetCursorPosCallback( window, MouseCallback );
    //glfwSetScrollCallback( window, ScrollCallback );
    
    // Options, removes the mouse cursor for a more immersive experience
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // Setup some OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // enable alpha support
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    // Setup and compile our shaders
    Shader ourShader( "res/shaders/core.vs", "res/shaders/core.frag" ); 
    
    // Set up our vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] =
    {
        glm::vec3( 0.0f, 0.0f, 0.0f ),
        glm::vec3( 2.0f, 5.0f, -15.0f ),
        glm::vec3( -1.5f, -2.2f, -2.5f ),
        glm::vec3( -3.8f, -2.0f, -12.3f ),
        glm::vec3( 2.4f, -0.4f, -3.5f ),
        glm::vec3( -1.7f, 3.0f, -7.5f ),
        glm::vec3( 1.3f, -2.0f, -2.5f ),
        glm::vec3( 1.5f, 2.0f, -2.5f ),
        glm::vec3( 1.5f, 0.2f, -1.5f ),
        glm::vec3( -1.3f, 1.0f, -1.5f )
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray( VAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray( 0 );
    // TexCoord attribute
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 2 );
    
    glBindVertexArray( 0 ); // Unbind VAO
    
    // Load and create a texture
    GLuint texture;
    // --== TEXTURE == --
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture ); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // Set texture filtering
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // Load, create texture and generate mipmaps
    int width, height;
    //unsigned char *image = SOIL_load_image( "res/images/image1.jpg", &width, &height, 0, SOIL_LOAD_RGB );
    // Begin loading texture
    // Not my creation, just something I found with google: https://www.gamedev.net/forums/topic/677152-textures-with-sdl2-opengl/
    const char * imgFileName = "rs/images/image1.jpg";
    SDL_Surface *surface = IMG_Load(imgFileName); // this surface will tell us the details of the image
	GLint nColors;
	GLenum textureFormat;
    if(surface){
        //get number of channels in the SDL surface
        nColors = surface->format->BytesPerPixel;

        //contains an alpha channel
        if(nColors == 4)
        {
            if(surface->format->Rmask==0x000000ff)
                textureFormat = GL_RGBA;
            else
                textureFormat = GL_BGRA;
        }
        else if(nColors == 3) //no alpha channel
        {
            if(surface->format->Rmask==0x000000ff)
                textureFormat = GL_RGB;
            else
                textureFormat = GL_BGR;
        }
        else
        {
            std::cerr << "warning: the image is not truecolor…this will break " << std::endl;
        }

        // Have OpenGL generate a texture object handle for us
        glTexImage2D( GL_TEXTURE_2D, 0, textureFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

		width = surface->w;
		height = surface->h;
    }
    else {
		std::cerr << "LoadTexture:: Could not load " << imgFileName << ": " << SDL_GetError() << std::endl;
    }

    // Free the SDL_Surface only if it was successfully created
    if(surface) {
        SDL_FreeSurface(surface);
    }
    // End loading texture
    glGenerateMipmap( GL_TEXTURE_2D );
    //SOIL_free_image_data( image );
    glBindTexture( GL_TEXTURE_2D, 0 ); // Unbind texture when done, so we won't accidentily mess up our texture.
                                           
    bool shouldClose = false;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // Game loop
    //while( !glfwWindowShouldClose( window ) )
    while(!shouldClose)
    {
        // Set frame time
        //GLfloat currentFrame = glfwGetTime( );
        GLfloat currentFrame = SDL_GetTicks() / 1000.0f; // Because SDL_GetTicks returns milliseconds
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        //glfwPollEvents( );
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                shouldClose = true;
                break;
            // Movement keys handling is all done in DoMovement
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    shouldClose = true;
                    break;
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                GLfloat xOffset, yOffset;
                xOffset = event.motion.xrel;
                yOffset = event.motion.yrel;
                
                camera.ProcessMouseMovement( xOffset, -yOffset );
            } else if (event.type == SDL_MOUSEWHEEL) {
                camera.ProcessMouseScroll( event.wheel.y );
            }
        }
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        // Draw our first triangle
        ourShader.Use( );
        
        // Bind Textures using texture units
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture );
        glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture1" ), 0 );
        
        glm::mat4 projection;
        projection = glm::perspective(camera.GetZoom( ), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
        
        // Create camera transformation
        glm::mat4 view;
        view = camera.GetViewMatrix( );

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation( ourShader.program, "model" );
        GLint viewLoc = glGetUniformLocation( ourShader.program, "view" );
        GLint projLoc = glGetUniformLocation( ourShader.program, "projection" );
        
        // Pass the matrices to the shader
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        
        glBindVertexArray( VAO );
        
        for( GLuint i = 0; i < 10; i++ )
        {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model;
            model = glm::translate( model, cubePositions[i] );
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
            glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
            
            glDrawArrays( GL_TRIANGLES, 0, 36 );
        }
        
        glBindVertexArray( 0 );
        
        // Swap the buffers
        //glfwSwapBuffers( window );
        SDL_GL_SwapWindow(window);
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    //glfwTerminate( );
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    keys = SDL_GetKeyboardState(NULL); 
    // Camera controls
    //if( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    //if( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );      
    }
    
    //if( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
    {
        camera.ProcessKeyboard( LEFT, deltaTime );      
    }
    
    //if( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );       
    }
}
/*
// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}	


void ScrollCallback( GLFWwindow *window, double xOffset, double yOffset )
{
    camera.ProcessMouseScroll( yOffset );
}
*/
