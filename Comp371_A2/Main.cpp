//example skeleton code 2019 winter comp371
//modified from http://learnopengl.com/

#include "stdafx.h"
#include "glew/2.1.0/include/GL/glew.h"	// include GL Extension Wrangler
#include "glfw/3.2.1/include/GLFW/glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "OBJloader.h"  //include the object loader
#include "shaderloader.h"
using namespace std;

//*******************************
//          VARIABLES
//*******************************

float xPrev = 0.0f, yPrev = 0.0f;
bool canMove = false;

// camera
glm::vec3 camera_position = glm::vec3(0,2,-5);
glm::vec3 camera_direction = glm::vec3(0,0,1);
glm::vec3 camera_up = glm::vec3(0,1,0);

glm::vec3 translate_factor = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 scale_factor = glm::vec3(0.1f,0.1f,0.1f);
glm::vec3 axis = glm::vec3(0,0,1);
float angle = 0.0f;

//*******************************
//            INPUT
//*******************************

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (canMove) {
        camera_position += glm::vec3(xpos - xPrev, 0, ypos - yPrev);
        xPrev = xpos;
        yPrev = ypos;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        canMove = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        canMove = false;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            // move camera forward
            case GLFW_KEY_W:
                camera_position += camera_direction;
                break;
            // move camera back
            case GLFW_KEY_S:
                camera_position -= camera_direction;
                break;
            // move camera left
            case GLFW_KEY_A:
                camera_position += glm::cross(camera_direction, camera_up);
                break;
            // move camera right
            case GLFW_KEY_D:
                camera_position -= glm::cross(camera_direction, camera_up);
                break;
            // rotate camera left about up
            case GLFW_KEY_LEFT:
                camera_position += glm::vec3(-1,0,0);
                break;
            // rotate camera right about up
            case GLFW_KEY_RIGHT:
                camera_position += glm::vec3(1,0,0);
                break;
            // rotate camera up about right
            case GLFW_KEY_UP:
                camera_position += glm::vec3(0,1,0);
                break;
            // rotate camera down about right
            case GLFW_KEY_DOWN:
                camera_position += glm::vec3(0,-1,0);
                break;
            // scale up object
            case GLFW_KEY_O:
                scale_factor += 0.1f;
                break;
            //scale down object
            case GLFW_KEY_P:
                scale_factor -= 0.1f;
                break;
            // rotate object about X
            case GLFW_KEY_B:
                axis = glm::vec3(1,0,0);
                angle += 5.0f;
                break;
            // rotate object about Y
            case GLFW_KEY_N:
                axis = glm::vec3(0,1,0);
                angle += 5.0f;
                break;
            // rotate object about Z
            case GLFW_KEY_M:
                axis = glm::vec3(0,0,1);
                angle += 5.0f;
                break;
            // move object along +X
            case GLFW_KEY_J:
                translate_factor += glm::vec3(0.5f,0,0);
                break;
            // move object along -X
            case GLFW_KEY_L:
                translate_factor += glm::vec3(-0.5f,0,0);
                break;
            // move object along +Y
            case GLFW_KEY_I:
                translate_factor += glm::vec3(0,0.5f,0);
                break;
            // move object along -Y
            case GLFW_KEY_K:
                translate_factor += glm::vec3(0,-0.5f,0);
                break;
            // move object along +Z
            case GLFW_KEY_Y:
                translate_factor += glm::vec3(0,0,0.5f);
                break;
            // move object along -Z
            case GLFW_KEY_U:
                translate_factor += glm::vec3(0,0,-0.5f);
                break;
            default:
                break;
        }
    }
}

//*******************************
//        CREATE WINDOW
//*******************************

const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;


int init() {
	std::cout << "Starting GLFW context, OpenGL 4.1" << std::endl;
	glfwInit();
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL version 4.x
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL version x.1
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//WINDOW is created
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
    
    // check version
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
    
	return 0;
}

//*******************************
//        MAIN FUNCTION
//*******************************
int main()
{
    
	if (init() != 0)
		return EXIT_FAILURE;
    
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
    
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    //*******************************
    //          SHADERS
    //*******************************
    
	GLuint shader = loadSHADER("/Users/nat/Desktop/Classes/Comp 371/Comp371_A1/Comp371_A1/vertex.shader", "/Users/nat/Desktop/Classes/Comp 371/Comp371_A1/Comp371_A1/fragment.shader");
    
    // OBJ file vertices
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("/Users/nat/Desktop/Classes/Comp 371/Comp371_A1/Comp371_A1/cat.obj", vertices, normals, UVs);
    
    //*******************************
    //      SET UP VAO AND VBO
    //*******************************
    
    // VAO and VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    //*******************************
    //            CAMERA
    //*******************************
    
    // send matrices to shader
    GLuint ModelID = glGetUniformLocation(shader, "Model");
    GLuint ViewID = glGetUniformLocation(shader, "View");
    GLuint ProjectionID = glGetUniformLocation(shader, "Projection");
    
    // view box
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

    // camera
    glm::mat4 View = glm::lookAt(camera_direction, camera_position, camera_up);
    
    // object
    glm::mat4 Model = glm::mat4(1.0f);
    
    // backface culling
    glEnable(GL_CULL_FACE);
    
    // depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    
    //*******************************
    //           GAME LOOP
    //*******************************
    
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

    // Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        
        // apply transformations
        View = glm::lookAt(camera_direction, camera_position, camera_up);
        
        glm::mat4 translator = glm::translate(glm::mat4(1.0f), translate_factor);
        glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), angle, axis);
        glm::mat4 scalor = glm::scale(glm::mat4(1.0f), scale_factor);
        Model = translator * rotator * scalor;
        
        
        // calculate MVP and pass to shader
        glm::mat4 MVP = Projection * View * Model;
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ViewID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
