// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "HlabGui.h"
#include "HlabLogic.h"

Fl_Double_Window* HlabGui::make_window() {
  { mainWindow = new Fl_Double_Window(604, 527, "Hex Lab");
    mainWindow->user_data((void*)(this));
    { menuBar = new Fl_Menu_Bar(0, 0, 604, 20, "menu");
      menuBar->color((Fl_Color)29);
    } // Fl_Menu_Bar* menuBar
    { hexView = new HexView(0, 20, 604, 480);
      hexView->box(FL_BORDER_BOX);
      hexView->color(FL_BACKGROUND2_COLOR);
      hexView->selection_color(FL_BACKGROUND_COLOR);
      hexView->labeltype(FL_NO_LABEL);
      hexView->labelfont(0);
      hexView->labelsize(14);
      hexView->labelcolor(FL_FOREGROUND_COLOR);
      hexView->align(Fl_Align(FL_ALIGN_CENTER));
      hexView->when(FL_WHEN_RELEASE);
    } // HexView* hexView
    { statusBar = new Fl_Output(0, 500, 604, 20);
      statusBar->color((Fl_Color)29);
      statusBar->labelfont(4);
      statusBar->textfont(4);
    } // Fl_Output* statusBar
    mainWindow->end();
  } // Fl_Double_Window* mainWindow
  return mainWindow;
}

int main(int argc, char **argv) {
  HlabGui gui;
  Fl_Double_Window *w = gui.make_window();
  onGuiFinished(&gui, argc, argv);
  //Fl::add_idle(onIdle, &gui);
  w->end();
  w->show();
  int rc = Fl::run();
  onExit();
  return rc;
  /* fluid will insert Fl::run() here... */
  return Fl::run();
}
