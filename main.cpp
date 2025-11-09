// Juliana Dietrich HW2

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "vec3.h"
using namespace std;

// global vars
float rotationX = 0.0f, rotationY = 0.0f, cameraDist = 5.0f;
float zoom = 5.0f;
float fov = 1.0f;
bool showWireframe = true;
const float rotationSpeed = 0.02f;
const float zoomSpeed = 0.1f;

int windowWidth = 800, windowHeight = 600;

// input processing
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Escape key to close
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Rotate up
        rotationX -= rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Rotate down
        rotationX += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Rotate left
        rotationY -= rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Rotate right
        rotationY += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Zoom in
        cameraDist -= zoomSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Zoom out
        cameraDist += zoomSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // Toggle wireframe/points
        showWireframe = !showWireframe;
}

// Perspective Projection View calculation
void perspectiveProj(const Vec3 &v, float &x2d, float &y2d, float fov, float camDist)
{
    float f = 1.0f / tan(fov / 2.0f);                                        // focal length
    x2d = (v.x * f / (v.z + camDist)) * windowWidth / 2 + windowWidth / 2;   // center in window
    y2d = windowHeight / 2 - (v.y * f / (v.z + camDist)) * windowHeight / 2; // invert y for screen coords
}

// file loader for vertices(.data)
vector<Vec3> loadVertices(const string &filename)
{
    vector<Vec3> vertices;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Could not open vertex file: " << filename << "\n";
        return vertices;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        // Replace commas with spaces so we can use >>
        replace(line.begin(), line.end(), ',', ' ');
        stringstream ss(line);
        float x, y, z;
        if (ss >> x >> y >> z)
            vertices.push_back({x, y, z});
    }

    cout << " Loaded " << vertices.size() << " vertices\n";
    return vertices;
}

// file loader for  indices(.txt)
vector<unsigned int> loadIndices(const string &filename)
{
    vector<unsigned int> indices;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Could not open index file: " << filename << "\n";
        return indices;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        replace(line.begin(), line.end(), ',', ' ');
        stringstream ss(line);
        unsigned int a, b, c;
        if (ss >> a >> b >> c)
            indices.insert(indices.end(), {a, b, c});
    }

    cout << " Loaded " << indices.size() / 3 << " triangles\n";
    return indices;
}

// main function
int main()
{
    if (!glfwInit())
        return -1;
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Homework 2: Question 3", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    vector<Vec3> vertices = loadVertices("data/face-vertices.data");
    vector<unsigned int> indices = loadIndices("data/face-index.txt");

    if (vertices.empty() || indices.empty())
    {
        cerr << "Error: Could not load vertex/index data!" << endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0f, 1.0f, 0.7f);

        // Draw the model
        if (showWireframe)
            glBegin(GL_LINES);
        else
            glBegin(GL_POINTS);

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            Vec3 v0 = vertices[indices[i + 2] % vertices.size()];
            Vec3 v1 = vertices[indices[i + 1] % vertices.size()];
            Vec3 v2 = vertices[indices[i] % vertices.size()];

            // Apply rotations to let the model spin when user presses keys
            v0 = rotateY(rotateX(v0, rotationX), rotationY);
            v1 = rotateY(rotateX(v1, rotationX), rotationY);
            v2 = rotateY(rotateX(v2, rotationX), rotationY);

            float x0, y0, x1, y1, x2, y2;
            // Project the 3D vertices to 2D screen coordinates
            perspectiveProj(v0, x0, y0, 3.14159f / 4.0f, cameraDist);
            perspectiveProj(v1, x1, y1, 3.14159f / 4.0f, cameraDist);
            perspectiveProj(v2, x2, y2, 3.14159f / 4.0f, cameraDist);

            if (showWireframe)
            {
                // Draw edges
                glVertex2f(x0 / (windowWidth / 2) - 1, 1 - y0 / (windowHeight / 2));
                glVertex2f(x1 / (windowWidth / 2) - 1, 1 - y1 / (windowHeight / 2));
                glVertex2f(x1 / (windowWidth / 2) - 1, 1 - y1 / (windowHeight / 2));
                glVertex2f(x2 / (windowWidth / 2) - 1, 1 - y2 / (windowHeight / 2));
                glVertex2f(x2 / (windowWidth / 2) - 1, 1 - y2 / (windowHeight / 2));
                glVertex2f(x0 / (windowWidth / 2) - 1, 1 - y0 / (windowHeight / 2));
            }
            else
            {
                // Draw points
                glVertex2f(x0 / (windowWidth / 2) - 1, 1 - y0 / (windowHeight / 2));
                glVertex2f(x1 / (windowWidth / 2) - 1, 1 - y1 / (windowHeight / 2));
                glVertex2f(x2 / (windowWidth / 2) - 1, 1 - y2 / (windowHeight / 2));
            }
        }

        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
