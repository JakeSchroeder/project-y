CC = cl
CFLAGS = /I./include /nologo /MD
LDFLAGS = /link /LIBPATH:./lib
LIBS = glfw3.lib opengl32.lib gdi32.lib user32.lib shell32.lib kernel32.lib

TARGET = main.exe
SRC = main.c glad.c vertex_buffer.c index_buffer.c renderer.c
OBJ = $(SRC:.c=.obj)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(LDFLAGS) /Fe:$(TARGET)

.c.obj:
	$(CC) $(CFLAGS) /c $<

clean:
	del $(TARGET) *.obj