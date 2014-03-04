EXT = cpp
CXX = g++

EXEC = exe

UNAME = $(shell uname)

ifeq ($(UNAME), Darwin)
CXXFLAGS = -Wall -Wno-deprecated-declarations  -g
LDFLAGS = -framework OpenGL -framework GLUT  -framework ApplicationServices -lSOIL -L/usr/local/lib -lglfw3
endif

ifeq ($(UNAME), Linux)
CXXFLAGS = -Wall
LDFLAGS = -lglut -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi
endif
 
 
OBJDIR = obj
SRC = $(wildcard *.$(EXT))
OBJ = $(SRC:.$(EXT)=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))
 
all: $(EXEC)
 
$(EXEC): $(OBJ)
	@$(CXX) -o $@ $^ $(LDFLAGS)
 
$(OBJDIR)/%.o: %.$(EXT)
	@$(CXX) -o $@ -c $< $(CXXFLAGS)
 
clean:
	@rm -rf $(OBJDIR)/*.o
	@rm -f $(EXEC)