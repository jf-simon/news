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
			ci_fontsize = list_data->fontsize;
			ci_indicator = list_data->indicator;
			ci_x_value = list_data->x_value;
			ci_y_value = list_data->y_value;
			ci_r = list_data->r;
			ci_g = list_data->g;
			ci_b = list_data->b;
			ci_a = list_data->a;
			found = 1;
		}
   }

   if(found == 0)
	{
		ci_url = eina_stringshare_add("https://github.com/jf-simon/news/commits/master.atom");
		ci_icon = eina_stringshare_add("");
		ci_icons = 0;
		ci_bigicons = 0;
		ci_popupnew = 0;
		ci_popupnew = 0;
		ci_refresh = 10;
		ci_fontsize = 10;
		ci_x_value = 480;
		ci_y_value = 600;
		ci_r = 11;
		ci_g = 54;
		ci_b = 71;
		ci_a = 255;
	}
	
	printf("COLOR LOAD: %i %i %i %i\n", ci_r, ci_g, ci_b, ci_a);
	printf("LOAD FOUND: %i\n", found);
	printf("LOAD FONTSIZE: %f\n", ci_fontsize);
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
	
	Evas_Object *tb = data;	
	
	if(data != NULL)
	{
   Evas_Object *en_url = evas_object_data_get(tb, "en_url");
   Evas_Object *en_icon = evas_object_data_get(tb, "en_icon");
   Evas_Object *check_icons = evas_object_data_get(tb, "check_icons");
   Evas_Object *check_bigicons = evas_object_data_get(tb, "check_bigicons");
   Evas_Object *check_indicator = evas_object_data_get(tb, "check_indicator");
   Evas_Object *check_popupnew = evas_object_data_get(tb, "check_popupnew");
   Evas_Object *sl_refresh = evas_object_data_get(tb, "sl_refresh");
   Evas_Object *sl_fontsize = evas_object_data_get(tb, "sl_fontsize");
   Evas_Object *sl_x_value = evas_object_data_get(tb, "sl_x_value");
   Evas_Object *sl_y_value = evas_object_data_get(tb, "sl_y_value");
   Evas_Object *cs = evas_object_data_get(tb, "cs");
	
	

   elm_colorselector_color_get(cs, &ci_r, &ci_g, &ci_b, &ci_a);
	printf("COLOR SAVE: %i %i %i %i\n", ci_r, ci_g, ci_b, ci_a);
	
	
		printf("en: %s\n", elm_object_text_get(en_icon));
		printf("ci; %s\n", ci_icon);
	
	if(strcmp(ci_icon, elm_object_text_get(en_icon)) != 0)
	{
		_set_feed_icon();
		printf("icon changed\n");
	}	
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
   ci_indicator = elm_check_state_get(check_indicator);
	ci_refresh = elm_slider_value_get(sl_refresh);
	ci_fontsize = elm_slider_value_get(sl_fontsize);
	ci_x_value = elm_slider_value_get(sl_x_value);
	ci_y_value = elm_slider_value_get(sl_y_value);
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
			list_data->indicator = ci_indicator;
			list_data->popupnew = ci_popupnew;
			list_data->refresh = ci_refresh;
			list_data->fontsize = ci_fontsize;
			list_data->x_value = ci_x_value;
			list_data->y_value = ci_y_value;
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
		list_data1->fontsize = ci_fontsize;
		list_data1->x_value = ci_x_value;
		list_data1->y_value = ci_y_value;
		list_data1->indicator = ci_indicator;
		list_data1->r = ci_r;
		list_data1->g = ci_g;
		list_data1->b = ci_b;
		list_data1->a = ci_a;

		configlist = eina_list_append(configlist, list_data1);
	}

	printf("SAVE FOUND: %0.lf\n", ci_fontsize);
_save_eet();

_timer_reset();
_set_feed_icon();
//  refresh feed if url has changed
// _get_data();
}


unsigned int createHEX(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}


static void
_colorselector_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *tb = data;
   Evas_Object *ic = evas_object_data_get(tb, "ic");
   Evas_Object *ly = evas_object_data_get(tb, "ly");

   elm_colorselector_color_get(obj, &ci_r, &ci_g, &ci_b, &ci_a);
	
// 	evas_object_color_set(ic, ci_r, ci_g, ci_b, ci_a);
	
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
/*
void
_config_save1(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	_config_save(data, NULL, NULL, NULL);	
}*/

static void
_config_show_feed(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Evas_Object *tb = data;
	
   Evas_Object *popup_frame = evas_object_data_get(tb, "popup_frame");
	Evas_Object *feed_frame = evas_object_data_get(tb, "feed_frame");
	Evas_Object *advanced_frame = evas_object_data_get(tb, "advanced_frame");

//    evas_object_hide();
   evas_object_hide(popup_frame);
   evas_object_hide(advanced_frame);
   evas_object_show(feed_frame);
	printf("SHOW FEED\n");
}


static void
_config_show_popup(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Evas_Object *tb = data;
	
   Evas_Object *popup_frame = evas_object_data_get(tb, "popup_frame");
	Evas_Object *feed_frame = evas_object_data_get(tb, "feed_frame");
	Evas_Object *advanced_frame = evas_object_data_get(tb, "advanced_frame");
//    evas_object_hide();
   evas_object_hide(feed_frame);
   evas_object_hide(advanced_frame);
   evas_object_show(popup_frame);
	printf("SHOW POPUP\n");
}


static void
_config_show_advanced(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Evas_Object *tb = data;
	
   Evas_Object *popup_frame = evas_object_data_get(tb, "popup_frame");
	Evas_Object *feed_frame = evas_object_data_get(tb, "feed_frame");
	Evas_Object *advanced_frame = evas_object_data_get(tb, "advanced_frame");
	
	
//    evas_object_hide();
   evas_object_hide(feed_frame);
   evas_object_hide(popup_frame);
   evas_object_show(advanced_frame);
	
	printf("SHOW ADVANCED\n");
}

static double
_step_size_calculate(double min, double max)
{
   double step = 0.0;
   int steps = 0;

   steps = max - min;
   if (steps) step = (10.0 / steps);
   return step;
}

static double
_step_size_calculate_1(double min, double max)
{
   double step = 0.0;
   int steps = 0;

   steps = max - min;
   if (steps) step = (1.0 / steps);
   return step;
}


static void
completion_cb(void *data, const char *file, int status)
{
	Evas_Object *en_icon = data;
	char buf[4096];
	if(status != 200)
	{
		ecore_file_unlink(file);
		// Fehlermeldung als Popup
	}else
	{
		elm_object_text_set(en_icon, file);
		_set_feed_icon();
	}
	
	printf("CODE %i\n", status);
	
	
	// dateiname dann nehmen um die neute datei damit anlegen und link dann zurück in en_icon
}

static void
_download_image_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_icon = data;
	Eina_Bool *rt;
	const char *icon_url = elm_object_text_get(en_icon);
	char buf[4096], buf1[4096], buf2[4096], cache_dir[4096];
	const char *filename;

	snprintf(cache_dir, sizeof(cache_dir), "%s/news/cache/%i/", efreet_config_home_get(), id_num);

	if(ecore_file_download_protocol_available(icon_url))
	{
		ecore_file_recursive_rm(cache_dir);
		if(!ecore_file_is_dir(cache_dir))
		{
// 		snprintf(buf2, sizeof(buf2), "%s/news/cache/%i", efreet_config_home_get(), id_num);
			ecore_file_mkpath(cache_dir);
			printf("MKDIR: %s\n", cache_dir);
		}
		
		filename = ecore_file_file_get(icon_url);
	
		snprintf(buf1, sizeof(buf1), "%s/news/cache/%i/%s", efreet_config_home_get(), id_num, filename);
			
		// Button disable bis completion_cb fertig
		
		rt = ecore_file_download(icon_url, buf1, completion_cb, NULL, en_icon, NULL);

	}
	else
	{
		_set_feed_icon();
	}
}


void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_url, *button, *en_keyword, *en_icon, *popup, *fr, *cs, *mainfr, *tb, *list, *it, *popup_frame, *tb_feed, *tb_popup, *tb_sizes, *tb_advanced, *feed_frame, *advanced_frame, *advanced_popup;
   Evas_Object *o, *box_settings, *box_url, *box_icon, *lbl, *check_icons, *check_bigicons, *check_popupnew, *check_popupkeyword, *check_indicator, *sl_refresh, *sl_fontsize, *sl_x_value, *sl_y_value;
	
	Evas_Object *ly = obj;
	Evas_Object *win = data;
	
	
	char buf[PATH_MAX];
	
	double step;
   popup = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup, 1);
	
	tb = elm_table_add(popup);
	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(tb);
	
	lbl = elm_label_add(tb);
   evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_style_set(lbl, "marker");
	snprintf(buf, sizeof(buf), "News Configuration <br> [%s]", feedname);
   elm_object_text_set(lbl, buf);
   elm_table_pack(tb, lbl, 0, 0, 2, 1);
   evas_object_show(lbl);
	
	list = elm_list_add(tb);
	evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, list, 0, 1, 1, 1);
   elm_list_select_mode_set(list, ELM_OBJECT_SELECT_MODE_ALWAYS);
   elm_scroller_content_min_limit(list, 1, 1);
   it = elm_list_item_append(list, "Feed", NULL, NULL,
                             _config_show_feed, tb);
   elm_list_item_selected_set(it, 1);
   it = elm_list_item_append(list, "Popup", NULL, NULL,
                             _config_show_popup, tb);
   it = elm_list_item_append(list, "Advanced", NULL, NULL,
                             _config_show_advanced, tb);
   elm_list_go(list);
   evas_object_show(list);
	
	
/// FEED FRAME ///
	feed_frame = elm_frame_add(popup);
   elm_object_text_set(feed_frame, "Feed");
	evas_object_size_hint_weight_set(feed_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(feed_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, feed_frame, 1, 1, 1, 1);
	
			tb_feed = elm_table_add(popup);
			evas_object_size_hint_weight_set(tb_feed, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(tb_feed, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_show(tb_feed);
			
			lbl = elm_label_add(popup);
			elm_object_text_set(lbl, "URL: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0.5);
			elm_table_pack(tb_feed, lbl, 0, 0, 1, 1);
			evas_object_show(lbl);
			
			en_url = elm_entry_add(popup);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_url, ci_url);
			elm_object_part_text_set(en_url, "elm.guide", "Enter Feed URL");
			elm_entry_editable_set(en_url, EINA_TRUE);
			elm_entry_single_line_set(en_url, EINA_TRUE);
			evas_object_size_hint_weight_set(en_url, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_url, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_table_pack(tb_feed, en_url, 1, 0, 2, 1);
			evas_object_show(en_url);
			evas_object_data_set(tb, "en_url", en_url);
	
			lbl = elm_label_add(popup);
			elm_object_text_set(lbl, "Image: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0.5);
			elm_object_tooltip_text_set(lbl, "local location or online link [http://]<br>[blank for standard]");
			elm_table_pack(tb_feed, lbl, 0, 1, 1, 1);
			evas_object_show(lbl);
						
			en_icon = elm_entry_add(popup);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_icon, ci_icon);
			elm_object_part_text_set(en_icon, "elm.guide", "Enter Feed Image - local or [http://]");
			elm_entry_editable_set(en_icon, EINA_TRUE);
			elm_entry_single_line_set(en_icon, EINA_TRUE);
			evas_object_size_hint_weight_set(en_icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_icon, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_object_tooltip_text_set(en_icon, "local location or online link [http://]<br>[blank for standard]");
			elm_table_pack(tb_feed, en_icon, 1, 1, 2, 1);
			evas_object_show(en_icon);
			evas_object_data_set(tb, "en_icon", en_icon);
			
			button = elm_button_add(popup);
			elm_object_text_set(button, "download/set");
			evas_object_size_hint_weight_set(button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(button, 0, 0);
			elm_object_tooltip_text_set(button, "download to local");
			elm_table_pack(tb_feed, button, 3, 1, 1, 1);
			evas_object_show(button);
			
			lbl = elm_label_add(popup);
			elm_object_text_set(lbl, "Refresh Intervall: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_feed, lbl, 0, 2, 1, 1);
			evas_object_show(lbl);

			sl_refresh = elm_slider_add(popup);
			evas_object_size_hint_weight_set(sl_refresh, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(sl_refresh, EVAS_HINT_FILL, 0);
			elm_slider_unit_format_set(sl_refresh, "%1.0f min");
			elm_slider_indicator_format_set(sl_refresh, "%1.0f");
			elm_slider_min_max_set(sl_refresh, 1, 60);
			elm_slider_value_set(sl_refresh, ci_refresh);
			step = _step_size_calculate_1(1, 60);
			elm_slider_step_set(sl_refresh, step );
			elm_table_pack(tb_feed, sl_refresh, 1, 2, 2, 1);
			evas_object_show(sl_refresh);
			evas_object_data_set(tb, "sl_refresh", sl_refresh);

   elm_object_content_set(feed_frame, tb_feed);
	evas_object_data_set(tb, "feed_frame", feed_frame);
/// FEED FRAME END ///

/// POPUP FRAME ///
	popup_frame = elm_frame_add(popup);
   elm_object_text_set(popup_frame, "Popup");
	evas_object_size_hint_weight_set(popup_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(popup_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, popup_frame, 1, 1, 1, 1);
	
			tb_popup = elm_table_add(popup);
			evas_object_size_hint_weight_set(tb_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(tb_popup, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_show(tb_popup);
			

			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Images: ");
			elm_table_pack(tb_popup, lbl, 0, 0, 2, 1);
			evas_object_show(lbl);
			
			check_icons = elm_check_add(popup);
			evas_object_size_hint_weight_set(check_icons, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(check_icons, 0, 0);
			elm_object_text_set(check_icons, "Don't show images");
			elm_check_state_set(check_icons, ci_icons);
			elm_table_pack(tb_popup, check_icons, 0, 1, 2, 1);
			evas_object_show(check_icons);
			evas_object_data_set(tb, "check_icons", check_icons);

			o = elm_separator_add(popup);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_popup, o, 0, 2, 2, 1);
			evas_object_show(o);

			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Handling on news updates: ");
			elm_table_pack(tb_popup, lbl, 0, 3, 2, 1);
			evas_object_show(lbl);
			
			check_popupnew = elm_check_add(popup);
			evas_object_size_hint_weight_set(check_popupnew, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(check_popupnew, 0, 0);
		// 	elm_object_disabled_set(check_popupnew, 1);
			elm_object_text_set(check_popupnew, "Popup on News");
			elm_check_state_set(check_popupnew, ci_popupnew);
			elm_table_pack(tb_popup, check_popupnew, 0, 4, 2, 1);
			evas_object_show(check_popupnew);
			evas_object_data_set(tb, "check_popupnew", check_popupnew);
			
			o = elm_separator_add(popup);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_popup, o, 0, 5, 2, 1);
			evas_object_show(o);
			
			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Sizes: ");
			elm_table_pack(tb_popup, lbl, 0, 6, 2, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Fontsize: ");
			elm_table_pack(tb_popup, lbl, 0, 7, 1, 1);
			evas_object_show(lbl);
			
			sl_fontsize = elm_slider_add(popup);
			evas_object_size_hint_weight_set(sl_fontsize, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(sl_fontsize, EVAS_HINT_FILL, 0);
			elm_slider_unit_format_set(sl_fontsize, "%1.0f px");
			elm_slider_indicator_format_set(sl_fontsize, "%1.0f");
			elm_slider_min_max_set(sl_fontsize, 1, 60);
			elm_slider_value_set(sl_fontsize, ci_fontsize);
			step = _step_size_calculate_1(1, 60);
			elm_slider_step_set(sl_fontsize, step);
			elm_table_pack(tb_popup, sl_fontsize, 1, 7, 1, 1);
			evas_object_show(sl_fontsize);
			evas_object_data_set(tb, "sl_fontsize", sl_fontsize);
			
			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Popupwidth: ");
			elm_table_pack(tb_popup, lbl, 0, 8, 1, 1);
			evas_object_show(lbl);
						
			sl_x_value = elm_slider_add(popup);
			evas_object_size_hint_weight_set(sl_x_value, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(sl_x_value, EVAS_HINT_FILL, 0);
			elm_slider_unit_format_set(sl_x_value, "%1.0f px");
			elm_slider_indicator_format_set(sl_x_value, "%1.0f");
			elm_slider_min_max_set(sl_x_value, 100, 1000);
			elm_slider_value_set(sl_x_value, ci_x_value);
			step = _step_size_calculate(100, 1000);
			elm_slider_step_set(sl_x_value, step);
			elm_table_pack(tb_popup, sl_x_value, 1, 8, 1, 1);
			evas_object_show(sl_x_value);
			evas_object_data_set(tb, "sl_x_value", sl_x_value);
			
			
			lbl = elm_label_add(popup);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Popuphight: ");
			elm_table_pack(tb_popup, lbl, 0, 9, 1, 1);
			evas_object_show(lbl);
			
			sl_y_value = elm_slider_add(popup);
			evas_object_size_hint_weight_set(sl_y_value, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(sl_y_value, EVAS_HINT_FILL, 0);
			elm_slider_unit_format_set(sl_y_value, "%1.0f px");
			elm_slider_indicator_format_set(sl_y_value, "%1.0f");
			elm_slider_min_max_set(sl_y_value, 100, 1000);
			elm_slider_value_set(sl_y_value, ci_y_value);
			step = _step_size_calculate(100, 1000);
			elm_slider_step_set(sl_y_value, step);
			elm_table_pack(tb_popup, sl_y_value, 1, 9, 1, 1);
			evas_object_show(sl_y_value);
			evas_object_data_set(tb, "sl_y_value", sl_y_value);

   elm_object_content_set(popup_frame, tb_popup);
	evas_object_data_set(tb, "popup_frame", popup_frame);
/// POPUP FRAME END ///
	
/// ADVANCED FRAME ///
	advanced_frame = elm_frame_add(popup);
   elm_object_text_set(advanced_frame, "Advanced");
	evas_object_size_hint_weight_set(advanced_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(advanced_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, advanced_frame, 1, 1, 1, 1);
	
			tb_advanced = elm_table_add(popup);
			evas_object_show(tb_advanced);

			check_indicator = elm_check_add(popup);
			elm_object_text_set(check_indicator, "don't show update indicator [white bottom bar]");
			elm_check_state_set(check_indicator, ci_indicator);
			evas_object_size_hint_weight_set(check_indicator, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(check_indicator, 0, 0);
			elm_table_pack(tb_advanced, check_indicator, 0, 0, 1, 1);
			evas_object_show(check_indicator);
			evas_object_data_set(tb, "check_indicator", check_indicator);	
			
			
			o = elm_separator_add(popup);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_advanced, o, 0, 1, 1, 1);
			evas_object_show(o);
			
			lbl = elm_label_add(popup);
			elm_object_text_set(lbl, "Set background color for standard feed image if no custom is set: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_advanced, lbl, 0, 2, 1, 1);
			evas_object_show(lbl);

			cs = elm_colorselector_add(popup);
			elm_colorselector_color_set(cs, ci_r, ci_g, ci_b, ci_a);
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
			elm_table_pack(tb_advanced, cs, 0, 3, 1, 1);
			evas_object_show(cs);
			evas_object_data_set(tb, "cs", cs);
	
   elm_object_content_set(advanced_frame, tb_advanced );
	evas_object_data_set(tb, "advanced_frame", advanced_frame);
/// ADVANCED FRAME END ///
	
	  evas_object_data_set(tb, "win", win);
   evas_object_data_set(tb, "ly", ly);
	
   elm_object_content_set(popup, tb);
	_config_show_feed(tb, NULL, NULL);
	
	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, tb);
	
// 	evas_object_smart_callback_add(button, "clicked", _config_save, tb); 
	evas_object_smart_callback_add(button, "clicked", _download_image_cb, en_icon); 
	evas_object_event_callback_add(en_icon, EVAS_CALLBACK_MOUSE_OUT, _config_save, tb);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_MOUSE_OUT, _config_save, tb);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_HIDE, _popup_del, NULL);
   evas_object_show(popup);
}

