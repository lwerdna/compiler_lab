CXX      = $(shell fltk-config --cxx)
DEBUG    = -g
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.
LDFLAGS  = $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags ) -lautils
LINK     = $(CXX)

TARGET = clab
OBJS = Gui.o logic.o Fl_Text_Editor_C.o Fl_Text_Editor_Asm.o
SRCS = Gui.cxx logic.cxx Fl_Text_Editor_C.cxx Fl_Text_Editor_Asm.cxx

.SUFFIXES: .o .cxx
%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

$(TARGET): $(OBJS)
Application.o: Gui.cxx logic.cxx Fl_Text_Editor_C.cxx Fl_Text_Editor_Asm.cxx

clean: $(TARGET) $(OBJS)
	rm -f *.o 2> /dev/null
	rm -f $(TARGET) 2> /dev/null

install:
	install ./clab /usr/local/bin
