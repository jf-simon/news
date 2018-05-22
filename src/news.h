#ifndef TAGESLOSUNG_H
#define TAGESLOSUNG_H

#endif
#include "config.h"

#include <Ecore_Con.h>
#include <Elementary.h>

#include <e_gadget_types.h>

#define E_WEIGHT evas_object_size_hint_weight_set
#define E_ALIGN  evas_object_size_hint_align_set
#define E_EXPAND(X) E_WEIGHT((X), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND)
#define E_FILL(X)   E_ALIGN((X), EVAS_HINT_FILL, EVAS_HINT_FILL)

void          _settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
void          _settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _sl_font_changed(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void          _set_text(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
int           _xml_parse(void *data);
void          set_color(Evas_Object *ly);
void          _read_eet();
void          _save_eet();
unsigned int  createHEX(int r, int g, int b, int a);
void         _config_load();
void         _config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED);
void         _set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
void 			_set_feed_icon();
void 			_get_data();
void 			_timer_reset();
void 			stringReplace(char *search, char *replace, char *string);

int        id_num;

const char *saved_title;
const char *feedname;


Evas_Object *win;
Evas_Object *ly;
Evas_Object *mainbox;

int         ci_id;
const char *ci_url;
const char *ci_icon;
Eina_Bool   ci_icons;
Eina_Bool   ci_bigicons;
Eina_Bool   ci_popupnew;
Eina_Bool   ci_indicator;
double   	ci_refresh;
double   	ci_fontsize;
double   	ci_x_value;
double   	ci_y_value;
int         ci_r;
int         ci_g;
int         ci_b;
int         ci_a;

Eina_List *configlist;
Eina_List *feed_data_list;
Eina_List *feed_data_list_tmp;

typedef struct {
        int         	id;
        const char 	*url;
        const char 	*icon;
        const char 	*saved_title;
		  Eina_Bool   	icons;
		  Eina_Bool   	bigicons;
		  Eina_Bool   	popupnew;
		  Eina_Bool   	indicator;
		  double   	  	refresh;
		  double   	  	fontsize;
		  double   		x_value;
	     double   		y_value;
		  int         	r;
	     int         	g;
        int         	b;
        int         	a;
} Config_Item;

