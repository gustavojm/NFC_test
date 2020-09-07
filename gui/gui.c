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
struct mot_pap_msg *pole_msg_snd;

enum dest {
	Pole, Arm, Lift
};

void msg_send(void *msg, enum dest dest)
{
	QueueHandle_t *queue;

	switch (dest) {
	case Pole:
		msg = (struct pole_msg*) msg;
		queue = pole_queue;
		break;
//	case Arm:
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

//int gui_main(int argc, char *argv[])
//{
//    GtkBuilder      *builder;
//    GtkWidget       *window;
//
//    gtk_init(&argc, &argv);
//
//    builder = gtk_builder_new();
//    gtk_builder_add_from_file (builder, "gui/NFC_gui.glade", NULL);
//
//    window = GTK_WIDGET(gtk_builder_get_object(builder, "NFC_test_main_window"));
//    gtk_builder_connect_signals(builder, NULL);
//
////    GTK_WINDOW (gtk_builder_get_object (xml, "NFC_test_main_window"));
////    g_object_set(main_window, "application", application, NULL);
//
//    g_object_unref(builder);
//
//    gtk_widget_show(window);
//    gtk_main();
//
//    return 0;
//}

GtkWidget *scale1;
GtkWidget *motor;

void on_button1_clicked()
{
}

void gui_task(void)
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

void on_button1_button_press_event()
{
	int32_t position = (int32_t) gtk_range_get_value(GTK_RANGE(scale1));
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg*));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = 1;
		pole_msg_snd->type = MOT_PAP_TYPE_CLOSED_LOOP;
		pole_msg_snd->closed_loop_setpoint = position;
		msg_send(pole_msg_snd, Pole);
	} else {
		lDebug(Error, "gui: out of memory \n");
	}
}

void on_button1_button_release_event()
{
	pole_msg_snd = (struct mot_pap_msg*) malloc(sizeof(struct mot_pap_msg*));
	if (pole_msg_snd != NULL) {
		pole_msg_snd->ctrlEn = 1;
		pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		msg_send(pole_msg_snd, Pole);
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

