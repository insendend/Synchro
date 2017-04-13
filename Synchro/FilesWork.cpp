#include "FilesWork.h"

// check for existence of the folder
bool IsExistFolder(const char *strFolder)
{
	_finddata_t fd = { 0 };

	// strFolder - source folder with mask "*.*"
	intptr_t handle = _findfirst(strFolder, &fd);
		
	if (handle == ERROR_FIND)
	{
		printf("Main folder is not exist!\n");
		return false;
	}
	_findclose(handle);
	return true;
}

// append "*.*" to a path
void AddMask(char *path)
{
	if (path[strlen(path) - 1] != '\\')
		strcat_s(path, FILENAME_MAX * sizeof(char) - 1, "\\");
	strcat_s(path, FILENAME_MAX * sizeof(char) - 1, "*.*");
}

void CreateFolder(const char *strFolder)
{
	// create folder (if subdir doesn't exist)
	if (!_mkdir(strFolder))
	{
		// note to a log file
		FILE *f = 0;
		errno_t err = fopen_s(&f, "Logs.txt", "a");	// log-file
		if (err)
		{
			printf("Error open log-file");
			return;
		}

		// note to the log only parrent directory
		fprintf(f, "%s Creating folder %s\n", GetCurrTime(), strFolder);
		fclose(f);
		return;
	}

	// if didn't create
	char strSubFolder[FILENAME_MAX] = { 0 };
	const char *FIND = strrchr(strFolder, '\\');

	// path without name of the last directory
	strncpy_s(strSubFolder, sizeof(strSubFolder) - 1, strFolder, FIND - strFolder);
		
	// create subdir
	CreateFolder(strSubFolder);
		
	// create parrent
	_mkdir(strFolder);
}

// copy a file to a folder
void Copy(const char *strPathFrom, const char *strPathTo, const int &index)
{
	// open the file which is copying
	FILE *from = 0, *to = 0;
	errno_t err = fopen_s(&from, strPathFrom, "rb");
		
	if (err)
	{
		printf("Error openinng file %s\n", strPathFrom);
		return;
	}

	// forming a destination path
	char strNewFileName[PATHNAME_MAX] = { 0 };
	strcpy_s(strNewFileName, sizeof(strNewFileName) - 1, strPathTo);

	strcat_s(strNewFileName, sizeof(strNewFileName) - strlen(strNewFileName) - 1, strPathFrom + index); // index - strlen(source)
		
	// try to open/create file in the destionation folder
	errno_t err2 = fopen_s(&to, strNewFileName, "wb");
	if (err2)
	{
		// file didn't create, need to create subdirs
		char strFolder[PATHNAME_MAX] = { 0 };
		const char *NAME = strrchr(strNewFileName, '\\');
		strncpy_s(strFolder, sizeof(strFolder) - 1, strNewFileName, strlen(strNewFileName) - strlen(NAME));
			
		// recursive func which create all subdirs in the params
		CreateFolder(strFolder);

		// create a file
		if (fopen_s(&to, strNewFileName, "wb"))
		{
			printf("Error creating file %s\n", strNewFileName);
			return;
		}
	}

	// buffer for storage 4 KB from a file
	char pBuff[COPY_BUFF] = { 0 };

	while (!feof(from))
	{
		// read 4 KB from a file and write to a copy
		size_t sizeRead = fread(pBuff, sizeof(char), sizeof(pBuff), from);
		fwrite(pBuff, sizeof(char), sizeRead, to);
	}

	fclose(from);
	fclose(to);
}
	
// search in the source and start copying files which doesn't exist in the destination
// realize by 2 search: external (source) and interior (destination)
bool SearchForCopy(const char *strPathFrom, const char *strPathTo, const int &index)
{
	_finddata_t fd_from = { 0 };

	// search "*.*" in the source
	intptr_t handle = _findfirst(strPathFrom, &fd_from);
		
	if (handle == ERROR_FIND)
	{
		printf("Not found directory\n");
		return true;
	}
	do
	{
		// ignore root and parrent catalog
		if (!strcmp(fd_from.name, ".") || !strcmp(fd_from.name, ".."))
			continue;

		// foriming full path of the found file/directory
		const char *FIND = strrchr(strPathFrom, '\\');
		char strName[PATHNAME_MAX] = { 0 };
		strncpy_s(strName, sizeof(strName) - 1, strPathFrom, ++FIND - strPathFrom);

		// strName - full path to the file/directory in the source
		strcat_s(strName, sizeof(strName) - strlen(strName) - 1, fd_from.name);
		if (fd_from.attrib & _A_SUBDIR)
		{
			// this is a folder
			strcat_s(strName, sizeof(strName) - strlen(strName) - 1, "\\*.*");

			if (SearchForCopy(strName, strPathTo, index))
			{
				// keep searching in this folder while file wasn't found
				_findclose(handle);

				// prevent searching
				return true;
			}
		}

		else
		{
			// this is a file
			// forming a filename for a seasrch in the destination folder
			char strCopy[PATHNAME_MAX] = { 0 };
			strcpy_s(strCopy, sizeof(strCopy) - 1, strPathTo);

			// strName + index - part of a file which stay after source path
			strcat_s(strCopy, strName + index);

			_finddata_t fd_to = { 0 };

			// search for the found file (by name) in the destionation directory
			intptr_t handle2 = _findfirst(strCopy, &fd_to);
			if (handle2 == ERROR_FIND)
			{
				// copy
				Copy(strName, strPathTo, index);

				// note in the log-file
				FILE *f = 0;

				// log-file
				errno_t err = fopen_s(&f, "Logs.txt", "a");
				if (err)
				{
					printf("Error open log-file");
					_findclose(handle);
					return true;
				}

				fprintf(f, "%s Copying file %s\n", GetCurrTime(), strCopy);
				fclose(f);

				// jump to next file/catalog
				continue;
			}

			do
			{
				// found a file with a same name
				if (fd_from.time_write > fd_to.time_write)
						
					// only if source file has more actual version
					Copy(strName, strPathTo, index);
			} while (!_findnext(handle2, &fd_to));
			_findclose(handle2);
		}
	} while (!_findnext(handle, &fd_from));
	_findclose(handle);

	// no errors
	return false;
}

// delete a folder
bool DeletingFolder(const char *strFolder)
{
	if (!_rmdir(strFolder))
		// delete was successfully
		return false;

	// folder doesn't exist or empty
	// forming a path for searching data and delete it
	char strFindInside[PATHNAME_MAX] = { 0 };
	strcpy_s(strFindInside, sizeof(strFindInside) - 1, strFolder);
	strcat_s(strFindInside, sizeof(strFindInside) - strlen(strFindInside) - 1, "\\*.*");

	_finddata_t fd = { 0 };
	intptr_t handle = _findfirst(strFindInside, &fd);
	if (handle == ERROR_FIND)
	{
		printf("Not found your directory\n");
		return true;
	}

	do
	{
		// ignore a root and parrent directory
		if (!strcmp(fd.name, ".") || !strcmp(fd.name, ".."))
			continue;

		// forming a full path with a path of the found file/directory which need to delete also
		char strName[PATHNAME_MAX] = { 0 };
		strcpy_s(strName, sizeof(strName) - 1, strFolder);
		strcat_s(strName, sizeof(strName) - strlen(strName) - 1, "\\");
		strcat_s(strName, sizeof(strName) - strlen(strName) - 1, fd.name);

		if (fd.attrib & _A_SUBDIR)
		{
			// this is a folder
			// try to delete it by the same recursive function
			DeletingFolder(strName);

			// don't forget delete a parrent folder
			_rmdir(strFolder);
		}

		else
			// this is a file
			// delete it
			remove(strName);
	} while (!_findnext(handle, &fd));
	_findclose(handle);

	// delete the target (already empty) directory
	_rmdir(strFolder);

	// no errors and folder has been deleted
	return false;
}

// all of source files has been copied to the destionation by this moment 
// need to delete "extra" files (which doesn't exist in the destination, but were in the destionation before copy

// research all data in the destionation and delete "extra" files which doesn't exist in the source
// realize by 2 search: external (destination) and interior (source)
bool SearchForDeleting(char *strPathFrom, char *strPathTo, const int &index)
{
	_finddata_t fd_to = { 0 };
	intptr_t handle = _findfirst(strPathTo, &fd_to);
		
	if (handle == ERROR_FIND)
	{
		printf("Not found directory\n");
		return true;
	}
	do
	{
		// ignore a root and parrent catalog
		if (!strcmp(fd_to.name, ".") || !strcmp(fd_to.name, ".."))
			continue;

		// foriming full path with a name of the file/directory in the destination folder
		char strName[PATHNAME_MAX] = { 0 };

		// pointer to *.*
		char *FIND = strrchr(strPathTo, '\\');

		// delete the mask from the path
		strncpy_s(strName, sizeof(strName) - 1, strPathTo, ++FIND - strPathTo);
			
		// strName - fullpath to the file/dir
		strcat_s(strName, sizeof(strName) - strlen(strName) - 1, fd_to.name);

		// a similar path at the source folder
		char strDel[PATHNAME_MAX] = { 0 };
			
		// pointer to "*.*"
		FIND = strrchr(strPathFrom, '\\');
			
		// delete the mask from the path
		strncpy_s(strDel, sizeof(strDel) - 1, strPathFrom, FIND - strPathFrom);
			
		// strDel - fullpath to the possible file/dir at the source directory
		strcat_s(strDel, strName + index);

		// serach this file/dir in the source folder
		_finddata_t fd_from = { 0 };
		intptr_t handle2 = _findfirst(strDel, &fd_from);
		if (handle2 == ERROR_FIND)
		{
			// if doesn't exist in the source
			// delete in the destination
			if (fd_to.attrib & _A_SUBDIR)
			{
				// this is a folder
				// recursive delete content of the folder
				DeletingFolder(strName);

				// note in the log-file
				FILE *f = 0;
				errno_t err = fopen_s(&f, "Logs.txt", "a");
				if (err)
				{
					printf("Error open log-file");
					_findclose(handle);
					return true;
				}

				fprintf(f, "%s Deleting folder %s\n", GetCurrTime(), strName);
				fclose(f);
			}
			else
			{
				// this is file
				// delete it
				remove(strName);

				// note in the log-file
				FILE *f = 0;
				errno_t err = fopen_s(&f, "Logs.txt", "a");
				if (err)
				{
					printf("Error open log-file");
					_findclose(handle);
					_findclose(handle2);
					return true;
				}
				fprintf(f, "%s Deleting file %s\n", GetCurrTime(), strName);

				fclose(f);
			}
		}

		// if file has been found in the source, close the search in the source dir and jump to the next file in the destionation
		_findclose(handle2);

	} while (!_findnext(handle, &fd_to));
	_findclose(handle);

	// no errors
	return false;
}

// get paths of the source and destination from the file paths.txt
bool PathsFromFile(char *strServer, char *strLocal)
{
	FILE *f = 0;
		
	// try to open the file 
	errno_t err = fopen_s(&f, "paths.txt", "rb");
		
	if (err)
	{
		printf("Error openinng file paths.txt\n");
		return true;
	}

	// set cursor to the end of the file
	fseek(f, 0, SEEK_END);

	// size of file
	long lSizeOfFile = ftell(f);

	// memory for content of the file
	char *pBuff = (char*)malloc((lSizeOfFile) * sizeof(char));
	if (!pBuff)
	{
		// check for allocking memory
		printf("Error malloc for paths.txt\n");
		fclose(f);
		return true;
	}

	// set cursor to the start for reading
	fseek(f, 0, SEEK_SET);
		
	// reading
	size_t sizeRead = fread(pBuff, sizeof(char), lSizeOfFile, f);

	if (!sizeRead)
	{
		// file is empty or error reading
		printf("Error fread in paths.txt\n");
		free(pBuff);
		fclose(f);
		return true;
	}

	// parse the buffer for paths
	char *FIND_serv = strchr(pBuff, '"');
	FIND_serv++;
	int i = 0;
	while (FIND_serv[i] != '"')
	{
		// copy by one symbol while not finding next symbol '"'
		strServer[i] = FIND_serv[i];
		i++;
	}

	// move the pointer for searching the next path
	FIND_serv += strlen(strServer) + 1;

	// find the next symbol '"' from the changed pointer
	char *FIND_loc = strchr(FIND_serv, '"');
	FIND_loc++;
	i = 0;
	while (FIND_loc[i] != '"')
	{
		// copy as similar by one symbol while not finding next symbol '"'
		strLocal[i] = FIND_loc[i];
		i++;
	}

	free(pBuff);
	fclose(f);

	// no errors
	return false;
}