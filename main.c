#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "track.h"

void draw_dots (cairo_t *cr, double width, double height);
static int t_running = 0;
static guint timer;

static gboolean
expose_event (GtkWidget *widget, GdkEventExpose *event) 
{
	cairo_t *cr;
	double cw, ch;
	int i;

	cr = gdk_cairo_create (widget->window);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 0.5);
	cw = (double)widget->allocation.width;
	ch = (double)widget->allocation.height;
        cairo_rectangle (cr, 5, 5, cw - 10, ch - 10);
	cairo_stroke (cr);
        draw_dots (cr, cw - 10, ch - 10);
	cairo_destroy (cr);
	return TRUE;
}

static gboolean timer_tick (GtkWidget *w)
{
        dot_t *d;
        int l, t, r, b;
	int cw, ch;
        int i;
        static int c = 150;

        if(!(--c)) {
            c = 150;
            if (d_cnt < 25)
                add_dot ();
        }

	cw = w->allocation.width;
	ch = w->allocation.height;
        l = cw;
        t = ch;
        r = 0;
        b = 0;

        for (i = 0; i < d_cnt; i++) {
            d = &dots[i];
            l = (l < d->x) ? l : d->x;
            t = (t < d->y) ? t : d->y;
            r = (r > d->x) ? r : d->x;
            b = (b > d->y) ? b : d->y;
            place_dot (i, 10, 10, cw - 10, ch - 10);
            l = (l < d->x) ? l : d->x;
            t = (t < d->y) ? t : d->y;
            r = (r > d->x) ? r : d->x;
            b = (b > d->y) ? b : d->y;
        }
        l -= 7;
        t -= 7;
        r += 7;
        b += 7;
        gtk_widget_queue_draw_area (w, l, t, r - l + 1, b - t + 1);

        return TRUE;
}

static void b_button_clicked (GObject *button, GtkWidget *w) 
{
        if (!t_running) {
            add_dot ();
            timer = g_timeout_add (20, timer_tick, w);
            t_running = 1;
        }
}

static void e_button_clicked (GObject *button)
{
        if (t_running) {
            d_cnt = 0;
            g_source_remove (timer);
            t_running = 0;
        }
}

int main(int argc, char *argv[]) 
{
	GtkWidget *window;
	GtkWidget *canvas;
	GtkWidget *b_button;
	GtkWidget *e_button;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label; 

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "DOT tracker");

	hbox = gtk_hbox_new (FALSE, 10);
	label = gtk_label_new ("00:00:00");

	b_button = gtk_button_new_with_label ("Start");
	e_button = gtk_button_new_with_label ("Stop");

	gtk_box_pack_start (GTK_BOX (hbox), b_button, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), e_button, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request (canvas, 1200, 600);

	vbox = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX (vbox), canvas, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);


	gtk_container_add (GTK_CONTAINER (window), vbox);

	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);  

	g_signal_connect(G_OBJECT (canvas), "expose_event",
		G_CALLBACK(expose_event), NULL);  

	g_signal_connect(G_OBJECT (b_button), "clicked",
		G_CALLBACK(b_button_clicked), canvas);  

	g_signal_connect(G_OBJECT (e_button), "clicked",
		G_CALLBACK(e_button_clicked), NULL);  

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
