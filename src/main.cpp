#include <iostream>
#include "TifCompress.h"


void doMain(const char *doType, const char *src, const char *dst, int threadCount) {
    TifCompress tifCompress(threadCount);
    if (strcmp(doType, "-f") == 0) {
        tifCompress.runWithFile(src, dst);
    } else if (strcmp(doType, "-d") == 0) {
        tifCompress.runWithDir(src, dst);
    } else {
        printf("Invalid doType: %s\n", doType);
    }

    int fc = tifCompress.failedCount.load();
    if (fc >0)
        printf("failedCount = %d\n", tifCompress.failedCount.load());
    else
        printf("all succeeded \n");
}

int main(int argc, char *argv[]) {
    /*  test
     * -f "F:\testdata\tiff\source\2020031.tif" "F:\testdata\tiff\target\2020031.tif"
     * -d "F:\testdata\tiff\source" "F:\testdata\tiff\target" 2
     */
    if ((argc != 4 && argc != 5) || strcmp(argv[1], "--help") == 0) {
        printf("Usage: tifCompress.exe [-f or -d] [srcDir] [dstDir] [threadCount (default=1)]\n");
        printf("   -f:          compress a single file\n");
        printf("   -d:          compress all files in a directory\n");
        printf("   srcDir:      the source file or directory\n");
        printf("   dstDir:      the destination directory\n");
        printf("   threadCount: the thread count (default=1)\n");

        return 0;
    }

    const char *doType = argv[1];
    const char *src = argv[2];
    const char *dst = argv[3];
    int threadCount = argc == 5 && strcmp(doType, "-d") == 0 ? strtol(argv[4], nullptr, 10) : 1;

    printf("================ run params ==============\n" );
    printf("doType = %s\nsrc = %s\ndst = %s\nthreadCount = %d\n", doType, src, dst, threadCount);
    printf("=========================================\n" );
    printf("\n");

    // run
    doMain(doType, src, dst, threadCount);

    std::cout << "\n>>>>>>>>>>>>> finish <<<<<<<<<<<<\n" << std::endl;

    return 0;
}


