#include "news.h"

Evas_Object *win = NULL;
Evas_Object *ly = NULL;
static Ecore_Timer *timer = NULL;

Ecore_Con_Url *ec_url = NULL;

Eina_Strbuf *feeddata = NULL;
const char* lastcheck;
int firststart = 0;
int gadget = 0;
int no_internet = 0;

typedef struct {
			const char *title;
			const char *link;
			const char *description;
			const char *content;
			const char *pubdate;
			const char *imagelink;
			const char *name;
} News_List_Eet;


typedef struct {
        const char *url;
        const char *icon;
        const char *title;
        const char *keywords;
		  Eina_Bool   icons;
		  Eina_Bool   popupnew;
		  Eina_Bool   popupkeywords;
		  Eina_Bool   checkkeywords;
		  Eina_Bool   indicator;
		  double      refresh;
		  double      fontsize;
		  double      x_value;
		  double      y_value;
		  int         r;
	     int         g;
        int         b;
        int         a;
		  const char *saved_title;

		  Eina_List   *feedlist_eet;
} My_Conf_Type;



typedef struct {
   const char *title;
   const char *link;
   const char *description;
   const char *content;
   const char *pubdate;
   const char *imagelink;
} Feed_Data;


static const char MY_CONF_FILE_ENTRY[] = "config";

static Eet_Data_Descriptor *_my_conf_descriptor;
static Eet_Data_Descriptor *_my_conf_sub_descriptor;

static void
_my_conf_descriptor_init(void)
{
    Eet_Data_Descriptor_Class eddc;
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, My_Conf_Type);
    _my_conf_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, News_List_Eet);
    _my_conf_sub_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    #define MY_CONF_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC             \
        (_my_conf_descriptor, My_Conf_Type, # member, member, eet_type)
		  
    #define MY_CONF_SUB_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC                 \
        (_my_conf_sub_descriptor, News_List_Eet, # member, member, eet_type)
		  


    MY_CONF_ADD_BASIC(url, EET_T_STRING);
    MY_CONF_ADD_BASIC(icon, EET_T_STRING);
    MY_CONF_ADD_BASIC(title, EET_T_STRING);
    MY_CONF_ADD_BASIC(keywords, EET_T_STRING);
    MY_CONF_ADD_BASIC(icons, EET_T_UCHAR);
    MY_CONF_ADD_BASIC(popupnew, EET_T_UCHAR);
    MY_CONF_ADD_BASIC(popupkeywords, EET_T_UCHAR);
    MY_CONF_ADD_BASIC(checkkeywords, EET_T_UCHAR);
    MY_CONF_ADD_BASIC(indicator, EET_T_UCHAR);
    MY_CONF_ADD_BASIC(refresh, EET_T_DOUBLE);
    MY_CONF_ADD_BASIC(fontsize, EET_T_DOUBLE);
    MY_CONF_ADD_BASIC(x_value, EET_T_DOUBLE);
    MY_CONF_ADD_BASIC(y_value, EET_T_DOUBLE);
	 MY_CONF_ADD_BASIC(r, EET_T_INT);
    MY_CONF_ADD_BASIC(g, EET_T_INT);
    MY_CONF_ADD_BASIC(b, EET_T_INT);
    MY_CONF_ADD_BASIC(a, EET_T_INT);
    MY_CONF_ADD_BASIC(saved_title, EET_T_STRING);
	 
    MY_CONF_SUB_ADD_BASIC(title, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(link, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(description, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(content, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(pubdate, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(imagelink, EET_T_STRING);
	 
	 
	 
	 
    // And add the sub descriptor as a linked list at 'subs' in the main struct
    EET_DATA_DESCRIPTOR_ADD_LIST
     (_my_conf_descriptor, My_Conf_Type, "feedlist_eet", feedlist_eet, _my_conf_sub_descriptor);


    #undef MY_CONF_SUB_ADD_BASIC
    #undef MY_CONF_ADD_BASIC
}


void
_read_eet()
{
   Eet_File *ef;
   My_Conf_Type *my_conf;

	eet_init();
	const char *profile;
	profile = elm_config_profile_get();
   char buf[4096], buf2[4096];

   snprintf(buf2, sizeof(buf2), "%s/news", efreet_config_home_get());
   ecore_file_mkpath(buf2);
//    snprintf(buf, sizeof(buf), "%s/countdown_config.cfg", buf2);
	
   snprintf(buf, sizeof(buf), "%s/news/news_gadget_%d_%s.cfg", efreet_config_home_get(), id_num, profile);
	
	ef = eet_open(buf, EET_FILE_MODE_READ);
   if(!ef)
   {
		ci_url = eina_stringshare_add("https://github.com/jf-simon/news/commits/master.atom");
		ci_icon = eina_stringshare_add("");
		ci_title = eina_stringshare_add("");
		ci_keywords = eina_stringshare_add("");
		ci_icons = 0;
		ci_popupnew = 0;
		ci_popupkeywords = 0;
		ci_checkkeywords = 0;
		ci_refresh = 10;
		ci_fontsize = 10;
		ci_x_value = 480;
		ci_y_value = 600;
		ci_r = 11;
		ci_g = 54;
		ci_b = 71;
		ci_a = 255;
	}else
	{
		my_conf = eet_data_read(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY);

		feed_data_list =  my_conf->feedlist_eet;
			
		ci_url = my_conf->url;
		ci_icon = my_conf->icon;
		ci_title = my_conf->title;
		ci_keywords = my_conf->keywords;
		ci_icons = my_conf->icons;
		ci_popupnew = my_conf->popupnew;
		ci_popupkeywords = my_conf->popupkeywords;
		ci_checkkeywords = my_conf->checkkeywords;
		ci_indicator = my_conf->indicator;
		ci_refresh = my_conf->refresh;
		ci_fontsize = my_conf->fontsize;
		ci_x_value = my_conf->x_value;
		ci_y_value = my_conf->y_value;
		ci_r = my_conf->r;
		ci_g = my_conf->g;
		ci_b = my_conf->b;
		ci_a = my_conf->a;
		saved_title = my_conf->saved_title;
			
		eet_close(ef);
	 }
    eet_shutdown();
}


void
_save_eet()
{
	if(id_num < 0)
		return;
		
	Eet_File *ef;
   eet_init();
	 
   char buf[4096];
	const char *profile;
	profile = elm_config_profile_get();
		  
   snprintf(buf, sizeof(buf), "%s/news/news_gadget_%d_%s.cfg", efreet_config_home_get(), id_num, profile);
 
	ef = eet_open(buf, EET_FILE_MODE_WRITE);

   if(!ef)
   {
		printf("ERROR\n");
//         fprintf(stderr, "ERROR: could not open '%s' for write\n", home_file);
//            return EINA_FALSE;   //TODO got elm_main -> END		
	}else
   {
		My_Conf_Type *my_conf = calloc(1, sizeof(My_Conf_Type));

		if (!my_conf)
      {
			fprintf(stderr, "ERROR: could not calloc My_Conf_Type\n");
//                 return NULL;   //TODO got elm_main -> END
		}


		my_conf->url = ci_url;
		my_conf->icon = ci_icon;
		my_conf->title = ci_title;
		my_conf->keywords = ci_keywords;
		my_conf->icons = ci_icons;
		my_conf->popupnew = ci_popupnew;
		my_conf->popupkeywords = ci_popupkeywords;
		my_conf->checkkeywords = ci_checkkeywords;
		my_conf->indicator = ci_indicator;
	 	my_conf->refresh = ci_refresh;
	 	my_conf->fontsize = ci_fontsize;
	 	my_conf->x_value = ci_x_value;
	 	my_conf->y_value= ci_y_value;
	 	my_conf->r = ci_r;
	 	my_conf->g = ci_g;
	 	my_conf->b = ci_b;
		my_conf->a = ci_a;
		my_conf->saved_title = saved_title;
 
		my_conf->feedlist_eet = feed_data_list;
		
        eet_data_write(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY, my_conf, EINA_TRUE);
    }
    
    eet_close(ef);
    eet_shutdown();
}
/*
static void
_my_conf_descriptor_shutdown(void)
{
    eet_data_descriptor_free(_my_conf_sub_descriptor);
    eet_data_descriptor_free(_my_conf_descriptor);
}*/


static Eina_Bool 
_gadget_exit(void *data, int type, void *event_data) 
{
   char buf[4096], cache_dir[4096];
	Ecore_Event_Signal_User *user = event_data;
	
	if (user->number == 2) 
	{
		const char *profile;

		profile = elm_config_profile_get();
		  
		snprintf(buf, sizeof(buf), "%s/news/news_gadget_%d_%s.cfg", efreet_config_home_get(), id_num, profile);
		ecore_file_unlink(buf);	
		
		snprintf(cache_dir, sizeof(cache_dir), "%s/news/cache/%i/", efreet_config_home_get(), id_num);

		ecore_file_recursive_rm(cache_dir);
	} 
	return EINA_TRUE;
}


/*
static void
delete_popup_edje(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
        return;
     }
}*/

void
_set_feed_settings()
{
	Evas_Object *ic = elm_icon_add(win);
		
	if(ci_icon == NULL || strcmp(ci_icon, "") == 0)
	{
		edje_object_signal_emit(ly, "image_feed", "on");
		elm_object_part_content_unset(ly, "image");
		evas_object_del(ic);
	}
	else
	{
		elm_image_file_set(ic, ci_icon, NULL);
		
		elm_image_resizable_set(ic, EINA_TRUE, EINA_TRUE);
		elm_object_part_content_set(ly, "image", ic);
		edje_object_signal_emit(ly, "image_feed", "off");
	}
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	edje_object_part_text_set(edje_obj, "title", ci_title);
	
}


static void
_it_clicked(void *data, Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
   if (!data) return;
	
   char buf[PATH_MAX];
   snprintf(buf, sizeof(buf), "%s", (char *)data);
   evas_object_smart_callback_call(win, "gadget_open_uri", (char *)data);
}

static void
_it_clicked_pressed(void *data, Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
	int a = 30, r = 0, g = 0, b = 0; 
	evas_color_argb_premul(a, &r, &g, &b);
	evas_object_color_set(obj, r, g, b, a);
}

static void
_it_clicked_unpressed(void *data, Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{	
	int a = 0, r = 0, g = 0, b = 0; 
	evas_color_argb_premul(a, &r, &g, &b);
	evas_object_color_set(obj, r, g, b, a);
}



static void
_reload_start(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
     }
   _get_data();
}


// static Evas_Object *
// _content_image(void *data, Evas_Object *obj EINA_UNUSED, Evas_Object *tt)
// {
//    Evas_Object *ic = elm_icon_add(tt);
//    char buf[PATH_MAX];
// 	
//    snprintf(buf, sizeof(buf), "%s.jpg", (const char*)data);
// 	int h, w;
//    elm_image_file_set(ic, buf, NULL);
// 	elm_image_object_size_get(ic, &w, &h);
// //    elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
//    evas_object_resize(ic, 300, 169);
// 	evas_object_size_hint_min_set(ic, 300, 169);
// 	printf("POPUP IMAGE: %i %i\n", w, h);
// 	return ic;
// }


static void
show_popup(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *lbl, *bt, *scroller;
	Evas_Object *tb, *ic;

	char buf2[PATH_MAX];
	char buf1[PATH_MAX];
	char buf[PATH_MAX];
	
	int y = 0;
	
	Feed_Data *list_data;
	Eina_List *l;
		
   if(popup)
   {
        evas_object_del(popup);
        popup = NULL;
        return;
	}
	
	edje_object_signal_emit(ly, "item_new", "default");
		
   popup = elm_win_add(win, "Popup",  ELM_WIN_POPUP_MENU);
	
	if(gadget == 1)
		elm_win_alpha_set(popup, 1);

	
////////////////// TABLE ///////////////////

	tb = elm_table_add(popup);
	elm_table_padding_set(tb, 5,10);
   evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, 0);
   evas_object_size_hint_align_set(tb, 0, 0);
	
	evas_object_show(tb);
	
	if(eina_list_count(feed_data_list) == 0)
	{
			lbl = elm_label_add(popup);
			
			elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);

				snprintf(buf2, sizeof(buf2), "<b>No feeds found</b><br>for <b>%s</b><br>"
											"please check if you have an active internet connection or/and if the URL is correct<br><br> "
											"If both is correct, please add a bug report at https://github.com/jf-simon/news/issues/new", ci_url);
			elm_object_text_set(lbl, buf2);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			
			elm_table_pack(tb, lbl, 0, 0, 1, 1);
			
			
			evas_object_show(lbl);
	}else
	{
		_save_eet();
	
	EINA_LIST_FOREACH(feed_data_list, l, list_data)
   {
		Evas_Object *rect;
		rect = evas_object_rectangle_add(evas_object_evas_get(popup));
		evas_object_size_hint_min_set(rect, 150, 1);
		evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);
		evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		int a = 64, r = 128, g = 128, b = 128; 
		evas_color_argb_premul(a, &r, &g, &b);
		evas_object_color_set(rect, r, g, b, a);
		
		if(y == 0)
		{
			lbl = elm_label_add(popup);
			
			if(list_data->description == NULL || !strcmp(list_data->description, ""))
				snprintf(buf1, sizeof(buf1), "<font_size=%f>%s</font><br><br><font_size=%f>Last check: %s</font>", ci_fontsize+2, list_data->title, ci_fontsize-1, lastcheck);
			else
				snprintf(buf1, sizeof(buf1), "<font_size=%f>%s</font_size><br><br><font_size=%f>%s</font_size><br><br><font_size=%f>Last check: %s</font_size>", ci_fontsize+2, list_data->title, ci_fontsize+1, list_data->description, ci_fontsize-1, lastcheck);
					
			elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
			elm_object_text_set(lbl, buf1);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			
			elm_table_pack(tb, lbl, 0, y, 2, 1);
			
			elm_table_pack(tb, rect, 0, y+1, 2, 1);
			
			evas_object_show(lbl);
			evas_object_show(rect);
			
			y++;
		}
		else
		{
			if(!ci_icons)
			{
				ic = elm_icon_add(popup);
					
				if(list_data->imagelink == NULL)
				{
					snprintf(buf, sizeof(buf), "%s/images/news.png", PACKAGE_DATA_DIR);
					evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(56), ELM_SCALE_SIZE(56));
				}
				else
				{	
					snprintf(buf, sizeof(buf), "%s.jpg", list_data->imagelink);
					evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(150), ELM_SCALE_SIZE(56));
				}
															
				elm_image_file_set(ic, buf, NULL);
				evas_object_size_hint_weight_set(ic, 0, EVAS_HINT_EXPAND);
				evas_object_size_hint_align_set(ic, EVAS_HINT_FILL, EVAS_HINT_FILL);
				elm_table_pack(tb, ic, 0, y+1, 1, 1);
				evas_object_show(ic);
			}
		
			lbl = elm_label_add(popup);
			snprintf(buf1, sizeof(buf1), "<b><font_size=%f>%s</font_size></b><br><font_size=%f><custom align=justify>%s</custom></font_size></a><br><br><custom align=right><font_size=%f>%s</font_size></custom>", ci_fontsize, list_data->title, ci_fontsize, list_data->description, ci_fontsize-2, list_data->pubdate);
			elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
			elm_object_text_set(lbl, buf1);
			evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					
			bt = elm_button_add(popup);
			evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
			evas_object_size_hint_weight_set(bt, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			
			
			int a = 0, r = 0, g = 0, b = 0; 
			evas_color_argb_premul(a, &r, &g, &b);
			evas_object_color_set(bt, r, g, b, a);
			evas_object_show(bt);
					
			evas_object_smart_callback_add(bt, "clicked", _it_clicked, list_data->link);
			evas_object_smart_callback_add(bt, "pressed", _it_clicked_pressed, list_data->link);
			evas_object_smart_callback_add(bt, "unpressed", _it_clicked_unpressed, list_data->link);
				
			if(!ci_icons)
			{
				elm_table_pack(tb, lbl, 1, y+1, 1, 1);
				elm_table_pack(tb, bt, 0, y+1, 2, 1);
				elm_table_pack(tb, rect, 0, y+2, 2, 1);
			}
			else
			{
				elm_table_pack(tb, lbl, 0, y+1, 1, 1);
				elm_table_pack(tb, bt, 0, y+1, 1, 1);
				elm_table_pack(tb, rect, 0, y+2, 1, 1);
			}
			
			evas_object_show(rect);
			evas_object_show(lbl);
			
			y=y+2;
		}
	}

	}
////////////////// TABLE END ///////////////////

	scroller = elm_scroller_add(popup);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(popup, scroller);
   evas_object_show(scroller);
//    elm_object_content_set(scroller, box);
   elm_object_content_set(scroller, tb);
   elm_scroller_bounce_set(scroller, EINA_TRUE, EINA_FALSE);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   elm_scroller_propagate_events_set(scroller, EINA_TRUE);
   elm_scroller_page_relative_set(scroller, 0, 1);
	
	if(!ci_icons)
		evas_object_resize(popup, ci_x_value, ci_y_value);
	else
	{
		if(ci_x_value - 30 <= 0)
			evas_object_resize(popup, ci_x_value, ci_y_value);
		else
			evas_object_resize(popup, ci_x_value-30, ci_y_value);
	}	

   evas_object_show(popup);
}


void
set_color(Evas_Object *ly)
{
	edje_object_color_class_set(ly, "colorclass",
                               ci_r, ci_g, ci_b, ci_a,
                               255, 255, 255, 0,
                               39, 90, 187, 255);
}



void stringReplace(char *search, char *replace, char *string)
{
   while(strstr(string, search) != NULL)
   {
	   char *tempString, *searchStart;
	   int len=0;
	   // Speicher reservieren
	   tempString = (char*) malloc(strlen(string)+1204 * sizeof(char));
	
	   searchStart = strstr(string, search);

		// temporaere Kopie anlegen
   	strcpy(tempString, string);

	   // ersten Abschnitt in String setzen
	   len = searchStart - string;
	   string[len] = '\0';

	   // zweiten Abschnitt anhaengen
	   strcat(string, replace);

	   // dritten Abschnitt anhaengen
	   len += strlen(search);
	   strcat(string, (char*)tempString+len);

	   // Speicher freigeben
	   free(tempString);
  }
	return;
}


void remove_space(char *src)
{
   char *dst;

    /* find position of first non-space character */
   for (dst=src; *src == ' '; src++) {;}

    /* nothing to do */
    if (dst==src) return;

    /* K&R style strcpy() */
    while ((*dst++ = *src++)) {;}

   return;
}


char*
find_data(char *string, char *start1, char *end1)
{
	char *string1 = calloc(strlen(string)+1, sizeof(char));
		
	if((strstr(string, start1) == NULL) || (strstr(string, end1) == NULL))
	{
		return "";
	}
	else
	{
		int start_len = strlen(start1);
		
		strncpy(string1, strstr(string, start1)+start_len, strstr(string, end1)-strstr(string, start1)-strlen(start1));
	
// 		string1[strlen(string1)] = '\0';

		char **arr;
		
		if(!strcmp(end1, ".jpg\" "))
		{
			return (char*)string1;
		}
		else if(!strcmp(end1, "/>"))
		{
			arr = eina_str_split(string1, "href=\"", 2);
		
			stringReplace("\"", "", arr[1]);

			return (char *)arr[1];
			free(arr[0]);
			free(arr);
		}
		else
		{
			arr = eina_str_split(string1, ">", 2);
			
			return (char *)arr[1];
			free(arr[0]);
			free(arr);
		}
	}
	free(string1);
}

const char *
highlight_words(Eina_Strbuf* tmp)
{
	char keywords_buf[PATH_MAX];
	char **arr;
	int i;
	
	arr = eina_str_split(ci_keywords, ";", 0);
	
	if(arr == NULL)
		return eina_strbuf_string_get(tmp);
	
   for (i = 0; arr[i]; i++)
	{
		remove_space((char*)arr[i]);
		snprintf(keywords_buf, sizeof(keywords_buf), "<custom backing=on backing_color=#cfcf00ff color=#000000ff> %s </custom>", arr[i]);
		if(strcmp(arr[i], ""))
			eina_strbuf_replace_all(tmp, arr[i], (const char*)keywords_buf);
	}
	
	return eina_strbuf_string_get(tmp);
}


static void
parse_rss(Eina_Strbuf *mybuffer)
{	
	char **arr;
	int i = 0, y = 0;
	Eina_Strbuf *tmp;
	tmp = eina_strbuf_new();
	
	arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
	if(arr == NULL)
		return;
	
   for (i = 0; arr[i]; i++)
	{
		stringReplace("<![CDATA[", "", arr[i]);
		stringReplace("]]>", "", arr[i]);

		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
				// Highlight words for title
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->title));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE)
					data_add->title = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->title = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);
		
		if(i == 0)
			feedname = eina_stringshare_add(data_add->title);
		
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "</link>"));
		
		// find description for title description
		data_add->description = eina_stringshare_add(find_data(arr[i], "<description", "</description>"));

		// find news content for the items
		if(!strcmp(data_add->description, ""))
		{
			data_add->description = eina_stringshare_add(find_data(arr[i], "<content:encoded>", "</content:encoded>"));
			y = 1;
		}
		
				// Highlight words for description
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->description));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE && y == 1) // y is checked for not highlight title descriptions content
					data_add->description = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->description = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);		

		data_add->imagelink = eina_stringshare_add(find_data(arr[i], "<img src=\"", ".jpg\" "));

		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<pubDate", "</pubDate>"));
		
		feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
	eina_strbuf_free(tmp);	
	eina_strbuf_reset(mybuffer);
	
	Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{
		
		edje_object_signal_emit(ly, "item_new", "new");
		saved_title = eina_stringshare_add(list_values->title);
		
// 		if(popup)
//       {
// 			evas_object_del(popup);
// 			popup = NULL;
// 			show_popup(NULL, NULL, NULL, NULL);
// 			printf("POPUP\n");
// 		}
// 		else 
			if(ci_popupnew == 1 && firststart != 0)
			show_popup(NULL, NULL, NULL, NULL);
	}
}


static void
parse_atom(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i = 0, y = 0;
	Eina_Strbuf *tmp;
	tmp = eina_strbuf_new();
		
   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<entry>", 0);
	
   for (i = 0; arr[i]; i++)
	{
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));

		// find title
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));

				// Highlight words for title
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->title));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE)
					data_add->title = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->title = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);
		
		// save first title for later checks
		if(i == 0)
			feedname = eina_stringshare_add(data_add->title);

		// find link
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "/>"));

		// find subtitle for title
		data_add->description = eina_stringshare_add(find_data(arr[i], "<subtitle", "</subtitle>"));
		
		// find summery
		if(!strcmp(data_add->description, ""))
		{
			data_add->description = eina_stringshare_add(find_data(arr[i], "<summary", "</summary>"));
			y =1;			
		}
		// some atom feed uses <content> tag instead as <summary>
		if(!strcmp(data_add->description, ""))
		{
			data_add->description = eina_stringshare_add(find_data(arr[i], "<content", "</content>"));
			y = 1;
		}
				// Highlight words for description
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->description));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE && y == 1) // y is checked for not highlight title descriptions content
					data_add->description = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->description = eina_stringshare_add(eina_strbuf_string_get(tmp));

				eina_strbuf_reset(tmp);
		
		// find puplication date
		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<updated", "</updated>"));
		
		// put data to the feed list
		feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
	eina_strbuf_free(tmp);
	eina_strbuf_reset(mybuffer);
	
   Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{
		edje_object_signal_emit(ly, "item_new", "new");
		saved_title = eina_stringshare_add(list_values->title);
		
// 		if(popup)
//       {
// 			evas_object_del(popup);
// 			popup = NULL;
// 			show_popup(NULL, NULL, NULL, NULL);
// 			printf("POPUP\n");
// 		}
// 		else 
			if(ci_popupnew == 1 && firststart != 0)
			show_popup(NULL, NULL, NULL, NULL);
	}
}


static void
parse_atom1(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i = 0, y = 0;
	Eina_Strbuf *tmp;
	tmp = eina_strbuf_new();

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item", 0);
	
   for (i = 0; arr[i]; i++)
	{
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
		
				// Highlight words for title
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->title));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE)
					data_add->title = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->title = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);

		// save first title for later checks
		if(i == 0)
			feedname = eina_stringshare_add(data_add->title);

		// find link
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "/>"));

		// find subtitle for title
		data_add->description = eina_stringshare_add(find_data(arr[i], "<subtitle", "</subtitle>"));
		
		// find summery
		if(!strcmp(data_add->description, ""))
		{
			data_add->description = eina_stringshare_add(find_data(arr[i], "<summary", "</summary>"));
			y =1;			
		}
		// some atom feed uses <content> tag instead as <summary>
		if(!strcmp(data_add->description, ""))
		{
			data_add->description = eina_stringshare_add(find_data(arr[i], "<content", "</content>"));
			y = 1;
		}
				// Highlight words for description
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->description));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE && y == 1) // y is checked for not highlight title descriptions content
					data_add->description = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->description = eina_stringshare_add(eina_strbuf_string_get(tmp));

				eina_strbuf_reset(tmp);
				
		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<pubDate", "</pubDate>"));
				
// 				data_add->subtitle = eina_stringshare_add(find_data(arr[i], "<subtitle", "</subtitle>"));
				
		feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
	eina_strbuf_free(tmp);
	eina_strbuf_reset(mybuffer);
	
   Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{		
		edje_object_signal_emit(ly, "item_new", "new");
		saved_title = eina_stringshare_add(list_values->title);
		
// 		if(popup)
//       {
// 			evas_object_del(popup);
// 			popup = NULL;
// 			show_popup(NULL, NULL, NULL, NULL);
// 			printf("POPUP\n");
// 		}
// 		else 
			if(ci_popupnew == 1 && firststart != 0)
			show_popup(NULL, NULL, NULL, NULL);
	}
}


static void
parse_rdf(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i=0;
	Eina_Strbuf *tmp;
	tmp = eina_strbuf_new();

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title>", "</title>"));
		
				// Highlight words for description
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->title));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE)
					data_add->title = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->title = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);
		
		if(i == 0)
			feedname = eina_stringshare_add(data_add->title);
		
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link>", "</link>"));
				
		data_add->description = eina_stringshare_add(find_data(arr[i], "<description>", "</description>"));
		
				// Highlight words for description
				eina_strbuf_append(tmp, elm_entry_markup_to_utf8(data_add->description));
				
				if(strcmp(ci_keywords, "") != 0 && ci_checkkeywords == EINA_TRUE)
					data_add->description = eina_stringshare_add(highlight_words(tmp));
				else
					data_add->description = eina_stringshare_add(eina_strbuf_string_get(tmp));
				
				eina_strbuf_reset(tmp);
		
		feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
	eina_strbuf_free(tmp);
	eina_strbuf_reset(mybuffer);
	
	eina_strbuf_reset(mybuffer);
}


static Eina_Bool
_url_data_cb(void *data, int type EINA_UNUSED, void *event_info)
{
	Ecore_Con_Event_Url_Data *url_data = event_info;
	
// 	eina_strbuf_append_length(feeddata, ((const char*) &url_data->data[0]), url_data->size);
	eina_strbuf_append_length(data, (const char*)url_data->data, url_data->size);
	
	//BINBUF FÜR BILDER ZUM CACHEN
// 	eina_binbuf_append_length(data, url_data->data, url_data->size);
	
   return ECORE_CALLBACK_PASS_ON;
}


static Eina_Bool
_data_complete(void *data, int type, void *event_info)
{
	Ecore_Con_Event_Url_Complete *url_complete = event_info;
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	
	struct tm *newtime;
	time_t long_time;

	time( &long_time );
	newtime = localtime( &long_time );
	
	char buf[PATH_MAX];

	snprintf(buf, sizeof(buf), "%d.%d.%d | %02d:%02d:%02d",newtime->tm_mday, newtime->tm_mon+1, newtime->tm_year+1900, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
	lastcheck = eina_stringshare_add(buf);
		
	if(url_complete->status >= 200 && url_complete->status <= 226)
	{
			Feed_Data *p;
			EINA_LIST_FREE(feed_data_list, p)
			{ 
				free(p);
			}
	
			edje_object_signal_emit(edje_obj, "reload", "default");
		
			if(strstr((char *)eina_strbuf_string_get(data), "<rss vers") != 0)
			{
				printf("RSS FEED\n");
				parse_rss(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "<rdf:RDF") != 0)
			{
				printf("RDF FEED\n");
				parse_rdf(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "<feed xmlns=\"http://www.w3.org/2005/Atom\"") != 0)
			{
				printf("ATOM FEED\n");
				parse_atom(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "<rss xmlns:content") != 0)
			{
				printf("RSS1 FEED\n");
				parse_rss(data);
			}
// 			else if(strstr((char *)eina_strbuf_string_get(data), "<rss xmlns") != 0)
// 			{
// 				printf("RSS1 FEED\n");
// 				parse_rss(data);
// 			}

			else if(strstr((char *)eina_strbuf_string_get(data), "<rss xmlns:atom") != 0)
			{
				printf("ATOM1 FEED\n");
				parse_atom1(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "xmlns:atom") != 0)
			{
				printf("ATOM1 FEED NEW\n");
				parse_atom1(data);
			}
			else
			{
				printf("NOT SUPPORTED\n");
			}
			no_internet = 0;
	}
	else
	{
		edje_object_signal_emit(edje_obj, "reload", "failed");
		
		if(no_internet != 1 && firststart != 0)
		{
			Feed_Data *data_add = calloc(1, sizeof(Feed_Data));

			// set title
			data_add->title = eina_stringshare_add("NO active Internet connection");

			// set summery
			data_add->description = eina_stringshare_add("NO active Internet connection");
			
			// set date
			data_add->pubdate = eina_stringshare_add(buf);
			
			// set data to 2nd position in list
			feed_data_list = eina_list_prepend_relative(feed_data_list, data_add, eina_list_nth(feed_data_list, 1));
			
			no_internet = 1;
			
		}
	}
	
	firststart = 1;



// 	ecore_con_url_free(ec_url);
   return ECORE_CALLBACK_DONE;
}


void
_get_data()
{
	Eina_Strbuf *downloaded_data;
	downloaded_data = eina_strbuf_new();
		
	Eina_Bool r;
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	if(!ci_indicator)
		edje_object_signal_emit(edje_obj, "reload", "visible");

	if(ec_url)
		ecore_con_url_free(ec_url);
	
	ec_url = ecore_con_url_custom_new(ci_url, "GET");
	
	ecore_event_handler_add(ECORE_CON_EVENT_URL_DATA, _url_data_cb, downloaded_data);
   ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _data_complete, downloaded_data);
	ecore_con_url_additional_header_add(ec_url, "User-Agent", "Enlightenment News Gadget");

	r = ecore_con_url_get(ec_url);
	
	if (!r)
   {
      ecore_con_url_free(ec_url);
	}
}

static Eina_Bool
_get_data_timer(void *data)
{
	_get_data();
	
	return ECORE_CALLBACK_RENEW;
}


void
_timer_reset()
{
	
	if(timer)
		ecore_timer_del(timer);
	
	timer = ecore_timer_add(ci_refresh*60, _get_data_timer, NULL);
}


int elm_main(int argc, char *argv[])
{
	ecore_con_init();
   ecore_con_url_init();
	
   char buf[PATH_MAX];
//    char buf2[PATH_MAX];
   char buf1[16];
	
	Config_Item *config;
	config = calloc(1, sizeof(Config_Item));

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
 
   if (getenv("E_GADGET_ID"))
     {
        gadget = 1;
        snprintf(buf1, sizeof(buf1), "%s", getenv("E_GADGET_ID"));
        id_num = atoi(buf1);
     }

   _my_conf_descriptor_init();
   _read_eet();
	
   //new window
   win = elm_win_add(NULL, "news", ELM_WIN_BASIC);
   elm_win_title_set(win, "News");
   elm_win_autodel_set(win, EINA_TRUE);
	elm_win_alpha_set(win, EINA_TRUE);	

	evas_object_size_hint_aspect_set(win, EVAS_ASPECT_CONTROL_BOTH, 1, 1);
	
   // LAYOUT CREATE START// 
   ly = elm_layout_add(win);		  
   snprintf(buf, sizeof(buf), "%s/themes/news.edj", PACKAGE_DATA_DIR);
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	

   elm_win_resize_object_add(win, ly);
   evas_object_show(ly);
    // LAYOUT CREATE END// 

   evas_object_data_set(win, "config", config);
	evas_object_resize(win, 50, 50);
   evas_object_show(win);
	elm_layout_file_set(ly, buf, "news");
	
   edje_object_signal_callback_add(ly, "settings", "settings", _settings_2, win);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
   evas_object_smart_callback_add(win, "gadget_configure", _settings_1, edje_obj);
	
   edje_object_signal_callback_add(ly, "show_popup", "show_popup", show_popup, win);
   edje_object_signal_callback_add(ly, "reload", "reload", _reload_start, win);
	
//    edje_object_signal_callback_add(ly, "delete_popup", "delete_popup", delete_popup_edje, win);
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, _gadget_exit, NULL);
	
	edje_object_color_class_set(edje_obj, "colorclass", ci_r, ci_g, ci_b, ci_a, 255, 255, 255, 0, 39, 90, 187, 255);
	_set_feed_settings();
	_get_data();
	_save_eet();
	
	if(ci_refresh <= 0){ci_refresh = 10;};
	timer = ecore_timer_add(ci_refresh*60, _get_data_timer, NULL);
	
  //run app RUN!
  elm_run();
  
	ecore_con_url_free(ec_url);
   ecore_con_url_shutdown();
   ecore_con_shutdown();
  //shutdown!
//         _my_conf_descriptor_shutdown();
  return 0;
}

ELM_MAIN();
