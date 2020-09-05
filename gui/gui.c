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

extern QueueHandle_t pole_queue;
struct mot_pap_msg *pole_msg_snd;


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

void on_button1_clicked() {
}

void gui_task(void)
{
    GtkBuilder      *builder;
    GtkWidget       *window;

    gtk_init(0, NULL);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "gui/NFC_gui.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "NFC_test_main_window"));
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

	xTaskCreate(gui_task, "Gui", configMINIMAL_STACK_SIZE, NULL,
	3, NULL);

	lDebug(Info, "Gui: task created \n");
}


void on_button1_button_press_event() {
	pole_msg_snd = (struct mot_pap_msg*) malloc(
			sizeof(struct mot_pap_msg*));
	if (pole_msg_snd) {
		pole_msg_snd->ctrlEn = 1;
		pole_msg_snd->type = MOT_PAP_TYPE_CLOSED_LOOP;
		pole_msg_snd->closed_loop_setpoint = random() % ((int32_t) pow(2, 16) - 1);
		if (xQueueSend(pole_queue, &pole_msg_snd,
				(TickType_t) 10) == pdPASS) {
			lDebug(Info, "comm: pole command sent \n");
		} else {
			lDebug(Warn, "comm: unable to send pole command \n");
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	} else {
		lDebug(Error, "comm: out of memory \n");
	}
}

void on_button1_button_release_event() {
	pole_msg_snd = (struct mot_pap_msg*) malloc(
			sizeof(struct mot_pap_msg*));
	if (pole_msg_snd) {
		struct tms time;
		srandom(times(&time));
		pole_msg_snd->ctrlEn = 1;
		pole_msg_snd->type = MOT_PAP_TYPE_STOP;
		pole_msg_snd->closed_loop_setpoint = random() % ((int32_t) pow(2, 16) - 1);
		if (xQueueSend(pole_queue, &pole_msg_snd,
				(TickType_t) 10) == pdPASS) {
			lDebug(Info, "comm: pole command sent \n");
		} else {
			lDebug(Warn, "comm: unable to send pole command \n");
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	} else {
		lDebug(Error, "comm: out of memory \n");
	}
}
