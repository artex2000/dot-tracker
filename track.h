#ifndef __TRACK_H__
#define __TRACK_H__

typedef struct {
	double x;
	double y;
	double a;
	double s;
        int dx;
        int dy;
} dot_t;


extern dot_t dots[];
extern int d_cnt;

void add_dot (void);
void place_dot (int idx, int min_x, int min_y,
                int max_x, int max_y);
#endif
