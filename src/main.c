#include "news.h"

Evas_Object *win = NULL;
// static    Evas_Object *popup = NULL;
Evas_Object *ly = NULL;
static    Evas_Object *popup = NULL;

	Ecore_Con_Url *ec_url = NULL;
// static E_Gadget_Site_Orient gorient;
// static E_Gadget_Site_Anchor ganchor;

	Eina_Strbuf *feeddata = NULL;

typedef struct {
        Eina_List   *configlist_eet;
} News_List_Eet;


typedef struct {
        int         id;
        const char *url;
		  Eina_Bool   icons;
		  Eina_Bool   bigicons;
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

Eina_List *feed_data_list = NULL;

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
    MY_CONF_SUB_ADD_BASIC(icons, EET_T_UCHAR);
    MY_CONF_SUB_ADD_BASIC(bigicons, EET_T_UCHAR);
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

/*
static void
_delete_id(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
	Eina_List *l;
	Config_Item *list_data;

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
         configlist = eina_list_remove(configlist, list_data);
		}
   }
   printf("DEL ID\n");
    _save_eet();
}*/

static
Eina_Bool _gadget_exit(void *data, int type, void *event_data) 
{
	
	Eina_List *l;
	Config_Item *list_data;
	Ecore_Event_Signal_User *user = event_data;
	
	if ( user->number == 2) 
	{

   EINA_LIST_FOREACH(configlist, l, list_data)
   {
	   if(list_data->id == id_num)
		{
         configlist = eina_list_remove(configlist, list_data);
		}
   }
   printf("DEL ID\n");
	
	_save_eet();
		
	} 
	return EINA_TRUE;
}

/*
static void
orient_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   gorient = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}

static void
anchor_change(void *data, Evas_Object *obj, void *event_info)
{
// 	Evas_Object *ly = data;
   ganchor = (uintptr_t)event_info;
//    update_anchor_orient(data, gorient, ganchor, obj);
}
*/



void
_set_content(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
// 	Evas_Object *ly = data;
//    char buf[64];
	
//    edje_object_part_text_set(ly, "name", ci_name); 
//    edje_object_part_text_set(ly, "unit", ci_unit);
	
	//TODO move calculation in seperat function
// 	edje_object_part_text_set(ly, "value", buf);
		
	printf("set content\n");
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

/*
static void
_it_clicked(void *data EINA_UNUSED, Evas_Object *li,
                 void *event_info EINA_UNUSED)
{
   Elm_Object_Item *lit = elm_list_selected_item_get(li);
   printf("Item clicked. %s is selected\n", elm_object_item_text_get(lit));
}*/


static void
_it_clicked(void *data, Evas_Object *obj,
                 void *event_info EINA_UNUSED)
{
   printf("item was clicked: %s\n", (char *)data);
//    if (!data) return;
   Evas_Object *li = obj;
//    Evas_Object *lb;
//    char str[128];
// 
//    Elm_Object_Item *lit = elm_list_selected_item_get(li);
//    if (!lit) return;
//    sprintf(str, "%s is selected", elm_object_item_text_get(lit));
// 
//    lb = evas_object_data_get(li, "label");
//    elm_object_text_set(lb, str);
// 	
	

   char buf[PATH_MAX];
   snprintf(buf, sizeof(buf), "%s", (char *)data);
   evas_object_smart_callback_call(win, "gadget_open_uri", (char *)data);
}



static void
_reload_start(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   Evas_Object *win = data;
	
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
     }
   _get_data();

}


static Evas_Object *
_tt_icon(void *data, Evas_Object *obj EINA_UNUSED, Evas_Object *tt)
{
   Evas_Object *ic = elm_icon_add(tt);
   char buf[PATH_MAX];
   snprintf(buf, sizeof(buf), "%s.png", (const char*)data);
   elm_image_file_set(ic, buf, NULL);
   elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
   evas_object_resize(ic, 300, 169);
			evas_object_size_hint_min_set(ic, 300, 169);
   return ic;
}



static void
show_popup(void *data, Evas_Object *obj EINA_UNUSED, const char *emission EINA_UNUSED, const char *source EINA_UNUSED)
{
   Evas_Object *win = data;
	Evas_Object *box, *lbl, *li, *scr, *bt, *scroller;
	
	if(eina_list_count(feed_data_list) == 0)
		return;
	
// 	if(!strcmp(edje_object_part_state_get(obj, "reload", NULL), "visible"))
// 		return;
	
   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
        return;
     }

	
   popup = elm_win_add(win, "Popup",  ELM_WIN_POPUP_MENU);
   elm_win_alpha_set(popup, 1);
	

	
   box = elm_box_add(popup);
   elm_box_horizontal_set(box, EINA_FALSE);
//    elm_win_resize_object_add(popup, box);
	
			elm_box_padding_set(box, 10, 0);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0);
   evas_object_show(box);
		
	Evas_Object *o, *boxh, *ic;
	char buf1[PATH_MAX];
	char buf[PATH_MAX];
	
	
		printf("LIST COUNT: %i\n", eina_list_count(feed_data_list));
	Feed_Data *list_data;
	int i = 0;
	Eina_List *l;
	EINA_LIST_FOREACH(feed_data_list, l, list_data)
   {
		
			if(i == 0)
			{
					lbl = elm_label_add(box);
					snprintf(buf1, sizeof(buf1), "<bigger>%s</bigger><br><br><big>%s</big>", list_data->title, list_data->description);
					elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
					elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(400));
					elm_object_text_set(lbl, buf1);
					evas_object_size_hint_align_set(lbl, 0.5, 0.5);
					elm_box_pack_end(box, lbl);
					evas_object_show(lbl);
										
					o = elm_separator_add(box);
					elm_separator_horizontal_set(o, EINA_TRUE);
					elm_box_pack_end(box, o);
					evas_object_show(o);
			}
			else
			{
		   boxh = elm_box_add(box);
			elm_box_horizontal_set(boxh, EINA_TRUE);
			elm_box_padding_set(boxh, 10, 0);
			evas_object_show(boxh);
			
					if(!ci_icons)
					{
					ic = elm_icon_add(boxh);
					if(list_data->imagelink == NULL)
						snprintf(buf, sizeof(buf), "/home/simon/CODING/news/data/themes/images/news.png");
					else
						snprintf(buf, sizeof(buf), "%s.jpg", list_data->imagelink);
					
					elm_image_file_set(ic, buf, NULL);
					evas_object_size_hint_min_set(ic, 150, 56);
					evas_object_size_hint_weight_set(ic, 0, EVAS_HINT_EXPAND);
					evas_object_size_hint_align_set(ic, EVAS_HINT_FILL, EVAS_HINT_FILL);
					elm_box_pack_end(boxh, ic);
					evas_object_show(ic);
					
						if(!ci_bigicons)
						{
							elm_object_tooltip_content_cb_set(ic, _tt_icon, list_data->imagelink, NULL);
						}
					}
					
				   lbl = elm_label_add(boxh);
					snprintf(buf1, sizeof(buf1), "<a href=%s><b>%s</b><br>%s</a><br><br><custom align=right><small>%s</small></custom>", list_data->link, list_data->title, list_data->description, list_data->pubdate);
					elm_label_line_wrap_set(lbl, ELM_WRAP_WORD);
					elm_label_wrap_width_set(lbl, ELM_SCALE_SIZE(300));
					elm_object_text_set(lbl, buf1);
					evas_object_size_hint_align_set(lbl, 0.5, 0.5);
					elm_box_pack_end(boxh, lbl);
					evas_object_show(lbl);
					evas_object_smart_callback_add(ic, "clicked", _it_clicked, list_data->link);
					
					
// 					evas_object_smart_callback_add(lbl, "anchor,clicked", _it_clicked, list_data->link);
								
			elm_box_pack_end(box, boxh);
			evas_object_show(boxh);
			
			o = elm_separator_add(box);
			elm_separator_horizontal_set(o, EINA_TRUE);
			elm_box_pack_end(box, o);
			evas_object_show(o);

			}
		
			i++;
	}
	
	///////////////////


	
//    bt = elm_button_add(box);
//    elm_object_text_set(bt, "refresh");
//    evas_object_smart_callback_add(bt, "pressed", _get_data, NULL);
//    elm_box_pack_end(box, bt);
//    evas_object_show(bt);

	scroller = elm_scroller_add(popup);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(popup, scroller);
   evas_object_show(scroller);
   elm_object_content_set(scroller, box);
   elm_scroller_bounce_set(scroller, EINA_TRUE, EINA_FALSE);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_ON);
   elm_scroller_propagate_events_set(scroller, EINA_TRUE);
   elm_scroller_page_relative_set(scroller, 0, 1);
	
	evas_object_resize(popup, 480, 600);
   evas_object_show(popup);
}


void
set_color(Evas_Object *ly)
{
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

/*
static void
parse_rdf(void *data)
{
	Eina_Strbuf *mybuffer = data;
	
// 	printf("%s : %ld TEST\n", eina_strbuf_string_get(mybuffer), eina_strbuf_length_get(mybuffer));
	
	char **arr, **arr1;
	int i, i1, found = 0;
	int x = 0;
	const char *str;
	str = malloc(sizeof(char) * 1024);

								
   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		arr1 = eina_str_split(arr[i], "\n", 0);
		
	   for (i1 = 0; arr1[i1]; i1++)
		{
			
			
			printf("Link: %s\n", arr1[i1]);
// 		if(strstr((char *)eina_strbuf_string_get(mybuffer), "<rss version=\"2.0\"") != 0)
			if(strstr(arr1[i1], "<title>"))
			{				
// 			remove_space(arr1[i1]);
				stringReplace("<title>", "", arr1[i1]);
				stringReplace("</title>", "", arr1[i1]);
				printf("TITLE: %s\n", arr1[i1]);
				
				data_add->title = eina_stringshare_add(arr1[i1]);
				found = 1;
			}
			else if(strstr(arr1[i1], "<link>"))
			{				
				stringReplace("<link>", "", arr1[i1]);
				stringReplace("</link>", "", arr1[i1]);
// 				printf("Link: %s\n\n", arr1[i1]);
				data_add->link = eina_stringshare_add(arr1[i1]);
			}
			else if(strstr(arr1[i1], "<description>"))
			{				
				stringReplace("<description>", "", arr1[i1]);
				stringReplace("</description>", "", arr1[i1]);
// 				printf("Link: %s\n\n", arr1[i1]);
				data_add->description = eina_stringshare_add(arr1[i1]);
			}
// 			else if(strstr(arr1[i1], "<pubDate>"))
// 			{				
// 				stringReplace("<pubDate>", "", arr1[i1]);
// 				stringReplace("</pubDate>", "", arr1[i1]);
// // 				printf("Link: %s\n\n", arr1[i1]);
// 				data_add->pubdate = eina_stringshare_add(arr1[i1]);
// 			}
// 			else if(eina_str_has_prefix(arr1[i1], "<content:encoded>"))
// 			{
// // 				char buffer[PATH_MAX];
// 				int x = i1;
// 				for(x; arr1[x]; x++)
// 				{
// 					if(eina_str_has_prefix(arr1[x], "]]>"))
// 						break;
// 					
// 					printf("STRING CAT: %s\n", arr1[x]);
// // 					strcat(buffer, arr1[x]);
// 				
// 				}
// 				
// 				printf("\n\n");
// 			}

				
			
		}
		
		
		if(found == 1)
		{
			feed_data_list = eina_list_append(feed_data_list, data_add);
			found = 0;
		}
		
		free(arr1[0]);
		free(arr1);
	}
	
   free(arr[0]);
   free(arr);
	
	   if(popup)
     {
        evas_object_del(popup);
        popup = NULL;
//         return;
     }
	
}
*/


char*
find_data(char *string, char *start1, char *end1)
{
// 	char *string1 = malloc(strlen(string) * sizeof(char));
	char *string1 = calloc(strlen(string)+1, sizeof(char));
		
	int start_len = strlen(start1);

	if((strstr(string, start1) == NULL) || (strstr(string, end1) == NULL))
	{
		return;
	}
	else
	{
// 	printf("TEST: %i\n" ,strstr(string, end1)-strstr(string, start1)-start_len);
	strncpy(string1, strstr(string, start1)+start_len, strstr(string, end1)-strstr(string, start1)-strlen(start1));
	
// 	printf("STRING 1 LENG: %i\n", (int)strlen(string1));
	
	string1[strlen(string1)] = '\0';
// 	str[strLen - 1] = '\0';
	
// 	strstr(string1, ">");
// 	string2 = strtok(string1, ">");
// 	strncpy(string2, strstr(string1, "<")+1, end-start-start_len);
// 	printf("NEU: %s\n", string1);
	
// 		printf("RETURN: %s\n", string1);
	return string1;
	}
	
	free(string1);
// 	free(string2);
}


static void
parse_rss(void *data)
{	
	Eina_Strbuf *mybuffer = data;

	char **arr;
	int i;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
		
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
				data_add->title = eina_stringshare_add(find_data(arr[i], "<title>", "</title>"));
				
				data_add->link = eina_stringshare_add(find_data(arr[i], "<link>", "</link>"));
				
				data_add->description = eina_stringshare_add(find_data(arr[i], "<description>", "</description>"));
				
				data_add->imagelink = eina_stringshare_add(find_data(arr[i], "<img src=\"", ".jpg\""));
				
				data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<pubDate>", "</pubDate>"));
				
// 		printf("TITLE: %s\n", find_data(arr[i], "<title>", "</title>"));
// 		printf("LINK: %s\n", find_data(arr[i], "<link>", "</link>"));
// 		printf("DESC: %s\n\n", find_data(arr[i], "<description>", "</description>"));
// 		printf("IMAGE LINK: %s.jpg\n\n", find_data(arr[i], "<img src=\"", ".jpg\""));
		
			feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
}


static void
parse_atom(void *data)
{	
	Eina_Strbuf *mybuffer = data;
// 	const char *string = eina_strbuf_string_get(mybuffer);

	char **arr;
	int i=0;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<entry>", 0);
	
		
		
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		
				data_add->title = eina_stringshare_add(find_data(arr[i], "<title", "</title>"));
				
				data_add->link = eina_stringshare_add(find_data(arr[i], "<link", "/>"));
				
				data_add->description = eina_stringshare_add(find_data(arr[i], "<summary>", "</summary>"));
				
				data_add->pubdate = eina_stringshare_add(find_data(arr[i], "<updated>", "</updated>"));
				
				printf("TITLE: %s\n", find_data(arr[i], "<title", "</title>"));
				printf("LINK: %s\n", find_data(arr[i], "<link>", "/>"));
				printf("DESC: %s\n", find_data(arr[i], "<summary>", "</summary>"));
				printf("PUBDATE: %s\n\n", find_data(arr[i], "<updated>", "</updated>"));
		
			feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
}


static void
parse_rdf(void *data)
{	
	Eina_Strbuf *mybuffer = data;
// 	const char *string = eina_strbuf_string_get(mybuffer);

	char **arr;
	int i=0;

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
		
		
   for (i = 0; arr[i]; i++)
	{
		
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
// 		strcat(item, arr[i]);
// 		int len = strlen(arr[i]);
// 		char item[len+6];
// 		= "<item>";
// 		item = "<item>";
		
// 		strncat(item, arr[i], len);
// 		printf("TITLE%i: %s\n",i , item);
		
				data_add->title = eina_stringshare_add(find_data(arr[i], "<title>", "</title>"));
				
				data_add->link = eina_stringshare_add(find_data(arr[i], "<link>", "</link>"));
				
				data_add->description = eina_stringshare_add(find_data(arr[i], "<description>", "</description>"));
				
		printf("TITLE: %s\n", find_data(arr[i], "<title>", "</title>"));
		printf("LINK: %s\n", find_data(arr[i], "<link>", "</link>"));
		printf("DESC: %s\n\n", find_data(arr[i], "<description>", "</description>"));
		
			feed_data_list = eina_list_append(feed_data_list, data_add);
	}
	
	free(arr[0]);
   free(arr);
}



/*
static void
parse_rss(Eina_Strbuf *data)
{
	Eina_Strbuf *mybuffer = data;
	
// 	printf("%s : %ld TEST\n", eina_strbuf_string_get(mybuffer), eina_strbuf_length_get(mybuffer));
	
	char **arr, **arr1;
	int i, i1, found = 0;
	int x = 0;
	const char *str;
	str = malloc(sizeof(char) * 1024);

   arr = eina_str_split(eina_strbuf_string_get(mybuffer), "<item>", 0);
	
   for (i = 0; arr[i]; i++)
	{
// 		printf("ARRAY\n");
		Feed_Data *data_add = calloc(1, sizeof(Feed_Data));
		arr1 = eina_str_split(arr[i], "\n", 0);
		
	   for (i1 = 0; arr1[i1]; i1++)
		{
// 		 printf("ARRAY 1 %s\n", arr1[i1]);
			if(eina_str_has_prefix(arr1[i1], "<title>"))
			{
				
				stringReplace("<title>", "", arr1[i1]);
				stringReplace("</title>", "", arr1[i1]);
				printf("TITLE: %s\n", arr1[i1]);
				
				data_add->title = eina_stringshare_add(arr1[i1]);
				found = 1;
			}
			if(eina_str_has_prefix(arr1[i1], "<link>"))
			{				
				stringReplace("<link>", "", arr1[i1]);
				stringReplace("</link>", "", arr1[i1]);
// 				printf("Link: %s\n\n", arr1[i1]);
				data_add->link = eina_stringshare_add(arr1[i1]);
			}
			if(eina_str_has_prefix(arr1[i1], "<description>"))
			{				
				stringReplace("<description>", "", arr1[i1]);
				stringReplace("</description>", "", arr1[i1]);
// 				printf("Link: %s\n\n", arr1[i1]);
				data_add->description = eina_stringshare_add(arr1[i1]);
			}
			if(eina_str_has_prefix(arr1[i1], "<pubDate>"))
			{				
				stringReplace("<pubDate>", "", arr1[i1]);
				stringReplace("</pubDate>", "", arr1[i1]);
// 				printf("Link: %s\n\n", arr1[i1]);
				data_add->pubdate = eina_stringshare_add(arr1[i1]);
			}
// 			if(eina_str_has_prefix(arr1[i1], "<img src=""))
// 			{				
// 				stringReplace("<img src="", "", arr1[i1]);
// 				stringReplace("</pubDate>", "", arr1[i1]);
// // 				printf("Link: %s\n\n", arr1[i1]);
// 				data_add->image = eina_stringshare_add(arr1[i1]);
// 			}
			
		}
		
		
		if(found == 1)
		{
			feed_data_list = eina_list_append(feed_data_list, data_add);
			found = 0;
		}
		
		free(arr1[0]);
		free(arr1);
	}
	
   free(arr[0]);
   free(arr);
	
// 	   if(popup)
//      {
//         evas_object_del(popup);
//         popup = NULL;
// //         return;
//      }
// 		eina_strbuf_free(feeddata);
}
*/

static Eina_Bool
_url_data_cb(void *data EINA_UNUSED, int type EINA_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Data *url_data = event_info;

	feeddata = eina_strbuf_new();
	
// 	Eina_Strbuf *feeddata1 = NULL;
// 	feeddata1 = eina_strbuf_new();
	

	eina_strbuf_append_length(feeddata, ((const char*) &url_data->data[0]), url_data->size);
	
// 	feeddata1 = feeddata;

	parse_rss(feeddata);

/*
	if(strstr((char *)eina_strbuf_string_get(feeddata), "<rss") != 0)
	{
		printf("RSS FEED\n");
		parse_rss(feeddata);
	}
	else if(strstr((char *)eina_strbuf_string_get(feeddata), "<rdf:RDF") != 0)
	{
		printf("RDF FEED\n");
		parse_rdf(feeddata);
	}
	else if(strstr((char *)eina_strbuf_string_get(feeddata), "<feed xmlns=\"http://www.w3.org/2005/Atom\">") != 0)
	{
		printf("ATOM FEED\n");
		parse_atom(feeddata);
	}
	*/
// 	printf("%s : %ld\n", eina_strbuf_string_get(feeddata), eina_strbuf_length_get(feeddata));
//    int i;
//    for (i = 0; i < url_data->size; i++)
//      printf("%c", url_data->data[i]);
	
// 	eina_strbuf_reset(feeddata);
	
   return ECORE_CALLBACK_PASS_ON;
}


static Eina_Bool
_data_complete(void *data EINA_UNUSED, int type, void *event_info)
{
	
	Ecore_Con_Event_Url_Complete *url_complete = event_info;


	printf("COMPLETE\n");
	
   printf("download completed with status code: %d\n", url_complete->status);
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	
	if(url_complete->status >= 200 && url_complete->status <= 226)
		edje_object_signal_emit(edje_obj, "reload", "default");
	else
		edje_object_signal_emit(edje_obj, "reload", "failed");
	
	
// 	printf("%s : %ld COMPLETE\n", eina_strbuf_string_get(feeddata), eina_strbuf_length_get(feeddata));
	
	
// 			parse_rss(feeddata);
/*
	if(strstr((char *)eina_strbuf_string_get(feeddata), "<rss") != 0)
	{
// 		printf("RSS FEED:%s\n", eina_strbuf_string_get(feeddata));
		parse_rss(feeddata);
// 		eina_list_free(feed_data_list);
	}
	else	if(strstr((char *)eina_strbuf_string_get(feeddata), "<rdf:RDF") != 0)
	{
// 		printf("RDF FEED:%s\n", eina_strbuf_string_get(feeddata));
		parse_rdf(feeddata);
	}
	else if(strstr((char *)eina_strbuf_string_get(feeddata), "<feed xmlns=\"http://www.w3.org/2005/Atom\">") != 0)
	{
// 		printf("ATOM FEED\n");
		parse_atom(feeddata);
	}*/

//    ecore_con_url_free(d->url);
//    eina_strbuf_free(feeddata);
   return ECORE_CALLBACK_DONE;
}


void
_get_data()
{
	Eina_Bool r;
					
   ecore_con_init();
   ecore_con_url_init();
	
	
	Evas_Object *edje_obj = elm_layout_edje_get(ly);
	edje_object_signal_emit(edje_obj, "reload", "visible");

	if(ec_url)
		ecore_con_url_free(ec_url);
	
	ec_url = ecore_con_url_custom_new(ci_url, "GET");
	
	ecore_event_handler_add(ECORE_CON_EVENT_URL_DATA, _url_data_cb, NULL);
   ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _data_complete, NULL);
	ecore_con_url_additional_header_add(ec_url, "User-Agent", "Enlightenment News Gadget");

	r = ecore_con_url_get(ec_url);
	
	if (!r)
   {
		printf("could not realize request.\n");
      ecore_con_url_free(ec_url);
	}
	else
	{
		printf("free: \n");
		
		   Feed_Data *o;
			EINA_LIST_FREE(feed_data_list, o);
			free(o);
			
			feed_data_list = NULL;
	}
	
	
// 	ecore_con_url_free(ec_url);
//    ecore_con_url_shutdown();
//    ecore_con_shutdown();
}


int elm_main(int argc, char *argv[])
{
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
	
// 	evas_object_smart_callback_add(win, "gadget_site_orient", orient_change, ly);
//    evas_object_smart_callback_add(win, "gadget_site_anchor", anchor_change, ly);
   evas_object_smart_callback_add(win, "gadget_configure", _settings_1, edje_obj);
	
   edje_object_signal_callback_add(ly, "show_popup", "show_popup", show_popup, win);
   edje_object_signal_callback_add(ly, "reload", "reload", _reload_start, win);
//    edje_object_signal_callback_add(ly, "show_popup", "show_popup", _get_data, NULL);
	
//    edje_object_signal_callback_add(ly, "delete_popup", "delete_popup", delete_popup_edje, win);
//    evas_object_smart_callback_add(win, "gadget_removed", _delete_id, NULL);
	ecore_event_handler_add(ECORE_EVENT_SIGNAL_USER, _gadget_exit, NULL);
	
	
	_config_load(ly);							// load config data from eet to tmp vars
	
	set_color(edje_obj);
	
// 	_set_content(edje_obj, NULL, NULL, NULL);
	_get_data();

	_save_eet();
  //run app RUN!
  elm_run();
  
	ecore_con_url_free(ec_url);
   ecore_con_url_shutdown();
   ecore_con_shutdown();
	eina_list_free(feed_data_list);
  //shutdown!
//         _my_conf_descriptor_shutdown();
  return 0;
}

ELM_MAIN();