#ifndef STARTUP_H
#define STARTUP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include "mediaparse.h"
#include <strsafe.h>

#include <tchar.h>


//request directory file to be searched
void startUp();

//takes the requested directory file and searches through for movie files
int directorySearch(char* mainStart);

#endif