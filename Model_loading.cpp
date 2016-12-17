#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Physics.h"
#include "third_person_camera.h"

using namespace glm;

// Properties
GLuint screenWidth = 1280, screenHeight = 900;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
ThirdPersonCamera camera;
const GLfloat CAMERA_FOVX = 80.0f;
const GLfloat CAMERA_ZFAR = 1000.0f;
const GLfloat CAMERA_ZNEAR = 0.1f;
const GLfloat CAMERA_SPEED = 20.0f;
//Player
btRigidBody *Player = nullptr;
const GLfloat PLAYER_SPEED = 50.0f;
const GLfloat MOUSE_SENSTIVITY = 20.0f;
//Controls
bool keys[1024];
GLfloat lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main(int argc, char** argv)
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Sameh", nullptr, nullptr); // Windowed
	if (window == nullptr)
		throw new exception();
	glfwMakeContextCurrent(window);

	
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader shader("model_loading.vs", "model_loading.frag");

    Physics* physics = new Physics();

    GraphicsObject* Nanosuit = new GraphicsObject((GLchar*)"NanoSuit/nanosuit.obj", true);
    physics->add(Nanosuit, 10, glm::vec3(0,5,0));
    Player = physics->physicsObjects[0].object;

    GraphicsObject* Platform = new GraphicsObject((GLchar*)"Platform/new.obj", true);
    physics->add(Platform, 0, glm::vec3(0,0,0));


    camera.perspective(CAMERA_FOVX, (float)screenWidth / (float)screenHeight, CAMERA_ZNEAR, CAMERA_ZFAR);
    camera.lookAt(Vector3(20,20,20), Vector3(0,10,0), Vector3(0,1,0));

    // Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Set frame time
		GLfloat currentFrame = (GLfloat) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        Player->activate(true);
		physics->update(deltaTime);

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();   // <-- Don't forget this one!
						// Transformation matrices
//		glm::mat4 projection = glm::perspective(CAMERA_FOVX, (float)screenWidth / (float)screenHeight, CAMERA_ZNEAR, CAMERA_ZFAR);

        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        for (int i = 0; i < physics->physicsObjects.size(); ++i)
        {
            physics->physicsObjects[i].model->graph(shader, projection, view);
        }

		//camera.lookAt(Vector3());
        camera.update(deltaTime);

        // Swap the buffers
		glfwSwapBuffers(window);
    }
	glfwTerminate();
	return 0;
}
// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
    {
        Vector3 x;
        x = Vector3(camera.getZAxis().x * deltaTime * CAMERA_SPEED, 0, camera.getZAxis().z * deltaTime * CAMERA_SPEED);
        camera.lookAt(camera.m_target - x);
        Player->setLinearVelocity(-btVector3(x.x,0,x.z)*PLAYER_SPEED);
    }
    if (keys[GLFW_KEY_S])
    {
        Vector3 x;
        x = Vector3(camera.getZAxis().x * deltaTime * CAMERA_SPEED, 0, camera.getZAxis().z * deltaTime * CAMERA_SPEED);
        camera.lookAt(camera.m_target + x);
        Player->setLinearVelocity(btVector3(x.x,0,x.z)*PLAYER_SPEED);
    }
    if (keys[GLFW_KEY_A])
    {
        Vector3 x;
        x = Vector3(camera.getXAxis().x * deltaTime * CAMERA_SPEED, 0,camera.getXAxis().z * deltaTime * CAMERA_SPEED);
        camera.lookAt(camera.m_target - x);
        Player->setLinearVelocity(-btVector3(x.x,0,x.z)*PLAYER_SPEED);
    }
    if (keys[GLFW_KEY_D])
    {
        Vector3 x;
        x = Vector3(camera.getXAxis().x * deltaTime * CAMERA_SPEED, 0, camera.getXAxis().z * deltaTime * CAMERA_SPEED);
        camera.lookAt(camera.m_target + x);
        Player->setLinearVelocity(btVector3(x.x,0,x.z)*PLAYER_SPEED);
    }
    if (keys[GLFW_KEY_F])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (keys[GLFW_KEY_T])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
    {
        keys[key] = false;
        if(key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_D || key == GLFW_KEY_S)
           Player->setLinearVelocity(btVector3(0,0,0));
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (xpos - lastX);
	GLfloat yoffset = (lastY - ypos);

	lastX = xpos;
	lastY = ypos;

    if(abs(xoffset) < 5)
        xoffset = 0;
    if(abs(yoffset) < 5)
        yoffset = 0;

        camera.rotate(xoffset * MOUSE_SENSTIVITY, yoffset * MOUSE_SENSTIVITY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}
