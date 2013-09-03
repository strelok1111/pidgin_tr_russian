#define TRANSLITERATE_RUSSIAN_PLUGIN_ID "gtk-strelok-transliterate_russian"
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "internal.h"
#include "pidgin.h"

#include "debug.h"
#include "signals.h"
#include "util.h"
#include "version.h"

#include "gtkplugin.h"
#include "gtkprefs.h"
#include "gtkutils.h"
#include "gtkconvwin.h"
#include <string.h>

const char *FROM = "qwertyuiop[]asdfghjkl;'zxcvbnm,./QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?@#$%^&";
const char *TO[] = {"й","ц","у","к","е","н","г","ш","щ","з","х","ъ","ф","ы","в","а","п","р","о","л","д","ж","э","я","ч","с","м","и","т","ь","б","ю",".","Й","Ц","У","К","Е","Н","Г","Ш","Щ","З","Х","Ъ","/","Ф","Ы","В","А","П","Р","О","Л","Д","Ж","Э","Я","Ч","С","М","И","Т","Ь","Б","Ю",",","\\","\"","№",";","%",":","?"};
const long CHAR_NOT_FOUND = -1;

long str_char_pos(const char *,char);
char *transliterate(char *);

long str_char_pos(const char *str,char ch){
	char *temp = strrchr(str,ch);
	if(temp == NULL){
		return CHAR_NOT_FOUND;
	}else{
		return temp - str;
	}
}

char *transliterate(char *str){
	char *tmp = str;
	long total_length = 0;
	char *result;
	while(*tmp != '\0'){
		long pos = str_char_pos(FROM,*tmp);
		if(pos != CHAR_NOT_FOUND) total_length += strlen(TO[pos]);
		else total_length++;
		tmp++;
	}
	tmp = str;
	result = (char *)g_malloc(total_length + 1);
	result[0] = '\0';
	while(*tmp != '\0'){
		int pos = str_char_pos(FROM,*tmp);
		if(pos != CHAR_NOT_FOUND){
			strcat(result,TO[pos]);
		}else{
			char temp_char[2] = {*tmp,'\0'};
			strcat(result,temp_char);
		}
		tmp++;
	}
	return result;
}

PurplePlugin *transliterate_russian_plugin_handle = NULL;

gboolean on_key_press_event (GtkWidget *, GdkEventKey *, gpointer);

gboolean on_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data){
	if ((event -> state & GDK_CONTROL_MASK) && event->hardware_keycode == 27){ // R button
		GtkTextIter start, end;
		GtkTextBuffer *buffer = (GtkTextBuffer *)user_data;
		gchar *text;
		char *tr_text;

		gtk_text_buffer_get_bounds (buffer, &start, &end);
		text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
		tr_text = transliterate((char *)text);
		gtk_text_buffer_set_text(buffer,(gchar *)tr_text,strlen(tr_text));
	}
	return FALSE; 
}

static void transliterate_russian_attach_conv(PurpleConversation *conv){
  PidginConversation *gtkconv;
  GtkTextView *view;
  GtkTextBuffer *buffer;

  gtkconv = PIDGIN_CONVERSATION(conv);
  view = GTK_TEXT_VIEW(gtkconv->entry);
  buffer = gtk_text_view_get_buffer(view);
  g_signal_connect(G_OBJECT(gtkconv->entry), "key_press_event", G_CALLBACK(on_key_press_event), buffer);

}

static void transliterate_russian_remove_conv(PidginConversation *gtkconv){
  g_signal_handlers_disconnect_matched(G_OBJECT(gtkconv->entry),G_SIGNAL_MATCH_FUNC, 0, 0, NULL, G_CALLBACK(on_key_press_event), NULL);
}

static gboolean plugin_load(PurplePlugin *plugin) {
    void *conv_handle = purple_conversations_get_handle();
    GList *convs;
    for (convs = purple_get_conversations(); convs != NULL; convs = convs->next){
		transliterate_russian_attach_conv((PurpleConversation *)convs->data);
	}

	purple_signal_connect(conv_handle, "conversation-created",plugin, PURPLE_CALLBACK(transliterate_russian_attach_conv), NULL);

    transliterate_russian_plugin_handle = plugin;

    return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin){
  GList *convs;

  for (convs = purple_get_conversations(); convs != NULL; convs = convs->next){
    PurpleConversation *conv = (PurpleConversation *)convs->data;
    if(PIDGIN_IS_PIDGIN_CONVERSATION(conv)){
      transliterate_russian_remove_conv(PIDGIN_CONVERSATION(conv));
    }
  }

  transliterate_russian_plugin_handle = NULL;

  return TRUE;
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    PIDGIN_PLUGIN_TYPE,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,
    TRANSLITERATE_RUSSIAN_PLUGIN_ID,
    "Transliterate symbols into russian",
    "1.1",

    "Transliterate Plugin",          
    "Transliterate Plugin",          
    "Strelok <info@strelok.us>",                          
    "http://transliterate.strelok.us",     
    
    plugin_load,                   
    plugin_unload,                   
    NULL,                                                      
    NULL,                          
    NULL,                          
    NULL,                        
    
    NULL,                   
    
    NULL,                          
    NULL,                          
    NULL,                          
    NULL                           
};                               
    
static void init_plugin(PurplePlugin *plugin){                              
}

PURPLE_INIT_PLUGIN(transliterate_russian, init_plugin, info)
