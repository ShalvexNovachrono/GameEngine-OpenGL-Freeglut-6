#pragma once
class r_window;

namespace callbacks {
	void init(r_window* rw);
	void display();
	void timer(int preferred_refresh);
	void mouse_click(int button, int state, int x, int y);
	void mouse_motion(int x, int y);
	void mouse_passive_motion(int x, int y);
}