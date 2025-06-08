#ifndef NETWORK_SCREEN_H
#define NETWORK_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define NETWORK_TYPE_SCREEN (network_screen_get_type())
G_DECLARE_FINAL_TYPE(NetworkScreen, network_screen, NETWORK, SCREEN, GObject)

NetworkScreen *network_screen_new(void);

G_END_DECLS

#endif // NETWORK_SCREEN_H
