#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <io.h>
#include <direct.h>


// consts for func EditCurrentTime()
#define DATE_AND_TIME 26				// symbols of string, forming ctime_s()
#define COUNT_OF_TIME_SYMBOLS 8			// amount of symbols of time ("hh:mm:ss")
#define START_INDEX_OF_TIME 11			// index of string (11th of 26) which forms ctime_s() and starts time-symbols
#define TIME_SIZE 9						// size of array for time storrage



#define USER_MENU_INPUT 16				// user ingput for 1st and 2nd menu

enum MODE { AUTO = 1, MANUAL, EXIT = 9 };
enum ACTION { REPLACE_LOCAL = 1, REPLACE_SERVER, SERVER_TO_LOCAL, LOCAL_TO_SERVER };



#define PATHNAME_MAX 4096	// max count of symbols of a full path to a file

#define ERROR_FIND -1		// value of search error
#define COPY_BUFF 4096		// max count of bytes for 1 copied block
#define PATHS_BUFF 512		// buffer for reading from the file paths.txt