#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "myShader.h"
//#include "doublyCircularLinkedList.h"

#include <math.h>
#include <functional>
#include <iostream>
#include <vector>
#include <list>
#include "SimplexNoise.h"

#include "Hash.h"
#include "Handlers.h"
using namespace std;


typedef std::list<OBJECT>::iterator nodeReference;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    ourShader = new Shader("vertexShader.glsl", "fragmentShader.glsl"); // you can name your shader files however you like

    float farZplane = 150.0;
    float nearZplane = 0.005;

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection    = glm::perspective(glm::radians(80.0f), 800.0f / 600.0f, nearZplane, farZplane);
    glm::vec3 objectColor   = glm::vec3(1,0,0);

    int colorLoc        = glGetUniformLocation(ourShader->ID, "objectColor");
    int modelLoc        = glGetUniformLocation(ourShader->ID, "model");
    int viewLoc         = glGetUniformLocation(ourShader->ID, "view");
    int projectionLoc   = glGetUniformLocation(ourShader->ID, "projection");
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glFrustum(0.0, SCR_WIDTH, SCR_HEIGHT, 0.0, nearZplane, farZplane);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    int subdivision = 6;
    float planetRadius = 7.0f;
    cameraPos = glm::vec3(0.0f, 0.0f, planetRadius);
    glm::vec3 playerPos = cameraPos;
    float visionDistance = planetRadius/50*5*2;

    
    OBJECT planetSphere;
    planetSphere.isEnabled = true;
    planetSphere.rasterization = GL_FILL;
    planetSphere.color = glm::vec3(0.5, 0.5, 1);
    generateUnitIcosphere(&planetSphere, subdivision);
    //object[0].modelMatrix = glm::scale(object[0].modelMatrix, glm::vec3(planetRadius, planetRadius, planetRadius));
    scale(&planetSphere.vertexBuffer, planetRadius-0.1f);
    bindObjectBuffers(&planetSphere);
    planetSphere.isEnabled = true;

    hashtable<nodeReference> worldTileHashTable;
    std::vector<nodeReference> outerTriangleIndices;

    
    while (!glfwWindowShouldClose(window))
    {
        glm::vec3 pointOnSphere = planetRadius * cameraPos / glm::distance(cameraPos, glm::vec3(0));
        
        sphericalPatch(subdivision, &object, &worldTileHashTable, &outerTriangleIndices, planetRadius, visionDistance, cameraPos);
        //sphericalPatch(subdivision, &object, &worldTileHashTable, &outerTriangleIndices, planetRadius, visionDistance, cameraPos);
        //sphericalPatch(subdivision, &object, &worldTileHashTable, &outerTriangleIndices, planetRadius, visionDistance, cameraPos);

        //spherify(,);
        
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        ourShader->use();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //render objects
        for (auto iter : object)
            renderObject(&iter, colorLoc, modelLoc);
        
        renderObject(&planetSphere, colorLoc, modelLoc);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    worldTileHashTable.clear();
    outerTriangleIndices.clear();
    object.clear();
    //while (1);

    for (auto iter : object)
    {
        glDeleteVertexArrays(1, &(iter.VAO));
        glDeleteBuffers(1, &(iter.VBO));
    }
    
    glfwTerminate();
    return 0;
}




// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
