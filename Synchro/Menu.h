#pragma once

#include "Header.h"
#include "FilesWork.h"

void TextMenuMode();									// text of menu with choosing a task mode
void TextMenuAction();									// text of menu with choosing a task with folders 
bool ChoosingMode(char *strServer, char *strLocal);		// task mode choise (auto / manual)
bool ChoosingAction(char *strServer, char *strLocal);	// task choise