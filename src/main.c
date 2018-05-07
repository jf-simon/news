#include "news.h"

Evas_Object *win = NULL;
Evas_Object *ly = NULL;
static Evas_Object *popup = NULL;
static Ecore_Timer *timer = NULL;

Ecore_Con_Url *ec_url = NULL;

Eina_Strbuf *feeddata = NULL;
const char* lastcheck;
int firststart = 0;
Eina_List *feed_data_list = NULL;
Eina_List *feed_data_list_tmp = NULL;

typedef struct {
        Eina_List   *configlist_eet;
} News_List_Eet;


typedef struct {
        int         id;
        const char *url;
        const char *icon;
        const char *saved_title;
		  Eina_Bool   icons;
		  Eina_Bool   bigicons;
		  Eina_Bool   popupnew;
		  Eina_Bool   indicator;
		  double      refresh;
		  double      fontsize;
		  double      x_value;
		  double      y_value;
		  int         r;
	     int         g;
        int         b;
        int         a;
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
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, News_List_Eet);
    _my_conf_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET(&eddc, My_Conf_Type);
    _my_conf_sub_descriptor = eet_data_descriptor_stream_new(&eddc);
   
    #define MY_CONF_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC             \
        (_my_conf_descriptor, News_List_Eet, # member, member, eet_type)
		  
    #define MY_CONF_SUB_ADD_BASIC(member, eet_type) \
        EET_DATA_DESCRIPTOR_ADD_BASIC                 \
        (_my_conf_sub_descriptor, My_Conf_Type, # member, member, eet_type)

    MY_CONF_SUB_ADD_BASIC(id, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(url, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(icon, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(saved_title, EET_T_STRING);
    MY_CONF_SUB_ADD_BASIC(icons, EET_T_UCHAR);
    MY_CONF_SUB_ADD_BASIC(bigicons, EET_T_UCHAR);
    MY_CONF_SUB_ADD_BASIC(popupnew, EET_T_UCHAR);
    MY_CONF_SUB_ADD_BASIC(indicator, EET_T_UCHAR);
    MY_CONF_SUB_ADD_BASIC(refresh, EET_T_DOUBLE);
    MY_CONF_SUB_ADD_BASIC(fontsize, EET_T_DOUBLE);
    MY_CONF_SUB_ADD_BASIC(x_value, EET_T_DOUBLE);
    MY_CONF_SUB_ADD_BASIC(y_value, EET_T_DOUBLE);
	 MY_CONF_SUB_ADD_BASIC(r, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(g, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(b, EET_T_INT);
    MY_CONF_SUB_ADD_BASIC(a, EET_T_INT);

    // And add the sub descriptor as a linked list at 'subs' in the main struct
    EET_DATA_DESCRIPTOR_ADD_LIST
     (_my_conf_descriptor, News_List_Eet, "configlist_eet", configlist_eet, _my_conf_sub_descriptor);

     
    #undef MY_CONF_ADD_BASIC
    #undef MY_CONF_SUB_ADD_BASIC
}


void
_read_eet()
{
    Eet_File *ef;
    
    News_List_Eet *my_conf;
        
    eet_init();
	const char *profile;
	profile = elm_config_profile_get();
   char buf[4096], buf2[4096];

   snprintf(buf2, sizeof(buf2), "%s/news", efreet_config_home_get());
   ecore_file_mkpath(buf2);
//    snprintf(buf, sizeof(buf), "%s/countdown_config.cfg", buf2);
	
   snprintf(buf, sizeof(buf), "%s/news/news_gadget_%d_%s.cfg", efreet_config_home_get(), id_num, profile);
	
    ef = eet_open(buf, EET_FILE_MODE_READ);
    if (!ef)
    {
//         printf("ERROR: could not open '%s' for read\n", home_file);
        return;
    }
    
    my_conf = eet_data_read(ef, _my_conf_descriptor, MY_CONF_FILE_ENTRY);
        
    configlist =  my_conf->configlist_eet;
// 	 name = my_conf->name;
  
    eet_close(ef);
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

        News_List_Eet *my_conf = calloc(1, sizeof(News_List_Eet));

        if (!my_conf)
        {
            fprintf(stderr, "ERROR: could not calloc My_Conf_Type\n");
//                 return NULL;   //TODO got elm_main -> END
        }

		  my_conf->configlist_eet = configlist;
		  
// 		  my_conf->name = ci_name;
 
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
   char buf[4096];
	Ecore_Event_Signal_User *user = event_data;
	
	if ( user->number == 2) 
	{
		const char *profile;
		profile = elm_config_profile_get();
		  
		snprintf(buf, sizeof(buf), "%s/news/news_gadget_%d_%s.cfg", efreet_config_home_get(), id_num, profile);
		ecore_file_unlink(buf);	
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
_set_feed_icon()
{
// 	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	Evas_Object *ic = elm_icon_add(win);
		
	if(strcmp(ci_icon, "") == 0 || ci_icon == NULL)
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
}


static void
_it_clicked(void *data, Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
   printf("item was clicked: %s\n", (char *)data);
   if (!data) return;
	
   char buf[PATH_MAX];
   snprintf(buf, sizeof(buf), "%s", (char *)data);
   evas_object_smart_callback_call(win, "gadget_open_uri", (char *)data);
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


static Evas_Object *
_content_image(void *data, Evas_Object *obj EINA_UNUSED, Evas_Object *tt)
{
   Evas_Object *ic = elm_icon_add(tt);
   char buf[PATH_MAX];
	
   snprintf(buf, sizeof(buf), "%s.jpg", (const char*)data);
	int h, w;
   elm_image_file_set(ic, buf, NULL);
	elm_image_object_size_get(ic, &w, &h);
//    elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
   evas_object_resize(ic, 300, 169);
	evas_object_size_hint_min_set(ic, 300, 169);
	printf("POPUP IMAGE: %i %i\n", w, h);
	return ic;
}


static void
show_popup(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
	Evas_Object *box, *lbl, *bt, *scroller, *en;
	
	Feed_Data *list_data;
	Eina_List *l;
	int i = 0;
		
	if(eina_list_count(feed_data_list) == 0)
		return;
	
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
        return;
     }

	_config_save(mainbox, NULL, NULL, NULL);
	
	edje_object_signal_emit(ly, "item_new", "default");
		
   popup = elm_win_add(win, "Popup",  ELM_WIN_POPUP_MENU);
   elm_win_alpha_set(popup, 1);

   box = elm_box_add(popup);
   elm_box_horizontal_set(box, EINA_FALSE);
	elm_box_padding_set(box, 10, 0);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0);
   evas_object_show(box);
		
	Evas_Object *o, *boxh, *ic;
	char buf1[PATH_MAX];
	char buf[PATH_MAX];
	
	EINA_LIST_FOREACH(feed_data_list, l, list_data)
   {
		
					
			if(i == 0)
			{					
					lbl = elm_label_add(box);
					if(list_data->description == NULL)
							snprintf(buf1, sizeof(buf1), "<bigger>%s</bigger><br><br><small>Last check: %s</small>", list_data->title, lastcheck);
					else
						snprintf(buf1, sizeof(buf1), "<bigger>%s</bigger><br><br><big>%s</big><br><br><small>Last check: %s</small>", list_data->title, list_data->description, lastcheck);
					
					elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
// 					elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(400));
					elm_object_text_set(lbl, buf1);
					evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
					evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					elm_box_pack_end(box, lbl);
					evas_object_show(lbl);

					o = elm_separator_add(box);
					elm_separator_horizontal_set(o, EINA_TRUE);
					elm_box_pack_end(box, o);
					evas_object_show(o);
			}
			else
			{
				if(i == 1)
					saved_title = eina_stringshare_add(list_data->title);

			boxh = elm_box_add(box);
			elm_box_horizontal_set(boxh, EINA_TRUE);
					evas_object_size_hint_align_set(boxh, EVAS_HINT_FILL, EVAS_HINT_FILL);
			
			if(!ci_icons)
			{
				elm_box_padding_set(boxh, 10, 0);
			}
				evas_object_show(boxh);
			
					if(!ci_icons)
					{
						ic = elm_icon_add(boxh);
						
						if(list_data->imagelink == NULL)
							snprintf(buf, sizeof(buf), "%s/images/news.png", PACKAGE_DATA_DIR);
						else
							snprintf(buf, sizeof(buf), "%s.jpg", list_data->imagelink);
															
						elm_image_file_set(ic, buf, NULL);
						evas_object_size_hint_min_set(ic, 150, 56);
						evas_object_size_hint_weight_set(ic, 0, EVAS_HINT_EXPAND);
						evas_object_size_hint_align_set(ic, EVAS_HINT_FILL, EVAS_HINT_FILL);
						elm_box_pack_end(boxh, ic);
						evas_object_show(ic);
					
						if(!ci_bigicons && list_data->imagelink != NULL)
						{
							elm_object_tooltip_content_cb_set(ic, _content_image, list_data->imagelink, NULL);
						}
						
						evas_object_smart_callback_add(ic, "clicked", _it_clicked, list_data->link);
					}
					
				   lbl = elm_label_add(boxh);
					snprintf(buf1, sizeof(buf1), "<b><font_size=%f>%s</font_size></b><br><font_size=%f>%s</font_size></a><br><br><custom align=right><small>%s</small></custom>", ci_fontsize, list_data->title, ci_fontsize, elm_entry_markup_to_utf8(list_data->description), list_data->pubdate);
// 					snprintf(buf1, sizeof(buf1), "<b>%s</b><br>%s</a><br><br><custom align=right><small>%s</small></custom>", list_data->title, ci_fontsize, elm_entry_markup_to_utf8(list_data->description), list_data->pubdate);
					elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
// 					elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(300));
					elm_object_text_set(lbl, buf1);
					evas_object_size_hint_align_set(lbl, EVAS_HINT_FILL, EVAS_HINT_FILL);
					evas_object_size_hint_weight_set(lbl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
					elm_box_pack_end(boxh, lbl);
					evas_object_show(lbl);
					
								
			elm_box_pack_end(box, boxh);
			evas_object_show(boxh);
			
			o = elm_separator_add(box);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_box_pack_end(box, o);
			evas_object_show(o);

			}
		
			i++;
	}

//    bt = elm_button_add(box);
//    elm_object_text_set(bt, "refresh");
//    evas_object_smart_callback_add(bt, "pressed", _get_data, NULL);
//    elm_box_pack_end(box, bt);
//    evas_object_show(bt);

////////////////// TABLE TEST ///////////////////
/*
		Evas_Object *tb;
		   
		tb = elm_table_add(popup);
		evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, 0);
// 		elm_win_resize_object_add(popup, tb);
		evas_object_data_set(win, "tb", tb);
		evas_object_show(tb);
		
		bt = elm_button_add(popup);
   elm_object_text_set(bt, "Button 1");
   evas_object_size_hint_weight_set(bt, 0.25, 0.25);
//    evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, bt, 0, 0, 1, 1);
   evas_object_show(bt);
	
			bt = elm_button_add(popup);
   elm_object_text_set(bt, "Button 1.1");
   evas_object_size_hint_weight_set(bt, 0.25, 0.25);
//    evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, bt, 1, 0, 1, 1);
   evas_object_show(bt);
	
			bt = elm_button_add(popup);
   elm_object_text_set(bt, "Button 2.0");
   evas_object_size_hint_weight_set(bt, 0.25, 0.25);
//    evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, bt, 0, 1, 1, 1);
   evas_object_show(bt);
	
			bt = elm_button_add(popup);
   elm_object_text_set(bt, "Button 2.1");
   evas_object_size_hint_weight_set(bt, 0.25, 0.25);
//    evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_table_pack(tb, bt, 1, 1, 1, 1);
   evas_object_show(bt);
*/

////////////////// TABLE TEST END ///////////////////



	scroller = elm_scroller_add(popup);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(popup, scroller);
   evas_object_show(scroller);
   elm_object_content_set(scroller, box);
//    elm_object_content_set(scroller, tb);
   elm_scroller_bounce_set(scroller, EINA_TRUE, EINA_FALSE);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   elm_scroller_propagate_events_set(scroller, EINA_TRUE);
   elm_scroller_page_relative_set(scroller, 0, 1);
	
	if(!ci_icons)
// 		evas_object_resize(popup, 480, 600);
		evas_object_resize(popup, ci_x_value, ci_y_value);
	else
	{
// 		evas_object_resize(popup, 450, 600);
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
	printf("COLOR CLASS: %i %i %i %i\n", ci_r, ci_g, ci_b, ci_a);
	edje_object_color_class_set(ly, "colorclass",
                               ci_r, ci_g, ci_b, ci_a,
                               255, 255, 255, 0,
                               39, 90, 187, 255);
	printf("SET COLOR: %i %i %i %i,\n", ci_r,ci_g,ci_b,ci_a);
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
		return 0;
	}
	else
	{
		int start_len = strlen(start1);
		
		strncpy(string1, strstr(string, start1)+start_len, strstr(string, end1)-strstr(string, start1)-strlen(start1));
	
		string1[strlen(string1)] = '\0';

// 		printf("STING1: %s\n", string1);

		char **arr;
		
		if(!strcmp(end1, ".jpg\" "))
		{
// 			printf("TEST: %s\n\n", string1);
			return string1;
		}
		else
		{
			arr = eina_str_split(string1, ">", 2);
		
// 			printf("TEST: %s\n\n", arr[1]);
			
			return arr[1];
			free(arr[0]);
			free(arr);
		}
	}
	free(string1);
}


static void
parse_rss(Eina_Strbuf *mybuffer)
{	
	char **arr;
	int i;
	
	arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
	if(arr == NULL)
		return;
	
   for (i = 0; arr[i]; i++)
	{	
		stringReplace("<![CDATA[", "", arr[i]);
		stringReplace("]]>", "", arr[i]);
// 		stringReplace("Trump","<highlight>Trump</higlight>", arr[i]);
// 						printf("Title: %s\n", arr[i]);
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
		
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "</link>"));

		data_add->description = eina_stringshare_add(find_data(arr[i], "<description", "</description>"));

		data_add->imagelink = eina_stringshare_add(find_data(arr[i], "<img src=\"", ".jpg\" "));

		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<pubDate", "</pubDate>"));
		
		feed_data_list = eina_list_append(feed_data_list, data_add);
		
// 		printf("TITLE: %s\n", data_add->title);
	}
	
	free(arr[0]);
   free(arr);
	
	eina_strbuf_reset(mybuffer);
	
	Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{
		printf("TITLE UNGLEICH\n");
		printf("TITLE ORG:\t%s\n", list_values->title);
		printf("TITLE SAVED:\t%s\n", saved_title);
		
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
	else
	{
		printf("TITLE GLEICH\n");
// 		edje_object_signal_emit(ly, "item_new", "default");
	}
}


static void
parse_atom(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<entry>", 0);
	
   for (i = 0; arr[i]; i++)
	{
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
				
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "/>"));
				
		data_add->description = eina_stringshare_add(find_data(arr[i], "<summary", "</summary>"));
				
		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<updated", "</updated>"));
				
// 				data_add->subtitle = eina_stringshare_add(find_data(arr[i], "<subtitle", "</subtitle>"));
				
		feed_data_list = eina_list_append(feed_data_list, data_add);
// 		printf("ARRAY = YES %s\n", data_add->title);
	}
	
	free(arr[0]);
   free(arr);
	
	eina_strbuf_reset(mybuffer);
	
   Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{
		printf("TITLE UNGLEICH\n");
		printf("TITLE ORG:\t%s\n", list_values->title);
		printf("TITLE SAVED:\t%s\n", saved_title);
		
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
	else
	{
		printf("TITLE GLEICH\n");
		printf("TITLE ORG1:\t%s\n", list_values->title);
		printf("TITLE SAVED1:\t%s\n", saved_title);
// 		edje_object_signal_emit(ly, "item_new", "default");
	}
}


static void
parse_atom1(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item", 0);
	
   for (i = 0; arr[i]; i++)
	{
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
				
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "/>"));
				
		data_add->description = eina_stringshare_add(find_data(arr[i], "<description", "</description>"));
				
		data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<pubDate", "</pubDate>"));
				
// 				data_add->subtitle = eina_stringshare_add(find_data(arr[i], "<subtitle", "</subtitle>"));
				
		feed_data_list = eina_list_append(feed_data_list, data_add);
// 		printf("ARRAY = YES %s\n", data_add->title);
	}
	
	free(arr[0]);
   free(arr);
	
	eina_strbuf_reset(mybuffer);
	
   Feed_Data *list_values = NULL;
	list_values = eina_list_nth(feed_data_list, 1);
	 
	if(saved_title == NULL || strcmp(list_values->title, saved_title) != 0)
	{
		printf("TITLE UNGLEICH\n");
		printf("TITLE ORG:\t%s\n", list_values->title);
		printf("TITLE SAVED:\t%s\n", saved_title);
		
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
	else
	{
		printf("TITLE GLEICH\n");
// 		edje_object_signal_emit(ly, "item_new", "default");
	}
}


static void
parse_rdf(Eina_Strbuf *mybuffer)
{
	char **arr;
	int i=0;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
		data_add->title = eina_stringshare_add(find_data(arr[i], "<title>", "</title>"));
				
		data_add->link = eina_stringshare_add(find_data(arr[i], "<link>", "</link>"));
				
		data_add->description = eina_stringshare_add(find_data(arr[i], "<description>", "</description>"));
		
		feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
	
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

	printf("COMPLETE\n");
	
   printf("download completed with status code: %d\n", url_complete->status);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	if(url_complete->status >= 200 && url_complete->status <= 226)
	{	
		Feed_Data *p;
		EINA_LIST_FREE(feed_data_list, p)
		{ 
			free(p); 
		}

		printf("LIST COUNT AFTER: %i\n", eina_list_count(feed_data_list));
		
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
			else if(strstr((char *)eina_strbuf_string_get(data), "<feed xmlns=\"http://www.w3.org/2005/Atom\">") != 0)
			{
				printf("ATOM FEED\n");
				parse_atom(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "<rss xmlns:content") != 0)
			{
				printf("RSS1 FEED\n");
				parse_rss(data);
			}
			else if(strstr((char *)eina_strbuf_string_get(data), "<rss xmlns:atom") != 0)
			{
				printf("ATOM1 FEED\n");
				parse_atom1(data);
			}
	}
	else
	{
		edje_object_signal_emit(edje_obj, "reload", "failed");
	}
	
		
		firststart = 1;
// 	create_content(NULL, NULL, NULL,  NULL);
	
	struct tm *newtime;
	time_t long_time;

	time( &long_time );
	newtime = localtime( &long_time );
	
	char buf[PATH_MAX];

	snprintf(buf, sizeof(buf), "%d.%d.%d | %02d:%02d:%02d",newtime->tm_mday, newtime->tm_mon+1, newtime->tm_year+1900, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
	lastcheck = eina_stringshare_add(buf);

// 	ecore_con_url_free(ec_url);
   return ECORE_CALLBACK_DONE;
}


void
_get_data()
{
	Eina_Strbuf *test;
	test = eina_strbuf_new();
		
	Eina_Bool r;
	
	printf("GET DATA\n");
			
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	if(!ci_indicator)
		edje_object_signal_emit(edje_obj, "reload", "visible");

	if(ec_url)
		ecore_con_url_free(ec_url);
	
	ec_url = ecore_con_url_custom_new(ci_url, "GET");
	
	ecore_event_handler_add(ECORE_CON_EVENT_URL_DATA, _url_data_cb, test);
   ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _data_complete, test);
	ecore_con_url_additional_header_add(ec_url, "User-Agent", "Enlightenment News Gadget");

	r = ecore_con_url_get(ec_url);
	
	if (!r)
   {
		printf("could not realize request.\n");
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
	{
		ecore_timer_del(timer);
	}
	
	timer = ecore_timer_add(ci_refresh*60, _get_data_timer, NULL);
}


int elm_main(int argc, char *argv[])
{
	ecore_con_init();
   ecore_con_url_init();
	
   char buf[PATH_MAX];
//    char buf2[PATH_MAX];
//    int gadget = 0;
   char buf1[16];
	
	Config_Item *config;
	config = calloc(1, sizeof(Config_Item));

	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
 
   if (getenv("E_GADGET_ID"))
     {
//         gadget = 1;
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
	
	
	_config_load(edje_obj);							// load config data from eet to tmp vars
	
// 	set_color(ly);
	
	edje_object_color_class_set(edje_obj, "colorclass", ci_r, ci_g, ci_b, ci_a, 255, 255, 255, 0, 39, 90, 187, 255);
	_set_feed_icon();
	_get_data();
	_save_eet();
	
	printf("REFRESH: %0.2lf\n", ci_refresh*60);
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
