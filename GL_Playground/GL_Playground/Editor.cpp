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
			ImGui::MenuItem("Point lights", nullptr, &pointLightsEditorWin);

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

	if (pointLightsEditorWin && ImGui::Begin("Point lights editor", &pointLightsEditorWin))
	{
		for (int i = 0; i < pointLights.size(); ++i)
		{
			if (ImGui::CollapsingHeader((("Point light") + std::to_string(i)).c_str()))
			{
				PointLight* p = pointLights[i];

				ImGui::Checkbox((("Enabled ") + std::to_string(i)).c_str(), &p->enabled);

				ImGui::DragFloat3((("Position ") + std::to_string(i)).c_str(), &p->position.x, .05f);
				ImGui::ColorEdit3((("Ambient ") + std::to_string(i)).c_str(), &p->ambient.x);
				ImGui::ColorEdit3((("Diffues ") + std::to_string(i)).c_str(), &p->diffuse.x);
				ImGui::ColorEdit3((("Specular ") + std::to_string(i)).c_str(), &p->specular.x);

				ImGui::Separator();

				ImGui::DragFloat((("Constant ") + std::to_string(i)).c_str(), &p->constant, 0.01f);
				ImGui::DragFloat((("Linear ") + std::to_string(i)).c_str(), &p->linear, 0.01f);
				ImGui::DragFloat((("Quadratic ") + std::to_string(i)).c_str(), &p->quadratic, 0.01f);
			}
		}

		ImGui::End();
	}
}