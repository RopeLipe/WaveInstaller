#ifndef LANGUAGE_SCREEN_H
#define LANGUAGE_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define LANGUAGE_TYPE_SCREEN (language_screen_get_type())
G_DECLARE_FINAL_TYPE(LanguageScreen, language_screen, LANGUAGE, SCREEN, GObject)

LanguageScreen *language_screen_new(void);

G_END_DECLS

#endif // LANGUAGE_SCREEN_H
