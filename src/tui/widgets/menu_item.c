/**************************************************************************
 **
 ** sngrep - SIP Messages flow viewer
 **
 ** Copyright (C) 2013-2019 Ivan Alonso (Kaian)
 ** Copyright (C) 2013-2019 Irontec SL. All rights reserved.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/
/**
 * @file menu_item.c
 * @author Ivan Alonso [aka Kaian] <kaian@irontec.com>
 *
 * @brief Source of functions defined in menu.h
 */

#include "config.h"
#include "glib-extra/glib.h"
#include "tui/widgets/menu_item.h"

enum
{
    SIG_ACTIVATE,
    SIGS
};

enum
{
    PROP_TEXT = 1,
    N_PROPERTIES
};

static guint signals[SIGS] = { 0 };

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

// Menu item class definition
G_DEFINE_TYPE(SngMenuItem, sng_menu_item, SNG_TYPE_WIDGET)

void
sng_menu_item_set_action(SngMenuItem *item, KeybindingAction action)
{
    item->action = action;
}

void
sng_menu_item_activate(SngMenuItem *item)
{
    g_signal_emit(SNG_WIDGET(item), signals[SIG_ACTIVATE], 0);
}

SngWidget *
sng_menu_item_new(const gchar *text)
{
    return g_object_new(
        SNG_TYPE_MENU_ITEM,
        "text", text,
        "can-focus", FALSE,
        NULL
    );
}

void
sng_menu_item_free(SngMenuItem *item)
{
    g_object_unref(item);
}

static void
sng_menu_item_set_property(GObject *self, guint property_id, const GValue *value, GParamSpec *pspec)
{
    SngMenuItem *item = SNG_MENU_ITEM(self);

    switch (property_id) {
        case PROP_TEXT:
            item->text = g_strdup(g_value_get_string(value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, property_id, pspec);
            break;
    }
}

static void
sng_menu_item_get_property(GObject *self, guint property_id, GValue *value, GParamSpec *pspec)
{
    SngMenuItem *item = SNG_MENU_ITEM(self);
    switch (property_id) {
        case PROP_TEXT:
            g_value_set_string(value, item->text);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, property_id, pspec);
            break;
    }
}

static void
sng_menu_item_init(G_GNUC_UNUSED SngMenuItem *self)
{
}

static void
sng_menu_item_class_init(SngMenuItemClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->set_property = sng_menu_item_set_property;
    object_class->get_property = sng_menu_item_get_property;

    obj_properties[PROP_TEXT] =
        g_param_spec_string("text",
                            "Menu item text",
                            "Menu item text",
                            "Untitled menu item",
                            G_PARAM_READWRITE
        );

    g_object_class_install_properties(
        object_class,
        N_PROPERTIES,
        obj_properties
    );

    signals[SIG_ACTIVATE] =
        g_signal_newv("activate",
                      G_TYPE_FROM_CLASS(klass),
                      G_SIGNAL_RUN_LAST,
                      NULL,
                      NULL, NULL,
                      NULL,
                      G_TYPE_NONE, 0, NULL
        );
}