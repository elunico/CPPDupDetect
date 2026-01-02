#include "scwindow.hpp"
#include <FL/Enumerations.H>
#include <FL/Fl_Output.H>

SurvivorChoiceWindow::SurvivorChoiceWindow(Fl_Window* parent)
    : Fl_Window(parent->x(), parent->y(), w, h, "Survivor Strategy"),
    parent(parent)
{
    static constexpr int padding = 20; 
    set_modal();

    auto *out = new Fl_Output(padding, 10, w - padding * 2, 30);
    out->box(FL_NO_BOX);
    out->value("Select a survivor strategy");

    choices = new Fl_Choice(padding, padding * 2, w - padding * 2, 30);

    // WARN: the order of these choices must match the order of cases in the
    // SurvivorChoiceType enum
    choices->add("Newest File Survives");
    choices->add("Oldest File Survives");
    choices->add("Random File Survives");
    choices->value(0);

    go_button = new Fl_Button(((w / 2) - (120 / 2)) + padding, 80, 100, 30, "OK");
    go_button->callback(
        []([[maybe_unused]] auto* widget, void* win) {
            auto* window = static_cast<SurvivorChoiceWindow*>(win);
            window->hide();
            window->m_was_cancelled = false;
        },
        this);

    end();
}

bool SurvivorChoiceWindow::was_cancelled() const
{
    return m_was_cancelled;
}

void SurvivorChoiceWindow::show_and_wait() 
{
    Fl_Window::show();
    while (shown()) {
        if (!parent->shown()) {
            break;
        }
        Fl::wait();
    }
}

SurvivorChoiceType SurvivorChoiceWindow::get_choice() const
{
    return static_cast<SurvivorChoiceType>(choices->value());
}
