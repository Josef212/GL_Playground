#ifndef __EDITR_H__
#define __EDITR_H__

// ImGui
#include "imgui-master\imgui.h"
#include "imgui-master\imgui_impl_glfw_gl3.h"

#include <vector>

class PointLight;

class Editor
{
public:
	Editor();
	virtual ~Editor();

	void Init(GLFWwindow* window);
	void NewFrame();
	void Update();
	void Render();
	void CleanUp();

	bool editorIsUsingMouse = false;
	bool editorIsUsingKeyboard = false;

private:
	bool dirLightEditorWin = false;

};

#endif // !__EDITR_H__