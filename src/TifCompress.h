//
// Created by saber on 2022/8/16.
//

#ifndef TIFCOMPRESS_TIFCOMPRESS_H
#define TIFCOMPRESS_TIFCOMPRESS_H

#include "vector"

class TifCompress {
public:
    explicit TifCompress(int threadCount);
    ~TifCompress();
public:
    void runWithFile(const char *src, const char *dst);
    void runWithDir(const char *srcDir, const char *dstDir);
    void getAllFiles(const char *dir, std::vector<std::string>& files);

    std::atomic<int> failedCount;
private:
    std::string _luaDir;
    int _threadCount = 0;
};


#endif //TIFCOMPRESS_TIFCOMPRESS_H
