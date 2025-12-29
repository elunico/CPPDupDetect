#include "MainUI.hpp"

int main()
{
    Fl::lock();
    const auto window = DupDetectWindow::create();
    window->show();
    return Fl::run();
}
