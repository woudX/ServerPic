#ifndef STDAFX
#define STDAFX

//  Linux System Include
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

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

//  Third libs
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "json.h"

using namespace std;
using namespace cv;

#define Debug
#define err_log(format, ...) printf(format, __VA_ARGS__)
#define is_null(val) ((val) == NULL)
#define not_null(val) !is_null(val)
#define safe_del(x) do { if ((x) != NULL) delete (x); } while(false)

#ifdef Debug
    #define debug_log(format, ...) err_log(format, __VA_ARGS__)
#else
    #define debug_log(format, ...)
#endif


#endif // STDAFX
