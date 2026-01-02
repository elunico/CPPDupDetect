#include "file-platform.hpp"
#include "FL/fl_ask.H"
#include "MainUI.hpp"

int main()
{
    Fl::lock();
    /*
     * The shared_ptr<DupDetectWindow> MUST live for the duration of the program
     */

    /* callbacks that update the UI are queued during a scan so the window must
     * live long enough to empty the queue of Fl::awake events*/
    const auto window = DupDetectWindow::create();

    /* set up a handler so the user is notified if they use an unavailable
     * platform dependant function */
    set_on_platform_unavailable([](char const*) {
        fl_alert("This platform does not support \"reveal item\"");
    });

    window->show();
    return Fl::run();
}
