#ifndef STARTUP_H
#define STARTUP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "mediaparse.h"
#include <strsafe.h>

#include <tchar.h>
#include "database_functions.h"


//request directory file to be searched
int startUp();

//takes the requested directory file and searches through for movie files
int directorySearch(char* mainStart);

#endif