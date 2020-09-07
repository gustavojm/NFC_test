#include <gtk/gtk.h>
#include <sys/times.h>
#include "gui.h"
#include "FreeRTOS.h"
#include "task.h"
#include "debug.h"
#include "pole.h"
#include "queue.h"
#include "stdlib.h"
#include "math.h"
#include "lift.h"

extern QueueHandle_t pole_queue;
//extern QueueHandle_t arm_queue;
extern QueueHandle_t lift_queue;

GtkWidget *scale1;
GtkWidget *motor;
GtkWidget *lift_dir_label;
GtkWidget *lift_motor_label;
GtkWidget *ctrlEn;

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
		lDebug(Info, "gui: pole command sent \n");
	} else {
		lDebug(Warn, "gui: unable to send pole command \n");
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
	scale1 = GTK_WIDGET(gtk_builder_get_object(builder, "scale1"));
	motor = GTK_WIDGET(gtk_builder_get_object(builder, "motor"));
	ctrlEn = GTK_WIDGET(gtk_builder_get_object(builder, "ctrlEn"));
	lift_dir_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "lift_dir_label"));
	lift_motor_label = GTK_WIDGET(
			gtk_builder_get_object(builder, "lift_motor_label"));

	gtk_builder_connect_signals(builder, NULL);

//    GTK_WINDOW (gtk_builder_get_object (xml, "NFC_test_main_window"));
//    g_object_set(main_window, "application", application, NULL);

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

	lDebug(Info, "Gui: task created \n");
}

void on_button1_button_press_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	int32_t position = (int32_t) gtk_range_get_value(GTK_RANGE(user_data));

	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg*));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		pole_msg_snd->type = MOT_PAP_TYPE_CLOSED_LOOP;
		pole_msg_snd->closed_loop_setpoint = position;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_button1_button_release_event()
{
	struct mot_pap_msg *pole_msg_snd;
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg*));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		;
		pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_lift_subir_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg*));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			lift_msg_snd->type = LIFT_TYPE_UP;
		if (event->type == GDK_BUTTON_RELEASE)
			lift_msg_snd->type = LIFT_TYPE_STOP;
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_lift_subir_toggle_toggled(GtkWidget *button, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg*));
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
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_lift_bajar_toggle_toggled(GtkWidget *button, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg*));
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
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_lift_bajar_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	struct lift_msg *lift_msg_snd;
	lift_msg_snd = (struct lift_msg*) malloc(sizeof(struct lift_msg*));
	if (lift_msg_snd != NULL) {
		lift_msg_snd->ctrlEn = gtk_toggle_button_get_active(
				GTK_TOGGLE_BUTTON(ctrlEn));
		if (event->type == GDK_BUTTON_PRESS)
			lift_msg_snd->type = LIFT_TYPE_DOWN;
		if (event->type == GDK_BUTTON_RELEASE)
			lift_msg_snd->type = LIFT_TYPE_STOP;
		msg_send(lift_msg_snd, Lift);
	} else {
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_lift_upLimit_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
	if (gtk_toggle_button_get_active(togglebutton)) {
		GPIO0_IRQHandler();
	}
}

void on_lift_downLimit_toggled(GtkToggleButton *togglebutton,
		gpointer user_data)
{
	if (gtk_toggle_button_get_active(togglebutton)) {
		GPIO1_IRQHandler();
	}
}

