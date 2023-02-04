#include"cpu.c"
#include<gtk/gtk.h>
static void activate(GtkApplication*app,gpointer userdata){
    GtkWidget*window=gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window),"CPUED GUI");
    gtk_window_set_default_size(GTK_WINDOW(window),200,200);
    gtk_widget_show_all(window);
}
int main(int argc,char*argv[]){
    GtkApplication*app=gtk_application_new("cpued.main",G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    int status=g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);
    return status;
}