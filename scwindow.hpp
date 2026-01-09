#ifndef SURVIVOR_CHOICE_HPP
#define SURVIVOR_CHOICE_HPP

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

enum struct SurvivorChoiceType { NEWEST, OLDEST, RANDOM };

struct SurvivorChoiceWindow : Fl_Window {
   private:
    Fl_Window* parent;
    Fl_Choice* choices;
    Fl_Button* go_button;
    bool       m_was_cancelled = true;

   public:
    static constexpr int w = 500;
    static constexpr int h = 130;

    explicit SurvivorChoiceWindow(Fl_Window* parent);

    [[nodiscard]] bool was_cancelled() const;

    void show_and_wait();

    [[nodiscard]] SurvivorChoiceType get_choice() const;
};

#endif
