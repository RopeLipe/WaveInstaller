#ifndef TIMEZONE_SCREEN_H
#define TIMEZONE_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define TIMEZONE_TYPE_SCREEN (timezone_screen_get_type())
G_DECLARE_FINAL_TYPE(TimezoneScreen, timezone_screen, TIMEZONE, SCREEN, GObject)

TimezoneScreen *timezone_screen_new(void);

G_END_DECLS

#endif // TIMEZONE_SCREEN_H
