#include "config.h"

void PollEvents();
void ProcessInput(GLFWwindow* window);
void CursorCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderScene();

/*-------- Camera Settings --------*/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float pitch = 0.0f, yaw = -90.0f;
float fov = 45.0;
bool firstMouse = true;

Camera mainCamera = Camera(cameraPos, fov, pitch, yaw);
/*---------------------------------*/
LightManager lightManager;


std::vector<GLuint> CreateCubeMap()
{
	// Load cubemap
	std::string textureFaces[] = {"../LightMap Engine/textures/skybox/right.jpg",
		"../LightMap Engine/textures/skybox/left.jpg",
		"../LightMap Engine/textures/skybox/top.jpg",
		"../LightMap Engine/textures/skybox/bottom.jpg",
		"../LightMap Engine/textures/skybox/front.jpg",
		"../LightMap Engine/textures/skybox/back.jpg"
	};

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, numChannels;
	unsigned char* data;
	for (int i = 0; i < 6; ++i)
	{
		stbi_set_flip_vertically_on_load(false);
		data = stbi_load(textureFaces[i].c_str(), &width, &height, &numChannels, 0);
		
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << textureFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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
	
	// Generate buffers and arrays
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO); 
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// Upload vertex data to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	// Set vertex attributes
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Bind to first attribute location

	glBindVertexArray(0); // Unbind VAO

	return { textureID, VBO, VAO };
}

void DrawSkyBox(GLuint textureID, GLuint VBO, GLuint VAO, Shader skyboxShader)
{
	glDepthFunc(GL_LEQUAL); 
	skyboxShader.Activate();
	skyboxShader.SetInt("skybox", 0);

	glm::mat4 view = glm::mat4(glm::mat3(mainCamera.ViewMatrix()));
	skyboxShader.SetMat4("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(mainCamera.FOV()), (float)mainCamera.screenWidth / (float)mainCamera.screenHeight, 0.1f, 100.0f);
	skyboxShader.SetMat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

int main()
{
	GLFWwindow* window = UI::CreateWindow(1280, 720, "LightMap Engine");
	UI::InitImGui(window);

	#pragma region INITIALIZE SCENE
	Shader shader("./shaders/vertex.vert", "./shaders/fragment.frag");
	Shader reflectiveShader("./shaders/reflective_vertex.vert", "./shaders/reflective_fragment.frag");
	Shader ds("./shaders/default_vertex.vert", "./shaders/default_fragment.frag");
	Shader lightShader("./shaders/light_vertex.vert", "./shaders/light_fragment.frag"); 
	Texture diffuse("./textures/container2.png", true, true);
	Texture specular("../OpenGL/textures/container2_specular.png", true, true);

	shader.AddTexture(diffuse);
	shader.SetInt("material.diffuse", 0);
	shader.AddTexture(specular);
	shader.SetInt("material.specular", 1);
	shader.SetFloat("material.shininess", 32.0f);

	DirectionalLight dirLight = DirectionalLight(glm::vec3(0.0f, 0.0f, 0.0f), Color::White(), 10.0f);
	//SpotLight spotLight = SpotLight(glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 0.0)-glm::vec3(-2.0f, 0.0f, 0.0f) , Color::White(), 1.0f, 12.5f, 17.5f);

	lightManager.directionalLights.push_back(dirLight);
	//lightManager.spotLights.push_back(spotLight);

	std::vector<GLuint> cubeMapReqs = CreateCubeMap();
	Shader skyboxShader("./shaders/skybox.vert", "./shaders/skybox.frag");

	#pragma endregion  

	std::string outlineVert = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
	)";

	std::string outlineFrag = R"(
		#version 330 core

		out vec4 FragColor;
		
		void main()
		{
			FragColor = vec4(1.0f, 0.784f, 0.0f, 1.0f);
		}
	)";
	std::vector<const char*> shader_plaintext = { outlineVert.c_str(), outlineFrag.c_str() };
	Shader outlineShader(shader_plaintext);
	Mesh cube = Mesh("CUBE", shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	while (!glfwWindowShouldClose(window))
	{
		PollEvents();

		UI::BeginFrame();

		ProcessInput(window);

		//UI::RenderUI();	// Render ImGui windows

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		/*-------- Render Scene --------*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00); // Disable writing to the stencil buffer

		lightManager.ApplyLightsToShader(shader);
		DrawSkyBox(cubeMapReqs[0], cubeMapReqs[1], cubeMapReqs[2], skyboxShader);

		for (int i = 0; i < UI::sceneObjects.size(); ++i)
		{
			UI::sceneObjects[i]->Render(mainCamera);
		}

		
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // All fragments should update the stencil buffer
		glStencilMask(0xFF); // Enable writing to the stencil buffer
		cube.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		Shader currentShader = cube.GetShader();
		cube.Draw(mainCamera);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is not 1
		glStencilMask(0x00); // Disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		outlineShader.Activate();
		cube.SetShader(outlineShader);
		cube.SetScale(glm::vec3(1.01f, 1.01f, 1.01f));
		cube.Draw(mainCamera);
		cube.SetShader(currentShader);
		glStencilMask(0xFF); // Enable writing to the stencil buffer
		glStencilFunc(GL_ALWAYS, 0, 0xFF); // All fragments should update the stencil buffer
		glEnable(GL_DEPTH_TEST);

		/*-------- Render Scene --------*/

		UI::EndFrame();


		glfwSwapBuffers(window);
	}

	UI::Cleanup(window);
	return 0;
}

void PollEvents()
{
	glfwPollEvents();

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() + (cameraSpeed * mainCamera.Front()));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() - (cameraSpeed * mainCamera.Front()));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() - (glm::normalize(glm::cross(mainCamera.Front(), mainCamera.Up())) * cameraSpeed));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera.SetPosition(mainCamera.Position() + (glm::normalize(glm::cross(mainCamera.Front(), mainCamera.Up())) * cameraSpeed));
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetFramebufferSize(window, &mainCamera.screenWidth, &mainCamera.screenHeight);
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetScrollCallback(window, ScrollCallback);
}

void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	// Forward event to ImGui's handler
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;  // Let ImGui handle it
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		mainCamera.SetYaw(mainCamera.Yaw() + xoffset);
		mainCamera.SetPitch(mainCamera.Pitch() + yoffset);

		if (mainCamera.Pitch() > 89.0f)
			mainCamera.SetPitch(89.0f);
		if (mainCamera.Pitch() < -89.0f)
			mainCamera.SetPitch(-89.0f);

		glm::vec3 direction;
		direction.x = cos(glm::radians(mainCamera.Yaw())) * cos(glm::radians(mainCamera.Pitch()));
		direction.y = sin(glm::radians(mainCamera.Pitch()));
		direction.z = sin(glm::radians(mainCamera.Yaw())) * cos(glm::radians(mainCamera.Pitch()));
		mainCamera.SetFrontVector(glm::normalize(direction));
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Forward event to ImGui's handler
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;  // Let ImGui handle it

	mainCamera.SetFOV(mainCamera.FOV() - (float)yoffset);
	if (mainCamera.FOV() < 1.0f)
		mainCamera.SetFOV(1.0f);
	if (mainCamera.FOV() > 45.0f)
		mainCamera.SetFOV(45.0f);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}