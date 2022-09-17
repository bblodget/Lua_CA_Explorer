# This Makefile uses emscripten to compile
# this app for a web browser.
# I run it using Windows Subsystem for Linux (WSL2)
# I can run a local server using the commane
# > emrun --no_browser docs/WASM/basic_template.html
# Then in a browser goto the URL 
# http://localhost:6931/basic_template.html


CC := emcc	# Use for *.c files
CFLAGS := -O2

CXX := em++
CXXFLAGS := -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 --preload-file ./assets

OUT_DIR := docs/WASM
OBJ_DIR := emscript_obj
SRC_DIR := lua-5.4.4/src

LUA_SRC := $(wildcard $(SRC_DIR)/*.c)
LUA_OBJ_ORIG := $(patsubst %.c, %.o, $(LUA_SRC))
LUA_OBJ := $(subst $(SRC_DIR), $(OBJ_DIR), $(LUA_OBJ_ORIG))

APP_SRC := $(wildcard *.cpp)

HTML_OUT := $(OUT_DIR)/pge.html


all: $(HTML_OUT)

$(HTML_OUT): $(LUA_OBJ) $(APP_SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/pge.html $(LUA_OBJ) $(APP_SRC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OUT_DIR)/pge.*

