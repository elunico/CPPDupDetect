#include "MainUI.hpp"

int main()
{
    Fl::lock();
    DupDetectWindow* window = DupDetectWindow::create();
    window->show();
    return Fl::run();
}