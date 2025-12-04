#include "../include/main.h"
#include "../include/r_window.h"

int main(int argc, char* argv[]) {
    LOG_DEBUG("Starting application...");

    r_window* main_window = new r_window(800, 600, "Shalvex Novachrono's OpenGL Window");

    main_window->init(argc, argv);
    main_window->cleanUp();

    delete main_window;
    return 0;
}