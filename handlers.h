#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <functional>
#include <vector>

#include <list>
#include "myShader.h"
#include "SimplexNoise.h"

struct OBJECT
{
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertexBuffer;
    std::vector<unsigned int> indexBuffer;
    glm::vec3 cornerVertices[3];
    glm::mat4 modelMatrix;
    glm::vec3 color;
    bool isEnabled = true;
    GLenum rasterization = GL_FILL;
};
std::list<OBJECT> object;

GLFWwindow* window;
Shader* ourShader;

void bindObjectBuffers(OBJECT* obj)
{
    glBindVertexArray(0);
    glGenVertexArrays(1, &(obj->VAO));
    glBindVertexArray(obj->VAO);

    glGenBuffers(1, &(obj->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);

    unsigned int elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indexBuffer.size() * sizeof(unsigned int), obj->indexBuffer.data(), GL_STATIC_DRAW);


    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj->vertexBuffer.size(), obj->vertexBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal vector attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    obj->modelMatrix = glm::mat4(1.0f);
}


void updateObjectBuffers(OBJECT* obj)
{
    glBindVertexArray(0);
    glGenVertexArrays(1, &(obj->VAO));
    glBindVertexArray(obj->VAO);

    glGenBuffers(1, &(obj->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);

    unsigned int elementBuffer;
    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indexBuffer.size() * sizeof(unsigned int), obj->indexBuffer.data(), GL_STATIC_DRAW);


    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj->vertexBuffer.size(), obj->vertexBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal vector attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    obj->modelMatrix = glm::mat4(1.0f);
}


void addTriangle(std::vector<unsigned int>* indices, unsigned int a, unsigned int b, unsigned int c)
{
    indices->push_back(a);
    indices->push_back(b);
    indices->push_back(c);
}

void recalculateNormals(OBJECT* obj)
{
    //iterate each triangle
    unsigned int nTriangles = obj->indexBuffer.size() / 3;
    for (int i = 0; i < nTriangles; i++)
    {
        glm::vec3 points[3];

        //find the vertices of the triangle and store them in points vector.
        for (int k = 0; k < 3; k++)
        {
            points[k].x = (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 0];
            points[k].y = (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 1];
            points[k].z = (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 2];
        }

        //calculate the normal vector using cross product.
        glm::vec3 a = points[0] - points[1];
        glm::vec3 b = points[0] - points[2];
        glm::vec3 normal = glm::cross(a, b);

        if (glm::dot(normal, points[0]) < 0)
            normal = glm::cross(b, a);

        //assign the calculated normal to the triangle vertices
        for (int k = 0; k < 3; k++)
        {
            (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 3] = normal.x;
            (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 4] = normal.y;
            (obj->vertexBuffer)[((obj->indexBuffer)[i * 3 + k]) * 6 + 5] = normal.z;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



void spherify(std::vector<float>* vertices,float rad)
{
    for (int i = 0; i < vertices->size() / 6; i++)
    {
        float x = (*vertices)[i * 6 + 0];
        float y = (*vertices)[i * 6 + 1];
        float z = (*vertices)[i * 6 + 2];

        float length = x * x + y * y + z * z;
        length = sqrt(length);

        (*vertices)[i * 6 + 0] /= length;
        (*vertices)[i * 6 + 1] /= length;
        (*vertices)[i * 6 + 2] /= length;

        (*vertices)[i * 6 + 0] *= rad;
        (*vertices)[i * 6 + 1] *= rad;
        (*vertices)[i * 6 + 2] *= rad;
    }
}
/*/
float lerp(float x, float y, float t)
{
    return x * (1.f - t) + y * t;
}


float interpolate1D(float v1, float v2, float x)
{
    return v1 * (1 - x) + v2 * x;
}

float interpolate2D(float v1, float v2, float v3, float v4, float x, float y) {

    float s = interpolate1D(v1, v2, x);
    float t = interpolate1D(v3, v4, x);
    return interpolate1D(s, t, y);
}
float interpolate3D(float v1, float v2, float v3, float v4, float v5, float v6, float v7,float v8, float x, float y, float z)
{
    float s = interpolate2D(v1, v2, v3, v4, x, y);//top
    float t = interpolate2D(v5, v6, v7, v8, x, y);//bottom
    return interpolate1D(s, t, z);
}
*/
std::hash<int> h;

void randomize(std::vector<float>* vertices)
{
    for (int i = 0; i < vertices->size() / 6; i++)
    {/*
        float x = (*vertices)[i * 6 + 0];
        float y = (*vertices)[i * 6 + 1];
        float z = (*vertices)[i * 6 + 2];

        float factor = 1.0f;
        glm::vec3 points[8];
        points[0] = glm::vec3(floor(x * factor), floor(y * factor), floor(z * factor));
        points[1] = glm::vec3(1 + floor(x * factor), floor(y * factor), floor(z * factor));
        points[2] = glm::vec3(floor(x * factor), 1 + floor(y * factor), floor(z * factor));
        points[3] = glm::vec3(1 + floor(x * factor), 1 + floor(y * factor), floor(z * factor));

        points[4] = glm::vec3(floor(x * factor), floor(y * factor), 1 + floor(z * factor));
        points[5] = glm::vec3(1 + floor(x * factor), floor(y * factor), 1 + floor(z * factor));
        points[6] = glm::vec3(floor(x * factor), 1 + floor(y * factor), 1 + floor(z * factor));
        points[7] = glm::vec3(1 + floor(x * factor), 1 + floor(y * factor), 1 + floor(z * factor));

        glm::vec3 p = glm::vec3((x * factor), (y * factor), (z * factor));

        float randomValues[8];
        for (int i = 0; i < 8; i++)
        {
            int key = (int)points[i].x;
            key += (int)points[i].y;
            key += (int)points[i].z;

            randomValues[i] = (float)(h(key) % 10000);
        }
        float distance[8];
        for (int i = 0; i < 8; i++)
        {
            distance[i] = glm::distance(points[i], p);
        }
        */

        float x = (*vertices)[i * 6 + 0];
        float y = (*vertices)[i * 6 + 1];
        float z = (*vertices)[i * 6 + 2];
        glm::vec3 p = glm::vec3(x,y,z);

        float r = 1;
        SimplexNoise n1(1, 10, 2, 0.5);
        //SimplexNoise n2(50.0f, 1, 2, 0.5);
        
        r = n1.fractal(10, p.x, p.y, p.z) * 0.007;
        r += n1.noise(p.x, p.y, p.z) * 0.02;
        r += 1;

        //r = r*0.01f + 1.0f;

        (*vertices)[i * 6 + 0] *= r;
        (*vertices)[i * 6 + 1] *= r;
        (*vertices)[i * 6 + 2] *= r;
    }
}

void scale(std::vector<float>* vertices, float scale)
{
    for (int i = 0; i < vertices->size() / 6; i++)
    {
        (*vertices)[i * 6 + 0] *= scale;
        (*vertices)[i * 6 + 1] *= scale;
        (*vertices)[i * 6 + 2] *= scale;
    }
}


void insertVertex(std::vector<float>* vertices, float x, float y, float z)
{
    vertices->push_back(x);
    vertices->push_back(y);
    vertices->push_back(z);
    vertices->push_back(0.0f);
    vertices->push_back(0.0f);
    vertices->push_back(0.0f);
}

void generateUnitIcosphere(OBJECT* obj, int samples)
{
    float triangleSideLength = 1 / pow(2.0, (double)samples);
    int nSideTriangles = pow(2.0, (double)samples);

    //samples > 0
    for (int iy = -nSideTriangles; iy <= 0; iy++)
    {
        int from = -nSideTriangles + abs(iy);
        int to = abs(from);

        for (int ix = from; ix <= to; ix++)
        {
            float x = (float)ix * triangleSideLength;
            float y = (float)iy * triangleSideLength;

            float z = x + y + 1;
            if (ix > 0)
                z = -x + y + 1;
            insertVertex(&(obj->vertexBuffer), x, z, y);
        }

        for (int ix = from + 1; ix <= to - 1; ix++)
        {
            float x = (float)ix * triangleSideLength;
            float y = (float)iy * triangleSideLength;

            float z = -x - y - 1;
            if (ix > 0)
                z = x - y - 1;

            insertVertex(&(obj->vertexBuffer), x, z, y);
        }
    }

    for (int iy = 1; iy <= nSideTriangles; iy++)
    {
        int from = -nSideTriangles + abs(iy);
        int to = abs(from);

        for (int ix = from; ix <= to; ix++)
        {
            float x = (float)ix * triangleSideLength;
            float y = (float)iy * triangleSideLength;

            float z = x - y + 1;
            if (ix > 0)
                z = -x - y + 1;

            insertVertex(&(obj->vertexBuffer), x, z, y);
        }

        for (int ix = from + 1; ix <= to - 1; ix++)
        {
            float x = (float)ix * triangleSideLength;
            float y = (float)iy * triangleSideLength;
            float z = -x + y - 1;
            if (ix > 0)
                z = x + y - 1;
            insertVertex(&(obj->vertexBuffer), x, z, y);
        }
    }

    std::vector<int> list;
    list.resize(nSideTriangles * 2 + 1);

    int previousIndex = 1;
    int currentIndex = 0;
    for (int iy = 1; iy < nSideTriangles + 1 + 1; iy++)
    {
        currentIndex = previousIndex + 4 * (iy - 1);
        list[iy] = currentIndex;
        previousIndex = currentIndex;
    }

    for (int iy = 1; iy < nSideTriangles; iy++)
    {
        float diff = list[(nSideTriangles + 2) - iy - 1] - list[(nSideTriangles + 1) - iy - 1];
        list[(nSideTriangles + 1) + iy] = diff + list[(nSideTriangles + 1) + iy - 1];

    }

    //topleft
    for (int iy = 1; iy < nSideTriangles + 1; iy++)
    {
        int prevStartingIndex = list[iy - 1];
        int currStartingIndex = list[iy];

        //topleft
        for (int ix = 0; ix < iy; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix;
            int bottomCurrentIndex = currStartingIndex + ix;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, bottomCurrentIndex, bottomCurrentIndex + 1);


            if (ix == 0)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex, bottomCurrentIndex, bottomCurrentIndex + (iy) * 2 + 1);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (iy - 1) * 2, bottomCurrentIndex + iy * 2, bottomCurrentIndex + iy * 2 + 1);
            }
        }

        for (int ix = 0; ix < iy - 1; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix;
            int bottomCurrentIndex = currStartingIndex + ix;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + 1, bottomCurrentIndex + 1);

            if (ix == 0)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + (iy - 1) * 2 + 1, bottomCurrentIndex + (iy) * 2 + 1);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (iy - 1) * 2, topCurrentIndex + (iy - 1) * 2 + 1, bottomCurrentIndex + iy * 2 + 1);
            }
        }

        //topright
        for (int ix = 0; ix < iy; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix + iy - 1;
            int bottomCurrentIndex = currStartingIndex + ix + iy;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, bottomCurrentIndex, bottomCurrentIndex + 1);

            if (ix == iy - 1)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex, bottomCurrentIndex + (iy) * 2, bottomCurrentIndex + 1);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (iy - 1) * 2, bottomCurrentIndex + iy * 2, bottomCurrentIndex + iy * 2 + 1);
            }
        }

        for (int ix = 0; ix < iy - 1; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix + iy - 1;
            int bottomCurrentIndex = currStartingIndex + ix + iy;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + 1, bottomCurrentIndex + 1);

            if (ix == iy - 1 - 1)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (iy - 1) * 2, topCurrentIndex + 1, bottomCurrentIndex + (iy) * 2 + 1);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (iy - 1) * 2, topCurrentIndex + (iy - 1) * 2 + 1, bottomCurrentIndex + iy * 2 + 1);
            }
        }

    }

    //bottom
    int nT = nSideTriangles;
    for (int iy = nSideTriangles; iy < 2 * nSideTriangles; iy++, nT--)
    {
        int prevStartingIndex = list[iy];
        int currStartingIndex = list[iy + 1];

        //bottomright
        for (int ix = 0; ix < nT; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix;
            int bottomCurrentIndex = currStartingIndex + ix;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + 1, bottomCurrentIndex);

            if (ix == 0)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2, topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex + (nT - 1) * 2);
            }
        }

        for (int ix = 0; ix < nT - 1; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix;
            int bottomCurrentIndex = currStartingIndex + ix;
            addTriangle(&(obj->indexBuffer), topCurrentIndex + 1, bottomCurrentIndex, bottomCurrentIndex + 1);

            if (ix == 0)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex, bottomCurrentIndex + (nT - 1) * 2 + 1);
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex + (nT - 1) * 2, bottomCurrentIndex + (nT - 1) * 2 + 1);
            }
        }


        //topright
        for (int ix = 0; ix < nT; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix + nT;
            int bottomCurrentIndex = currStartingIndex + ix + nT - 1;
            addTriangle(&(obj->indexBuffer), topCurrentIndex, topCurrentIndex + 1, bottomCurrentIndex);


            if (ix == nT - 1)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2, topCurrentIndex + 1, bottomCurrentIndex); // problem???
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2, topCurrentIndex + nT * 2 + 1, bottomCurrentIndex + (nT - 1) * 2);
            }
        }

        for (int ix = 0; ix < nT - 1; ix++)
        {
            int topCurrentIndex = prevStartingIndex + ix + nT;
            int bottomCurrentIndex = currStartingIndex + ix + nT - 1;
            addTriangle(&(obj->indexBuffer), topCurrentIndex + 1, bottomCurrentIndex, bottomCurrentIndex + 1);

            if (ix == nT - 2)
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex + (nT - 1) * 2, bottomCurrentIndex + 1); // problem???
            }
            else
            {
                addTriangle(&(obj->indexBuffer), topCurrentIndex + (nT) * 2 + 1, bottomCurrentIndex + (nT - 1) * 2, bottomCurrentIndex + (nT - 1) * 2 + 1);
            }
        }

    }

    spherify(&(obj->vertexBuffer),1);
    recalculateNormals(obj);
    bindObjectBuffers(obj);
}



double mouseSensitivity_X = 2.5f;
double mouseSensitivity_Y = 2.5f;
void processInput(GLFWwindow* window)
{
    static double absolutePosX = 0.0f, absolutePosY = 0.0f;

    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    absolutePosX += mousePosX;
    absolutePosY += mousePosY;
    glfwSetCursorPos(window, 0, 0);

    cameraFront.x = sin(mouseSensitivity_X * absolutePosX / SCR_WIDTH); //right
    cameraFront.y = 0.0f; //up
    cameraFront.z = -cos(mouseSensitivity_Y * absolutePosX / SCR_WIDTH); //backward

    const float cameraSpeed = 0.01f; // adjust accordingly
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
        glfwSetWindowShouldClose(window, true);
}





void randomNoise(std::vector<float>* vertices, float noiseFactor)
{
    std::hash<int> h;

    for (int i = 0; i < vertices->size() / 6; i++)
    {
        float a = (*vertices)[i * 6 + 0];
        float b = (*vertices)[i * 6 + 1];
        float c = (*vertices)[i * 6 + 2];

        int randomNumber = (h((int)(100.0f * (a + b + c))));

        noiseFactor = 0.01f;
        float k = (float)((randomNumber % 100) / 100.0f) * noiseFactor + 1.0f;

        //k = randomNumber;
        (*vertices)[i * 6 + 0] *= k;
        (*vertices)[i * 6 + 1] *= k;
        (*vertices)[i * 6 + 2] *= k;
    }
}



glm::vec3 markerPos[8];
void calculateTriangleVertices(glm::vec3* vertices, glm::vec3 p, int radius, int samples)
{
    float polarity_x = 1, polarity_y = 1, polarity_z = 1;

    if (p.x < 0)
        polarity_x = -1;
    if (p.y < 0)
        polarity_y = -1;
    if (p.z < 0)
        polarity_z = -1;

    float a = 1 / (polarity_y * p.y + polarity_x * p.x + polarity_z * p.z);
    p.x *= a;
    p.y *= a;
    p.z *= a;

    float triangleNsegments = pow(2.0, (double)samples);

    float upperZ = ceil(triangleNsegments * p.z) / triangleNsegments;
    float lowerZ = floor(triangleNsegments * p.z) / triangleNsegments;

    float upperY = ceil(triangleNsegments * p.y) / triangleNsegments;
    float lowerY = floor(triangleNsegments * p.y) / triangleNsegments;

    float upperX = ceil(triangleNsegments * p.x) / triangleNsegments;
    float lowerX = floor(triangleNsegments * p.x) / triangleNsegments;


    //edge case handling: when p is perfect integer
    float segLength = 1 / triangleNsegments;
    if (p.x == (int)(p.x))lowerX = p.x + segLength;
    if (p.y == (int)(p.y))lowerY = p.y + segLength;
    if (p.z == (int)(p.z))upperZ = p.z - segLength;

    markerPos[0].x = lowerX;
    markerPos[0].y = upperY;
    markerPos[0].z = lowerZ;

    markerPos[1].x = lowerX;
    markerPos[1].y = upperY;
    markerPos[1].z = upperZ;

    markerPos[2].x = upperX;
    markerPos[2].y = upperY;
    markerPos[2].z = lowerZ;

    markerPos[3].x = upperX;
    markerPos[3].y = upperY;
    markerPos[3].z = upperZ;

    markerPos[4].x = lowerX;
    markerPos[4].y = lowerY;
    markerPos[4].z = lowerZ;

    markerPos[5].x = lowerX;
    markerPos[5].y = lowerY;
    markerPos[5].z = upperZ;

    markerPos[6].x = upperX;
    markerPos[6].y = lowerY;
    markerPos[6].z = lowerZ;

    markerPos[7].x = upperX;
    markerPos[7].y = lowerY;
    markerPos[7].z = upperZ;

    for (int i = 0, i2 = 0; i < 8; i++)
    {
        bool a = polarity_y * markerPos[i].y < 1 - polarity_x * markerPos[i].x - polarity_z * markerPos[i].z + 1 / triangleNsegments / 2;
        bool b = polarity_y * markerPos[i].y > 1 - polarity_x * markerPos[i].x - polarity_z * markerPos[i].z - 1 / triangleNsegments / 2;

        //assert(vertices == NULL);

        if (a && b && i2 < 3)
        {
            vertices[i2] = markerPos[i] / glm::length(markerPos[i]);
            vertices[i2] *= radius;
            i2++;
        }
    }
}

int findNeigborId(glm::vec3 tri00, glm::vec3 tri01, glm::vec3 theTriangle_0, glm::vec3 theTriangle_1, glm::vec3 theTriangle_2)
{
    if (tri00 == theTriangle_0 && tri01 == theTriangle_1 || tri00 == theTriangle_1 && tri01 == theTriangle_0)
        return 0;
    else if (tri00 == theTriangle_1 && tri01 == theTriangle_2 || tri00 == theTriangle_2 && tri01 == theTriangle_1)
        return 1;
    else if (tri00 == theTriangle_2 && tri01 == theTriangle_0 || tri00 == theTriangle_0 && tri01 == theTriangle_2)
        return 2;

    return -1;
}

void insertTriangle(std::vector<float>* vertices, std::vector<unsigned int>* indices, glm::vec3* triangleVertices)
{
    int nVertices = vertices->size();

    //allocate more space for new vertices
    vertices->resize(nVertices + 6 * 3);
    (*vertices)[nVertices + 6 * 0 + 0] = triangleVertices[0].x;
    (*vertices)[nVertices + 6 * 0 + 1] = triangleVertices[0].y;
    (*vertices)[nVertices + 6 * 0 + 2] = triangleVertices[0].z;

    (*vertices)[nVertices + 6 * 1 + 0] = triangleVertices[1].x;
    (*vertices)[nVertices + 6 * 1 + 1] = triangleVertices[1].y;
    (*vertices)[nVertices + 6 * 1 + 2] = triangleVertices[1].z;

    (*vertices)[nVertices + 6 * 2 + 0] = triangleVertices[2].x;
    (*vertices)[nVertices + 6 * 2 + 1] = triangleVertices[2].y;
    (*vertices)[nVertices + 6 * 2 + 2] = triangleVertices[2].z;

    indices->push_back(nVertices / 6 + 0);
    indices->push_back(nVertices / 6 + 1);
    indices->push_back(nVertices / 6 + 2);
}

struct triangle
{
    int neighbor_ID[3];
};
std::vector<triangle> neighborList;

bool compareFloat(glm::vec3 a, glm::vec3 b, float precision)
{
    glm::vec3 diff = (a - b);

    if (abs(diff.x) < precision && abs(diff.y) < precision && abs(diff.z) < precision)
        return true;

    return false;
}

bool compareTriangles(glm::vec3 a[3], glm::vec3 b[3], float precision)
{
    for (int j = 0; j < 3; j++)
    {
        bool res_0 = compareFloat(b[0], a[(j + 0) % 3], precision);
        bool res_1 = compareFloat(b[1], a[(j + 1) % 3], precision);
        bool res_2 = compareFloat(b[2], a[(j + 2) % 3], precision);

        if (res_0 && res_1 && res_2)
        {
            return true;
        }
    }
    return false;

}

glm::vec3  getVertex(std::vector<float>* vertices, int vertexIndex)
{
    assert(vertices->size() != 0);

    glm::vec3 v;
    if (vertexIndex * 6 + 0 < 0)return {1,1,1};

    v.x = (*vertices)[vertexIndex * 6 + 0];
    v.y = (*vertices)[vertexIndex * 6 + 1];
    v.z = (*vertices)[vertexIndex * 6 + 2];
    return v;
}


bool compareTriangles(std::vector<float>* vertices, std::vector<unsigned int>* indices, int a, int b)
{
    glm::vec3 v0[3];//wrong
    v0[0] = getVertex(vertices, (*indices)[a * 3 + 0]);//
    v0[1] = getVertex(vertices, (*indices)[a * 3 + 1]);//
    v0[2] = getVertex(vertices, (*indices)[a * 3 + 2]);//

    glm::vec3 v1[3];//wrong
    v1[0] = getVertex(vertices, (*indices)[b * 3 + 0]);//
    v1[1] = getVertex(vertices, (*indices)[b * 3 + 1]);//
    v1[2] = getVertex(vertices, (*indices)[b * 3 + 2]);//

    return compareTriangles(v0, v0, 0.0001f);
}

// recursionDepth should be around 4
int checkDuplicateNeighbor(std::vector<float>* vertices, std::vector<unsigned int>* indices, int recursionDepth, int triangleID, glm::vec3 newTriangleVertices[3], int previousTriqangle)
{
    if (recursionDepth <= 0)return -1;

    for (int i = 0; i < 3; i++)
    {
        int neighbor = neighborList[triangleID].neighbor_ID[i];
        if (neighbor < 0 || neighbor == triangleID || previousTriqangle == neighbor)continue;

        glm::vec3 neighborVertices[3];//
        neighborVertices[0] = getVertex(vertices, (*indices)[neighbor * 3 + 0]);//
        neighborVertices[1] = getVertex(vertices, (*indices)[neighbor * 3 + 1]);//
        neighborVertices[2] = getVertex(vertices, (*indices)[neighbor * 3 + 2]);//

        bool compResult = compareTriangles(newTriangleVertices, neighborVertices, 0.0000000000000000001f);
        if (compResult == true)
        {
            return neighbor;
        }
        else
        {
            int result = checkDuplicateNeighbor(vertices, indices, recursionDepth - 1, neighbor, newTriangleVertices, triangleID);
            if (result != -1)
                return result;
        }
    }

    return -1;
}



//first elements of v should be the common verticse of the old and new triangles, order does not matter
glm::vec3 estimateNeighborTrianglePoint(glm::vec3 v_[3], int uncommonVertexIndex)
{
    glm::vec3 v[3];
    v[0] = v_[(uncommonVertexIndex+2)%3];
    v[1] = v_[(uncommonVertexIndex+1)%3];
    v[2] = v_[uncommonVertexIndex];
    
    //calculate the middle vertex of the current (not the neighbor) triangle
    glm::vec3 v_middle = (v[0] + v[1]) / 2.0f;
    glm::vec3 v_marker[2];
    v_marker[0] = v_middle + (v_middle - v[2]) / 5.0f;
    v_marker[1] = v_middle - (v_middle - v[2]) / 5.0f;

    //find a vertex that is inside the non_existing triangle
    glm::vec3 result;
    float dist_a = distance(v_marker[0], v[2]);
    float dist_b = distance(v_marker[1], v[2]);
    result = dist_a > dist_b ? v_marker[0] : v_marker[1];
    return result;
}


void renderObject(OBJECT* objects, int colorLoc, int modelLoc)
{
    if (!objects->isEnabled)
        return;

    //updates object uniforms
    glUniform3fv(colorLoc, 1, glm::value_ptr(objects->color));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(objects->modelMatrix));

    //render
    glPolygonMode(GL_FRONT_AND_BACK, objects->rasterization);
    glBindVertexArray(objects->VAO);

    int offset = 0;
    glDrawElements(GL_TRIANGLES, objects->indexBuffer.size(), GL_UNSIGNED_INT, (void*)offset);
}

void renderIsolated()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(170.0f), 2 * 800.0f / 600.0f, 0.05f, 1000.0f);
    glm::vec3 objectColor = glm::vec3(1, 0, 0);

    int colorLoc = glGetUniformLocation(ourShader->ID, "objectColor");
    int modelLoc = glGetUniformLocation(ourShader->ID, "model");
    int viewLoc = glGetUniformLocation(ourShader->ID, "view");
    int projectionLoc = glGetUniformLocation(ourShader->ID, "projection");

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

    glfwSwapBuffers(window);
}




void generateTriangle(OBJECT* obj, glm::vec3* vertices)
{
    obj->color = glm::vec3(1, 0, 0);
    obj->isEnabled = true;
    obj->indexBuffer.resize(3);
    obj->indexBuffer[0] = 0;
    obj->indexBuffer[1] = 1;
    obj->indexBuffer[2] = 2;

    obj->vertexBuffer.resize(6 * 3);
    obj->vertexBuffer[0] = vertices[0].x;
    obj->vertexBuffer[1] = vertices[0].y;
    obj->vertexBuffer[2] = vertices[0].z;
    obj->vertexBuffer[0 + 6] = vertices[1].x;
    obj->vertexBuffer[1 + 6] = vertices[1].y;
    obj->vertexBuffer[2 + 6] = vertices[1].z;
    obj->vertexBuffer[0 + 6 * 2] = vertices[2].x;
    obj->vertexBuffer[1 + 6 * 2] = vertices[2].y;
    obj->vertexBuffer[2 + 6 * 2] = vertices[2].z;

    bindObjectBuffers(obj);
}

void insertVertex(std::vector<float>* vertexList, glm::vec3 vertex)
{
    insertVertex(vertexList, vertex.x, vertex.y, vertex.z);
}

glm::vec3 normalize(glm::vec3 v)
{
    return v / glm::distance(v, glm::vec3(0, 0, 0));
}

glm::vec3 randomVector(glm::vec3 v)
{
    int randomNumber = (h((int)(10000.0f * (v.x + v.y + v.z))));
    float noiseFactor = 0.1f;
    float nnn = 1.0f;

    float k = (float)((randomNumber % 10000)) / 10000 * noiseFactor + nnn;

    //v =  normalize(v);

    return v;
}


/*

std::vector<int> outlineTriangleList[2];
int outlineTriangleList_id = 0;
void generateCircularPatch(glm::vec3 point, float boundaryRadius, float radius, int samples, OBJECT* obj)
{
    outlineTriangleList_id = (outlineTriangleList_id + 1) % 2;

    std::vector<float>* vertices = &(obj->vertexBuffer);
    std::vector<unsigned int>* indices = &(obj->indexBuffer);
    float y = glm::distance(point, { 0,0,0 });
    if (indices->size() == 0 and boundaryRadius + radius > y)
    {
        //generate new triangle
        glm::vec3 tri[3];
        calculateTriangleVertices(tri, point, radius, samples);

        //new triangle
        insertTriangle(vertices, indices, tri);

        //update hash list
        Triangularchunk newChunk;
        newChunk.points[0] = tri[0];
        newChunk.points[1] = tri[1];
        newChunk.points[2] = tri[2];

        int key = keyFromVertex(tri[0], tri[1], tri[2]);
        chunks.ht.put(key,newChunk);

        //update outline triangle list
        outlineTriangleList[outlineTriangleList_id].clear();
        outlineTriangleList[outlineTriangleList_id].push_back(key);
    }

    //iterate through all of the outer triangles
    for (int i = 0; outlineTriangleList[outlineTriangleList_id].size() != 0; i++)
    {
        int currentKey = outlineTriangleList[outlineTriangleList_id][i];
        Triangularchunk currentChunk = chunks.ht.get(currentKey);

        //for each neighbor check it
        for (int i2 = 0; i2 < 3; i2++)
        {
            //calculate the vertices of a neighbor triangle
            glm::vec3 v_result = estimateNeighborTrianglePoint(currentChunk.points, i);
            glm::vec3 newNeighborTriangle[3];
            calculateTriangleVertices(newNeighborTriangle, v_result, radius, samples);

            //check if this neighbor exist in the hash table
            int neighborKey = keyFromVertex(newNeighborTriangle[0], newNeighborTriangle[1], newNeighborTriangle[2]);
            bool contains = chunks.ht.contains(neighborKey);

            if (contains)
            {
                continue;
            }
            else if (not contains)
            {

            }
        }
    }
}*/

#include <algorithm>

glm::vec3 middlePoint(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    return (a + b + c) / 3.0f;
}

typedef std::list<OBJECT>::iterator nodeReference;
bool search_(std::vector<nodeReference>* t, nodeReference item)
{
    for (nodeReference i : *t)
    {
        if (i == item)return true;
    }
    return false;
}

void setColor(std::vector<OBJECT>* objects)
{
    for (int i = 0; i < objects->size(); i++)
    {
        objects->at(i).vertexBuffer[i * 6 + 3] = 1;
        objects->at(i).vertexBuffer[i * 6 + 4] = 1;
        objects->at(i).vertexBuffer[i * 6 + 5] = 1;
    }

}

bool timer(int period)
{
    static unsigned int counter = 0;
    if (counter > period)
    {
        counter = 0;
        return true;
    }

    counter++;
    return false;
}

void generateSubdividedTriangularMesh(OBJECT* meshObject, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int subdivision);
void generateTriangle(glm::vec3 *tri, glm::vec3 hashKey,std::list<OBJECT>* objects, hashtable<nodeReference>* hashTable, std::vector<nodeReference>* outerTriangleIndices)
{
    OBJECT newObj;
    newObj.isEnabled = true;
    newObj.modelMatrix = glm::mat4(0.10f);
    newObj.color = glm::vec3(1, 1, 1);
    
    
    objects->push_back(newObj);

    std::list<OBJECT>::iterator lastElement = objects->end();
    lastElement--;
    
    hashTable->put(hashKey, lastElement);
    
    outerTriangleIndices->push_back(lastElement);
  
    (*lastElement).rasterization = GL_FILL;


    //insertTriangle(&(*lastElement).vertexBuffer, &(*lastElement).indexBuffer, tri);
    //assert(newObj.vertexBuffer.size() != 0);

    
    generateSubdividedTriangularMesh(&(*lastElement), tri[0], tri[1], tri[2], 7);
    (*lastElement).cornerVertices[0] = tri[0];
    (*lastElement).cornerVertices[1] = tri[1];
    (*lastElement).cornerVertices[2] = tri[2];

    if (true or lastElement->vertexBuffer.size() == 0)
    {
      //  insertTriangle(&(*lastElement).vertexBuffer, &(*lastElement).indexBuffer, tri);

    }
    
    assert(lastElement->indexBuffer.size() != 0);
    assert(lastElement->vertexBuffer.size() != 0);

    spherify(&(*lastElement).vertexBuffer,7);

    randomize(&(*lastElement).vertexBuffer);

    bindObjectBuffers(&(*lastElement));
    assert(lastElement->VAO < 100000);
    
}

void deleteTriangle(int outerTriangleIndices_index,glm::vec3 hashKey ,std::list<OBJECT>* objects, hashtable<nodeReference>* hashTable, std::vector<nodeReference>* outerTriangleIndices)
{
    //remove the current triangle from hash table list
    hashTable->erase(hashKey);

    //remove the current triangle from objects list
    nodeReference theTriangle = outerTriangleIndices->at(outerTriangleIndices_index);
    OBJECT o = *theTriangle;

    int size = o.vertexBuffer.size();
    assert(size != 0);

    //glBindVertexArray(0);

    glDeleteVertexArrays(1, &((*theTriangle).VAO));
    glDeleteBuffers(1, &((*theTriangle).VBO));

    //remove the current triangle from outerTriangleIndices list
    outerTriangleIndices->at(outerTriangleIndices_index) = outerTriangleIndices->at(outerTriangleIndices->size() - 1);
    outerTriangleIndices->pop_back();

    objects->erase(theTriangle);
}

void deleteAllTriangle(std::list<OBJECT>* objects, hashtable<nodeReference>* hashTable, std::vector<nodeReference>* outerTriangleIndices)
{
    //delete everything
    if (objects->size() != 0)
        objects->clear();

    for (auto& iter : *objects)
    {
        //glBindVertexArray(0);
        glDeleteVertexArrays(1, &(iter.VAO));
        glDeleteBuffers(1, &(iter.VBO));
    }

    if (outerTriangleIndices->size() != 0)
        outerTriangleIndices->clear();

    if (hashTable->size() != 0)
        hashTable->clear();
}

/*struct OBJECT
{
    unsigned int VAO;

    unsigned int VBO;
    std::vector<float> vertexBuffer;
    std::vector<unsigned int> indexBuffer;
    glm::mat4 modelMatrix;
    glm::vec3 color;
    bool isEnabled = true;
    GLenum rasterization = GL_FILL;
};*/

bool pointIsInsViewRange(glm::vec3 cameraPos, float planetRadius ,float viewRadius)
{
    float cameraDistance = glm::distance(cameraPos, glm::vec3(0));
    return cameraDistance < (planetRadius + viewRadius);
}

int sphericalPatch(int subdivision ,std::list<OBJECT>* objects, hashtable<nodeReference>* hashTable, std::vector<nodeReference>* outerTriangleIndices, float planetRadius, float viewRadius, glm::vec3 cameraPos)
{
    int nTrianglesAdded = 0;
    
    if (not pointIsInsViewRange(cameraPos, planetRadius ,viewRadius))
    {
        deleteAllTriangle(objects, hashTable, outerTriangleIndices);
        return nTrianglesAdded;
    }

    if (objects->size() == 0)
    {
        //finding the triangle
        glm::vec3 pointOnSphere = planetRadius * cameraPos / glm::distance(cameraPos, glm::vec3(0));
        glm::vec3 tri[3];         calculateTriangleVertices(tri, pointOnSphere, planetRadius, subdivision);
        glm::vec3 midPoint = middlePoint(tri[0], tri[1], tri[2]);
        glm::vec3 hashKey = midPoint;

        //insert a single triangle
        generateTriangle(tri,hashKey ,objects, hashTable,outerTriangleIndices);
        nTrianglesAdded++;
    }
        
    //check all outer triangles
    for (int i1 = 0; i1 < outerTriangleIndices->size(); i1++)
    {

        nodeReference curTriangle = outerTriangleIndices->at(i1);
        assert(curTriangle->vertexBuffer.size()!=0);

        //check each negihbor
        int nNeighbors = 0;
        Key neighborHashKeys[3];
        bool neighborHashKey_exists[3] = {false,false,false};
        for (int i2 = 0; i2 < 3; i2++)
        {
            static int counter = 0;
            counter++;
            glm::vec3 v[3];
            OBJECT triangle = *&(*curTriangle);
            OBJECT triangle2 = (*curTriangle);

            assert(triangle.indexBuffer.size() != 0);

            int i = (*curTriangle).indexBuffer.at((i2 + 0) % 3);
            //v[0] = getVertex(&(*curTriangle).vertexBuffer, i);
            //v[1] = getVertex(&(*curTriangle).vertexBuffer, (*curTriangle).indexBuffer.at((i2 + 1) % 3));
            //v[2] = getVertex(&(*curTriangle).vertexBuffer, (*curTriangle).indexBuffer.at((i2 + 2) % 3));
            v[0] = curTriangle->cornerVertices[(i2 + 0) % 3];
            v[1] = curTriangle->cornerVertices[(i2 + 1) % 3];
            v[2] = curTriangle->cornerVertices[(i2 + 2) % 3];

            glm::vec3 v_result = estimateNeighborTrianglePoint(v, 0);
            glm::vec3 tri[3];    calculateTriangleVertices(tri, v_result, planetRadius, subdivision);
            glm::vec3 midPoint = middlePoint(tri[0], tri[1], tri[2]);
            glm::vec3 hashKey = midPoint;
            neighborHashKeys[i2] = hashKey;
            bool pointIsInRange = glm::distance(cameraPos, midPoint) < viewRadius;

            bool neighborExist = hashTable->contains(hashKey);
            if (pointIsInRange and not neighborExist)
            {
                //insert a single triangle
                generateTriangle(tri, hashKey, objects, hashTable, outerTriangleIndices);
                nTrianglesAdded++;
            }

            if (pointIsInRange)
            {
                neighborHashKey_exists[i2] = true;
                nNeighbors++;
            }
        }

        //check if the curTriangle is inside the view
        glm::vec3 v[3];
        //v[0] = getVertex(&(curTriangle->vertexBuffer), curTriangle->indexBuffer.at(0));
        //v[1] = getVertex(&(curTriangle->vertexBuffer), curTriangle->indexBuffer.at(1));
        //v[2] = getVertex(&(curTriangle->vertexBuffer), curTriangle->indexBuffer.at(2));

        v[0] = curTriangle->cornerVertices[0];
        v[1] = curTriangle->cornerVertices[1];
        v[2] = curTriangle->cornerVertices[2];

        //check if the current triangle is inside the view range, if its not remove it
        glm::vec3 midPoint = middlePoint(v[0], v[1], v[2]);
        bool pointIsInRange = glm::distance(cameraPos, midPoint) < viewRadius;
        
        if (not pointIsInRange)
        {
            //remove triangle
            deleteTriangle(i1, midPoint, objects, hashTable, outerTriangleIndices);
            
            for (int i3 = 0; i3 < 3; i3++)
            {
                if (neighborHashKey_exists[i3] == true)
                {
                    nodeReference currentTriangle = hashTable->get(neighborHashKeys[i3]);
                    bool found = search_(outerTriangleIndices, currentTriangle);
                    if (not found)
                    {   //triangle is not found
                        outerTriangleIndices->push_back(currentTriangle);
                    }
                }
            }
        }
        else if (nNeighbors == 3)
        {
            //remove this triangle from outerTriangle list
            outerTriangleIndices->at(i1) = outerTriangleIndices->at(outerTriangleIndices->size() - 1);
            outerTriangleIndices->pop_back();
        }
    }
    
    
    for (OBJECT &n: *objects)
    {
        n.color = glm::vec3(1, 1, 1);
    }
     
    for (int i = 0; i < outerTriangleIndices->size(); i++)
    {
        nodeReference n = outerTriangleIndices->at(i);
        n->color = glm::vec3(1, 0, 0);
    }
    
    return nTrianglesAdded;
    //std::cout << outerTriangleIndices->size() << std::endl << objects->size() << std::endl << hashTable->size();
    //std::cout << outerTriangleIndices->capacity() << std::endl << objects->size() << std::endl << hashTable->capacity();
    //std::cout << std::endl << std::endl;
}


std::vector<int> outlineTriangleList[2];
int outlineTriangleList_id = 0;
void generateSphericalPatch(glm::vec3 point, float boundaryRadius, float radius, int samples, OBJECT* obj)
{
    //hashtable<int, std::string> ht;

    outlineTriangleList_id = (outlineTriangleList_id + 1) % 2;

    std::vector<float>* vertices = &(obj->vertexBuffer);
    std::vector<unsigned int>* indices = &(obj->indexBuffer);
    float y = glm::distance(point, { 0,0,0 });
    if (indices->size() == 0 and boundaryRadius + radius > y)
    {
        //generate new triangle
        glm::vec3 tri[3];
        calculateTriangleVertices(tri, point, radius, samples);

        //new triangle
        insertTriangle(vertices, indices, tri);

        //update neihbor list
        triangle newTri;
        newTri.neighbor_ID[0] = -1;
        newTri.neighbor_ID[1] = -1;
        newTri.neighbor_ID[2] = -1;
        neighborList.push_back(newTri);

        //update outline triangle list
        outlineTriangleList[outlineTriangleList_id].clear();
        outlineTriangleList[outlineTriangleList_id].push_back(0);
    }

    //iterate through all of the outer triangles
    int nOutOfRegionTriangles = 0;
    for (int i = 0; outlineTriangleList[outlineTriangleList_id].size() != 0; i++)
    {
        if (i >= outlineTriangleList[outlineTriangleList_id].size())
            i = 0;

        int index = i;
        int currentTriangle_index = outlineTriangleList[outlineTriangleList_id][index];

        bool isFullyConnected = neighborList[currentTriangle_index].neighbor_ID[0] != -1 &&
            neighborList[currentTriangle_index].neighbor_ID[1] != -1 &&
            neighborList[currentTriangle_index].neighbor_ID[2] != -1;

        if (isFullyConnected)
        {
            bool isInOutline = neighborList[currentTriangle_index].neighbor_ID[0] == -2 ||
                neighborList[currentTriangle_index].neighbor_ID[1] == -2 ||
                neighborList[currentTriangle_index].neighbor_ID[2] == -2;

            if (isInOutline)
            {
                for (int k = 0; k < 3; k++)
                    if (neighborList[currentTriangle_index].neighbor_ID[k] == -2) neighborList[currentTriangle_index].neighbor_ID[k] = -1;

                outlineTriangleList[(outlineTriangleList_id + 1) % 2].push_back(outlineTriangleList[outlineTriangleList_id][index]);
            }

            outlineTriangleList[outlineTriangleList_id].erase(outlineTriangleList[outlineTriangleList_id].begin() + index);
        }
        else if (not isFullyConnected)
        {
            //iterate through all neighbors of the current triangle
            for (int i2 = 0; i2 < 3; i2++)
            {
                int neighborTriangle_index = neighborList[currentTriangle_index].neighbor_ID[i2];
                if (neighborTriangle_index == -1)
                {
                    //find the 2 common vertices
                    //v[0] and v[1] are the common verticse, v[2] is the other vertex
                    glm::vec3 v[3];
                    v[0] = getVertex(vertices, (*indices)[3 * currentTriangle_index + (i2 + 0) % 3]);
                    v[1] = getVertex(vertices, (*indices)[3 * currentTriangle_index + (i2 + 1) % 3]);
                    v[2] = getVertex(vertices, (*indices)[3 * currentTriangle_index + (i2 + 2) % 3]);

                    glm::vec3 v_result = estimateNeighborTrianglePoint(v, 0);
                    glm::vec3 newTriangleVertices[3];
                    calculateTriangleVertices(newTriangleVertices, v_result, radius, samples);

                    float distance_0 = glm::distance(newTriangleVertices[0], point);
                    float distance_1 = glm::distance(newTriangleVertices[1], point);
                    float distance_2 = glm::distance(newTriangleVertices[2], point);

                    bool triangleIsInside = (distance_0 < boundaryRadius || distance_1 < boundaryRadius || distance_2 < boundaryRadius);
                    if (triangleIsInside == true)
                    {
                        //add the new triangle (newTriangleVertices)
                        int duplicateNeighborIndex = -1;
                        duplicateNeighborIndex = checkDuplicateNeighbor(vertices, indices, 6, currentTriangle_index, newTriangleVertices, currentTriangle_index);

                        if (duplicateNeighborIndex == -1)
                        {
                            //insert triangle
                            int recycledTriangleIndex = -1;
                            insertTriangle(vertices, indices, newTriangleVertices);

                            int newNeighborIndex = vertices->size() / (3 * 6) - 1;
                            outlineTriangleList[outlineTriangleList_id].push_back(newNeighborIndex);

                            neighborList[currentTriangle_index].neighbor_ID[i2] = newNeighborIndex;

                            triangle newNeighbor;
                            int neighborID = findNeigborId(v[0], v[1], newTriangleVertices[0], newTriangleVertices[1], newTriangleVertices[2]);
                            if (neighborID != -1)
                            {
                                newNeighbor.neighbor_ID[(neighborID + 0) % 3] = currentTriangle_index;
                                newNeighbor.neighbor_ID[(neighborID + 1) % 3] = -1;
                                newNeighbor.neighbor_ID[(neighborID + 2) % 3] = -1;
                                neighborList.push_back(newNeighbor);
                            }
                        }
                        else
                        {
                            neighborList[currentTriangle_index].neighbor_ID[i2] = duplicateNeighborIndex;
                            glm::vec3 vert[3];
                            vert[0] = getVertex(vertices, (*indices)[3 * duplicateNeighborIndex + 0]);
                            vert[1] = getVertex(vertices, (*indices)[3 * duplicateNeighborIndex + 1]);
                            vert[2] = getVertex(vertices, (*indices)[3 * duplicateNeighborIndex + 2]);

                            int id = findNeigborId(v[0], v[1], vert[0], vert[1], vert[2]);
                            neighborList[duplicateNeighborIndex].neighbor_ID[id] = currentTriangle_index;
                        }
                    }
                    else if (triangleIsInside == false)
                    {
                        neighborList[currentTriangle_index].neighbor_ID[i2] = -2;
                    }
                }
                else
                {   //check if this existing triangle is really inside the boundary
                    //if it is fully outside, remove this triangle
                }
            }
        }
    }

    //reset the color of all
    for (int i = 0; i < (*vertices).size() / 6; i++)
    {
        int index = i;
        (*vertices)[index * 6 + 3] = 0;
        (*vertices)[index * 6 + 4] = 1;
        (*vertices)[index * 6 + 5] = 0;
    }
}

void generateSubdividedTriangularMesh(OBJECT* meshObject, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int subdivision)
{
    //edge length of a unit triangle
    float nTrianglesPerMajorEdge = powf(2.0f, (float)subdivision);
    float stepUnitLength = 1.0f / ((float)nTrianglesPerMajorEdge);

    //contruction unit vectors: refer to the image:"constructing subdivided triangle"
    glm::vec3 downLeft_UnitVector = (v2 - v0) / nTrianglesPerMajorEdge;
    glm::vec3 downRight_UnitVector = (v1 - v0) / nTrianglesPerMajorEdge;
    glm::vec3 right_UnitVector = (v1 - v2) / nTrianglesPerMajorEdge;

    //inserting vertices
    insertVertex(&(meshObject->vertexBuffer), v0);
    for (float iy = 0; iy < nTrianglesPerMajorEdge; iy++)
    {
        int nBottomVertices = iy + 2;

        glm::vec3 initialVertex_bottom = v0 + downLeft_UnitVector * (iy + 1);
        for (float ix = 0; ix < nBottomVertices; ix++)
        {
            glm::vec3 currentVertex = initialVertex_bottom + right_UnitVector * ix;
            insertVertex(&(meshObject->vertexBuffer), currentVertex);
        }
    }

    //inserting A triangles
    for (int iy = 0; iy < nTrianglesPerMajorEdge; iy++)
    {
        int nTriangles = iy + 1;
        for (int ix = 0; ix < nTriangles; ix++)
        {
            int topVertexIndex = (iy + 0) * ((iy + 0) + 1) / 2 + (ix + 0);
            int bottomVertexIndex_A = (iy + 1) * ((iy + 1) + 1) / 2 + (ix + 0);
            int bottomVertexIndex_B = (iy + 1) * ((iy + 1) + 1) / 2 + (ix + 1);

            addTriangle(&(meshObject->indexBuffer), topVertexIndex, bottomVertexIndex_A, bottomVertexIndex_B);
        }
    }

    //inserting B triangles
    for (int iy = 1.0f; iy < nTrianglesPerMajorEdge; iy++)
    {
        int nTriangles = iy;
        for (int ix = 0; ix < nTriangles; ix++)
        {
            int topVertexIndex_A = (iy + 0) * ((iy + 0) + 1) / 2 + (ix + 0);
            int topVertexIndex_B = (iy + 0) * ((iy + 0) + 1) / 2 + (ix + 1);
            int bottomVertexIndex = (iy + 1) * ((iy + 1) + 1) / 2 + (ix + 1);

            addTriangle(&(meshObject->indexBuffer), topVertexIndex_A, topVertexIndex_B, bottomVertexIndex);
        }
    }
}