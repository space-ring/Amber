CC = g++
TARGET = test
LIB = Engine
LIBPATH = .
CFLAGS = -I$(HDIR) -Ideps/glad/include -Ideps/glfw-3.3.6/include

SRCDIR = src
HDIR = inc
OBJDIR = build
# copy headers to inc, remember to delete inc

HS := $(wildcard $(SRCDIR)/*.h)
OBJ := $(patsubst $(SRCDIR)/*.cpp, $(OBJDIR)/%.o, $(SRC))
HEADERS := $(patsubst $(SRCDIR)/%.h, $(HDIR)/%.h, $(HS))

all: test.cpp $(HEADERS)
	$(CC) -o $(TARGET) $< -L$(LIBPATH) -l$(LIB) $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) build
	$(CC) -c -o $@ $< $(CFLAGS)

$(HDIR)/%.h: $(SRCDIR)/%.h inc
	cp $< $@

library: $(OBJ)
	ar rcs $(LIBPATH)/$(LIB)

inc:
	mkdir "inc"

clean:
	rm -rf $(HDIR)