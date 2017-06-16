#pragma once

#include "Header.h"
#include "Log.h"

bool IsExistFolder(const char *strFolder);												// check for folder exists
void AddMask(char *path);																// append mask "*.*" to path
void CreateFolder(const char *strFolder);												// create folder
void Copy(const char *strPathFrom, const char *strPathTo, const int &index);			// copy content of source to destination
bool SearchForCopy(const char *strPathFrom, const char *strPathTo, const size_t &index);	// search for files which doesn't exist in a destination folder
bool DeletingFolder(const char *strFolder);												// remove folder
bool SearchForDeleting(char *strPathFrom, char *strPathTo, const size_t &index);			// search for "extra" files in a destination
bool PathsFromFile(char *strServer, char *strLocal);									// get paths from the file paths.txt