#include "UI.h"

std::vector<Object*> UI::sceneObjects;
Object* UI::currentObject = nullptr;

void UI::RenderUI()
{
	const char* items[] = { "Cube", "UV Sphere", "Custom" };
	static int selected = -1; // Here we store our selection data as an index.

	// Rendering Settings
	ImGui::Begin("Renderer Settings");
	
	ImGui::End();

	//------ Scene Hierarchy ------//

	ImGui::Begin("Scene Hierarchy");
	static bool newObject = false;
	static int clicked = 0;
	if (ImGui::Button("New Object", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		clicked++;
	if (clicked & 1)
	{
		Object* obj = new Object("New Object");
		sceneObjects.push_back(obj);
		obj->ID = sceneObjects.size() - 1;
		clicked = 0;
		selected = 0;
	}

	ImGui::Separator();
	
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		static int selectedNode = -1;

		// Leaf nodes
		if (sceneObjects[i]->childObjects.size() == 0)
		{
			ImGuiTreeNodeFlags nodeFlags = baseFlags;
			nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

			const bool isSelected = (selectedNode == i);
			if (isSelected)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, sceneObjects[i]->name.c_str());
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				selectedNode = i;
				currentObject = sceneObjects[i];
			}
		}
		// TODO: Full tree hierarchy
	}
	ImGui::End();

	//------ Object Settings ------//
	ImGui::Begin("Object Settings");

	if (currentObject)
	{
		char objectName[128] = "New Object";
		strcpy_s(objectName, currentObject->name.c_str());
		if (ImGui::InputText("Name", objectName, IM_ARRAYSIZE(objectName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			currentObject->name = objectName;
		}

		ImGui::NewLine();
		ImGui::SeparatorText("Mesh");
		static ImGuiComboFlags flags = 0;

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* comboPreviewValue = (selected >= 0 && selected < IM_ARRAYSIZE(items)) ? items[selected] : "Select Type";

		if (ImGui::BeginCombo("Type", comboPreviewValue, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool isSelected = (selected == n);
				if (ImGui::Selectable(items[n], isSelected))
					selected = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (selected == 0)
		{
			Mesh cube = Mesh("CUBE");

			currentObject->SetMesh(cube);
		}
		else if (selected == 1)
		{
			Mesh uvSphere = Mesh("UV_SPHERE");
			currentObject->SetMesh(uvSphere);
		}
		else if (selected == 2)
		{
			static std::string path = "";
			ImGui::Text("Selected Model Path:");
			ImGui::SameLine();
			ImGui::Text("%s", path.c_str());

			if (ImGui::Button("Choose Model File"))
			{
				// Open the file dialog
				ImGuiFileDialog::Instance()->OpenDialog("ChooseModel", "Choose a 3D Model", ".obj,.fbx,.gltf,.glb");
			}

			// Display the file dialog if it's open
			if (ImGuiFileDialog::Instance()->Display("ChooseModel"))
			{
				// Check if the user selected a file
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					path = ImGuiFileDialog::Instance()->GetFilePathName();
					Model custom = Model(path);
					currentObject->SetMesh(custom);
					currentObject->isPrimitive = false;
				}

				// Close the dialog
				ImGuiFileDialog::Instance()->Close();
			}
		}

		ImGui::SeparatorText("Transform");
		glm::vec3 pos = currentObject->transform.position;
		ImGui::DragFloat3("Position", &currentObject->transform.position.x, 1.0f);
		glm::vec3 rot = currentObject->transform.eulerRotation;
		ImGui::DragFloat3("Rotation", &currentObject->transform.eulerRotation.x, 1.0f);
		glm::vec3 scale = currentObject->transform.scale;
		ImGui::DragFloat3("Scale", &currentObject->transform.scale.x, 1.0f);

		currentObject->UpdatePosition(pos);
		currentObject->UpdateRotation(rot);
		currentObject->UpdateScale(scale);

		ImGui::SeparatorText("Shader");

		static std::string vertexPath = "";
		ImGui::Text("Selected Vertex Shader Path:");
		ImGui::SameLine();
		ImGui::Text("%s", vertexPath.c_str());

		if (ImGui::Button("Choose Vertex Shader Files"))
		{
			// Open the file dialog
			ImGuiFileDialog::Instance()->OpenDialog("ChooseVertShader", "Choose a vertex shader file", ".vert");
		}

		// Display the file dialog if it's open
		if (ImGuiFileDialog::Instance()->Display("ChooseVertShader"))
		{
			// Check if the user selected a file
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				vertexPath = ImGuiFileDialog::Instance()->GetFilePathName();
			}

			// Close the dialog
			ImGuiFileDialog::Instance()->Close();
		}

		static std::string fragmentPath = "";
		ImGui::Text("Selected Fragment Shader Path:");
		ImGui::SameLine();
		ImGui::Text("%s", fragmentPath.c_str());

		if (ImGui::Button("Choose Fragment Shader Files"))
		{
			// Open the file dialog
			ImGuiFileDialog::Instance()->OpenDialog("ChooseFragShader", "Choose a fragment shader file", ".frag");
		}

		// Display the file dialog if it's open
		if (ImGuiFileDialog::Instance()->Display("ChooseFragShader"))
		{
			// Check if the user selected a file
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				fragmentPath = ImGuiFileDialog::Instance()->GetFilePathName();
			}

			// Close the dialog
			ImGuiFileDialog::Instance()->Close();
		}

		if (!vertexPath.empty() && !fragmentPath.empty())
		{
			Shader shader = Shader(vertexPath.c_str(), fragmentPath.c_str());
			currentObject->SetShader(shader);
		}
	}

	ImGui::End();

	//------ Console ------//
	ImGui::Begin("Console");

	ImGui::End();
}

void UI::SetWindowIcon(GLFWwindow* window)
{
	GLFWimage images[1]; // Array to hold the icon image
	int width, height, channels;

	// Load the icon image
	const char* path = "../OpenGL/assets/icon.png";
	images[0].pixels = stbi_load(path, &width, &height, &channels, 4); // Force 4 channels (RGBA)

	if (images[0].pixels)
	{
		images[0].width = width;
		images[0].height = height;

		// Set the window icon
		glfwSetWindowIcon(window, 1, images);

		// Free the image memory after setting the icon
		stbi_image_free(images[0].pixels);
	}
	else
	{
		std::cout << "Failed to load window icon!" << std::endl;
	}
}

GLFWwindow* UI::CreateWindow(int width, int height, const char* name)
{
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(width, height, name, nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	glClearColor(0.025f, 0.025f, 0.05f, 1.0f);
	SetWindowIcon(window);

	return window;
}

void UI::InitImGui(GLFWwindow* window)
{
	// Initialize ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup ImGui configuration flags
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable multi-viewports

	// Setup ImGui style
	ImGui::StyleColorsDark();  // Dark theme
	//ImGui::StyleColorsClassic(); // Classic theme
	// ImGui::StyleColorsLight(); // Light theme

	// Initialize Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void UI::BeginFrame()
{
	// Start new ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		// Fullscreen window flags (no title bar, no resize, etc.)
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // Another approach

		ImGui::Begin("DockSpace Window", nullptr, windowFlags);
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(); // if you pushed the transparent color

		// Create the dockspace
		ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");

		// Add pass-thru to see the scene behind the center region:
		ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
		ImGui::End();
	}
}

void UI::EndFrame()
{
	// Render ImGui draw data
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and render additional Platform Windows (for multi-viewport)
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UI::Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}