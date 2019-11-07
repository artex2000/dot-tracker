#include <stdio.h>
#include <math.h>
#include <glib.h>
#include <cairo.h>

#include "track.h"

#define MAX_DOTS 25

dot_t dots[MAX_DOTS];
int d_cnt = 0;

void add_dot (void)
{
    dot_t *d;
    int gr_angle;

    d = &dots[d_cnt++];

    d->x = 0;
    d->y = 0;
    d->dx = 0;
    d->dy = 0;
    d->s = g_random_double_range (1, 4);
    gr_angle = g_random_int_range (10, 80);
    d->a = (double)gr_angle * M_PI / 180;
}

void place_dot (int idx, int min_x, int min_y,
                int max_x, int max_y)
{
    dot_t *d = &dots[idx];
    double sx, sy;
    double nx, ny;

    if (d->x == 0 && d->y == 0) {
        d->x = min_x;
        d->y = min_y;
        return;
    }

    sx = d->s * cos (d->a);
    sy = d->s * sin (d->a);
    if (sx < 0)
        sx = -sx;
    if (sy < 0)
        sy = -sy;

    if (d->dx == 0) {
        nx = d->x + sx;
        if (nx > max_x) {
            d->dx = 1;
            nx = max_x;
        }
    } else {
        nx = d->x - sx;
        if (nx < min_x) {
            d->dx = 0;
            nx = min_x;
        }
    }

    if (d->dy == 0) {
        ny = d->y + sy;
        if (ny > max_y) {
            d->dy = 1;
            ny = max_y;
        }
    } else {
        ny = d->y - sy;
        if (ny < min_y) {
            d->dy = 0;
            ny = min_y;
        }
    }
    d->x = nx;
    d->y = ny;
}

void draw_dot (cairo_t *cr, dot_t *d)
{
    cairo_arc (cr, d->x, d->y, 5, 0, 2 * M_PI);
    cairo_stroke_preserve (cr);
    cairo_fill (cr);
}

void draw_dots (cairo_t *cr)
{
    int i;

    for (i = 0; i < d_cnt; i++) {
        draw_dot (cr, &dots[i]);
    }
}
