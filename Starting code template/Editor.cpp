#include "Editor.h"
#include "Ligth.h"

Editor::Editor()
{
}


Editor::~Editor()
{
}

void Editor::Init(GLFWwindow* window)
{
	ImGui_ImplGlfwGL3_Init(window, true);
}

void Editor::NewFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	editorIsUsingMouse = io.WantCaptureMouse;
	editorIsUsingKeyboard = io.WantCaptureKeyboard;
}

void Editor::Render()
{
	ImGui::Render();
}

void Editor::CleanUp()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void Editor::Update()
{
	ImGui::BeginMainMenuBar();
	{
		if (ImGui::BeginMenu("Editors"))
		{
			ImGui::MenuItem("Directional light", nullptr, &dirLightEditorWin);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (dirLightEditorWin && ImGui::Begin("Directional light editor",&dirLightEditorWin))
	{
		ImGui::DragFloat3("Direction", &dirLight->direction.x, .05f);
		ImGui::ColorEdit3("Ambient", &dirLight->ambient.x);
		ImGui::ColorEdit3("Diffuse", &dirLight->diffuse.x);
		ImGui::ColorEdit3("Specular", &dirLight->specular.x);
		
		ImGui::End();
	}
}