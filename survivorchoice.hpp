#ifndef SURVIVOR_CHOICE_HPP
#define SURVIVOR_CHOICE_HPP

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>

enum struct SurvivorChoiceType { OLDEST, NEWEST, RANDOM };

struct SurvivorChoice : Fl_Window {
  private: 
    Fl_Choice* choices;
    Fl_Button* go_button;
    bool m_was_cancelled = true; 

  public:
    explicit SurvivorChoice(Fl_Window* parent)
        : Fl_Window(parent->x(), parent->y(), 500, 200, "Choose survivor strategy")
    {
        set_modal();

        new Fl_Text_Display(20, 20, 200, 28, "Select a survivor strategy") ;
        
        choices = new Fl_Choice(20, 20, 250, 30);

        choices->add("Oldest File Survives");
        choices->add("Newest File Survives");
        choices->add("Random File Survives");
        choices->value(0);

        go_button = new Fl_Button(20, 60, 120, 24, "Ok");
        go_button->callback(
            [](auto* widget, void* win) {
                SurvivorChoice* window = static_cast<SurvivorChoice*>(win);
                window->hide();
                window->m_was_cancelled = false; 
            },
            this);

        end();
        
    }

    bool was_cancelled() const {
        return m_was_cancelled;
    }

    void show() override
    {
        Fl_Window::show();
        while(shown()) Fl::wait();
    }

    SurvivorChoiceType get_choice() const
    {
        return static_cast<SurvivorChoiceType>(choices->value());
    }
};

#endif
