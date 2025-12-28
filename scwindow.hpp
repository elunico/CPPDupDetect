#ifndef SURVIVOR_CHOICE_HPP
#define SURVIVOR_CHOICE_HPP

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>

enum struct SurvivorChoiceType { NEWEST, OLDEST, RANDOM };

struct SurvivorChoiceWindow : Fl_Window {
  private: 
    Fl_Window* parent;
    Fl_Choice* choices;
    Fl_Button* go_button;
    bool m_was_cancelled = true; 

  public:
    explicit SurvivorChoiceWindow(Fl_Window* parent);

    bool was_cancelled() const;

    void show_and_wait(); 

    SurvivorChoiceType get_choice() const;
};

#endif
