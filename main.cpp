#include "MainUI.hpp"

int main()
{
    Fl::lock();
    auto window = DupDetectWindow::create();
    window->show();
    return Fl::run();
}
