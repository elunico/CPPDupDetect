#include "scwindow.hpp"

SurvivorChoiceWindow::SurvivorChoiceWindow(Fl_Window* parent)
    : Fl_Window(parent->x(), parent->y(), 500, 200, "Choose survivor strategy")
{
    set_modal();

    new Fl_Text_Display(20, 20, 200, 28, "Select a survivor strategy");

    choices = new Fl_Choice(20, 20, 250, 30);

    // WARN: the order of these choices must match the order of cases in the
    // SurvivorChoiceType enum
    choices->add("Newest File Survives");
    choices->add("Oldest File Survives");
    choices->add("Random File Survives");
    choices->value(0);

    go_button = new Fl_Button(20, 60, 120, 24, "Ok");
    go_button->callback(
        []([[maybe_unused]] auto* widget, void* win) {
            SurvivorChoiceWindow* window =
                static_cast<SurvivorChoiceWindow*>(win);
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

void SurvivorChoiceWindow::show() 
{
    Fl_Window::show();
    while (shown())
        Fl::wait();
}

SurvivorChoiceType SurvivorChoiceWindow::get_choice() const
{
    return static_cast<SurvivorChoiceType>(choices->value());
}
