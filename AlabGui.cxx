// generated by Fast Light User Interface Designer (fluid) version 1.0304

#include "AlabGui.h"
#include "AlabLogic.h"

void AlabGui::cb_iArch_i(Fl_Input*, void*) {
  onInpArch();
}
void AlabGui::cb_iArch(Fl_Input* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_iArch_i(o,v);
}

void AlabGui::cb_iOs_i(Fl_Input*, void*) {
  onInpOs();
}
void AlabGui::cb_iOs(Fl_Input* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_iOs_i(o,v);
}

void AlabGui::cb_iEnviron_i(Fl_Input*, void*) {
  onInpEnviron();
}
void AlabGui::cb_iEnviron(Fl_Input* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_iEnviron_i(o,v);
}

void AlabGui::cb_iVendor_i(Fl_Input*, void*) {
  onInpVendor();
}
void AlabGui::cb_iVendor(Fl_Input* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_iVendor_i(o,v);
}

void AlabGui::cb_x86_i(Fl_Button*, void*) {
  onBtnX86();
}
void AlabGui::cb_x86(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_x86_i(o,v);
}

void AlabGui::cb_x861_i(Fl_Button*, void*) {
  onBtnX86_();
}
void AlabGui::cb_x861(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_x861_i(o,v);
}

void AlabGui::cb_x64_i(Fl_Button*, void*) {
  onBtnX64();
}
void AlabGui::cb_x64(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_x64_i(o,v);
}

void AlabGui::cb_x641_i(Fl_Button*, void*) {
  onBtnX64_();
}
void AlabGui::cb_x641(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_x641_i(o,v);
}

void AlabGui::cb_arm_i(Fl_Button*, void*) {
  onBtnArm();
}
void AlabGui::cb_arm(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_arm_i(o,v);
}

void AlabGui::cb_arm64_i(Fl_Button*, void*) {
  onBtnArm64();
}
void AlabGui::cb_arm64(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_arm64_i(o,v);
}

void AlabGui::cb_ppc_i(Fl_Button*, void*) {
  onBtnPpc();
}
void AlabGui::cb_ppc(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_ppc_i(o,v);
}

void AlabGui::cb_ppc64_i(Fl_Button*, void*) {
  onBtnPpc64();
}
void AlabGui::cb_ppc64(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_ppc64_i(o,v);
}

void AlabGui::cb_ppc641_i(Fl_Button*, void*) {
  onBtnPpc64le();
}
void AlabGui::cb_ppc641(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_ppc641_i(o,v);
}

void AlabGui::cb_mips_i(Fl_Button*, void*) {
  onBtnMips();
}
void AlabGui::cb_mips(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_mips_i(o,v);
}

void AlabGui::cb_icCodeModel_i(Fl_Input_Choice*, void*) {
  onIcCodeModel();
}
void AlabGui::cb_icCodeModel(Fl_Input_Choice* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_icCodeModel_i(o,v);
}

void AlabGui::cb_icRelocModel_i(Fl_Input_Choice*, void*) {
  onIcRelocModel();
}
void AlabGui::cb_icRelocModel(Fl_Input_Choice* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_icRelocModel_i(o,v);
}

void AlabGui::cb_thumb_i(Fl_Button*, void*) {
  onBtnThumb();
}
void AlabGui::cb_thumb(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_thumb_i(o,v);
}

void AlabGui::cb_arm1_i(Fl_Button*, void*) {
  onBtnArmBE();
}
void AlabGui::cb_arm1(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_arm1_i(o,v);
}

void AlabGui::cb_thumb1_i(Fl_Button*, void*) {
  onBtnThumbBE();
}
void AlabGui::cb_thumb1(Fl_Button* o, void* v) {
  ((AlabGui*)(o->parent()->user_data()))->cb_thumb1_i(o,v);
}

Fl_Double_Window* AlabGui::make_window() {
  { mainWindow = new Fl_Double_Window(1071, 652, "Assembler Lab");
    mainWindow->user_data((void*)(this));
    { menuBar = new Fl_Menu_Bar(0, 0, 1070, 20, "menu");
      menuBar->color((Fl_Color)29);
    } // Fl_Menu_Bar* menuBar
    { Fl_Text_Editor_Asm* o = srcCode = new Fl_Text_Editor_Asm(3, 64, 462, 485);
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
    { iArch = new Fl_Input(264, 38, 60, 23, "arch/sub:");
      iArch->callback((Fl_Callback*)cb_iArch);
      iArch->align(Fl_Align(FL_ALIGN_TOP));
      iArch->when(FL_WHEN_CHANGED);
    } // Fl_Input* iArch
    { iOs = new Fl_Input(386, 38, 60, 23, "os/sys:");
      iOs->callback((Fl_Callback*)cb_iOs);
      iOs->align(Fl_Align(FL_ALIGN_TOP));
      iOs->when(FL_WHEN_CHANGED);
    } // Fl_Input* iOs
    { iEnviron = new Fl_Input(447, 38, 60, 23, "envir/abi:");
      iEnviron->callback((Fl_Callback*)cb_iEnviron);
      iEnviron->align(Fl_Align(FL_ALIGN_TOP));
      iEnviron->when(FL_WHEN_CHANGED);
    } // Fl_Input* iEnviron
    { iVendor = new Fl_Input(325, 38, 60, 23, "vendor:");
      iVendor->callback((Fl_Callback*)cb_iVendor);
      iVendor->align(Fl_Align(FL_ALIGN_TOP));
      iVendor->when(FL_WHEN_CHANGED);
    } // Fl_Input* iVendor
    { log = new Fl_Text_Display_Log(3, 553, 1064, 96);
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
    { hexView = new HexView(469, 64, 597, 485);
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
    { Fl_Button* o = new Fl_Button(3, 22, 29, 20, "x86");
      o->callback((Fl_Callback*)cb_x86);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(32, 22, 33, 20, "x86*");
      o->callback((Fl_Callback*)cb_x861);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(65, 22, 30, 20, "x64");
      o->callback((Fl_Callback*)cb_x64);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(95, 22, 34, 20, "x64*");
      o->callback((Fl_Callback*)cb_x641);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(3, 42, 30, 20, "arm");
      o->callback((Fl_Callback*)cb_arm);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(164, 22, 47, 20, "arm64");
      o->callback((Fl_Callback*)cb_arm64);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(211, 22, 48, 20, "ppc");
      o->callback((Fl_Callback*)cb_ppc);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(163, 42, 45, 20, "ppc64");
      o->callback((Fl_Callback*)cb_ppc64);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(208, 42, 51, 20, "ppc64*");
      o->callback((Fl_Callback*)cb_ppc641);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(129, 22, 35, 20, "mips");
      o->callback((Fl_Callback*)cb_mips);
    } // Fl_Button* o
    { icCodeModel = new Fl_Input_Choice(594, 21, 83, 20, "code model:");
      icCodeModel->callback((Fl_Callback*)cb_icCodeModel);
    } // Fl_Input_Choice* icCodeModel
    { icRelocModel = new Fl_Input_Choice(593, 41, 83, 20, "reloc model:");
      icRelocModel->callback((Fl_Callback*)cb_icRelocModel);
    } // Fl_Input_Choice* icRelocModel
    { Fl_Button* o = new Fl_Button(67, 42, 45, 20, "thumb");
      o->callback((Fl_Callback*)cb_thumb);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(33, 42, 34, 20, "arm*");
      o->callback((Fl_Callback*)cb_arm1);
    } // Fl_Button* o
    { Fl_Button* o = new Fl_Button(112, 42, 51, 20, "thumb*");
      o->callback((Fl_Callback*)cb_thumb1);
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
  //Fl::add_idle(onIdle, &gui);
  w->end();
  w->show();
  int rc = Fl::run();
  onExit();
  return rc;
  /* fluid will insert Fl::run() here... */
  return Fl::run();
}
