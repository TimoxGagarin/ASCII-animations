#include <stdlib.h>
#include "headers/screen.h"
#include "headers/list.h"
#include <string.h>
#include <CL/cl.h>

screen_t *create_screen(int cols, int rows)
{
    screen_t *scr = malloc(sizeof(screen_t));
    if (scr == NULL)
        return NULL;

    scr->cols = cols;
    scr->rows = rows;

    scr->buffer = (char *)malloc(rows * cols);
    if (scr->buffer == NULL)
    {
        free(scr);
        return NULL;
    }
    memset(scr->buffer, ' ', cols * rows);
    return scr;
}

void realloc_screen(screen_t *scr, int cols, int rows)
{
    int old_rows = scr->rows;
    int old_cols = scr->cols;

    scr->cols = cols;
    scr->rows = rows;

    scr->buffer = (char *)realloc(scr->buffer, rows * cols);
    if (scr->buffer == NULL)
    {
        free(scr);
        return NULL;
    }
    memset(scr->buffer, ' ', cols * rows);
}

void destroy_screen(screen_t *scr)
{
    if (scr != NULL)
    {
        for (int i = 0; i < scr->rows; i++)
            free(scr->buffer[i]);
        free(scr->buffer);
        free(scr);
    }
}

void clear_buf(screen_t *screen)
{
    memset(screen->buffer, ' ', screen->cols * screen->rows);
}

void write_buf(screen_t *screen)
{
    mvprintw(0, 0, "%s", screen->buffer);
    refresh();
}