#pragma once
class r_window;

namespace callbacks {
	void init(r_window* rw);
	void display();
	void reshapeWindow(int width, int height);
	void timer(int preferred_refresh);
	void mouseClick(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mousePassiveMotion(int x, int y);
	void mouseScrollwheel(int button, int scrollDirection, int x, int y);
}