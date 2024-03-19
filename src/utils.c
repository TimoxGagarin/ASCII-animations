#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include "headers/screen.h"
#include "headers/utils.h"
#include "headers/transform.h"
#include <CL/cl.h>

#define GRAD_LEN 16
#define GRAD_LAST (float)(GRAD_LEN - 1)
#define PI 3.14159265358979323846

const char gradient[GRAD_LEN] = ".:!/r(l1Z4H9W8$@";
const float light_power = 1.5f;

volatile sig_atomic_t quit_flag = 0;

void handle_sigint(int signum)
{
    quit_flag = 1;
}

void handle_sigsegv(int signum)
{

    refresh();
    endwin();
    puts("Ошибка сегментирования (образ памяти сброшен на диск)");
    exit(EXIT_FAILURE);
}

/**
 * @brief Инициализирует обработчики сигналов.
 */
void initSignalHandlers()
{
    signal(SIGINT, &handle_sigint);
    signal(SIGSEGV, &handle_sigsegv);
}

typedef struct sph_domain
{
    float theta_min;
    float theta_max;
    float theta_step;

    float phi_min;
    float phi_max;
    float phi_step;
} sph_domain_t;

float clamp(int color, int min, int max)
{
    return fmin(max, fmax(color, min));
}

void decorator(screen_t *screen, transform_t *transf, vector3_t *light, sph_domain_t *domain, vector3_t (*figure)(float, float))
{
    for (float phi = domain->phi_min; phi < domain->phi_max; phi += domain->phi_step)
        for (float theta = domain->theta_min; theta < domain->theta_max; theta += domain->theta_step)
        {
            int rows = screen->rows;
            int cols = screen->cols;

            int global_id_x = get_global_id(0);
            int global_id_y = get_global_id(1);

            vector3_t point = figure(phi, theta);
            transform(&point, transf);

            vector3_t light_dir = norm((vector3_t){0.5, 0.5, 1});
            vector3_t n = norm(point);
            float diff = dot(n, light_dir) * light_power;
            int color = clamp((int)(diff * GRAD_LAST), 0, GRAD_LAST);

            if (point.y >= 0 && point.y < rows && point.x >= 0 && point.x < cols &&
                color > (strchr(gradient, screen->buffer[(int)point.y * cols + (int)point.x]) - gradient))
            {
                screen->buffer[(int)point.y * cols + (int)point.x] = gradient[color];
            }
        }
}

vector3_t torus(float phi, float theta)
{
    vector3_t point;
    point.x = (5.0f / 14.0f + 1.0f / 7.0f * cos(theta)) * cos(phi);
    point.y = (5.0f / 14.0f + 1.0f / 7.0f * cos(theta)) * sin(phi);
    point.z = 1.0f / 7.0f * sin(theta);
    return point;
}

vector3_t conus(float phi, float theta)
{
    vector3_t point;
    point.x = cos(theta) * sin(phi);
    point.z = sin(theta) * sin(phi);
    point.y = sin(phi);
    return point;
}

vector3_t sphere(float phi, float theta)
{
    vector3_t point;
    point.x = sin(phi) * cos(theta);
    point.z = sin(phi) * sin(theta);
    point.y = cos(phi);
    return point;
}

vector3_t cylinder(float phi, float theta)
{
    vector3_t point;
    point.x = cos(phi);
    point.z = sin(phi);
    point.y = sin(theta);
    return point;
}

// TODO: REWRITE IN SPHERICAL COORDINATES
// void taubian_heart(screen_t *screen)
// {
//     // float min = screen->rows < screen->cols ? screen->rows : screen->cols;
//     float rad = 256.0f;

//     float min_z = -20.0f;
//     float max_z = 20.0f;

//     for (int k = 0; k < 359; k++)
//     {
//         clear_buf(screen);
//         for (float z = min_z; z < max_z; z += 0.05f)
//         {
//             for (float x = -screen->cols / 2.0; x < floor((float)screen->cols / 2); x += 0.05f)
//             {
//                 float x_sq = x * x;
//                 float z_sq = z * z;
//                 float intercept = powf(80.0f * x_sq + 9.0f * z_sq, 1.0 / 3.0) / 2.0f;
//                 float sqrt_pol = sqrt(-x_sq - 2.25f * z_sq + intercept * intercept + rad);

//                 float y1 = sqrt_pol + intercept;
//                 float y2 = -sqrt_pol + intercept;
//                 vector3_t p1 = {x, y1, z};
//                 vector3_t p2 = {x, y2, z};

//                 vector3_t axis = {0.0, (float)k * PI / 180, 0.0};
//                 vector3_t new_p1 = rotate(p1, axis);
//                 vector3_t new_p2 = rotate(p2, axis);

//                 if (!isnan(y1))
//                 {
//                     screen->buffer[(int)-new_p1.y / 2 + screen->rows / 2][(int)new_p1.x / 2 + screen->cols / 2] = gradient[(int)(15.0 * (max_z + z) / (max_z - min_z))];
//                     screen->buffer[(int)-new_p2.y / 2 + screen->rows / 2][(int)new_p2.x / 2 + screen->cols / 2] = gradient[(int)(15.0 * (max_z + z) / (max_z - min_z))];
//                 }
//             }
//         }
//         write_buf(screen);
//     }
// }

vector3_t julia_heart(float i, float j)
{
    vector3_t point;
    point.x = sin(i) * (sin(j) / 2 - 2.0f * sin(3.0f * j) / 15);
    point.y = -sin(i) * (cos(j) / 2 - cos(2.0f * j) / 5 - cos(3.0f * j) / 15);
    point.z = 4.0f * cos(i) / 15;
    return point;
}

vector3_t trefoil(float i, float j)
{
    vector3_t point;
    point.x = sin(i) + 2.0f * sin(2.0f * i);
    point.y = cos(i) - 2 * cos(2 * i);
    point.z = -sin(3.0f * i);
    return point;
}

void check_error(cl_int err, const char *msg)
{
    if (err != CL_SUCCESS)
    {
        fprintf(stderr, "%s: %d\n", msg, err);
        exit(EXIT_FAILURE);
    }
}

void render()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    cbreak();
    attrset(COLOR_PAIR(1));
    clear();
    refresh();

    screen_t *screen = NULL;
    keypad(stdscr, TRUE);

    for (float t = 0.0f; !quit_flag; t += PI / 180.0f)
    {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        float min = rows < cols ? rows : cols;
        if (!screen)
            screen = create_screen(cols, rows);
        else
            realloc_screen(screen, cols, rows);

        vector3_t light = norm(new_vector3(sin(t * 0.01), cos(t * 0.01), -1.0));

        transform_t transf1 = new_transform(
            new_vector3((float)cols / 4.0, (float)rows / 4, 0.0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain1 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        transform_t transf2 = new_transform(
            new_vector3(cols * 2.0f / 4, rows / 4, 0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain2 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        transform_t transf3 = new_transform(
            new_vector3(cols * 3.0f / 4, rows / 4, 0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain3 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        transform_t transf4 = new_transform(
            new_vector3((float)cols / 4.0, (float)rows * 3.0 / 4, 0.0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain4 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        transform_t transf5 = new_transform(
            new_vector3(cols * 2.0f / 4, rows * 3.0 / 4, 0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain5 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        transform_t transf6 = new_transform(
            new_vector3(cols * 3.0f / 4, rows * 3.0 / 4, 0),
            new_vector3(0, t, 0),
            new_vector3(40.0f, 20.0f, 40.0f));
        sph_domain_t domain6 = (sph_domain_t){0.0f, 2 * PI, PI / 180.0f, 0.0f, 2 * PI, PI / 180.0f};

        clear_buf(screen);
        clear();
        decorator(screen, &transf1, &light, &domain1, torus);
        decorator(screen, &transf2, &light, &domain2, torus);
        decorator(screen, &transf3, &light, &domain3, torus);
        decorator(screen, &transf4, &light, &domain4, torus);
        decorator(screen, &transf5, &light, &domain5, torus);
        decorator(screen, &transf6, &light, &domain6, torus);
        write_buf(screen);
        usleep(10000);
    }

    destroy_screen(screen);

    curs_set(1);
    endwin();
}