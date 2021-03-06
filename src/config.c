#include "news.h"

void
_config_save(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Evas_Object *en_url = evas_object_data_get(tb, "en_url");
   Evas_Object *en_icon = evas_object_data_get(tb, "en_icon");
   Evas_Object *en_title = evas_object_data_get(tb, "en_title");
   Evas_Object *en_keywords = evas_object_data_get(tb, "en_keywords");
   Evas_Object *check_icons = evas_object_data_get(tb, "check_icons");
   Evas_Object *check_indicator = evas_object_data_get(tb, "check_indicator");
   Evas_Object *check_popupnew = evas_object_data_get(tb, "check_popupnew");
   Evas_Object *check_popupkeywords = evas_object_data_get(tb, "check_popupkeywords");
   Evas_Object *check_keywords = evas_object_data_get(tb, "check_keywords");
   Evas_Object *sl_refresh = evas_object_data_get(tb, "sl_refresh");
   Evas_Object *sl_fontsize = evas_object_data_get(tb, "sl_fontsize");
   Evas_Object *sl_x_value = evas_object_data_get(tb, "sl_x_value");
   Evas_Object *sl_y_value = evas_object_data_get(tb, "sl_y_value");	

	ci_url = elm_object_text_get(en_url);
	ci_icon = elm_object_text_get(en_icon);
	ci_title = elm_object_text_get(en_title);
	ci_keywords = elm_object_text_get(en_keywords);
   ci_icons = elm_check_state_get(check_icons);
   ci_popupnew = elm_check_state_get(check_popupnew);
   ci_popupkeywords = elm_check_state_get(check_popupkeywords);
   ci_checkkeywords = elm_check_state_get(check_keywords);
   ci_indicator = elm_check_state_get(check_indicator);
	ci_refresh = elm_slider_value_get(sl_refresh);
	ci_fontsize = elm_slider_value_get(sl_fontsize);
	ci_x_value = elm_slider_value_get(sl_x_value);
	ci_y_value = elm_slider_value_get(sl_y_value);

	_save_eet();

	_timer_reset();
	_set_feed_settings();

// TODO  refresh feed if url has changed
	_get_data(); // TODO is needed for new highting keywords, but must on a better place
}

unsigned int createHEX(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}

static void
_colorselector_changed_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *tb = data;
   Evas_Object *ly = evas_object_data_get(tb, "ly");

   elm_colorselector_color_get(obj, &ci_r, &ci_g, &ci_b, &ci_a);

	set_color(ly);
}

static void
_popup_del(void *data EINA_UNUSED, Evas *e EINA_UNUSED, Evas_Object *obj, void *event_data EINA_UNUSED)
{	
	_get_data(); //TODO Tut nicht
   evas_object_del(obj);
	printf("GET DATA POPUP DEL\n");
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

static void
_config_show_feed(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Evas_Object *tb = data;
	
   Evas_Object *popup_frame = evas_object_data_get(tb, "popup_frame");
	Evas_Object *feed_frame = evas_object_data_get(tb, "feed_frame");
	Evas_Object *advanced_frame = evas_object_data_get(tb, "advanced_frame");
	Evas_Object *help_frame = evas_object_data_get(tb, "help_frame");

//    evas_object_hide();
   evas_object_hide(popup_frame);
   evas_object_hide(advanced_frame);
   evas_object_hide(help_frame);
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
	Evas_Object *help_frame = evas_object_data_get(tb, "help_frame");
//    evas_object_hide();
   evas_object_hide(feed_frame);
   evas_object_hide(advanced_frame);
   evas_object_hide(help_frame);
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
	Evas_Object *help_frame = evas_object_data_get(tb, "help_frame");

//    evas_object_hide();
   evas_object_hide(feed_frame);
   evas_object_hide(popup_frame);
   evas_object_hide(help_frame);
   evas_object_show(advanced_frame);

	printf("SHOW ADVANCED\n");
}

static void
_config_show_help(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Evas_Object *tb = data;

   Evas_Object *popup_frame = evas_object_data_get(tb, "popup_frame");
	Evas_Object *feed_frame = evas_object_data_get(tb, "feed_frame");
	Evas_Object *advanced_frame = evas_object_data_get(tb, "advanced_frame");
	Evas_Object *help_frame = evas_object_data_get(tb, "help_frame");

   evas_object_hide(feed_frame);
   evas_object_hide(popup_frame);
   evas_object_hide(advanced_frame);
	evas_object_show(help_frame);

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
	if(status != 200)
	{
		ecore_file_unlink(file);
		/*
		   Evas_Object *notify = elm_notify_add(popup);
			elm_notify_allow_events_set(notify, EINA_FALSE);
			evas_object_size_hint_weight_set(notify, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_notify_align_set(notify, 0.5, 1.0);

			elm_notify_timeout_set(notify, 3.0);
			printf("I = FALSE\n");*/
		// Fehlermeldung als Popup
	}else
	{
		elm_object_text_set(en_icon, file);
		_set_feed_settings();
	}
}

static void
_download_image_cb(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{	
	Evas_Object *en_icon = data;
	const char *icon_url = elm_object_text_get(en_icon);
	char buf1[4096], cache_dir[4096];
	const char *filename;

	snprintf(cache_dir, sizeof(cache_dir), "%s/news/cache/%i/", efreet_config_home_get(), id_num);

	if(!strcmp(icon_url, ""))
	{
		ecore_file_recursive_rm(cache_dir);
		return;
	}

	if(ecore_file_download_protocol_available(icon_url))
	{
		ecore_file_recursive_rm(cache_dir);
		
		if(!ecore_file_is_dir(cache_dir))
		{
			ecore_file_mkpath(cache_dir);
			printf("MKDIR: %s\n", cache_dir);
		}
		
		filename = ecore_file_file_get(icon_url);
	
		snprintf(buf1, sizeof(buf1), "%s/news/cache/%i/%s", efreet_config_home_get(), id_num, filename);
			
		// TODO: Button disable bis completion_cb fertig
		
		ecore_file_download(icon_url, buf1, completion_cb, NULL, en_icon, NULL);
	}
	else
	{
		_set_feed_settings();
	}
}

static void 
check_keywords_clicked(void *data, Evas_Object *obj, void *event_info)
{
	Eina_Bool state = elm_check_state_get(obj);
	if(state == EINA_TRUE)
		elm_object_disabled_set(data, EINA_FALSE);
	else
		elm_object_disabled_set(data, EINA_TRUE);
}

void
_settings(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
	if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
     }

	if(popup_settings)
     {
        evas_object_del(popup_settings);
        popup_settings = NULL;
     }
     else
     {

	char buf[PATH_MAX];
	double step;

	// Feed frame Objects
	Evas_Object *feed_frame;
	Evas_Object *tb_feed;
	Evas_Object *en_url;
	Evas_Object *en_icon;
	Evas_Object *button;
	Evas_Object *en_title;
	Evas_Object *en_keywords;
	Evas_Object *sl_refresh;

	// Popup frame Objects
	Evas_Object *popup_frame;
	Evas_Object *tb_popup;
	Evas_Object *check_icons;
	Evas_Object *check_popupnew;
	Evas_Object *check_popupkeywords;
	Evas_Object *check_keywords;
	Evas_Object *tb_sizes;
	Evas_Object *sl_fontsize;
	Evas_Object *sl_x_value;
	Evas_Object *sl_y_value;

	// Advanced frame Objects
	Evas_Object *tb_advanced;
	Evas_Object *advanced_frame;
	Evas_Object *check_indicator;
	Evas_Object *cs;

	// Help frame Objects
	Evas_Object *help_frame;
	Evas_Object *tb_help;
	Evas_Object *en_help;

	// Separator
	Evas_Object *o;

	// Lable
	Evas_Object *lbl;

	// Rect background for Entries
	Evas_Object *rect;

	// List Objects
	Elm_Object_Item *list;
	Evas_Object *it;

	Evas_Object *ly = obj;
	Evas_Object *win = data;

   popup_settings = elm_win_add(win, "win", ELM_WIN_BASIC);
   elm_win_alpha_set(popup_settings, 1);

	tb = elm_table_add(popup_settings);
	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(tb);

	lbl = elm_label_add(tb);
   evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_style_set(lbl, "marker");
	snprintf(buf, sizeof(buf), "<b>News Configuration<br>[%s]", feedname);
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

	it = elm_list_item_append(list, "Help", NULL, NULL,
                             _config_show_help, tb);
   elm_list_go(list);
   evas_object_show(list);

/// FEED FRAME ///
	feed_frame = elm_frame_add(popup_settings);
   elm_object_text_set(feed_frame, "Feed");
	evas_object_size_hint_weight_set(feed_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(feed_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, feed_frame, 1, 1, 1, 1);

			tb_feed = elm_table_add(popup_settings);
			elm_table_padding_set(tb_feed, 0, 2);
			evas_object_size_hint_weight_set(tb_feed, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(tb_feed, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_show(tb_feed);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "URL: ");
			evas_object_size_hint_weight_set(lbl, 0, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_feed, lbl, 0, 0, 1, 1);
			evas_object_show(lbl);

			rect = evas_object_rectangle_add(popup_settings);
			evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_color_set(rect, 56, 56, 53, 255);
			elm_table_pack(tb_feed, rect, 1, 0, 2, 1);
			evas_object_show(rect);

			en_url = elm_entry_add(popup_settings);
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

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Image: ");
			evas_object_size_hint_weight_set(lbl, 0, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_tooltip_text_set(lbl, "local location or online link [http://]<br>[blank for standard]");
			elm_table_pack(tb_feed, lbl, 0, 1, 1, 1);
			evas_object_show(lbl);

			rect = evas_object_rectangle_add(popup_settings);
			evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_color_set(rect, 56, 56, 53, 255);
			elm_table_pack(tb_feed, rect, 1, 1, 2, 1);
			evas_object_show(rect);

			en_icon = elm_entry_add(popup_settings);
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

			button = elm_button_add(popup_settings);
			elm_object_text_set(button, "download/set");
			evas_object_size_hint_align_set(button, 0, 0);
			elm_object_tooltip_text_set(button, "download to local");
			elm_table_pack(tb_feed, button, 3, 1, 1, 1);
			evas_object_show(button);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Title: ");
			evas_object_size_hint_weight_set(lbl, 0, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_feed, lbl, 0, 2, 1, 1);
			evas_object_show(lbl);

			rect = evas_object_rectangle_add(popup_settings);
			evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_color_set(rect, 56, 56, 53, 255);
			elm_table_pack(tb_feed, rect, 1, 2, 2, 1);
			evas_object_show(rect);

			en_title = elm_entry_add(popup_settings);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_text_set(en_title, ci_title);
			elm_object_part_text_set(en_title, "elm.guide", "Enter Title");
			elm_entry_editable_set(en_title, EINA_TRUE);
			elm_entry_single_line_set(en_title, EINA_TRUE);
			evas_object_size_hint_weight_set(en_title, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_title, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_table_pack(tb_feed, en_title, 1, 2, 2, 1);
			evas_object_show(en_title);
			evas_object_data_set(tb, "en_title", en_title);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_feed, o, 0, 3, 4, 1);
			evas_object_show(o);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Refresh Intervall: ");
			evas_object_size_hint_weight_set(lbl, 0, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_feed, lbl, 0, 4, 1, 1);
			evas_object_show(lbl);

			sl_refresh = elm_slider_add(popup_settings);
			evas_object_size_hint_weight_set(sl_refresh, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(sl_refresh, EVAS_HINT_FILL, 0);
			elm_slider_unit_format_set(sl_refresh, "%1.0f min");
			elm_slider_indicator_format_set(sl_refresh, "%1.0f");
			elm_slider_min_max_set(sl_refresh, 1, 60);
			elm_slider_value_set(sl_refresh, ci_refresh);
			step = _step_size_calculate_1(1, 60);
			elm_slider_step_set(sl_refresh, step );
			elm_table_pack(tb_feed, sl_refresh, 1, 4, 2, 1);
			evas_object_show(sl_refresh);
			evas_object_data_set(tb, "sl_refresh", sl_refresh);

   elm_object_content_set(feed_frame, tb_feed);
	evas_object_data_set(tb, "feed_frame", feed_frame);
/// FEED FRAME END ///

/// POPUP FRAME ///
	popup_frame = elm_frame_add(popup_settings);
   elm_object_text_set(popup_frame, "Popup");
	evas_object_size_hint_weight_set(popup_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(popup_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, popup_frame, 1, 1, 1, 1);

			tb_popup = elm_table_add(popup_settings);
			evas_object_size_hint_weight_set(tb_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(tb_popup, EVAS_HINT_FILL, 0);
			evas_object_show(tb_popup);

			check_icons = elm_check_add(popup_settings);
			evas_object_size_hint_weight_set(check_icons, EVAS_HINT_EXPAND, 0);
			evas_object_size_hint_align_set(check_icons, 0, 0);
			elm_object_text_set(check_icons, "Don't show images");
			elm_check_state_set(check_icons, ci_icons);
			elm_table_pack(tb_popup, check_icons, 0, 1, 2, 1);
			evas_object_show(check_icons);
			evas_object_data_set(tb, "check_icons", check_icons);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_popup, o, 0, 2, 3, 1);
			evas_object_show(o);

			check_popupnew = elm_check_add(popup_settings);
			evas_object_size_hint_weight_set(check_popupnew, EVAS_HINT_EXPAND, 0);
			evas_object_size_hint_align_set(check_popupnew, 0, 0);
			elm_object_text_set(check_popupnew, "Popup on News");
			elm_check_state_set(check_popupnew, ci_popupnew);
			elm_table_pack(tb_popup, check_popupnew, 0, 3, 2, 1);
			evas_object_show(check_popupnew);
			evas_object_data_set(tb, "check_popupnew", check_popupnew);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_popup, o, 0, 4, 3, 1);
			evas_object_show(o);

			check_popupkeywords = elm_check_add(popup_settings);
			evas_object_size_hint_weight_set(check_popupkeywords, EVAS_HINT_EXPAND, 0);
			evas_object_size_hint_align_set(check_popupkeywords, 0, 0);
			elm_object_text_set(check_popupkeywords, "Popup on Keyword");
			elm_check_state_set(check_popupkeywords, ci_popupkeywords);
			elm_table_pack(tb_popup, check_popupkeywords, 0, 5, 2, 1);
			evas_object_show(check_popupkeywords);
			evas_object_data_set(tb, "check_popupkeywords", check_popupkeywords);

			check_keywords = elm_check_add(popup_settings);
			evas_object_size_hint_weight_set(check_keywords, EVAS_HINT_EXPAND, 0);
			evas_object_size_hint_align_set(check_keywords, 0, 0);
			elm_object_text_set(check_keywords, "Highlight Keywords");
			elm_check_state_set(check_keywords, ci_checkkeywords);
			elm_table_pack(tb_popup, check_keywords, 0, 6, 1, 1);
			evas_object_show(check_keywords);
			evas_object_data_set(tb, "check_keywords", check_keywords);

			rect = evas_object_rectangle_add(popup_settings);
			evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_color_set(rect, 56, 56, 53, 255);
			elm_table_pack(tb_popup, rect, 1, 6, 2, 1);
			evas_object_show(rect);

			en_keywords = elm_entry_add(popup_settings);
			elm_config_context_menu_disabled_set(EINA_FALSE);
			elm_object_part_text_set(en_keywords, "elm.guide", "Enter Keywords");
			elm_entry_editable_set(en_keywords, EINA_TRUE);
			elm_object_text_set(en_keywords, ci_keywords);
			elm_entry_single_line_set(en_keywords, EINA_TRUE);
			evas_object_size_hint_weight_set(en_keywords, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_keywords, EVAS_HINT_FILL, EVAS_HINT_FILL);
			elm_table_pack(tb_popup, en_keywords, 1, 6, 2, 1);
			evas_object_show(en_keywords);
			evas_object_data_set(tb, "en_keywords", en_keywords);

			evas_object_smart_callback_add(check_keywords, "clicked", check_keywords_clicked, en_keywords); 

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Keywords must be ; seperated");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_popup, lbl, 1, 7, 2, 1);
			evas_object_show(lbl);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_popup, o, 0, 8, 3, 1);
			evas_object_show(o);

			lbl = elm_label_add(popup_settings);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_object_text_set(lbl, "Sizes: ");
			elm_table_pack(tb_popup, lbl, 0, 9, 2, 1);
			evas_object_show(lbl);

					tb_sizes = elm_table_add(popup_settings);
					evas_object_size_hint_weight_set(tb_sizes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					evas_object_size_hint_align_set(tb_sizes, EVAS_HINT_FILL, 0);
					elm_table_pack(tb_popup, tb_sizes, 0, 10, 3, 2);
					evas_object_show(tb_sizes);

					lbl = elm_label_add(popup_settings);
					evas_object_size_hint_align_set(lbl, 0, 0);
					elm_object_text_set(lbl, "Fontsize: ");
					elm_table_pack(tb_sizes, lbl, 0, 0, 1, 1);
					evas_object_show(lbl);

					sl_fontsize = elm_slider_add(popup_settings);
					evas_object_size_hint_weight_set(sl_fontsize, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					evas_object_size_hint_align_set(sl_fontsize, EVAS_HINT_FILL, 0);
					elm_slider_unit_format_set(sl_fontsize, "%1.0f px");
					elm_slider_indicator_format_set(sl_fontsize, "%1.0f");
					elm_slider_min_max_set(sl_fontsize, 1, 60);
					elm_slider_value_set(sl_fontsize, ci_fontsize);
					step = _step_size_calculate_1(1, 60);
					elm_slider_step_set(sl_fontsize, step);
					elm_table_pack(tb_sizes, sl_fontsize, 1, 0, 1, 1);
					evas_object_show(sl_fontsize);
					evas_object_data_set(tb, "sl_fontsize", sl_fontsize);

					lbl = elm_label_add(popup_settings);
					evas_object_size_hint_align_set(lbl, 0, 0);
					elm_object_text_set(lbl, "Popupwidth: ");
					elm_table_pack(tb_sizes, lbl, 0, 1, 1, 1);
					evas_object_show(lbl);

					sl_x_value = elm_slider_add(popup_settings);
					evas_object_size_hint_weight_set(sl_x_value, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					evas_object_size_hint_align_set(sl_x_value, EVAS_HINT_FILL, 0);
					elm_slider_unit_format_set(sl_x_value, "%1.0f px");
					elm_slider_indicator_format_set(sl_x_value, "%1.0f");
					elm_slider_min_max_set(sl_x_value, 100, 1000);
					elm_slider_value_set(sl_x_value, ci_x_value);
					step = _step_size_calculate(100, 1000);
					elm_slider_step_set(sl_x_value, step);
					elm_table_pack(tb_sizes, sl_x_value, 1, 1, 1, 1);
					evas_object_show(sl_x_value);
					evas_object_data_set(tb, "sl_x_value", sl_x_value);

					lbl = elm_label_add(popup_settings);
					evas_object_size_hint_align_set(lbl, 0, 0);
					elm_object_text_set(lbl, "Popuphight: ");
					elm_table_pack(tb_sizes, lbl, 0, 2, 1, 1);
					evas_object_show(lbl);

					sl_y_value = elm_slider_add(popup_settings);
					evas_object_size_hint_weight_set(sl_y_value, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					evas_object_size_hint_align_set(sl_y_value, EVAS_HINT_FILL, 0);
					elm_slider_unit_format_set(sl_y_value, "%1.0f px");
					elm_slider_indicator_format_set(sl_y_value, "%1.0f");
					elm_slider_min_max_set(sl_y_value, 100, 1000);
					elm_slider_value_set(sl_y_value, ci_y_value);
					step = _step_size_calculate(100, 1000);
					elm_slider_step_set(sl_y_value, step);
					elm_table_pack(tb_sizes, sl_y_value, 1, 2, 1, 1);
					evas_object_show(sl_y_value);
					evas_object_data_set(tb, "sl_y_value", sl_y_value);

   elm_object_content_set(popup_frame, tb_popup);
	evas_object_data_set(tb, "popup_frame", popup_frame);
/// POPUP FRAME END ///
	
/// ADVANCED FRAME ///
	advanced_frame = elm_frame_add(popup_settings);
   elm_object_text_set(advanced_frame, "Advanced");
	evas_object_size_hint_weight_set(advanced_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(advanced_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, advanced_frame, 1, 1, 1, 1);

			tb_advanced = elm_table_add(popup_settings);
			evas_object_show(tb_advanced);

			check_indicator = elm_check_add(popup_settings);
			elm_object_text_set(check_indicator, "don't show update indicator [white bottom bar]");
			elm_check_state_set(check_indicator, ci_indicator);
			evas_object_size_hint_weight_set(check_indicator, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(check_indicator, 0, 0);
			elm_table_pack(tb_advanced, check_indicator, 0, 0, 1, 1);
			evas_object_show(check_indicator);
			evas_object_data_set(tb, "check_indicator", check_indicator);	

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_advanced, o, 0, 1, 1, 1);
			evas_object_show(o);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Set background color for standard feed image if no custom is set: ");
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_advanced, lbl, 0, 2, 1, 1);
			evas_object_show(lbl);
   
			cs = elm_colorselector_add(popup_settings);
			evas_color_argb_premul(ci_a, &ci_r, &ci_g, &ci_b);
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
	
/// HELP FRAME ///
	help_frame = elm_frame_add(popup_settings);
   elm_object_text_set(help_frame, "Help");
	evas_object_size_hint_weight_set(help_frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(help_frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, help_frame, 1, 1, 1, 1);

			tb_help = elm_table_add(popup_settings);
			evas_object_size_hint_weight_set(tb_help, 0, 0);
			evas_object_size_hint_align_set(tb_help, 0, 0);
			evas_object_show(tb_help);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<b>Mouse Bindings:</b>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 0, 2, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Left Mouse Button: ");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 1, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "open/close popup");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 1, 1, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Middle Mouse Button:  ");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 2, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "check for new articles");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 1, 2, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "Right Mouse Button: ");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 3, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "open gadget bar menu");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 1, 3, 1, 1);
			evas_object_show(lbl);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, 0, 0);
			evas_object_size_hint_align_set(o, 0, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_help, o, 0, 5, 3, 1);
			evas_object_show(o);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<b>Color Indicator:</b>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 6, 2, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<color=#64ff00ff>Green bar: </color>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 7, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "the green bar on top indicates new articles");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 1, 7, 2, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<color=#ffffffff>White bar: </color>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 8, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "the white bar at the button indicates ongoing check for new articles");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 01, 8, 2, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<color=#ff0000ff>Red bar: </color>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 0, 9, 1, 1);
			evas_object_show(lbl);

			lbl = elm_label_add(popup_settings);
			elm_object_text_set(lbl, "<custom align=left>the red bar at the button indicates a failure while checking for new articles.<br>Please check you internet connection and feed url</custom>");
			evas_object_size_hint_weight_set(lbl, 0, 0);
			evas_object_size_hint_align_set(lbl, 0, 0);
			elm_table_pack(tb_help, lbl, 1, 9, 2, 1);
			evas_object_show(lbl);

			o = elm_separator_add(popup_settings);
			evas_object_size_hint_weight_set(o, 0, 0);
			evas_object_size_hint_align_set(o, 0, 0);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_table_pack(tb_help, o, 0, 10, 3, 1);
			evas_object_show(o);

			en_help = elm_entry_add(popup_settings);
			evas_object_size_hint_weight_set(en_help, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(en_help, 0.5, 0.5);
			elm_entry_single_line_set(en_help, EINA_TRUE);
			elm_entry_editable_set(en_help, EINA_FALSE);
			evas_object_smart_callback_add(en_help, "clicked", _it_clicked, "https://github.com/jf-simon/news/");
			elm_object_text_set(en_help, "<b>Bugs, Feedback, Help on <a href=anc-02>https://github.com/jf-simon/news/</a></b>");
			elm_table_pack(tb_help, en_help, 0, 11, 3, 1);
			evas_object_show(en_help);

   elm_object_content_set(help_frame, tb_help );
	evas_object_data_set(tb, "help_frame", help_frame);
/// HELP FRAME END///

	evas_object_data_set(tb, "win", win);
   evas_object_data_set(tb, "ly", ly);

   elm_object_content_set(popup_settings, tb);
	_config_show_feed(tb, NULL, NULL);

	evas_object_smart_callback_add(cs, "changed", _colorselector_changed_cb, tb);
	evas_object_smart_callback_add(button, "clicked", _download_image_cb, en_icon); 
	evas_object_event_callback_add(en_icon, EVAS_CALLBACK_MOUSE_OUT, _config_save, tb);
	evas_object_event_callback_add(popup_settings, EVAS_CALLBACK_MOUSE_OUT, _config_save, tb);
	evas_object_event_callback_add(popup_settings, EVAS_CALLBACK_HIDE, _popup_del, NULL);
   evas_object_show(popup_settings);
     }
}
