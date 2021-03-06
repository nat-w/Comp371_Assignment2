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
bool gourard = false;
bool red = true, green = true, blue = true, grey = false;
bool lightOff = false;
bool normalColor = false;

// camera
glm::vec3 camera_position = glm::vec3(0,5,20);
glm::vec3 camera_direction = glm::vec3(0,0,-1);
glm::vec3 camera_up = glm::vec3(0,1,0);

// model
glm::vec3 translate_factor = glm::vec3(0.0f,0.0f,4.0f);
glm::vec3 scale_factor = glm::vec3(0.1f,0.1f,0.1f);
glm::vec3 axis = glm::vec3(1,0,0);
float angle = -90.0f;

// light and object color
glm::vec3 light_color = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 object_color = glm::vec3(0.6f, 0.4f, 0.2f);

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
        std::cout << key << std::endl;
        switch (key) {
            // close window
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
            // resets model and camera
            case GLFW_KEY_ENTER:
                camera_position = glm::vec3(0,5,20);
                camera_direction = glm::vec3(0,0,-1);
                camera_up = glm::vec3(0,1,0);
                translate_factor = glm::vec3(0.0f,0.0f,4.0f);
                scale_factor = glm::vec3(0.1f,0.1f,0.1f);
                angle = -90.0f;
                axis = glm::vec3(1,0,0);
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
                camera_position.x += 0.5f;
                break;
            // rotate camera right about up
            case GLFW_KEY_RIGHT:
                camera_position.x -= 0.5f;
                break;
            // rotate camera up about right
            case GLFW_KEY_UP:
                camera_position.y += 0.5f;
                break;
            // rotate camera down about right
            case GLFW_KEY_DOWN:
                camera_position.y -= 0.5f;
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
            case GLFW_KEY_1:
                if (red) {
                    red = !red;
                    light_color.x = 0.0f;
                    object_color.x = 0.0f;
                }
                else {
                    light_color.x = 0.8f;
                    object_color.x = 0.6f;
                    red = !red;
                }
                break;
            case GLFW_KEY_2:
                if (green) {
                    light_color.y = 0.0f;
                    object_color.y = 0.0f;
                    green = !green;
                }
                else {
                    light_color.y = 0.8f;
                    object_color.y = 0.4f;
                    green = !green;
                }
                break;
            case GLFW_KEY_3:
                if (blue) {
                    light_color.z = 0.0f;
                    object_color.z = 0.0f;
                    blue = !blue;
                }
                else {
                    light_color.z = 0.8f;
                    object_color.z = 0.2f;
                    blue = !blue;
                }
                break;
            case GLFW_KEY_4:
                light_color = glm::vec3(0.8, 0.8, 0.8);
                object_color = glm::vec3(0.6, 0.4, 0.2);
                red = true;
                green = true;
                blue = true;
                break;
            case GLFW_KEY_5:
                gourard = !gourard;
                break;
            case GLFW_KEY_6:
                lightOff = !lightOff;
                break;
            case GLFW_KEY_7:
                normalColor = !normalColor;
                break;
            case GLFW_KEY_8:
                if (!grey) {
                    light_color = glm::vec3(0.2989 * 0.8, 0.5870 * 0.8, 0.1140 * 0.8);
                    object_color = glm::vec3(0.2989 * 0.6, 0.5870 * 0.4, 0.1140 * 0.2);
                    grey = !grey;
                }
                else {
                    light_color = glm::vec3(0.8, 0.8, 0.8);
                    object_color = glm::vec3(0.6, 0.4, 0.2);
                    grey = !grey;
                }
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2", nullptr, nullptr);

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
    
	GLuint shader = loadSHADER("/Users/nat/Desktop/Classes/Comp 371/Comp371_A2/Comp371_A2/vertex.shader", "/Users/nat/Desktop/Classes/Comp 371/Comp371_A2/Comp371_A2/fragment.shader");
    
    // OBJ file vertices
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	loadOBJ("/Users/nat/Desktop/Classes/Comp 371/Comp371_A2/Comp371_A2/heracles.obj", vertices, normals, UVs);
    
    //*******************************
    //      SET UP VAO AND VBO
    //*******************************
    
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(int), &vertices.front(), GL_STATIC_DRAW);
    
    // Unbind VAO
    glBindVertexArray(0);
    
    //*******************************
    //            CAMERA
    //*******************************
    
    GLuint ModelID = glGetUniformLocation(shader, "Model");
    GLuint ViewID = glGetUniformLocation(shader, "View");
    GLuint ProjectionID = glGetUniformLocation(shader, "Projection");
    
    // view box
    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

    // camera
    glm::mat4 View = glm::lookAt(camera_direction, camera_position, camera_up);
    
    // model matrix
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
		glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shader);
        
        // apply transformations
        View = glm::lookAt(camera_direction, camera_position + camera_direction, camera_up);
        
        glm::mat4 translator = glm::translate(glm::mat4(1.0f), translate_factor);
        glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), angle, axis);
        glm::mat4 scalor = glm::scale(glm::mat4(1.0f), scale_factor);
        Model = translator * rotator * scalor;
        
        // pass MVP to shader
        glUniformMatrix4fv(ModelID, 1, 0, glm::value_ptr(Model));
        glUniformMatrix4fv(ViewID, 1, 0, glm::value_ptr(View));
        glUniformMatrix4fv(ProjectionID, 1, 0, glm::value_ptr(Projection));
        
        // pass light color and variables to shader
        glUniform3fv(glGetUniformLocation(shader, "object_color"), 1, glm::value_ptr(object_color));
        glUniform3fv(glGetUniformLocation(shader, "light_color"), 1, glm::value_ptr(light_color));
        glUniform3fv(glGetUniformLocation(shader, "light_position"), 1, glm::value_ptr(glm::vec3(0,20,5)));
        glUniform3fv(glGetUniformLocation(shader, "view_position"), 1, glm::value_ptr(camera_position));
        
        glUniform1i(glGetUniformLocation(shader, "gourard"), gourard);
        glUniform1i(glGetUniformLocation(shader, "lightOff"), lightOff);
        glUniform1i(glGetUniformLocation(shader, "normalColor"), normalColor);

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
