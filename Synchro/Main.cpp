// synchronize 2 directories

// testing on:
// 1th dir is empty
// 2th dir consists of 10.7 GB (Files: 8 375; Folders: 1 152)
// Result:
// CP ~ 0.5% - 1.8%
// memory - 0.8 MB
// HDD - 49% (45-100 MB/s)
// copyied all data
// time: ~7min

#include <Windows.h> // for ShellExecuteA()
#include "Menu.h"
#include "Log.h"

int main()
{
	char strServer[FILENAME_MAX] = { 0 };
	char strLocal[FILENAME_MAX] = { 0 };

	// task mode choice
	if (ChoosingMode(strServer, strLocal))
	{
		// create log-file
		CreateLogFile();
		
		// task choice
		if (!ChoosingAction(strServer, strLocal))
			return 0;

		// open the log-file
		ShellExecuteA(NULL, "open", "Logs.txt", NULL, NULL, SW_NORMAL);
	}
	return 0;
}