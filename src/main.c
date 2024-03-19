#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "headers/utils.h"

/*
    TODO:
    1) divide into packages
    2) add cache for screen
    3) add params for funcs
    4) add colors
*/

int main()
{
    initSignalHandlers();
    render();
    return 0;
}