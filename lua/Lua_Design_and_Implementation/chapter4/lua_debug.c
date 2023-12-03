#include <stdio.h>
#include "../lua-5.1.4-master/lua.h"
#include "../lua-5.1.4-master/lualib.h"
#include "../lua-5.1.4-master/lauxlib.h"



int main(int argc, char* argv[]) {
    char *file = NULL;

    if (argc == 1) {
        file = "test.lua";
    } else {
        file = argv[1];
    }

    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, file);

    return 0;
}

// gcc -g -o lua_debug lua_debug.c  ../lua-5.1.4-master/liblua.a  -ldl  -lm
// ../lua-5.1.4-master/luac test.lua
// ../lua-5.1.4-master/lua ../ChunkSpy.lua luac.out test.lua --brief
