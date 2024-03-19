#pragma once
#include "list.h"
#include <ncurses.h>

typedef struct screen
{
    int cols;
    int rows;
    char *buffer;
} screen_t;

screen_t *create_screen(int cols, int rows);
void realloc_screen(screen_t *scr, int cols, int rows);
void destroy_screen(screen_t *scr);
void clear_buf(screen_t *screen);
void write_buf(screen_t *screen);