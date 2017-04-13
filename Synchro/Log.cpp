#include "Log.h"

// get current time format: hh:mm:ss
const char * GetCurrTime()
{
	time_t t = 0;
		
	// amount of sec (since 01.01.1970)
	time(&t);

	char strDateAndTime[DATE_AND_TIME] = { 0 };

	// convert time in sec to default format (day of week, month ...)
	ctime_s(strDateAndTime, sizeof(strDateAndTime), &t);

	// var which will store a current time
	static char strTimeOnly[TIME_SIZE] = { 0 };

	// copy a part of memory which consists of hours:minutes:seconds
	memcpy_s(strTimeOnly, sizeof(strTimeOnly) - 1, strDateAndTime + START_INDEX_OF_TIME, COUNT_OF_TIME_SYMBOLS);

	return strTimeOnly;
}

// create log-file
void CreateLogFile()
{
	FILE *f = 0;

	errno_t err = fopen_s(&f, "Logs.txt", "a");

	if (err)
	{
		printf("Error open log-file\r\n");
		return;
	}

	// write a current date
	fprintf(f, "\t\t%s\n", __DATE__);

	fclose(f);
}