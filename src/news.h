#ifndef NEWS_H
#define NEWS_H

#endif
#include "config.h"
#include <Ecore_Con.h>
#include <Elementary.h>
#include <e_gadget_types.h>

#define E_WEIGHT evas_object_size_hint_weight_set
#define E_ALIGN  evas_object_size_hint_align_set
#define E_EXPAND(X) E_WEIGHT((X), EVAS_HINT_EXPAND, EVAS_HINT_EXPAND)
#define E_FILL(X)   E_ALIGN((X), EVAS_HINT_FILL, EVAS_HINT_FILL)

void           _settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void           _settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
void           _settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void           _sl_font_changed(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);
void           _set_text(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
int            _xml_parse(void *data);
void           set_color(Evas_Object *ly);
void           _read_eet();
void           _save_eet();
unsigned int   createHEX(int r, int g, int b, int a);
void           _config_load();
void           _config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED);
void           _set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED);
void           _set_feed_settings();
void           _get_data();
void           _timer_reset();
void           stringReplace(char *search, char *replace, char *string);
void           _it_clicked(void *data, Evas_Object *obj, void *event_info EINA_UNUSED);

extern int id_num;

extern const char *feedname;

extern Evas_Object *win;
extern Evas_Object *ly;
extern Evas_Object *tb;
extern Evas_Object *popup;
extern Evas_Object *popup_settings;

extern const char *ci_url;
extern const char *ci_icon;
extern const char *saved_title;
extern const char *ci_title;
extern const char *ci_keywords;
extern Eina_Bool   ci_icons;
extern Eina_Bool   ci_popupnew;
extern Eina_Bool   ci_popupkeywords;
extern Eina_Bool   ci_checkkeywords;
extern Eina_Bool   ci_indicator;
extern double   	ci_refresh;
extern double   	ci_fontsize;
extern double   	ci_x_value;
extern double   	ci_y_value;
extern int         ci_r;
extern int         ci_g;
extern int         ci_b;
extern int         ci_a;

extern Eina_List *configlist;
extern Eina_List *feed_data_list;
extern Eina_List *keywords_data_list;
extern Eina_List *feed_data_list_tmp;


typedef struct {
        const char 	*url;
        const char 	*icon;
        const char 	*title;
		  const char   *keywords;
		  Eina_Bool   	icons;
		  Eina_Bool   	popupnew;
		  Eina_Bool   	popupkeywords;
		  Eina_Bool   	checkkeywords;
		  Eina_Bool   	indicator;
		  double   	  	refresh;
		  double   	  	fontsize;
		  double   		x_value;
	     double   		y_value;
		  int         	r;
	     int         	g;
        int         	b;
        int         	a;
		  const char   *saved_title;
} Config_Item;
