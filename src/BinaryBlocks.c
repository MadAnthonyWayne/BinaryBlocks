/*binary clock with small date at bottom, I like squares instead of circles though.
 *Largely based on the just_a_bit watch by the Pebble Team, binary clocks are cool!
 *Also used the awesome small date from Paul Pullen and his Text with Small Date watchface (http://www.mypebblefaces.com/view?fID=1350&aName=PEP&pageTitle=Text+with+Small+Date&auID=1585) 

*/

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xA1, 0x22, 0xD0, 0x9F, 0x23, 0x1D, 0x4C, 0x27, 0xA2, 0xA0, 0xC7, 0x26, 0xE4, 0xFB, 0x14, 0x1A }
PBL_APP_INFO(MY_UUID, "Binary Blocks", "Andy Alvarez", 0x4, 0x0, DEFAULT_MENU_ICON, APP_INFO_WATCH_FACE);

Window window;

Layer display_layer;		//binary blocks
TextLayer TopDayLayer;		//Weekday Layer (used to be on top but didn't fit)
TextLayer BottomDayLayer;	//MonthDay Layer

PblTm t;

//Days of the week
static const char* const WEEKDAYS[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};

//Days of the month
static const char* const DAYS[] = {
	"",
	"the first",
	"the second",
	"the third",
	"the fourth",
	"the fifth",
	"the sixth",
	"the seventh",
	"the eighth",
	"the ninth",
	"the tenth",
	"the eleventh",
	"the twelfth",
	"the thirteenth",
	"the fourteenth",
	"the fifteenth",
	"the sixteenth",
	"the seventeenth",
	"the eighteenth",
	"the nineteenth",
	"the twentieth",
	"the twenty-first",
	"the twenty-second",
	"the twenty-third",
	"the twenty-fourth",
	"the twenty-fifth",
	"the twenty-sixth",
	"the twenty-seventh",
	"the twenty-eighth",
	"the twenty-ninth",
	"the thirtieth",
	"the thirty-first"
};


static bool textInitialized = false; //neccessary?

void draw_cell(GContext* ctx, int x, int y, int w, int h, bool filled) {
  // Each "cell" represents a binary digit or 0 or 1.

  graphics_context_set_fill_color(ctx, GColorWhite);  //fill blocks with white

	graphics_fill_rect(ctx, GRect(x,y,w,h), 0, GCornerNone); //draw square white

  if (!filled) { //if not filled color it black
    graphics_context_set_fill_color(ctx, GColorBlack);  

	graphics_fill_rect(ctx, GRect(x,y,w,h), 0, GCornerNone); //draw square black
  }

}
//number of cells for the minute columns
#define CellsPerRow 8		//Minutes are 8x8
#define CellsPerColumn 8

//figure which cells to draw for minutes
void draw_cell_column_for_digit(GContext* ctx, unsigned short digit, unsigned short max_rows_to_display, unsigned short cell_column) {
	
	for (int cell_row_index = 0; cell_row_index < max_rows_to_display; cell_row_index++) {
		draw_cell(ctx, 36 + cell_column*36, 18 + (3 - cell_row_index)*36, 18, 18, (digit >> cell_row_index) & 0x1); 
	}
}

//figure which cells to draw for hour (hour cells are bigger than minutes)
void draw_cell_column_for_hour_digit(GContext* ctx, unsigned short digit, unsigned short max_rows_to_display, unsigned short cell_column) {

	for (int cell_row_index = 0; cell_row_index < max_rows_to_display; cell_row_index++) {
		draw_cell(ctx, 21, 12 + (3 - cell_row_index)*36, 30, 30, (digit >> cell_row_index) & 0x1); 
	}
}



// The cell column offsets for each digit
#define HourOffset 0
#define MinuteOneOffset 1
#define MinuteTwoOffset 2



// The maximum number of cell columns to display
// (Used so that if a binary digit can never be 1 then no un-filled
// placeholder is shown.)

#define DefaultMaxRows 4
#define MaxRowsMinuteOne 3


unsigned short get_display_hour(unsigned short hour) {

  if (clock_is_24h_style()) {
    return hour;
  }

  unsigned short display_hour = hour % 12;

  // Converts "0" to "12"
  //still trying to figure this out, I don't think it ever worked, so if the hour is 12 no cells are diplayed
  display_hour = (display_hour == 0 ? 12 : display_hour);
  
  return display_hour; // ? display_hour : 12;

}

//update cells and date
void display_layer_update_callback(Layer *me, GContext* ctx) {

  PblTm t;

  get_time(&t);

  unsigned short display_hour = get_display_hour(t.tm_hour);

	//draw hour cells
  draw_cell_column_for_hour_digit(ctx, display_hour % 12, DefaultMaxRows, HourOffset);
	//draw minute cells
  draw_cell_column_for_digit(ctx, t.tm_min / 10, MaxRowsMinuteOne, MinuteOneOffset);
  draw_cell_column_for_digit(ctx, t.tm_min % 10, DefaultMaxRows, MinuteTwoOffset);

}

void display_date(PblTm *t)
{
	//see if updating date is neccessary
	if(strcmp(TopDayLayer.text, WEEKDAYS[t->tm_wday]) != 0){
		text_layer_set_text(&TopDayLayer, WEEKDAYS[t->tm_wday]);
		text_layer_set_text(&BottomDayLayer, DAYS[t->tm_mday]);
	}
}


//set initial date
void initial_date(PblTm *t)
{
	text_layer_set_text(&TopDayLayer, WEEKDAYS[t->tm_wday]);
	text_layer_set_text(&BottomDayLayer, DAYS[t->tm_mday]);
}

//configure top day layer (weekday)
void configureTopDayLayer(TextLayer *textlayer)
{
	text_layer_init(&TopDayLayer, GRect(0, 150, 72, 18));
	text_layer_set_font(textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_color(textlayer, GColorWhite);
	text_layer_set_background_color(textlayer, GColorClear);
	text_layer_set_text_alignment(textlayer, GTextAlignmentRight);
}

//configure bottom day (month day)
void configureBottomDayLayer(TextLayer *textlayer)
{
	text_layer_init(&BottomDayLayer, GRect(73, 150, 71, 18));
	text_layer_set_font(textlayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_color(textlayer, GColorWhite);
	text_layer_set_background_color(textlayer, GColorClear);
	text_layer_set_text_alignment(textlayer, GTextAlignmentLeft);
}


void handle_init(AppContextRef ctx) {

  window_init(&window, "Binary Blocks");
  window_stack_push(&window, true);
	//set background color black
  window_set_background_color(&window, GColorBlack);

	// Init the layer for the display: removed window.layer.frame replaced with GRect
	layer_init(&display_layer, GRect(0, 0, 144, 149));
	display_layer.update_proc = &display_layer_update_callback;
	layer_add_child(&window.layer, &display_layer);

	//initiate topday layer (weekday)
	configureTopDayLayer(&TopDayLayer);
	
	//bottom day layer (month)
	configureBottomDayLayer(&BottomDayLayer);
		
	//set initial date
	get_time(&t);
	initial_date(&t);	
		
	//add child layers to show date
	layer_add_child(&window.layer, &TopDayLayer.layer);
	layer_add_child(&window.layer, &BottomDayLayer.layer);


}

//update times
void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {

    layer_mark_dirty(&display_layer); 
	display_date(t->tick_time);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
