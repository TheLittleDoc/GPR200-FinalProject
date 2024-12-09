/******************************************************************************
* File name:                File author(s): Evan Noel, Elysium Hosack,        *
*                                           based on template from Eric       *
* File description:                         Winebrenner                       *
*                                                                             *
*                                                                             *
*                                                                             *
* Cloned:                   Edited last:                                      *
* Key Methods:                                                                *
*                                                                             *
******************************************************************************/

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <idk/shader.h>
#include <idk/cubemap.h>
#include <idk/camera.h>

#include <idk/texture2d.h>
#include <ew/external/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 1080;
const unsigned int NUM_PATCH_PTS = 4;

// camera
idk::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const float TERR_WIDTH = 100.0f;
const float TERR_LENGTH = 100.0f;

//imgui variables
glm::vec3 testEditer = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 editModel = glm::mat4(1.0f);
glm::vec3 lightPosEditable = glm::vec3(1.2f, 1.0f, 2.0f);
glm::vec3 lightColorEditable = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 objectColorEditable = glm::vec3(1.0f, 0.5f, 0.31f);
float ambientStrengthEditable = 0.1f;
float specularStrengthEditable = 0.5f;
float shininessEditable = 32.0f;
float diffuseStrengthEditable = 0.5f;


int main()
{
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final", nullptr, nullptr);
	if (window == nullptr) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	stbi_set_flip_vertically_on_load(true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glPatchParameteri(GL_PATCH_vertices, 4);

	idk::Shader shader("assets/cube.vert", "assets/cube.frag");
	idk::Shader skyboxShader("assets/cubemap.vert", "assets/cubemap.frag");
	idk::Shader heightMapShader("assets/terrain/terrain.vert", "assets/terrain/terrain.frag", "assets/terrain/terrain.tesc", "assets/terrain/terrain.tese");
	idk::Texture2D texture("assets/AverageNebraskaResident.png", GL_NEAREST, GL_CLAMP_TO_EDGE, true);
	texture.Bind(1);
	//idk::Texture2D heightmapTexture("assets/terrain/heightmap_terrain.png", GL_NEAREST, GL_CLAMP_TO_EDGE, true);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f ,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// Terrain Setup
	GLint maxTessLevel;
	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("assets/terrain/heightmap_terrain.png", &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		heightMapShader.setInt("heightMap", 0);
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//get texture handle


	std::vector<float> heightmapVerts;
	
    unsigned rez = 10;
    for(unsigned i = 0; i <= rez-1; i++)
    {
        for(unsigned j = 0; j <= rez-1; j++)
        {
            heightmapVerts.push_back(-TERR_WIDTH/2.0f + TERR_WIDTH*i/(float)rez); // v.x
            heightmapVerts.push_back(0.0f); // v.y
            heightmapVerts.push_back(-TERR_LENGTH/2.0f + TERR_LENGTH*j/(float)rez); // v.z
            heightmapVerts.push_back(i / (float)rez); // u
            heightmapVerts.push_back(j / (float)rez); // v

            heightmapVerts.push_back(-TERR_WIDTH/2.0f + TERR_WIDTH*(i+1)/(float)rez); // v.x
            heightmapVerts.push_back(0.0f); // v.y
            heightmapVerts.push_back(-TERR_LENGTH/2.0f + TERR_LENGTH*j/(float)rez); // v.z
            heightmapVerts.push_back((i+1) / (float)rez); // u
            heightmapVerts.push_back(j / (float)rez); // v

            heightmapVerts.push_back(-TERR_WIDTH/2.0f + TERR_WIDTH*i/(float)rez); // v.x
            heightmapVerts.push_back(0.0f); // v.y
            heightmapVerts.push_back(-TERR_LENGTH/2.0f + TERR_LENGTH*(j+1)/(float)rez); // v.z
            heightmapVerts.push_back(i / (float)rez); // u
            heightmapVerts.push_back((j+1) / (float)rez); // v

            heightmapVerts.push_back(-TERR_WIDTH/2.0f + TERR_WIDTH*(i+1)/(float)rez); // v.x
            heightmapVerts.push_back(0.0f); // v.y
            heightmapVerts.push_back(-TERR_LENGTH/2.0f + TERR_LENGTH*(j+1)/(float)rez); // v.z
            heightmapVerts.push_back((i+1) / (float)rez); // u
            heightmapVerts.push_back((j+1) / (float)rez); // v
        }
    }
    std::cout << "Loaded " << rez*rez << " patches of 4 control points each" << std::endl;
    std::cout << "Processing " << rez*rez*4 << " heightmapVerts in vertex shader" << std::endl;

    // first, configure the cube's VAO (and terrainVBO)
    unsigned int terrainVAO, terrainVBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * heightmapVerts.size(), &heightmapVerts[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glPatchParameteri(GL_PATCH_vertices, NUM_PATCH_PTS);;

	// cube VAO
	unsigned int cubeVAO, VBO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	//Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	//Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// skybox textures
	std::vector<std::string> faces
	{
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
	};

	idk::Cubemap skyboxCubemap = (faces);

	glm::mat4 model;
	// Render loop
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	while (!glfwWindowShouldClose(window)) {

		// update time varaiables
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// Clear framebuffer
		glClearColor(0.07f, 0.03f, 0.05f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update the time variable
		float time = (float)glfwGetTime();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);

		heightMapShader.use();
		//bind texture


		heightMapShader.setInt("heightMap", 0);
		heightMapShader.setMat4("model", model);
		heightMapShader.setMat4("projection", projection);
		heightMapShader.setMat4("view", view);

		// render the terrain
		glBindVertexArray(terrainVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS * rez * rez);

		// draw
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		shader.use();
		glActiveTexture(GL_TEXTURE1);
		texture.Bind(1);
		shader.setInt("uTexture", 1);
		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPos", lightPosEditable);
		shader.setVec3("lightColor", lightColorEditable);
		shader.setVec3("objectColor", objectColorEditable);
		shader.setFloat("ambientStrength", ambientStrengthEditable);
		shader.setFloat("specularStrength", specularStrengthEditable);
		shader.setFloat("shininess", shininessEditable);
		shader.setFloat("diffuseStrength", diffuseStrengthEditable);


		shader.setMat4("model", model);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);


		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw skybox
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubemap.GetID());
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthFunc(GL_LESS); // set depth function back to default

		// Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Create a window called settings
		ImGui::Begin("Settings");
		ImGui::Text("Controls");
		if (ImGui::CollapsingHeader("ShaderStuff"))
		{
			ImGui::DragFloat3("Light Position", &lightPosEditable.x, 0.01f);
			ImGui::DragFloat3("Light Color", &lightColorEditable.x, 0.01f);
			ImGui::DragFloat3("Object Color", &objectColorEditable.x, 0.01f);
			ImGui::DragFloat("Ambient Strength", &ambientStrengthEditable, 0.01f);
			ImGui::DragFloat("Specular Strength", &specularStrengthEditable, 0.01f);
			ImGui::DragFloat("Shininess", &shininessEditable, 0.01f);
			ImGui::DragFloat("Diffuse Strength", &diffuseStrengthEditable, 0.01f);
			if (ImGui::Button("Reset Values"))
			{
				editModel = glm::mat4(1.0f);
				lightPosEditable = glm::vec3(1.2f, 1.0f, 2.0f);
				lightColorEditable = glm::vec3(1.0f, 1.0f, 1.0f);
				objectColorEditable = glm::vec3(1.0f, 0.5f, 0.31f);
				ambientStrengthEditable = 0.1f;
				specularStrengthEditable = 0.5f;
				shininessEditable = 32.0f;
				diffuseStrengthEditable = 0.5f;
			}
		}
		ImGui::End();

		// Render IMGUI elements using OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Drawing happens here
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = camera.BaseMovementSpeed * 2;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.MovementSpeed = camera.BaseMovementSpeed;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Locks
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Unlocks

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}