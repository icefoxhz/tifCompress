#include <iostream>
#include <windows.h>
#include <libloaderapi.h>
#include <string>
#include "lua.hpp"
#include "lua_tinker.h"
#include "TifCompress.h"
#include "ThreadPool.h"

std::string getCurrentDir() {
    const int MY_MAX_PATH = 1024;
    char buf[MY_MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MY_MAX_PATH);
    std::string::size_type pos = std::string(buf).find_last_of("\\/");
    return std::string(buf).substr(0, pos);
}

TifCompress::TifCompress(int threadCount): _threadCount(threadCount), failedCount(0){

    _luaDir = getCurrentDir() + "" + "\\lua";
}

TifCompress::~TifCompress() = default;

void TifCompress::runWithFile(const char *src, const char *dst) {
    //初始化Lua （最后记得调用lua_close(lua_state)释放）
    lua_State *L = luaL_newstate();
    //加载Lua基本库
    luaL_openlibs(L);

    // 设置 luaDir
    lua_tinker::set(L, "luaDir", _luaDir.c_str());

    //加载/执行 lua 文件。
    lua_tinker::dofile(L, "lua/entryPoint.lua");

    // 用 lua_tinker::table 要加作用域， 让它出作用域后触发析构。不然等 lua_close了再触发析构就出错了
    {
        //接收函数返回的表。
        lua_tinker::table resultTable = lua_tinker::call<lua_tinker::table>(L, "run", src, dst);

        lua_tinker::table runResult = resultTable.get<lua_tinker::table>("runResult");
        int succeeded = runResult.get<bool>("succeeded");
        printf("================ run result ==============\nfile = %s\nsuccess = %d\nerror = %s\n",
               src,
               succeeded,
               runResult.get<const char *>("message"));

        if (!succeeded){
            failedCount ++;
        }
    }

    //关闭Lua
    lua_close(L);
}

void TifCompress::runWithDir(const char *srcDir, const char *dstDir) {
    std::vector<std::string> files;
    getAllFiles(srcDir, files);

    // ThreadPool 的析构函数中处理的等待结束。所以加个作用域
    {
        ThreadPool pool(_threadCount);
        for (const auto &file: files) {
            pool.enqueue([&] {
                const char *fileName = strrchr(file.c_str(), '\\');
                std::string dstFile = std::string(dstDir) + std::string(fileName);
                runWithFile(file.c_str(), dstFile.c_str());
            });
        }
    }
}

void TifCompress::getAllFiles(const char *dir, std::vector<std::string> &files) {

    //初始化Lua （最后记得调用lua_close(lua_state)释放）
    lua_State *L = luaL_newstate();
    //加载Lua基本库
    luaL_openlibs(L);

    // 设置 luaDir
    lua_tinker::set(L, "luaDir", _luaDir.c_str());

    //加载/执行 lua 文件。
    lua_tinker::dofile(L, "lua/entryPoint.lua");

    // 用 lua_tinker::table 要加作用域， 让它出作用域后触发析构。不然等 lua_close了再触发析构就出错了
    {
        //接收函数返回的表。
        lua_tinker::table tifTable = lua_tinker::call<lua_tinker::table>(L, "listFilesRecursive", dir, ".tif");

        int size = tifTable.getLength();
        for (int i = 1; i <= size; ++i) {
            std::string file = tifTable.get<const char *>(i);
            files.push_back(file);
        }
    }

    //关闭Lua
    lua_close(L);
}
