#include "callbacks.h"
#include "r_window.h"

namespace callbacks {
	namespace {
		r_window* rw_instance = nullptr;
	}


	void init(r_window* rw) {
		rw_instance = rw;
	}

	/// <summary>
	/// DISPLAY function.
	/// This function is used to display the OpenGLRenderer display.
	/// </summary>
	void display() {
		if (rw_instance != nullptr) {
			rw_instance->draw();
		}
	}

	void reshapeWindow(int width, int height) {
		if (rw_instance != nullptr) {
			rw_instance->reshapeWindow(width, height);
		}
	}

	/// <summary>
	/// This will constantly call the timer function of the r_window at a preferred refresh rate.
	/// </summary>
	/// <param name="preferred_refresh"></param>
	void timer(int preferred_refresh) {
		if (rw_instance != nullptr) {
			rw_instance->timer();
			glutTimerFunc(preferred_refresh, callbacks::timer, preferred_refresh);
		}
	}



	/// <summary>
	/// Mouse click function callback.
	/// </summary>
	/// <param name="Button">The mouse button involved (e.g., GLUT_LEFT_BUTTON).</param>
	/// <param name="State">The state of the button (GLUT_DOWN or GLUT_UP).</param>
	/// <param name="X">The window-relative x-coordinate of the mouse.</param>
	/// <param name="Y">The window-relative y-coordinate of the mouse.</param>
	void mouseClick(int button, int state, int x, int y) {
		if (rw_instance != nullptr) {
			rw_instance->mouseClick(button, state, x, y);
		}
		glutPostRedisplay();
	}

	/// <summary>
	/// Mouse motion function callback.
	/// </summary>
	/// <param name="x">The current window-relative x-coordinate of the mouse cursor.</param>
	/// <param name="y">The current window-relative y-coordinate of the mouse cursor.</param>
	void mouseMotion(int x, int y) {
		if (rw_instance != nullptr) {
			rw_instance->mouseMotion(x, y);
		}
	}


	/// <summary>
	/// Mouse passive motion function callback.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void mousePassiveMotion(int x, int y) {
		if (rw_instance != nullptr) {
			rw_instance->mousePassiveMotion(x, y);
		}
	}

	void mouseScrollwheel(int button, int scrollDirection, int x, int y) {
		if (rw_instance != nullptr) {
			rw_instance->mouseScrollwheel(button, scrollDirection, x, y);
		}
	}
}