#ifndef STDAFX
#define STDAFX

//  Linux System Include
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <netinet/in.h>
#include <errno.h>

//  Build-in C
#include <stdlib.h>
#include <regex.h>
#include <stdarg.h>

//  Build-in C++
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <thread>
#include <vector>
#include <sstream>
#include <mutex>

//  Third libs
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "json.h"
#include "curl/multi.h"

using namespace std;
using namespace cv;

#define Debug
#define log(format, ...) printf(format, __VA_ARGS__)
#define err_log(format, ...) log(format, __VA_ARGS__)

#define is_null(val) ((val) == NULL)
#define not_null(val) !is_null(val)
#define safe_del(x) do { if ((x) != NULL) delete (x); } while(false)
#define safe_del_container(val)                                     \
do {                                                                \
    for (auto itor = val.begin(); itor != val.end(); ++itor)        \
    {                                                               \
        safe_del(*itor);                                            \
        (*itor) = NULL;                                             \
    }                                                               \
    val.clear();                                                    \
} while(false)                                                      \

#ifdef Debug
    #define debug_log(format, ...) log(format, __VA_ARGS__)
    #define debug_mode 1
#else
    #define debug_log(format, ...)
    #define debug_mode 0
#endif


#endif // STDAFX
