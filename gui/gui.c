#include <gtk/gtk.h>
#include <sys/times.h>

#include "FreeRTOS.h"
#include "task.h"
#include "gui.h"
#include "debug.h"
#include "pole.h"
#include "queue.h"
#include "stdlib.h"
#include "math.h"
#include "lift.h"
#include "string.h"

extern QueueHandle_t pole_queue;
//extern QueueHandle_t arm_queue;
extern QueueHandle_t lift_queue;

GtkWidget *ctrlEn;
GtkWidget *pole_direction_label;
GtkWidget *pole_pulse_label;
GtkWidget *pole_rdc_scale;
GtkWidget *lift_dir_label;
GtkWidget *lift_motor_label;
GtkWidget *lift_upLimit;
GtkWidget *lift_downLimit;

enum dest {
	Pole, Arm, Lift
};

void msg_send(void *msg, enum dest dest)
{
	QueueHandle_t *queue = NULL;

	switch (dest) {
	case Pole:
		msg = (struct pole_msg*) msg;
		queue = pole_queue;
		break;
	case Arm:
//		msg = (struct arm_msg *) msg;
//		queue = arm_queue
//		break;
	case Lift:
		msg = (struct lift_msg*) msg;
		queue = lift_queue;
		break;
	default:
		break;
	}

	if (xQueueSend(queue, &msg,
			(TickType_t) 10) == pdPASS) {
		lDebug(Info, "gui: command sent");
	} else {
		lDebug(Warn, "gui: unable to send command");
	}
}

void gui_task(void *args)
{
	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init(0, NULL);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "gui/NFC_gui.glade", NULL);

	window = GTK_WIDGET(
			gtk_builder_get_object(builder, "NFC_test_main_window"));

	ctrlEn = GTK_WIDGET(gtk_builder_get_object(builder, "ctrlEn"));
	pole_direction_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "pole_direction_label"));

	GtkWidget *pole_free_run_speed_scale = GTK_WIDGET(
			gtk_builder_get_object(builder, "pole_free_run_speed_scale"));

	for (int i = 0; i < 9; i++) {
		char v[3];
		sprintf(v, "%i", i);
		gtk_scale_add_mark(GTK_SCALE(pole_free_run_speed_scale), i,
				GTK_POS_RIGHT, v);
	}
	pole_rdc_scale = GTK_WIDGET(
			gtk_builder_get_object(builder, "pole_rdc_scale"));
	pole_pulse_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "pole_pulse_label"));
	lift_dir_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "lift_dir_label"));
	lift_motor_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "lift_motor_label"));
	lift_upLimit = GTK_WIDGET(gtk_builder_get_object(builder, "lift_upLimit"));
	lift_downLimit = GTK_WIDGET(
			gtk_builder_get_object(builder, "lift_downLimit"));

	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(builder);

	gtk_widget_show(window);
	gtk_main();

	return;
}

void gui_init(void)
{
	struct tms time;
	srandom(times(&time));

	xTaskCreate(gui_task, "Gui", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

	lDebug(Info, "Gui: task created");
}

/*
 * Pole Handlers
 */

void on_pole_close_loop_button_press_event(GtkWidget *widget,
		GdkEventButton *event, gpointer user_data)
{
	int32_t position = (int32_t) gtk_range_get_value(GTK_RANGE(user_data));

	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		pole_msg_snd->type = MOT_PAP_TYPE_CLOSED_LOOP;
		pole_msg_snd->closed_loop_setpoint = position;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_pole_free_run_cw_button_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	int32_t vel = (int32_t) gtk_range_get_value(GTK_RANGE(user_data));

	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			pole_msg_snd->type = MOT_PAP_TYPE_FREE_RUNNING;
		if (event->type == GDK_BUTTON_RELEASE)
			pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		pole_msg_snd->free_run_direction = MOT_PAP_DIRECTION_CW;
		pole_msg_snd->free_run_speed = vel;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_pole_free_run_ccw_button_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	int32_t vel = (int32_t) gtk_range_get_value(GTK_RANGE(user_data));

	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			pole_msg_snd->type = MOT_PAP_TYPE_FREE_RUNNING;
		if (event->type == GDK_BUTTON_RELEASE)
			pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		pole_msg_snd->free_run_direction = MOT_PAP_DIRECTION_CCW;
		pole_msg_snd->free_run_speed = vel;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void draw_limits(gpointer scale)
{
	gtk_scale_clear_marks(GTK_SCALE(scale));

	gtk_scale_add_mark(GTK_SCALE(scale), pole_get_status().cwLimit, GTK_POS_TOP,
			"cwLimit");

	gtk_scale_add_mark(GTK_SCALE(scale), pole_get_status().ccwLimit,
			GTK_POS_TOP, "ccwLimit");
}

void on_pole_set_cwLimit_button_press_event(GtkWidget *widget,
		GdkEventButton *event, gpointer user_data)
{
	uint16_t pos = pole_get_RDC_position();
	pole_set_cwLimit(pos);
	draw_limits(user_data);
	lDebug(Info, "gui: Pole cwLimit set to %u", pole_get_status().cwLimit);
}

void on_pole_set_ccwLimit_button_press_event(GtkWidget *widget,
		GdkEventButton *event, gpointer user_data)
{
	uint16_t pos = pole_get_RDC_position();
	pole_set_ccwLimit(pos);
	draw_limits(user_data);
	lDebug(Info, "gui: Pole ccwLimit set to %u", pole_get_status().ccwLimit);
}

void on_pole_stop_button_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

/*
 * Lift Handlers
 */

void on_lift_subir_button_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			lift_msg_snd->type = LIFT_TYPE_UP;
		if (event->type == GDK_BUTTON_RELEASE)
			lift_msg_snd->type = LIFT_TYPE_STOP;
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_lift_bajar_button_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			lift_msg_snd->type = LIFT_TYPE_DOWN;
		if (event->type == GDK_BUTTON_RELEASE)
			lift_msg_snd->type = LIFT_TYPE_STOP;
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_lift_subir_toggle_toggled(GtkWidget *button, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
			lift_msg_snd->type = LIFT_TYPE_UP;
		} else {
			lift_msg_snd->type = LIFT_TYPE_STOP;
		}
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_lift_bajar_toggle_toggled(GtkWidget *button, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) {
			lift_msg_snd->type = LIFT_TYPE_DOWN;
		} else {
			lift_msg_snd->type = LIFT_TYPE_STOP;
		}
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory");
	}
}

void on_lift_upLimit_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	if (gtk_toggle_button_get_active(togglebutton)) { //simulate rising edge
		GPIO0_IRQHandler();
	}
}

void on_lift_downLimit_toggled(GtkToggleButton *togglebutton,
		gpointer user_data)
{
	if (gtk_toggle_button_get_active(togglebutton)) { //simulate rising edge
		GPIO1_IRQHandler();
	}
}

void pole_pulse_handler(bool state)
{
	uint16_t cur_pos = 0;
	const char *dir;
	if (state) {
		gtk_label_set_text(GTK_LABEL(pole_pulse_label), "ON");
		dir = gtk_label_get_text(GTK_LABEL(pole_direction_label));
		if (strcmp(dir, "CW") == 0) {
			cur_pos = (uint16_t) gtk_range_get_value(GTK_RANGE(pole_rdc_scale));
			cur_pos++;
			gtk_range_set_value(GTK_RANGE(pole_rdc_scale), cur_pos);
		}

		if (strcmp(dir, "CCW") == 0) {
			cur_pos = (uint16_t) gtk_range_get_value(GTK_RANGE(pole_rdc_scale));
			cur_pos--;
			gtk_range_set_value(GTK_RANGE(pole_rdc_scale), cur_pos);
		}

	} else {
		gtk_label_set_text(GTK_LABEL(pole_pulse_label), "OFF");
	}
}

