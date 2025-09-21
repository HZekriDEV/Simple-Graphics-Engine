#ifndef UI_H
#define UI_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "gl_buffers.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include "Light.h"
#include "Color.h"
#include "mesh.h"
#include "model.h"
#include "Object.h"
#include <ImGuiFileDialog/ImGuiFileDialog.h>


class UI
{
	public:
		static std::vector<Object*> sceneObjects;
		static Object* currentObject;

		static void RenderUI();

		static void SetWindowIcon(GLFWwindow* window);

		static GLFWwindow* CreateWindow(int width, int height, const char* name);

		static void InitImGui(GLFWwindow* window);

		static void BeginFrame();

		static void EndFrame();

		static void Cleanup(GLFWwindow* window);
};


#endif 