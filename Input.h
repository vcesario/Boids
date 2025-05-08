#include <GLFW/glfw3.h>

namespace Input
{
	extern bool ExitActionPerformed;
	extern double XDiff, YDiff;
	extern bool IsDragging;

	void Init(GLFWwindow* window);
	void Process(GLFWwindow* window);
}
