#include <GLFW/glfw3.h>

namespace Input
{
	enum Input_Action
	{
		LEFTDRAG,
		RIGHTDRAG,
		FORWARD,
		EXIT,
		BACKWARD,
		PANRIGHT,
		PANLEFT,
		ZOOM,
		RESETFOCUS,
	};

	extern bool BackwardActionActive;
	extern bool PanLeftActionActive, PanRightActionActive;
	extern double XDiff, YDiff, ScrollYDiff;

	void Init(GLFWwindow* window);
	void Process(GLFWwindow* window);
	bool IsActionActive(Input_Action action);
	bool IsActionDown(Input_Action action);
}
