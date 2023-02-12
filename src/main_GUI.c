#include"cpu.c"
#include<gtk/gtk.h>
#include<time.h>
#include<pthread.h>
enum {voltage_label,power_label,family_label,model_label,threads_label,label_num};
char*label_str[label_num]={"voltage_label","power_label","family_label","model_label","threads_label"};
GtkWidget*labels[label_num];
int closed=0;
void window_destroyed(){
    closed=1;
}
struct timespec interval,time_remain={1,0};
static void activate(GtkApplication*app,gpointer userdata){
    GtkWidget*window=gtk_application_window_new(app);
    g_signal_connect(window,"destroy",G_CALLBACK(window_destroyed),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"CPUED GUI");
    gtk_window_set_default_size(GTK_WINDOW(window),300,300);
    GtkWidget*window_box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,3);
    gtk_container_add(GTK_CONTAINER(window),window_box);
    GtkWidget*label_box=gtk_box_new(GTK_ORIENTATION_VERTICAL,3);
    gtk_container_add(GTK_CONTAINER(window_box),label_box);

    for(int i=0;i<label_num;i++){
        labels[i]=gtk_label_new(label_str[i]);
        gtk_container_add(GTK_CONTAINER(label_box),labels[i]);
    }
    gtk_widget_show_all(window);
}
int main(int argc,char*argv[]){
    GtkApplication*app=gtk_application_new("cpued.main",G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    int status=g_application_run(G_APPLICATION(app),argc,argv);
    // while(!closed){
    //     nanosleep(&interval,&time_remain);
    //     char text[30];
    //     sprintf(text,"%s : %f v",label_str[voltage_label],rdvol());
    //     gtk_label_set_text(GTK_LABEL(labels[voltage_label]),text);
    // }
    g_object_unref(app);
    return status;
}