// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef AlabGui_h
#define AlabGui_h
#include <FL/Fl.H>
#include "Fl_Text_Editor_Asm.h"
#include "Fl_Text_Display_Log.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Check_Button.H>

class AlabGui {
public:
  Fl_Double_Window* make_window();
  Fl_Double_Window *mainWindow;
  Fl_Menu_Bar *menuBar;
  Fl_Text_Editor_Asm *srcCode;
  Fl_Text_Editor *asmCode;
  Fl_Output *oArch;
  Fl_Output *oSubArch;
  Fl_Output *oOs;
  Fl_Output *oEnviron;
  Fl_Output *oFormat;
  Fl_Input_Choice *icPresets;
private:
  inline void cb_icPresets_i(Fl_Input_Choice*, void*);
  static void cb_icPresets(Fl_Input_Choice*, void*);
public:
  Fl_Output *oVendor;
  Fl_Input_Choice *icExamples;
private:
  inline void cb_icExamples_i(Fl_Input_Choice*, void*);
  static void cb_icExamples(Fl_Input_Choice*, void*);
public:
  Fl_Text_Display_Log *log;
  Fl_Check_Button *cbAtt;
private:
  inline void cb_cbAtt_i(Fl_Check_Button*, void*);
  static void cb_cbAtt(Fl_Check_Button*, void*);
public:
  Fl_Text_Buffer *srcBuf; 
  Fl_Text_Buffer *bytesBuf; 
};
#endif
