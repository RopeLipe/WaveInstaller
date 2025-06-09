/* main.c - Application entry point */
#include <gtk/gtk.h>
#include <adwaita.h>
#include "wave-installer.h"

int main(int argc, char *argv[]) {
    g_autoptr(WaveInstallerApplication) app = NULL;
    
    /* Initialize Adwaita */
    adw_init();
    
    /* Create application */
    app = wave_installer_application_new();
    
    /* Run application */
    return g_application_run(G_APPLICATION(app), argc, argv);
}
