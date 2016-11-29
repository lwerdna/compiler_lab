// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "AlabGui.h"
#include "AlabLogic.h"

void AlabGui::cb_icExamples_i(Fl_Input_Choice*, void*) {
  onExampleSelection();
}
void AlabGui::cb_icExamples(Fl_Input_Choice* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_icExamples_i(o,v);
}

void AlabGui::cb_cbAtt_i(Fl_Check_Button*, void*) {
  onDialectChange();
}
void AlabGui::cb_cbAtt(Fl_Check_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_cbAtt_i(o,v);
}

void AlabGui::cb_test_i(Fl_Button*, void*) {
  onBtnTest();
}
void AlabGui::cb_test(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_test_i(o,v);
}

Fl_Double_Window* AlabGui::make_window() {
  { mainWindow = new Fl_Double_Window(1071, 713, "Assembler Lab");
    mainWindow->user_data((void*)(this));
    { menuBar = new Fl_Menu_Bar(0, 0, 1071, 20, "menu");
      menuBar->color((Fl_Color)29);
    } // Fl_Menu_Bar* menuBar
    { Fl_Text_Editor_Asm* o = srcCode = new Fl_Text_Editor_Asm(4, 103, 462, 505);
      srcCode->box(FL_DOWN_FRAME);
      srcCode->color(FL_BACKGROUND2_COLOR);
      srcCode->selection_color(FL_SELECTION_COLOR);
      srcCode->labeltype(FL_NORMAL_LABEL);
      srcCode->labelfont(0);
      srcCode->labelsize(14);
      srcCode->labelcolor(FL_FOREGROUND_COLOR);
      srcCode->textfont(4);
      srcCode->align(Fl_Align(FL_ALIGN_TOP_LEFT|FL_ALIGN_INSIDE));
      srcCode->when(FL_WHEN_RELEASE);
      srcBuf = new Fl_Text_Buffer();
      o->buffer(srcBuf);
      srcBuf->text("testes");
      srcBuf->add_modify_callback(onSourceModified, this);
    } // Fl_Text_Editor_Asm* srcCode
    { oArch = new Fl_Output(238, 39, 60, 23, "arch:");
      oArch->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oArch
    { oSubArch = new Fl_Output(300, 39, 60, 23, "subarch:");
      oSubArch->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oSubArch
    { oOs = new Fl_Output(424, 39, 60, 23, "os:");
      oOs->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oOs
    { oEnviron = new Fl_Output(486, 39, 60, 23, "environ:");
      oEnviron->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oEnviron
    { oFormat = new Fl_Output(548, 39, 60, 23, "format:");
      oFormat->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oFormat
    { oVendor = new Fl_Output(362, 39, 60, 23, "vendor:");
      oVendor->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Output* oVendor
    { icExamples = new Fl_Input_Choice(5, 39, 227, 24, "examples:");
      icExamples->callback((Fl_Callback*)cb_icExamples);
      icExamples->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Input_Choice* icExamples
    { log = new Fl_Text_Display_Log(4, 612, 1063, 96);
      log->box(FL_DOWN_FRAME);
      log->color(FL_FOREGROUND_COLOR);
      log->selection_color(FL_SELECTION_COLOR);
      log->labeltype(FL_NORMAL_LABEL);
      log->labelfont(0);
      log->labelsize(14);
      log->labelcolor(FL_FOREGROUND_COLOR);
      log->align(Fl_Align(FL_ALIGN_TOP));
      log->when(FL_WHEN_RELEASE);
    } // Fl_Text_Display_Log* log
    { cbAtt = new Fl_Check_Button(612, 35, 28, 28, "at&&t syntax");
      cbAtt->down_box(FL_DOWN_BOX);
      cbAtt->callback((Fl_Callback*)cb_cbAtt);
    } // Fl_Check_Button* cbAtt
    { hexView = new HexView(471, 103, 596, 505);
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
    { Fl_Button* o = new Fl_Button(16, 68, 28, 28, "test");
      o->callback((Fl_Callback*)cb_test);
    } // Fl_Button* o
    mainWindow->end();
  } // Fl_Double_Window* mainWindow
  srcCode->linenumber_width(24);
  return mainWindow;
}

int main(int argc, char **argv) {
  AlabGui gui;
  Fl_Double_Window *w = gui.make_window();
  onGuiFinished(&gui);
  Fl::add_idle(onIdle, &gui);
  w->end();
  w->show();
  int rc = Fl::run();
  onExit();
  return rc;
  /* fluid will insert Fl::run() here... */
  return Fl::run();
}
