#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void generateMeshSurface(int width,int height, std::vector<float> *vertices, std::vector<unsigned int> *indices)
{
    const int nAttributesPerVertex = 6;

    vertices->resize(width * height * nAttributesPerVertex);

    const int nIndices = (width - 1) * (height - 1) * 2 * 3;
    indices->resize(nIndices);

    for (int iy = 0; iy < height; iy++)
    {
        for (int ix = 0; ix < width; ix++)
        {
            int vertexIndex = (ix + iy * width) * nAttributesPerVertex;//6 attributes
            (*vertices)[vertexIndex + 0] = (float)ix * 10;
            (*vertices)[vertexIndex + 1] = -2.5f;
            (*vertices)[vertexIndex + 2] = (float)iy * 10;
            (*vertices)[vertexIndex + 3] = 0.0f;
            (*vertices)[vertexIndex + 4] = 0.0f;//cosf(float(ix)) * 1.0f;
            (*vertices)[vertexIndex + 5] = 0.0f;//sinf(float(ix)) * 1.0f;
        }
    }

    for (int iy = 0; iy < height - 1; iy++)
    {
        for (int ix = 0; ix < width - 1; ix++)
        {
            //rectangle
            int vertexIndex = (ix + iy * (width - 1)) * 2*3; //triangles per square * vertices per triangle = 6
            //first (upper) triangle
            (*indices)[vertexIndex + 0] = ((iy + 0) * (width)+(ix + 0));
            (*indices)[vertexIndex + 1] = ((iy + 0) * (width)+(ix + 1));
            (*indices)[vertexIndex + 2] = ((iy + 1) * (width)+(ix + 1));
            //second (bottom) triangle
            (*indices)[vertexIndex + 3] = ((iy + 0) * (width)+(ix + 0));
            (*indices)[vertexIndex + 4] = ((iy + 1) * (width)+(ix + 1));
            (*indices)[vertexIndex + 5] = ((iy + 1) * (width)+(ix + 0));

        }
    }
}

glm::vec3 mean(glm::vec3 p1, glm::vec3 p2)
{
    return { (p1.x + p2.x) / 2,(p1.y + p2.y) / 2 ,(p1.z + p2.z) / 2 };
}

void generateGeodesicSphere(unsigned int triangleIndices[3], glm::vec3 triangleVertex[3], std::vector<float>* vertices, std::vector<unsigned int>* indices, float nResursiveSteps)
{
    if (nResursiveSteps <= 0)
    {
        (*indices).push_back(triangleIndices[0]);
        (*indices).push_back(triangleIndices[1]);
        (*indices).push_back(triangleIndices[2]);
        return;
    }

    glm::vec3 subTriangle_1[3] = { triangleVertex[0], mean(triangleVertex[0], triangleVertex[1]), mean(triangleVertex[0], triangleVertex[2]) };
    
    glm::vec3 subTriangle_2[3] = { triangleVertex[1], mean(triangleVertex[1], triangleVertex[0]), mean(triangleVertex[1], triangleVertex[2]) };
    glm::vec3 subTriangle_3[3] = { triangleVertex[2], mean(triangleVertex[2], triangleVertex[0]), mean(triangleVertex[2], triangleVertex[1]) };
    glm::vec3 subTriangle_4[3] = { mean(triangleVertex[0], triangleVertex[1]), mean(triangleVertex[1], triangleVertex[2]), mean(triangleVertex[2], triangleVertex[0]) };

    unsigned int lastIndices[3];
    for (int i = 0; i < 3;i++)
    {       
        lastIndices[i] = vertices->size() / 6 ;

        vertices->push_back(subTriangle_4[i].x);
        vertices->push_back(subTriangle_4[i].y);
        vertices->push_back(subTriangle_4[i].z);
        vertices->push_back(0);
        vertices->push_back(0);
        vertices->push_back(0);
    }

    unsigned int subtriangleVertexIndex_1[3] = { triangleIndices[0], lastIndices[0] ,lastIndices[2] };
    unsigned int subtriangleVertexIndex_2[3] = { triangleIndices[1], lastIndices[0] ,lastIndices[1] };
    unsigned int subtriangleVertexIndex_3[3] = { triangleIndices[2], lastIndices[2] ,lastIndices[1] };
    unsigned int subtriangleVertexIndex_4[3] = { lastIndices[0], lastIndices[1] ,lastIndices[2] };

    generateGeodesicSphere(subtriangleVertexIndex_1, subTriangle_1, vertices, indices, nResursiveSteps - 1);
    generateGeodesicSphere(subtriangleVertexIndex_2, subTriangle_2, vertices, indices, nResursiveSteps - 1);
    generateGeodesicSphere(subtriangleVertexIndex_3, subTriangle_3, vertices, indices, nResursiveSteps - 1);
    generateGeodesicSphere(subtriangleVertexIndex_4, subTriangle_4, vertices, indices, nResursiveSteps - 1);
}


double mouseSensitivity_X = 2.5f;
double mouseSensitivity_Y = 2.5f;
void processInput(GLFWwindow* window)
{
    static double absolutePosX = 0.0f, absolutePosY = 0.0f;

    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX,&mousePosY);
    absolutePosX += mousePosX;    
    absolutePosY += mousePosY;
    glfwSetCursorPos(window, 0,0);
    
    cameraFront.x = sin(mouseSensitivity_X*absolutePosX/SCR_WIDTH); //right
    cameraFront.y = 0.0f; //up
    cameraFront.z = -cos(mouseSensitivity_Y*absolutePosX/SCR_WIDTH); //backward

        const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window,true);
}

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glEnable(GL_DEPTH_TEST);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("vertexShader.glsl", "fragmentShader.glsl"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    //generateMeshSurface(100, 100, &vertices, &indices);
    vertices.resize(36);
    //indices.resize(3);

    vertices[0] = 1.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    vertices[3] = 0.0f;
    vertices[4] = 0.0f;
    vertices[5] = 1.0f;
    
    vertices[6] = 0.0f;
    vertices[7] = 0.0f;
    vertices[8] = 1.0f;
    vertices[9] = 0.0f;
    vertices[10] = 0.0f;
    vertices[11] = 1.0f;

    vertices[12] = -1.0f;
    vertices[13] = 0.0f;
    vertices[14] = 0.0f;
    vertices[15] = 0.0f;
    vertices[16] = 0.0f;
    vertices[17] = 1.0f;


    vertices[18] = 0.0f;
    vertices[19] = 0.0f;
    vertices[20] = -1.0f;
    vertices[21] = 0.0f;
    vertices[22] = 0.0f;
    vertices[23] = 1.0f;

    vertices[24] = 0.0f;
    vertices[25] = 1.0f;
    vertices[26] = 0.0f;
    vertices[27] = 0.0f;
    vertices[28] = 0.0f;
    vertices[29] = 1.0f;

    vertices[30] = 0.0f;
    vertices[31] = -1.0f;
    vertices[32] = 0.0f;
    vertices[33] = 0.0f;
    vertices[34] = 0.0f;
    vertices[35] = 1.0f;


    unsigned int triangleIndices[8][3] = { { 0,1,4 }, { 1,2,4 }, { 2,3,4 }, { 3,0,4 }, { 0,1,5 }, { 1,2,5 }, { 2,3,5 },{ 3,0,5 }};
    
    
    glm::vec3 triangleVertex[8][3];
    
    for (int i = 0; i < 8; i++)
    {
        for (int i2 = 0; i2 < 3; i2++)
        {
            triangleVertex[i][i2].x = vertices[triangleIndices[i][i2]*6 + 0];
            triangleVertex[i][i2].y = vertices[triangleIndices[i][i2]*6 + 1];
            triangleVertex[i][i2].z = vertices[triangleIndices[i][i2]*6 + 2];

        }
    }
     
    for (int i = 0; i< 8;i++)
        generateGeodesicSphere(&(triangleIndices[i][0]), &(triangleVertex[i][0]), &vertices, &indices, 5);
    
    for (int i = 0; i < vertices.size() / 6; i++)
    {
        float a = vertices[i*6 + 0];
        float b = vertices[i*6 + 1];
        float c = vertices[i*6 + 2];
        float length = a * a + b * b + c * c;
        length = sqrtf(length);

        vertices[i*6 + 0] /= length/10;
        vertices[i*6 + 1] /= length/10;
        vertices[i*6 + 2] /= length/10;

        vertices[i * 6 + 3] = vertices[i * 6 + 0] / 10;
        vertices[i * 6 + 4] = vertices[i * 6 + 1] / 10;
        vertices[i * 6 + 5] = vertices[i * 6 + 2] / 10;
    }

    //randomize
    /*for (int i = 0; i < vertices.size() / 6; i++)
    {
        float k = (float)((rand() % 100) / 100.0f) * 0.02f + 1.0f;
        vertices[i * 6 + 0] *= k;
        vertices[i * 6 + 1] *= k;
        vertices[i * 6 + 2] *= k;
    }*/


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    unsigned int elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);


    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal vector attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(120.0f), 800.0f / 600.0f, 0.05f, 1000.0f);
    
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        static float aa = 0.1f;
        //model = glm::rotate(model, glm::radians(aa), glm::vec3(1.0f, 0.0f, 0.0f));
        
         // camera/view transformation
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        
        // render the triangle
        ourShader.use();
        glBindVertexArray(VAO);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}