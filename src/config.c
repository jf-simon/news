#include "news.h"

void
_config_load(void *data)
{
   int found = 0;
	Eina_List *l;
	Config_Item *list_data;

   printf("LIST COUNT %i\n", eina_list_count(configlist));

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			ci_url = eina_stringshare_add(list_data->url);
			ci_icon = eina_stringshare_add(list_data->icon);
			saved_title = eina_stringshare_add(list_data->saved_title);
			ci_icons = list_data->icons;
			ci_bigicons = list_data->bigicons;
			ci_popupnew = list_data->popupnew;
			ci_refresh = list_data->refresh;
			ci_r = list_data->r;
			ci_g = list_data->g;
			ci_b = list_data->b;
			ci_a = list_data->a;
			found = 1;
		}
   }

   if(found == 0)
	{
		ci_url = eina_stringshare_add("http://www.tagesschau.de/xml/rss2");
		ci_icon = eina_stringshare_add("");
		ci_icons = 0;
		ci_bigicons = 0;
		ci_popupnew = 0;
		ci_refresh = 10;
		ci_r = 11;
		ci_g = 54;
		ci_b = 71;
		ci_a = 255;
	}
	
	printf("LOAD FOUND: %i\n", found);
}


void
_config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Eina_List *l;
	Config_Item *list_data;
	Config_Item *list_data1;
	list_data1 = calloc(1, sizeof(Config_Item));
	int found = 0;
	
	printf("SAVE\n");
	
	Evas_Object *mainbox = data;
//    Evas_Object *ly = evas_object_data_get(mainbox, "ly");
	
	
	
	if(data != NULL)
	{
   Evas_Object *en_url = evas_object_data_get(mainbox, "en_url");
   Evas_Object *en_icon = evas_object_data_get(mainbox, "en_icon");
   Evas_Object *check_icons = evas_object_data_get(mainbox, "check_icons");
   Evas_Object *check_bigicons = evas_object_data_get(mainbox, "check_bigicons");
   Evas_Object *check_popupnew = evas_object_data_get(mainbox, "check_popupnew");
   Evas_Object *sl_refresh = evas_object_data_get(mainbox, "sl_refresh");
	
// 	if(strcmp(ci_icon, elm_object_text_get(en_icon)) != 0)
		_set_feed_icon();
		
	if(strcmp(ci_url, elm_object_text_get(en_url)) != 0)
	{
		printf("changed\n");
		 _get_data();
	}
	ci_url = elm_object_text_get(en_url);
	ci_icon = elm_object_text_get(en_icon);
   ci_icons = elm_check_state_get(check_icons);
   ci_bigicons = elm_check_state_get(check_bigicons);
   ci_popupnew = elm_check_state_get(check_popupnew);
	ci_refresh = elm_slider_value_get(sl_refresh);
	}
	
   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
			list_data->url = ci_url;
			list_data->icon = ci_icon;
			list_data->icons = ci_icons;
			list_data->saved_title = saved_title;
			list_data->bigicons = ci_bigicons;
			list_data->popupnew = ci_popupnew;
			list_data->refresh = ci_refresh;
			list_data->r = ci_r;
			list_data->g = ci_g;
			list_data->b = ci_b;
			list_data->a = ci_a;
			found = 1;
			}
   }
		  
	if(found == 0)
	{
		list_data1->id = id_num;
		list_data1->url = ci_url;
		list_data1->icon = ci_icon;
		list_data1->icons = ci_icons;
		list_data1->bigicons = ci_bigicons;
		list_data1->popupnew = ci_popupnew;
		list_data1->refresh = ci_refresh;
		list_data1->r = ci_r;
		list_data1->g = ci_g;
		list_data1->b = ci_b;
		list_data1->a = ci_a;

		configlist = eina_list_append(configlist, list_data1);
	}

	printf("SAVE FOUND: %0.2lf\n", ci_refresh);
_save_eet();

_timer_reset();
//  refresh feed it url has changed
// _get_data();
}


unsigned int createHEX(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}


static void
_colorselector_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *mainbox = data;
   Evas_Object *ic = evas_object_data_get(mainbox, "ic");
   Evas_Object *ly = evas_object_data_get(mainbox, "ly");

   elm_colorselector_color_get(obj, &ci_r, &ci_g, &ci_b, &ci_a);
	
	evas_object_color_set(ic, ci_r, ci_g, ci_b, ci_a);
	
	set_color(ly);
}


static void
_popup_del(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_data EINA_UNUSED)
{
   evas_object_del(obj);
}


void
_settings_1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	Evas_Object *win = obj;
	Evas_Object *ly = data;
	printf("SETTINGS1\n");
	_settings(win, ly, NULL);
}


void
_settings_2(void *data, Evas_Object *obj, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *win = data;
	Evas_Object *ly = obj;
	
	printf("SETTINGS2\n");
	_settings(win, ly, NULL);
}

void
_config_save1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	_config_save(data, NULL, NULL, NULL);	
}


void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_url, *en_keyword, *en_icon, *popup, *fr, *cs;
   Evas_Object *o, *box_settings, *box_url, *box_icon, *lbl, *check_icons, *check_bigicons, *check_popupnew, *check_popupkeyword, *sl_refresh;
	
	Evas_Object *ly = obj;
	Evas_Object *win = data;
		  
   popup = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup, 1);
	
   mainbox = elm_box_add(popup);
	
	
   elm_box_horizontal_set(mainbox, EINA_FALSE);
   E_EXPAND(mainbox);
   E_ALIGN(mainbox, 0.0, 0.0);
   evas_object_show(mainbox);
	
	
   fr = elm_frame_add(mainbox);
   elm_object_text_set(fr, "News Settings");
   E_EXPAND(fr);
   E_ALIGN(fr, 0.5, 0.5);
   E_FILL(fr);
   evas_object_show(fr);

   box_settings = elm_box_add(fr);
   elm_box_horizontal_set(box_settings, EINA_FALSE);
   E_EXPAND(box_settings);
   evas_object_show(box_settings);

	
		   box_url = elm_box_add(box_settings);
			elm_box_horizontal_set(box_url, EINA_TRUE);
			E_EXPAND(box_url);
			E_ALIGN(box_url, 0.0, 0.5);
			evas_object_show(box_url);
			
			lbl = elm_label_add(box_url);
			elm_object_text_set(lbl, "Feed URL: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_url, lbl);
			evas_object_show(lbl);
			
			en_url = elm_entry_add(box_url);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_url, ci_url);
			elm_entry_editable_set(en_url, EINA_TRUE);
			elm_entry_single_line_set(en_url, EINA_TRUE);
			evas_object_size_hint_weight_set(en_url, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_url, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_url, en_url);
			evas_object_show(en_url);
			evas_object_data_set(mainbox, "en_url", en_url);
			
			elm_box_pack_end(box_settings, box_url);
	


   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
			box_icon = elm_box_add(box_settings);
			elm_box_horizontal_set(box_icon, EINA_TRUE);
			E_EXPAND(box_icon);
			E_ALIGN(box_icon, 0.0, 0.5);
			evas_object_show(box_icon);
			
			lbl = elm_label_add(box_icon);
			elm_object_text_set(lbl, "Feed Image <small>[blank for standard]</small>: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_icon, lbl);
			evas_object_show(lbl);
			
			en_icon = elm_entry_add(box_icon);
// 			elm_object_disabled_set(en_icon, 1);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_icon, ci_icon);
			elm_entry_editable_set(en_icon, EINA_TRUE);
			elm_entry_single_line_set(en_icon, EINA_TRUE);
			evas_object_size_hint_weight_set(en_icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_icon, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_box_pack_end(box_icon, en_icon);
			evas_object_show(en_icon);
			evas_object_data_set(mainbox, "en_icon", en_icon);
			
			elm_box_pack_end(box_settings, box_icon);
	


   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	check_icons = elm_check_add(box_settings);
	elm_object_text_set(check_icons, "Don't show images");
   elm_check_state_set(check_icons, ci_icons);
   E_ALIGN(check_icons, 0.0, 0.0);
 	E_WEIGHT(check_icons, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_icons);
	evas_object_show(check_icons);
   evas_object_data_set(mainbox, "check_icons", check_icons);
	
	check_bigicons = elm_check_add(box_settings);
	elm_object_text_set(check_bigicons, "Don't show big images on mouse over");
   elm_check_state_set(check_bigicons, ci_bigicons);
   E_ALIGN(check_bigicons, 0.0, 0.0);
 	E_WEIGHT(check_bigicons, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_bigicons);
	evas_object_show(check_bigicons);
   evas_object_data_set(mainbox, "check_bigicons", check_bigicons);
	
	check_popupnew = elm_check_add(box_settings);
// 	elm_object_disabled_set(check_popupnew, 1);
	elm_object_text_set(check_popupnew, "Popup on new News");
   elm_check_state_set(check_popupnew, ci_popupnew);
   E_ALIGN(check_popupnew, 0.0, 0.0);
 	E_WEIGHT(check_popupnew, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_popupnew);
	evas_object_show(check_popupnew);
   evas_object_data_set(mainbox, "check_popupnew", check_popupnew);
	
	
   o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	/*
	lbl = elm_label_add(box_settings);
	elm_object_text_set(lbl, "Keyword handling");
	evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, lbl);
	evas_object_show(lbl);
	
	check_popupkeyword = elm_check_add(box_settings);
// 	elm_object_disabled_set(check_popupkeyword, 1);
	elm_object_text_set(check_popupkeyword, "Popup on new News");
   elm_check_state_set(check_popupkeyword, ci_popupkeyword);
   E_ALIGN(check_popupkeyword, 0.0, 0.0);
 	E_WEIGHT(check_popupkeyword, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_popupkeyword);
	evas_object_show(check_popupkeyword);
   evas_object_data_set(mainbox, "check_popupkeyword", check_popupkeyword);
	
	check_popupkeyword = elm_check_add(box_settings);
// 	elm_object_disabled_set(check_popupkeyword, 1);
	elm_object_text_set(check_popupkeyword, "Highlight Keywords");
   elm_check_state_set(check_popupkeyword, ci_popupkeyword);
   E_ALIGN(check_popupkeyword, 0.0, 0.0);
 	E_WEIGHT(check_popupkeyword, EVAS_HINT_EXPAND, 0);
	elm_box_pack_end(box_settings, check_popupkeyword);
	evas_object_show(check_popupkeyword);
   evas_object_data_set(mainbox, "check_popupkeyword", check_popupkeyword);
	
	en_keyword = elm_entry_add(box_settings);
	elm_config_context_menu_disabled_set(EINA_FALSE);
	elm_object_text_set(en_keyword, ci_popupkeyword);
	elm_entry_editable_set(en_keyword, EINA_TRUE);
	elm_entry_single_line_set(en_keyword, EINA_TRUE);
	evas_object_size_hint_weight_set(en_keyword, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(en_keyword, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box_settings, en_keyword);
	evas_object_show(en_keyword);
	evas_object_data_set(mainbox, "en_keyword", en_keyword);
	*/
   sl_refresh = elm_slider_add(box_settings);
	E_ALIGN(sl_refresh, 0.0, 0.5);
	elm_slider_unit_format_set(sl_refresh, "%1.0f min");
	elm_slider_indicator_format_set(sl_refresh, "%1.0f");
	elm_slider_value_set(sl_refresh, ci_refresh);
	elm_slider_span_size_set(sl_refresh, 120);
	elm_slider_min_max_set(sl_refresh, 1, 60);
	elm_object_text_set(sl_refresh, "Refresh Intervall: ");
	elm_slider_value_set(sl_refresh, ci_refresh);
// 				step = _step_size_calculate(0, 9);
// 				elm_slider_step_set(sl_refresh, 50.0);
   elm_box_pack_end(box_settings, sl_refresh);
   evas_object_show(sl_refresh);
	evas_object_data_set(mainbox, "sl_refresh", sl_refresh);

	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);
	
	
	cs = elm_colorselector_add(box_settings);
   evas_object_size_hint_weight_set(cs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(cs, EVAS_HINT_FILL, EVAS_HINT_FILL);
    
	elm_colorselector_mode_set(cs, ELM_COLORSELECTOR_BOTH);
	elm_colorselector_palette_name_set(cs, "news");
	elm_colorselector_palette_color_add(cs, 253, 232, 82, 255);
   elm_colorselector_palette_color_add(cs, 255, 186, 0, 255);
   elm_colorselector_palette_color_add(cs, 223, 147, 37, 255);
   elm_colorselector_palette_color_add(cs, 238, 119, 99, 255);
   elm_colorselector_palette_color_add(cs, 164, 182, 166, 255);
   elm_colorselector_palette_color_add(cs, 146, 175, 29, 255);
   elm_colorselector_palette_color_add(cs, 41, 68, 59, 255);
   elm_colorselector_palette_color_add(cs, 0, 109, 128, 255);
   elm_colorselector_palette_color_add(cs, 11, 54, 71, 255);
   elm_colorselector_palette_color_add(cs, 41, 46, 68, 255);
   elm_colorselector_palette_color_add(cs, 116, 13, 14, 255);
   elm_colorselector_palette_color_add(cs, 101, 33, 44, 255);
   elm_colorselector_palette_color_add(cs, 132, 50, 118, 255);
   elm_colorselector_palette_color_add(cs, 68, 40, 55, 255);
   elm_colorselector_palette_color_add(cs, 63, 37, 60, 255);
   elm_colorselector_palette_color_add(cs, 115, 87, 63, 255);
   elm_colorselector_palette_color_add(cs, 66, 70, 73, 255);
   elm_colorselector_palette_color_add(cs, 255, 255, 255, 255);
   elm_colorselector_palette_color_add(cs, 0, 0, 0, 255);
   elm_box_pack_end(box_settings, cs);
   evas_object_show(cs);
	evas_object_data_set(mainbox, "cs", cs);
	
	
	o = elm_separator_add(box_settings);
   elm_separator_horizontal_set(o, EINA_TRUE);
   elm_box_pack_end(box_settings, o);
   evas_object_show(o);


   elm_object_content_set(fr, box_settings);
   elm_box_pack_end(mainbox, fr);

	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, mainbox); 
	

   evas_object_data_set(mainbox, "win", win);
   evas_object_data_set(mainbox, "ly", ly);
	
   elm_object_content_set(popup, mainbox);
	
	evas_object_event_callback_add(popup, EVAS_CALLBACK_MOUSE_OUT, _config_save, mainbox);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_HIDE, _popup_del, NULL);
   evas_object_show(popup);
}

