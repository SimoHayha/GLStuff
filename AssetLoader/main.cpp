#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "AssetManager.h"

int main(int ac, char* av[])
{
	if (!glfwInit())
		return false;
	GLFWwindow* win = glfwCreateWindow(1, 1, "id_Vox", nullptr, nullptr);
	glfwMakeContextCurrent(win);
	glfwHideWindow(win);

	glewInit();

	AssetManager manager("C:\\Users\\Valentin\\Documents\\VoxEngine\\AssetLoader\\Assets");
	manager.ScanProject();

	return 0;
}