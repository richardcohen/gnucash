/********************************************************************\
 * gnc-plugin-qif-import.c -- window for importing QIF files        *
 *                        (GnuCash)                                 *
 * Copyright (C) 2003 Jan Arne Petersen <jpetersen@uni-bonn.de>     *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652       *
 * Boston, MA  02110-1301,  USA       gnu@gnu.org                   *
\********************************************************************/

#include <config.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "dialog-new-user.h"
#include "dialog-preferences.h"
#include "assistant-qif-import.h"
#include "gnc-plugin-manager.h"
#include "gnc-plugin-qif-import.h"

static void gnc_plugin_qif_import_class_init (GncPluginQifImportClass *klass);
static void gnc_plugin_qif_import_init (GncPluginQifImport *plugin);
static void gnc_plugin_qif_import_finalize (GObject *object);

/* Command callbacks */
static void gnc_plugin_qif_import_cmd_new_qif_import (GSimpleAction *simple, GVariant *parameter, gpointer user_data);

#define PLUGIN_ACTIONS_NAME "gnc-plugin-qif-import-actions"
#define PLUGIN_UI_FILENAME  "gnc-plugin-qif-import.ui"

static GActionEntry gnc_plugin_actions [] =
{
    { "QIFImportAction", gnc_plugin_qif_import_cmd_new_qif_import, NULL, NULL, NULL },
};
/** The number of actions provided by this plugin. */
static guint gnc_plugin_n_actions = G_N_ELEMENTS(gnc_plugin_actions);

/** The default menu items that need to be add to the menu */
static const gchar *gnc_plugin_load_ui_items [] =
{
    "FilePlaceholder1",
    NULL,
};

typedef struct GncPluginQifImportPrivate
{
    gpointer dummy;
} GncPluginQifImportPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GncPluginQifImport, gnc_plugin_qif_import, GNC_TYPE_PLUGIN)

#define GNC_PLUGIN_QIF_IMPORT_GET_PRIVATE(o)  \
   ((GncPluginQifImportPrivate*)gnc_plugin_qif_import_get_instance_private((GncPluginQifImport*)o))

static GObjectClass *parent_class = NULL;

GncPlugin *
gnc_plugin_qif_import_new (void)
{
    return GNC_PLUGIN (g_object_new (GNC_TYPE_PLUGIN_QIF_IMPORT, NULL));
}

static void
gnc_plugin_qif_import_class_init (GncPluginQifImportClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    GncPluginClass *plugin_class = GNC_PLUGIN_CLASS (klass);

    parent_class = g_type_class_peek_parent (klass);

    object_class->finalize = gnc_plugin_qif_import_finalize;

    /* plugin info */
    plugin_class->plugin_name  = GNC_PLUGIN_QIF_IMPORT_NAME;

    /* widget addition/removal */
    plugin_class->actions_name    = PLUGIN_ACTIONS_NAME;
    plugin_class->actions         = gnc_plugin_actions;
    plugin_class->n_actions       = gnc_plugin_n_actions;
    plugin_class->ui_filename     = PLUGIN_UI_FILENAME;
    plugin_class->ui_updates      = gnc_plugin_load_ui_items;
}

static void
gnc_plugin_qif_import_init (GncPluginQifImport *plugin)
{
}

static void
gnc_plugin_qif_import_finalize (GObject *object)
{
    g_return_if_fail (GNC_IS_PLUGIN_QIF_IMPORT (object));

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

/************************************************************
 *              Plugin Function Implementation              *
 ************************************************************/

/************************************************************
 *                    Command Callbacks                     *
 ************************************************************/

static void
gnc_plugin_qif_import_cmd_new_qif_import (GSimpleAction *simple,
                                          GVariant      *parameter,
                                          gpointer       user_data)
{
    gnc_file_qif_import();
}


/************************************************************
 *                    Plugin Bootstrapping                   *
 ************************************************************/

void
gnc_plugin_qif_import_create_plugin (void)
{
    GncPlugin *plugin = gnc_plugin_qif_import_new ();
    gnc_plugin_manager_add_plugin (gnc_plugin_manager_get (), plugin);

    gnc_new_user_dialog_register_qif_assistant
        ((void (*)())gnc_file_qif_import);

    scm_c_use_module("gnucash qif-import");

    /* Add to preferences under Online Banking */
    /* The parameters are; glade file, items to add from glade file - last being the dialog, preference tab name */
    gnc_preferences_add_to_page ("dialog-account-picker.glade", "prefs_table",
                                 _("Import"));
}
