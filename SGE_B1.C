/********************************
 *                              *
 *   Simple graphic edit        *
 *                              *
 *   Beta v1.000                *
 *                              *
 ********************************/



#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <math.h>



#define ERROR_MOUSE_DRIVER "ERROR [0x0000]: mouse driver not install !"

#define PROG_NAME       "Simple Graphic Editor"
#define PROG_VER	"Beta v1.000"
#define LOAD_CAP	"Loading..."
#define LOAD_PIC	0xdb



// Boolean type
enum bool_ {false = 0, true = 1};
typedef enum bool_ bool;



//
// Elements UI identifers
//
enum ElemUI_ID_ {
		EUI_CANVAS         = 0,
		EUI_PALETTE        = 1,
		EUI_BTN_FREE_PAINT = 2,
		EUI_BTN_LINE_PAINT = 3,
		EUI_BTN_RECT_PAINT = 4,
		EUI_BTN_TRIA_PAINT = 5,
		EUI_BTN_CIRC_PAINT = 6
};
typedef enum ElemUI_ID_ ElemUI_ID;



//
// User interface color
//
enum ColorUI_ {
	       CUI_MENU_BORDER       = 20,
	       CUI_PALETTE_BORDER    = 22,
	       CUI_PALETTE_ACTIVE    = 57,
	       CUI_CANVAS            = 31,
	       CUI_BTN_FILL	     = 22,
	       CUI_BTN_BORDER	     = 23,
	       CUI_BTN_BORDER_ACTIVE = 55,
	       CUI_BTN_ICON          = 23,
	       CUI_MENU_FILL         = 19,
	       CUI_CURSOR            = 0,
	       CUI_COMB_HINT	     = 0x0f,
	       CUI_COLOR_PALETTE_MIN = 31,
	       CUI_COLOR_PALETTE_MAX = 56
};
typedef enum ColorUI_ ColorUI;



//
// User interface elements sizes
//
enum SizeUI_ {
		SUI_MENU_BORDER       = 2,
		SUI_TMENU_HEIGHT      = 26,
		SUI_BMENU_HEIGHT      = 16,
		SUI_CANVAS_HEIGHT     = 150,
		SUI_COLOR_SQUARE_SIZE = 6,
		SUI_BTN		      = 60,
		SUI_BTN_BORDER	      = 2,
		SUI_BTN_SEP	      = 2
};
typedef enum SizeUI_ SizeUI;



//
// Element activity state
//
enum ActiveState_ {AS_OFF, AS_ON, AS_NULL};
typedef enum ActiveState_ ActiveState;



//
// Graphic mode {
//	height	    - screen height
//	width  	    - screen width
//	number 	    - graphic mode number
//	color_count - count color in graphic mode
// }
//
struct GraphMode_ {
	unsigned short height;
	unsigned short width;
	unsigned char  number;
	unsigned short color_count;
};
typedef struct GraphMode_ GraphMode;



//
// Element user interface {
// 	y       - y position
//	x       - x position
//	height  - element height
//	width   - element width
//	factive - flag active
//	id	- element identifer
// }
//
struct ElemUI_ {
	unsigned short y;
	unsigned short x;
	unsigned short height;
	unsigned short width;
	ActiveState    factive;
	ElemUI_ID      id;
};
typedef struct ElemUI_ ElemUI;



//
// Mouse cursor {
// 	y           - y position
//      x           - x position
//      prevy(1)    - previons y position
//	prevx(1)    - previons x position
//      color       - current cursor color
//	prevcolor   - previons point color
//      lbc_count    - left button click count
//
// }
//
struct Cursor_ {
	unsigned short y;
	unsigned short x;
	unsigned short prevy;
	unsigned short prevx;
	unsigned short prevy1;
	unsigned short prevx1;
	unsigned short color;
	unsigned short prevcolor;
	unsigned char  lbc_count;
};
typedef struct Cursor_ Cursor;



//
// Colors Palette {
// 	cur   - current color
//	count - colors count
//      min   - number minimal color
//	max   - number maximal color
// }
//
struct ColorPalette_ {
	unsigned short cur;
	unsigned short count;
	unsigned short min;
	unsigned short max;
};
typedef struct ColorPalette_ ColorPalette;



//
// Palete UI {
// 	color - colors pallete
//	ui    - property user interface elemet
// }
//
struct Palette_ {
	ColorPalette color;
	ElemUI       ui;
};
typedef struct Palette_ Palette;



void usswap(unsigned short* a, unsigned short* b);

void elements_init(void);

void far onmouse_event(void);
bool     mouse_in_tmenu(void);
bool	 mouse_in_canvas(void);
bool	 mouse_in_palette(void);
void     mouse_init   (void);
void     mouse_free   (void);

void run        (void);
void load_screen(void);

void trigger_error(char* message);

void graph_init    (void);
void out_ui        (void);
void out_palette   (void);
void out_comb_hint (void);
void out_btn	   (void);
void out_all_colors(void);
void clear_canvas  (void);
void out_text	   (unsigned short y,      unsigned short x,
		    unsigned short color,  char* text,
		    unsigned char  page);
void out_pixel     (unsigned short y,      unsigned short x,
		    unsigned char  color,  unsigned char page);
void out_fill_rect (unsigned short y1,     unsigned short x1,
		    unsigned short y2,     unsigned short x2,
		    unsigned short color,  unsigned char page);
void out_rect      (unsigned short y1,     unsigned short x1,
		    unsigned short y2,     unsigned short x2,
		    unsigned short color,  unsigned char page);
void out_line      (unsigned short y1,     unsigned short x1,
		    unsigned short y2,     unsigned short x2,
		    unsigned short color,  unsigned char page);
void out_circle    (unsigned short y,      unsigned short x,
		    unsigned short radius, unsigned short color,
		    unsigned char  page);
void select_color  (ActiveState active_state);
void select_btn    (ActiveState active_state);
unsigned short read_color (unsigned short y, unsigned short x,
			   unsigned char page);



// Canvas for paint
ElemUI canvas;

// Button for free paint
ElemUI btn_free_paint;

// Button for line paint
ElemUI btn_line_paint;

// Button for triangle paint
ElemUI btn_tria_paint;

// Button for circle paint
ElemUI btn_circ_paint;

// Button for rectangle paint
ElemUI btn_rect_paint;

// Current active button
ElemUI* cur_btn;

// Use VGA 320x200 8bit Graphic mode
GraphMode graph_mode;

// Mouse cursor
Cursor cursor;

// Palette colors
Palette palette;

short main_sp;                  // Main ss
short main_ss;                  // Main sp
short main_bp;			// Main bp
short driver_sp;                // Driver sp
short driver_ss;		// Driver ss
short driver_bp;		// Driver bp





int main(void)
{
	run();

	return EXIT_SUCCESS;
}





/********************
 *                  *
 *  Others library  *
 *                  *
 ********************/



//
// Spaw 2 unsigned short
//
// a - word 1
// b - word 2
//
void usswap (unsigned short* a, unsigned short* b)
{
	unsigned short temp;

	temp = *a;
	*a = *b;
	*b = temp;
}





/********************
 *                  *
 *  Mouse library   *
 *                  *
 ********************/



//
// Function initialization mouse
//
void mouse_init(void)
{
	extern GraphMode graph_mode;	// Graphic mode
	extern Cursor    cursor;        // Mouse cursor
	unsigned short   vcenter;	// Vertical center
	unsigned short   hcenter;	// Horizontal center
	unsigned short   width;		// Screen width
	unsigned short   height;	// Screen height

	vcenter            = graph_mode.height/2;
	hcenter            = graph_mode.width/2;
	cursor.color       = CUI_CURSOR;
	cursor.prevcolor   = CUI_CANVAS;
	cursor.x	   = hcenter;
	cursor.y	   = vcenter;
	cursor.lbc_count   = 0;
	width		   = graph_mode.width;
	height             = graph_mode.height;

	asm {
		// Initialization mouse driver

		xor	ax, ax
		int	33h

		// Mouse driver install

		xor	ax, 0ffffh
		jnz	DRIVER_ERROR

		// Hidden mouse cursor

		mov	ax, 0002h
		int	33h

		// Set mouse max and min horizontal positions

		mov	ax, 0007h
		mov	cx, 0
		mov     dx, width
		dec	dx
		int	33h

		// Set mouse max and min horizontal positions

		mov	ax, 0008h
		mov	cx, 0
		mov	dx, height
		dec	dx
		int	33h

		// Set mouse sensetive options

		mov	ax, 001ah
		mov	bx, 1
		mov	cx, 1
		mov	dx, 1
		int	33h

		// Set cursor position

		mov	ax, 0004h
		mov	cx, word ptr hcenter
		mov	dx, word ptr vcenter
		int	33h
	}

		// Paint cursor
		out_pixel(cursor.y, cursor.x, cursor.color, 0);

	asm{
		// Set mouse event function

		push	es
		mov	ax, 0ch
		mov     cx, 11b
		mov	dx, seg onmouse_event
		mov	es, dx
		mov	dx, offset onmouse_event
		int	33h
		pop	es

		jmp	EXIT
	}

DRIVER_ERROR:

	trigger_error(ERROR_MOUSE_DRIVER);

EXIT:
}





//
// Function free mouse
//
void mouse_free(void)
{
	asm {
		xor	ax, ax
		int	33h

		mov	ax, 000ch
		xor	cx, cx
		int	33h
	}
}





//
// Function event mouse
//
// AX - call expression
// BX - buttons state
// CX - position X
// DX - position Y
// SI - counter last move horizontal
// DI - counter last move vertical
// DS - segment data mouse driver
//
void far onmouse_event(void)
{
	extern short  main_ss;	   // Main ss
	extern short  main_sp;     // Main sp
	extern short  main_bp;	   // Main bp
	extern short  driver_ss;   // Driver ss
	extern short  driver_sp;   // Driver sp
	extern short  driver_bp;   // Driver bp
	extern Cursor cursor;      // Mouse cursor

	asm {
		// Save driver segments

		push	ds
		push    es

		// Save driver stack

		mov	ax, ss
		mov	word ptr cs:driver_ss, ax
		mov	word ptr cs:driver_sp, sp
		mov	word ptr cs:driver_bp, bp

		// Load main stack addresses

		cli
		mov	ax, word ptr cs:main_ss
		mov	ss, ax
		mov	sp, word ptr cs:main_sp
		mov	bp, word ptr cs:main_bp

		// Defense main stack data

		sub	sp, 0ffh
		sub	bp, 0ffh
		sti

		// Preparing main programm segments

		push	cs
		pop	ds
		push    cs
		pop	es

		push	bx
		push	dx
		push    cx
	}

	// Paint previons color point
	out_pixel(cursor.y, cursor.x, cursor.prevcolor, 0);

	asm {
		// Read current cursor position

		pop	cx
		pop	dx
		mov	word ptr cursor, dx
		mov	word ptr cursor+2, cx

		// Read current point color

		mov	ah, 0dh
		xor	bh, bh
		int	10h

		xor	ah, ah
		mov	word ptr cursor.prevcolor, ax
	}

	// Paint cursor
	out_pixel(cursor.y, cursor.x, cursor.color, 0);

	asm {
		// If mouse button not pressed

		pop	bx
		cmp	bx, 1
		jnz     EXIT
	}

	// Paint only in canvas
	if (mouse_in_canvas())
		goto EXIT;

	// Select color in palette
	if (mouse_in_palette())
		goto EXIT;

	// In top menu
	if (mouse_in_tmenu())
		goto EXIT;

EXIT:

	asm {
		// Load driver stack

		cli
		mov	ax, word ptr cs:driver_ss
		mov	ss, ax
		mov	sp, word ptr cs:driver_sp
		mov	bp, word ptr cs:driver_bp
		sti

		// Load driver segments

		pop	es
		pop	ds
	}
}





//
// Process and check mouse cursor in canvas
//
// return true  - mouse cursor in canvas
//	  false - else
//
bool mouse_in_canvas(void)
{
	extern Cursor    cursor;           // Mouse cursor
	extern Palette   palette;          // Current palette
	extern GraphMode graph_mode;	   // Graphic mode
	extern ElemUI    canvas;           // Canvas for paint
	extern ElemUI*   cur_btn;	   // Current active button

	if ((cursor.x>=canvas.x) && (cursor.x<canvas.x+canvas.width) &&
	    (cursor.y>=canvas.y) && (cursor.y<canvas.y+canvas.height)) {

		// Action depend press paint button
		switch (cur_btn->id) {

		case EUI_BTN_FREE_PAINT:
			cursor.prevcolor = palette.color.cur;
			break;

		case EUI_BTN_LINE_PAINT:
			if (!cursor.lbc_count) {
				cursor.
				lbc_count = 1;
				cursor.prevy = cursor.y;
				cursor.prevx = cursor.x;
			}
			else if ((cursor.x!=cursor.prevx) ||
				 (cursor.y!=cursor.prevy)){

				out_line(cursor.prevy, cursor.prevx,
					 cursor.y, cursor.x,
					 palette.color.cur, 0);
				cursor.lbc_count = 0;
			}
			break;

		case EUI_BTN_RECT_PAINT:
			if (!cursor.lbc_count) {
				cursor.lbc_count = 1;
				cursor.prevy = cursor.y;
				cursor.prevx = cursor.x;
			}
			else if ((cursor.x!=cursor.prevx) ||
				 (cursor.y!=cursor.prevy)) {

				out_rect(cursor.prevy, cursor.prevx,
					 cursor.y, cursor.x,
					 palette.color.cur, 0);
				cursor.lbc_count = 0;
			}
			break;

		case EUI_BTN_TRIA_PAINT:
			switch (cursor.lbc_count) {

			case 0:
				cursor.lbc_count = 1;
				cursor.prevy = cursor.y;
				cursor.prevx = cursor.x;
				break;
			case 1:
				if ((cursor.y == cursor.prevy) &&
				    (cursor.x == cursor.prevx)) {

					cursor.lbc_count = 0;
				}
				else {
					cursor.lbc_count = 2;
					cursor.prevy1 = cursor.y;
					cursor.prevx1 = cursor.x;
				}
				break;
			case 2:
				if ((cursor.y != cursor.prevy1) ||
				    (cursor.x != cursor.prevx1)) {

					out_line(cursor.prevy,  cursor.prevx,
						 cursor.prevy1, cursor.prevx1,
						 palette.color.cur, 0);
					out_line(cursor.prevy1, cursor.prevx1,
						 cursor.y,      cursor.x,
						 palette.color. cur, 0);
					out_line(cursor.y,  cursor.x,
						 cursor.prevy, cursor.prevx,
						 palette.color. cur, 0);
				}
				cursor.lbc_count = 0;
				break;
			}
			break;

		case EUI_BTN_CIRC_PAINT:
			if (!cursor.lbc_count) {
				cursor.lbc_count = 1;
				cursor.prevy = cursor.y;
				cursor.prevx = cursor.x;
			}
			else if ((cursor.x!=cursor.prevx) ||
				 (cursor.y!=cursor.prevy)) {

				unsigned short radius;	// Radius circle
				radius = sqrt(pow(abs(cursor.x-cursor.prevx), 2) +
					      pow(abs(cursor.y-cursor.prevy), 2));

				// Size circle only < screen
				if (((int)(cursor.prevx-radius) < 0)          ||
				    (cursor.prevx+radius >= graph_mode.width) ||
				    ((int)(cursor.prevy-radius) <  canvas.y)  ||
				    (cursor.prevy+radius >= canvas.y+canvas.height)) {
					// Imposible paint circle > screen
					cursor.lbc_count = 0;
				}
				else {
					// Paint circle
					out_circle(cursor.prevy, cursor.prevx,
						   radius,
						   palette.color.cur, 0);
					cursor.lbc_count = 0;
				}
			}
			break;

		}

		cursor.prevcolor = palette.color.cur;
		return true;
	}

	cursor.lbc_count = 0;
	return false;
}





//
// Process and check mouse cursor in palette
//
// return true  - mouse cursor in palette
//	  false - else
//
bool mouse_in_palette(void)
{
	extern Cursor  cursor;             // Mouse cursor
	extern Palette palette;            // Current palette

	if ((cursor.x>=palette.ui.x) && (cursor.x<palette.ui.x+palette.ui.width) &&
	    (cursor.y>=palette.ui.y) && (cursor.y<palette.ui.y+palette.ui.height)) {

		if ((cursor.prevcolor != CUI_PALETTE_BORDER) &&
		    (cursor.prevcolor != CUI_PALETTE_ACTIVE)) {

			select_color(AS_OFF);
			palette.color.cur = cursor.prevcolor;
			select_color(AS_ON);
		}
		return true;
	}

	return false;
}





//
// Process and check mouse cursor in top menu
//
// return true  - mouse cursor in top menu
//        false - else
//
bool mouse_in_tmenu(void)
{
	extern Cursor cursor;	           // Mouse cursor
	extern ElemUI  btn_free_paint;	   // Button for free paint
	extern ElemUI  btn_line_paint;	   // Button for line paint
	extern ElemUI  btn_rect_paint;	   // Button for rectangle paint
	extern ElemUI  btn_tria_paint;     // Button for triangle paint
	extern ElemUI  btn_circ_paint;	   // Button for circle paint
	extern ElemUI* cur_btn;		   // Current active button
	ElemUI*        cur_btn_;	   // Temp current active button

	if ((cursor.y>=SUI_MENU_BORDER) &&
	    (cursor.y<SUI_MENU_BORDER+SUI_TMENU_HEIGHT)) {

	    // Button paint click
	    // Free
	    if (!btn_free_paint.factive && (cursor.x>=btn_free_paint.x) &&
		(cursor.x<btn_free_paint.x+btn_free_paint.width))
			cur_btn_ = &btn_free_paint;
	    // Rectangle
	    else if (!btn_rect_paint.factive && (cursor.x>=btn_rect_paint.x) &&
		     (cursor.x<btn_rect_paint.x+btn_rect_paint.width))
			cur_btn_ = &btn_rect_paint;
	    // Circle
	    else if (!btn_circ_paint.factive && (cursor.x>=btn_circ_paint.x) &&
		     (cursor.x<btn_circ_paint.x+btn_circ_paint.width))
			cur_btn_ = &btn_circ_paint;
	    // Line
	    else if (!btn_line_paint.factive && (cursor.x>=btn_line_paint.x) &&
		     (cursor.x<btn_line_paint.x+btn_line_paint.width))
			cur_btn_ = &btn_line_paint;
	    // Triangle
	    else if (!btn_tria_paint.factive && (cursor.x>=btn_tria_paint.x) &&
		     (cursor.x<btn_tria_paint.x+btn_tria_paint.width))
			cur_btn_ = &btn_tria_paint;
	    else
			return true;

	    // Set active button
	    select_btn(AS_OFF);
	    cur_btn = cur_btn_;
	    select_btn(AS_ON);

	    return true;
	}

	return false;
}





/*************************
 *                       *
 *  Run program library  *
 *                       *
 *************************/



//
// Function run the main program with UI
//
void run(void)
{
	int c;
	extern short main_sp;         // Main sp
	extern short main_ss;         // Main ss
	extern short main_bp;	      // Main bp

	setlocale(LC_ALL, "ru");
	load_screen();

	graph_init();
	elements_init();
	out_ui();

	mouse_init();

	// Press "Q" for exit programm
	// Press "C" for clear canvas
	while (1) {
		asm {
			// Save address stack segment

			mov	ax, ss
			mov	word ptr main_ss, ax

			// Save address stack pointers

			mov	word ptr main_sp, sp
			mov	word ptr main_bp, bp
		}

		c = tolower(getch());

		switch (c) {
			case 'q':
				mouse_free();
				return;
			case 'c':
				clear_canvas();
		}
	}
}





//
// Out programm load screen
//
void load_screen(void)
{
	unsigned char x;	// X position

	// Text mode initialization
	textmode(C80);
	textbackground(BLACK);
	_setcursortype(_NOCURSOR);
	clrscr();

	// Out programm name
	textcolor(LIGHTBLUE);
	gotoxy(1,1);
	cputs(PROG_NAME);

	// Out programm version
	gotoxy(80-strlen(PROG_VER), 1);
	cputs(PROG_VER);

	// Out loading caption
	textcolor(LIGHTGREEN);
	gotoxy(40-strlen(LOAD_CAP)/2, 11);
	cprintf(LOAD_CAP);

	// Out loading line
	srand((unsigned)time(NULL));
	for (x=20; x<60; ++x) {
		gotoxy(x, 13);
		putch(LOAD_PIC);
		delay(50+rand()%50);
	}
}





/**************************
 *                        *
 *  Elements UI library   *
 *                        *
 **************************/



//
// Function initialization UI elements
//
void elements_init(void)
{
	extern ElemUI canvas;	       		 // Canvas for paint
	extern ElemUI btn_free_paint;		 // Button for free paint
	extern ElemUI btn_line_paint;	       	 // Button for line paint
	extern ElemUI btn_rect_paint;		 // Button for rectangle paint
	extern ElemUI btn_tria_paint;          	 // Button for triangle paint
	extern ElemUI btn_circ_paint;		 // Button for circle paint
	extern GraphMode graph_mode;		 // Graphic mode
	extern Palette   palette;		 // Palette colors
	extern ElemUI*   cur_btn;		 // Current active button

	// Canvas initialization
	canvas.x       = 0;
	canvas.y       = SUI_TMENU_HEIGHT+SUI_MENU_BORDER*2;
	canvas.width   = graph_mode.width;
	canvas.height  = SUI_CANVAS_HEIGHT;
	canvas.factive = AS_NULL;
	canvas.id      = EUI_CANVAS;

	// Colors pallete initialization
	// 2 rows color line
	palette.color.cur   = palette.color.min = CUI_COLOR_PALETTE_MIN;
	palette.color.max   = CUI_COLOR_PALETTE_MAX;
	palette.color.count = palette.color.max-palette.color.min+1;
	palette.ui.width    = palette.color.count*SUI_COLOR_SQUARE_SIZE/2+palette.color.count/2 + 1;
	palette.ui.height   = 3+SUI_COLOR_SQUARE_SIZE*2;
	palette.ui.y        = graph_mode.height - palette.ui.height - 3;
	palette.ui.x        = graph_mode.width  - palette.ui.width - 1;
	palette.ui.factive  = AS_NULL;
	palette.ui.id       = EUI_PALETTE;

	// Start button free paint
	cur_btn = &btn_free_paint;

	// Button free paint
	btn_free_paint.x       = (graph_mode.width%SUI_BTN-4*SUI_BTN_SEP)/2;
	btn_free_paint.y       = SUI_MENU_BORDER;
	btn_free_paint.height  = SUI_TMENU_HEIGHT+1;
	btn_free_paint.width   = SUI_BTN;
	btn_free_paint.id      = EUI_BTN_FREE_PAINT;
	btn_free_paint.factive = AS_OFF;

	// Button line paint
	btn_line_paint.x       = btn_free_paint.x+btn_free_paint.width+SUI_BTN_SEP;
	btn_line_paint.y       = SUI_MENU_BORDER;
	btn_line_paint.height  = SUI_TMENU_HEIGHT+1;
	btn_line_paint.width   = SUI_BTN;
	btn_line_paint.id      = EUI_BTN_LINE_PAINT;
	btn_line_paint.factive = AS_OFF;

	// Button rectangle paint
	btn_rect_paint.x       = btn_line_paint.x+btn_line_paint.width+SUI_BTN_SEP;
	btn_rect_paint.y       = SUI_MENU_BORDER;
	btn_rect_paint.height  = SUI_TMENU_HEIGHT+1;
	btn_rect_paint.width   = SUI_BTN;
	btn_rect_paint.id      = EUI_BTN_RECT_PAINT;
	btn_rect_paint.factive = AS_OFF;

	// Button triangle paint
	btn_tria_paint.x       = btn_rect_paint.x+btn_rect_paint.width+SUI_BTN_SEP;
	btn_tria_paint.y       = SUI_MENU_BORDER;
	btn_tria_paint.height  = SUI_TMENU_HEIGHT+1;
	btn_tria_paint.width   = SUI_BTN;
	btn_tria_paint.id      = EUI_BTN_TRIA_PAINT;
	btn_tria_paint.factive = AS_OFF;

	// Button circle paint
	btn_circ_paint.x       = btn_tria_paint.x+btn_tria_paint.width+SUI_BTN_SEP;
	btn_circ_paint.y       = SUI_MENU_BORDER;
	btn_circ_paint.height  = SUI_TMENU_HEIGHT+1;
	btn_circ_paint.width   = SUI_BTN;
	btn_circ_paint.id      = EUI_BTN_CIRC_PAINT;
	btn_circ_paint.factive = AS_OFF;

	// Start button
	cur_btn->factive       = AS_ON;
}





/*******************
 *                 *
 * Errors library  *
 *                 *
 *******************/



//
// Function output error message and halt program
//
void trigger_error(char* msg)
{
	struct text_info* tif = NULL;	// Information current text mode

	// Initialization text mode
	textmode(C80);
	textcolor(RED);
	textbackground(WHITE);
	clrscr();

	// Set text position
	tif = (struct text_info*)malloc(sizeof(*tif));
	if (tif != NULL) {
		gettextinfo(tif);
		gotoxy(tif->screenwidth/2 - strlen(msg)/2, tif->screenheight/2);
		free(tif);
	}

	// Out message and exit program
	cprintf(msg);
	exit(EXIT_FAILURE);
}




/*********************
 *                   *
 * Graphic library   *
 *                   *
 *********************/



//
// Print user interface
//
void out_ui(void)
{
	extern GraphMode graph_mode;    // Graphic mode information
	unsigned short   x;	        // X position
	unsigned short   y;	        // Y position
	unsigned short   width;	        // Screen width
	unsigned short   height;        // Screem height

	width  = graph_mode.width;
	height = graph_mode.height;

	// Top menu
	// Border
	out_fill_rect(0, 0, SUI_MENU_BORDER-1, width-1, CUI_MENU_BORDER, 0);
	out_fill_rect(SUI_TMENU_HEIGHT+SUI_MENU_BORDER, 0,
		      SUI_TMENU_HEIGHT+SUI_MENU_BORDER*2-1, width-1,
		      CUI_MENU_BORDER, 0);
	// Fill
	out_fill_rect(SUI_MENU_BORDER, 0,
		      SUI_TMENU_HEIGHT+SUI_MENU_BORDER-1, width-1,
		      CUI_MENU_FILL, 0);

	// Canvas
	clear_canvas();

	// Bottom menu
	// Border
	out_fill_rect(height-SUI_BMENU_HEIGHT-SUI_MENU_BORDER*2, 0,
		      height-SUI_BMENU_HEIGHT-SUI_MENU_BORDER-1, width-1,
		      CUI_MENU_BORDER, 0);
	out_fill_rect(height-SUI_MENU_BORDER, 0, height-1, width-1,
		      CUI_MENU_BORDER, 0);
	// Fill
	out_fill_rect(height-SUI_BMENU_HEIGHT-SUI_MENU_BORDER, 0,
		      height-SUI_MENU_BORDER-1, width-1,
		      CUI_MENU_FILL, 0);

	// Palette
	out_palette();

	// Hint combinations
	out_comb_hint();

	// Paint buttons
	out_btn();
}





//
// Paint buttons
//
void out_btn(void)
{
	extern ElemUI btn_free_paint;		 // Button for free paint
	extern ElemUI btn_line_paint;	       	 // Button for line paint
	extern ElemUI btn_rect_paint;		 // Button for rectangle paint
	extern ElemUI btn_tria_paint;          	 // Button for triangle paint
	extern ElemUI btn_circ_paint;		 // Button for circle paint
	extern ElemUI* cur_btn;			 // Start button
	extern GraphMode graph_mode;		 // Graphic mode
	unsigned char btn_num;   		 // Button number
	unsigned short offset_x;		 // Offset x position
	unsigned short x;			 // X position
	unsigned short y;                        // Y position

	// Paint buttons
	out_fill_rect(btn_free_paint.y,
		      btn_free_paint.x,
		      btn_free_paint.height,
		      btn_free_paint.width+btn_free_paint.x,
		      CUI_BTN_FILL, 0);
	out_fill_rect(btn_rect_paint.y,
		      btn_rect_paint.x,
		      btn_rect_paint.height,
		      btn_rect_paint.width+btn_rect_paint.x,
		      CUI_BTN_FILL, 0);
	out_fill_rect(btn_line_paint.y,
		      btn_line_paint.x,
		      btn_line_paint.height,
		      btn_line_paint.width+btn_line_paint.x,
		      CUI_BTN_FILL, 0);
	out_fill_rect(btn_circ_paint.y,
		      btn_circ_paint.x,
		      btn_circ_paint.height,
		      btn_circ_paint.width+btn_circ_paint.x,
		      CUI_BTN_FILL, 0);
	out_fill_rect(btn_tria_paint.y,
		      btn_tria_paint.x,
		      btn_tria_paint.height,
		      btn_tria_paint.width+btn_tria_paint.x,
		      CUI_BTN_FILL, 0);

	// Paint borders
	out_rect(btn_free_paint.y, btn_free_paint.x,
		 btn_free_paint.height,
		 btn_free_paint.width+btn_free_paint.x,
		 CUI_BTN_BORDER, 0);
	out_rect(btn_rect_paint.y, btn_rect_paint.x,
		 btn_rect_paint.height,
		 btn_rect_paint.width+btn_rect_paint.x,
		 CUI_BTN_BORDER, 0);
	out_rect(btn_line_paint.y, btn_line_paint.x,
		 btn_line_paint.height,
		 btn_line_paint.width+btn_line_paint.x,
		 CUI_BTN_BORDER, 0);
	out_rect(btn_circ_paint.y, btn_circ_paint.x,
		 btn_circ_paint.height,
		 btn_circ_paint.width+btn_circ_paint.x,
		 CUI_BTN_BORDER, 0);
	out_rect(btn_tria_paint.y, btn_tria_paint.x,
		 btn_tria_paint.height,
		 btn_tria_paint.width+btn_tria_paint.x,
		 CUI_BTN_BORDER, 0);
	out_rect(cur_btn->y, cur_btn->x, cur_btn->height,
		 cur_btn->width+cur_btn->x,
		 CUI_BTN_BORDER_ACTIVE, 0);

	// Paint icons on button
	// Free
	x = btn_free_paint.x+(btn_free_paint.width/2);
	y = btn_free_paint.y+(btn_free_paint.height/2);
	out_pixel(y+4, x+5, CUI_BTN_ICON, 0);
	out_pixel(y+4, x+4, CUI_BTN_ICON, 0);
	out_pixel(y+3, x+3, CUI_BTN_ICON, 0);
	out_pixel(y+3, x+2, CUI_BTN_ICON, 0);
	out_pixel(y+2, x+1, CUI_BTN_ICON, 0);
	out_pixel(y+1, x,   CUI_BTN_ICON, 0);
	out_pixel(y,   x,   CUI_BTN_ICON, 0);
	out_pixel(y-1, x,   CUI_BTN_ICON, 0);
	out_pixel(y-1, x-1, CUI_BTN_ICON, 0);
	out_pixel(y-2, x-1, CUI_BTN_ICON, 0);
	out_pixel(y-2, x-2, CUI_BTN_ICON, 0);
	out_pixel(y-2, x-3, CUI_BTN_ICON, 0);

	// Line
	x = btn_line_paint.x+(btn_line_paint.width/2);
	y = btn_line_paint.y+(btn_line_paint.height/2);
	out_line(y-5, x-5, y+5, x+5, CUI_BTN_ICON, 0);

	// Rectangle
	x = btn_rect_paint.x+(btn_rect_paint.width/2);
	y = btn_rect_paint.y+(btn_rect_paint.height/2);
	out_rect(y-8, x-8, y+8, x+8, CUI_BTN_ICON, 0);

	// Triangle
	x = btn_tria_paint.x+(btn_tria_paint.width/2);
	y = btn_tria_paint.y+(btn_tria_paint.height/2);
	out_line(y-5, x,   y+5, x-5, CUI_BTN_ICON, 0);
	out_line(y-5, x,   y+5, x+5, CUI_BTN_ICON, 0);
	out_line(y+5, x-5, y+5, x+5, CUI_BTN_ICON, 0);

	// Circle
	x = btn_circ_paint.x+(btn_circ_paint.width/2);
	y = btn_circ_paint.y+(btn_circ_paint.height/2);
	out_circle(y, x, 5, CUI_BTN_ICON, 0);
}





//
// Paint circle (Brasenham's algorithm)
//
// y and x   - positions center circle
// r         - radius
// color     - line color
// page	     - number page videomemory
//
void out_circle(unsigned short y,     unsigned short x, unsigned short r,
		unsigned short color, unsigned char  page)
{
	unsigned short x_;	// X position
	unsigned short y_;      // Y position
	short r_error;          // Radius error

	x_ = r;
	y_ = 0;
	r_error = 1 - x_;

	// Paint circle
	while (x_ >= y_) {
		out_pixel(y_+y, x_+x, color, page);
		out_pixel(x_+y, y_+x, color, page);
		out_pixel(y_+y, -x_+x, color, page);
		out_pixel(x_+y, -y_+x, color, page);
		out_pixel(-y_+y, -x_+x, color, page);
		out_pixel(-x_+y, -y_+x, color, page);
		out_pixel(-y_+y, x_+x, color, page);
		out_pixel(-x_+y, y_+x, color, page);

		++y_;

		if (r_error < 0)
			r_error += 2 * y_ + 1;
		else {
			r_error += 2 * (y_ - x_ + 1);
			--x_;
		}
	}
}





//
// Paint line (Brasenham's algorithm)
//
// y1 and x1 - position begin line
// y2 and x2 - position end line
// color     - line color
// page	     - number page videomemory
//
void out_line(unsigned short y1,    unsigned short x1,
	      unsigned short y2,    unsigned short x2,
	      unsigned short color, unsigned char page)
{
	bool           step;	// Size line Ox and Oy
	char	       ystep;   // Direction sized y
	short          error;   // Vertical length current y - precision y for current x
	unsigned short y;	// Y position
	unsigned short x;	// X position
	unsigned short dx;	// Substract xs
	unsigned short dy;	// Substract ys

	step = abs(y2-y1) > abs(x2-x1);

	// Reflection line diagonal
	if (step) {
		usswap(&x1, &y1);
		usswap(&x2, &y2);
	}

	// Swap begin and end, if line sized not left-right
	if (x1 > x2) {
		usswap(&x1, &x2);
		usswap(&y1, &y2);
	}

	dx    = x2 - x1;
	dy    = abs(y2-y1);
	error = dx / 2;
	ystep = (y1 < y2)? 1 : -1;
	y     = y1;

	// Paint line
	for (x=x1; x<=x2; ++x) {
		// Return x,y and paint point line
		out_pixel(step? x : y, step? y : x,
			  color, page);
		error -= dy;

		if (error < 0) {
			y     += ystep;
			error += dx;
		}
	}
}





//
// Paint palette
//
void out_palette(void)
{
	extern Palette palette;		// Color palette
	unsigned short x;		// X position
	unsigned short y;		// Y position
	unsigned short color;           // Current color
	unsigned short x2;		// X position end palette
	unsigned char  xc;		// Count x position
	unsigned char  line_count;	// Line count for color square

	// Paint outside border
	x = palette.ui.x;
	y = palette.ui.y;
	out_rect(y, x, y+palette.ui.height, x+palette.ui.width,
		 CUI_PALETTE_BORDER, 0);

	// Paint inside border and colors square

	x2         = palette.ui.x + palette.ui.width;
	color      = palette.color.min;
	line_count = palette.ui.y + palette.ui.height - SUI_COLOR_SQUARE_SIZE - 2;

	for (y=palette.ui.y+1; y<line_count; ++y, color=palette.color.min) {
		for (x=palette.ui.x+1, xc=0; x<x2; ++x, ++xc)
			if (xc == SUI_COLOR_SQUARE_SIZE+1) {
				// Inside border
				out_pixel(y, x, CUI_PALETTE_BORDER, 0);
				xc = 0;
				++color;
			}
			else    // Color square
				out_pixel(y, x, color, 0);
	}

	for (x=palette.ui.x+1; x<x2; ++x)
		out_pixel(y, x, CUI_PALETTE_BORDER, 0);

	color      = palette.color.min + palette.color.count/2;
	line_count = palette.ui.y + palette.ui.height;

	for (++y; y<line_count; ++y, color=palette.color.min+palette.color.count/2)
		for (x=palette.ui.x+1, xc=0; x<x2; ++x, ++xc)
			if (xc == SUI_COLOR_SQUARE_SIZE+1) {
				// Inside border
				out_pixel(y, x, CUI_PALETTE_BORDER, 0);
				xc = 0;
				++color;
			}
			else    // Color square
				out_pixel(y, x, color, 0);

	// Paint border round current color
	select_color(AS_ON);
}





//
// Set active state current button
//
void select_btn(ActiveState as)
{
	extern ElemUI* cur_btn;		// Current active button
	unsigned short color;		// Color border button

	// Switch current button active state
	cur_btn->factive = as;

	// Swtich color
	color = (as == AS_ON)? CUI_BTN_BORDER_ACTIVE : CUI_BTN_BORDER;

	// Paint border round current button
	out_rect(cur_btn->y, cur_btn->x, cur_btn->height,
		 cur_btn->width+cur_btn->x, color, 0);
}





//
// Paint border round select color square
//
// as - active state current color
//
void select_color  (ActiveState as)
{
	extern Palette palette;		// Colors palette
	unsigned short color;		// Border color
	unsigned short x;		// X position
	unsigned short y;		// Y position
	char           fx;              // X position flag corect
	char           fy;              // Y position flag corect

	// Positions color square
	fx = fy = 0;
	color = (palette.color.cur-palette.color.min) % (palette.color.count/2);
	x = palette.ui.x + (color)*SUI_COLOR_SQUARE_SIZE + (color);
	if (color)
		fx = 1;
	y = palette.ui.y;
	if ((palette.color.cur-palette.color.min) >= (palette.color.count/2)) {
		y += SUI_COLOR_SQUARE_SIZE+1;
		fy = 1;
	}

	// Switch border color
	color = (as == AS_ON)? CUI_PALETTE_ACTIVE : CUI_PALETTE_BORDER;

	// Paint border
	out_rect(y, x+fx, y+7+fy, x+8, color, 0);
}





//
// Read color current point
//
// y    - y cursor position
// x    - x cursor position
// page - number videopage
//
unsigned short read_color (unsigned short y, unsigned short x,
			   unsigned char page)
{
	extern Cursor cursor;		// Mouse cursor
	unsigned short color;		// Color current point

	asm {
		// Read current color point

		mov	ah, 0dh
		xor	bh, byte ptr page
		mov	dx, word ptr y
		mov	cx, word ptr x
		int	10h

		xor	ah, ah
		mov	word ptr color, ax
	}

	return color;
}





//
// Paint all colors line on screen graphic mode
//
void out_all_colors(void)
{
	extern GraphMode graph_mode;    // Graphic mode information
	unsigned short   x;		// X position
	unsigned short   y;		// Y position
	unsigned short   color_count;	// Count color graphic mode

	// Each line - new color
	color_count = (graph_mode.color_count > graph_mode.height)?
		graph_mode.color_count - graph_mode.color_count%graph_mode.height
								  :
		graph_mode.color_count;

	for (y=0; y<color_count; ++y)
		for (x=0; x<graph_mode.width; ++x)
			out_pixel(y, x, y, 0);
}





//
// Paint hint key combination
//
void out_comb_hint (void)
{
	out_text(23, 1, CUI_COMB_HINT, "Exit: Q", 0);
	out_text(23, 10, CUI_COMB_HINT, "Clear: C", 0);
}





//
// Function paint pixel
//
// y     - position y
// x     - position x
// color - pixel color
// page  - number page videomemory
//
void out_pixel(unsigned short y,    unsigned short x,
	       unsigned char color, unsigned char page)
{
	asm {
		// Paint pixel

		mov	ah, 0ch
		mov	al, byte ptr color
		mov	bh, byte ptr page
		mov	dx, word ptr y
		mov	cx, word ptr x
		int	10h
	}
}





//
// Paint text
//
// y     - position y
// x     - position x
// color - text color
// text  - text for paint
// page  - number page videomemory
//
void out_text(unsigned short y,     unsigned short x,
	      unsigned short color, char* text,
	      unsigned char  page)
{
	unsigned char len;	// Text length
	unsigned char i;	// Number current character
	unsigned char c;	// Current character

	len = strlen(text);

	// Out text
	for (i=0; i<len; ++i, ++x) {
		c = text[i];

		asm {
			// Set cursor position

			mov	ah, 02h
			mov	bh, byte ptr page
			mov	dh, byte ptr y
			mov	dl, byte ptr x
			int	10h

			// Out character

			mov	ah, 0eh
			mov	al, byte ptr c
			mov	bl, byte ptr color
			int	10h
		}
	}
}





//
// Function paint fill rectangle
//
// y1 and x1 - top left point
// y2 and x2 - bottom right point
// color     - fill color
// page	     - number page videomemory
//
void out_fill_rect(unsigned short y1,    unsigned short x1,
		   unsigned short y2,    unsigned short x2,
		   unsigned short color, unsigned char  page)
{
	unsigned short y;          // Y position
	unsigned short x;          // X position

	for (y=y1; y<=y2; ++y)
		for (x=x1; x<=x2; ++x)
			out_pixel(y, x, color, page);
}





//
// Paint rectangle
//
// y1 and x1 - top left point
// y2 and x2 - bottom right point
// color     - fill color
// page	     - number page videomemory
//
void out_rect(unsigned short y1,    unsigned short x1,
	      unsigned short y2,    unsigned short x2,
	      unsigned short color, unsigned char  page)
{
	unsigned short y;          // Y position
	unsigned short x;          // X position

	// Correct x and y positions

	y = y1+1;
	x = x1;

	if ((y1>y2) && (x2>x1)) {
		usswap(&y1, &y2);
		y = y1;
	}
	else if ((x1>x2) && (y1>y2)) {
		usswap(&x1, &x2);
		usswap(&y1, &y2);
		x = x1;
		y = y1;
	}
	else if ((x1>x2) && (y1<y2)) {
		usswap(&x1, &x2);
		x = x1;
	}

	// Top and bottom
	for (; x<=x2; ++x) {
		out_pixel(y1, x, color, page);
		out_pixel(y2, x, color, page);
	}

	// Left and right
	for (; y<y2; ++y) {
		out_pixel(y, x1, color, page);
		out_pixel(y, x2, color, page);
	}
}





//
// Function clear canvas
//
void clear_canvas(void)
{
	extern ElemUI canvas;       // Canvas for paint
	extern Cursor cursor;	    // Mouse cursor

	cursor.prevcolor = CUI_CANVAS;

	out_fill_rect(canvas.y, canvas.x,
		      canvas.y+canvas.height-1,
		      canvas.x+canvas.width-1,
		      CUI_CANVAS, 0);
}





//
// Function initialization graphic mode
//
void graph_init(void)
{
	extern GraphMode graph_mode;   		// Graphic mode information

	graph_mode.width       = 320;
	graph_mode.height      = 200;
	graph_mode.number      = 0x13;
	graph_mode.color_count = 256;

	asm {
		// Switch to graphic mode

		xor	ah, ah
		mov	al, byte ptr graph_mode.number
		int	10h

		// Switch vidopage 0

		mov	ax, 0500h
		int	10h
	}
}