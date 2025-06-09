/* timezone-screen.h - Timezone selection screen */
#pragma once

#include <gtk/gtk.h>
#include <adwaita.h>
#include "../wave-installer.h" // Include the main application header

G_BEGIN_DECLS

// void timezone_screen_create(struct _WaveInstallerApplication *app);
void timezone_screen_create(WaveInstallerApplication *app);

G_END_DECLS
