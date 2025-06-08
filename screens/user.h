#ifndef USER_SCREEN_H
#define USER_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define USER_TYPE_SCREEN (user_screen_get_type())
G_DECLARE_FINAL_TYPE(UserScreen, user_screen, USER, SCREEN, GObject)

UserScreen *user_screen_new(void);

G_END_DECLS

#endif // USER_SCREEN_H
