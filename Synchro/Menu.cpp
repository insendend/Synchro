#include "Menu.h"

void TextMenuMode()
{
	printf("Chose a mode for synchronization\n");
	printf("1. Auto (from paths.txt)\n");
	printf("2. Enter a paths\n");
	printf("9. Exit\n");
	printf("Your choice -> ");
}

void TextMenuAction()
{
	printf("What to do with folders?\n");
	printf("1. Replace local data\n");
	printf("2. Replace server data\n");
	printf("3. Unite all data in local folder\n");
	printf("4. Unite all data in server folder\n");
	printf("9. Exit\n");
	printf("Your choice -> ");
}

// 1th menu: task mode choice
bool ChoosingMode(char *strServer, char *strLocal)
{
	int choice = 0;

	do
	{
		// for user input
		char input[USER_MENU_INPUT] = { 0 };

		TextMenuMode();

		gets_s(input, sizeof(input) - 1);
			
		// covert user choice
		choice = atoi(input);

		system("cls");

	} while ((choice < 1 || choice > 2) && choice != 9);

	switch (choice)
	{
	case AUTO:
	{
		// get paths from file
		if (PathsFromFile(strServer, strLocal))
			return false;
		return true;
	}
	case MANUAL:
	{
		// input paths manually
		printf("Enter a path to server-folder: ");
		gets_s(strServer, FILENAME_MAX - 1);

		printf("Enter a path to local-folder: ");
		gets_s(strLocal, FILENAME_MAX - 1);

		return true;
	}
	case EXIT:
	{
		return false;
	}
	default:
	{
		printf("Incorrect data...");
		return false;
	}
	}
}

// 2nd menu: task choice
bool ChoosingAction(char *strServer, char *strLocal)
{
	int choice = 0;
	do
	{
		// user input
		char input[USER_MENU_INPUT] = { 0 };

		TextMenuAction();

		gets_s(input, sizeof(input) - 1);
			
		// covert user choice
		choice = atoi(input);

		system("cls");

	} while ((choice < 1 || choice > 4) && choice != 9);

	switch (choice)
	{
	case REPLACE_LOCAL:
	{
		// replace local data to server data

		// index, next after last
		size_t LastSymb = strlen(strServer);
		AddMask(strServer);

		// check for main folder existing
		if (!IsExistFolder(strServer))
			return false;
			
		// copying missing files to a destination folder
		if (SearchForCopy(strServer, strLocal, LastSymb))
			return false;

		LastSymb = strlen(strLocal);
		AddMask(strLocal);

		// delete "extra" data in a destination folder
		if (SearchForDeleting(strServer, strLocal, LastSymb))
			return false;

		return true;
	}
	case REPLACE_SERVER:
	{
		// replace server data to local
		size_t LastSymb = strlen(strLocal);
		AddMask(strLocal);

		if (!IsExistFolder(strLocal))
			return false;

		if (SearchForCopy(strLocal, strServer, LastSymb))
			return false;

		LastSymb = strlen(strServer);
		AddMask(strServer);

		if (SearchForDeleting(strLocal, strServer, LastSymb))
			return false;

		return true;
	}
	case SERVER_TO_LOCAL:
	{
		// copy server data to local
		size_t LastSymb = strlen(strServer);
		AddMask(strServer);

		if (!IsExistFolder(strServer))
			return false;

		if (SearchForCopy(strServer, strLocal, LastSymb))
			return false;

		return true;
	}
	case LOCAL_TO_SERVER:
	{
		// copy local data to server
		size_t LastSymb = strlen(strLocal);
		AddMask(strLocal);

		if (!IsExistFolder(strLocal))
			return false;

		if (SearchForCopy(strLocal, strServer, LastSymb))
			return false;

		return true;
	}
	case EXIT:
	{
		return false;
	}
	default:
	{
		printf("Incorrect data...");
		return false;
	}
	}
}