//#define DEBUG 1
#include <vector>
#include <string>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include "gr.hh"
#include "extern.hh"
#include "GriPath.hh"
#include "superus.hh"
#include "defaults.hh"
#define NCODES 100
// symbol_code (p 604 new PostScript book): (1) define name, (2) Postscript
// code, (3) symbol-font crossref code (used for estimage of symbol
// size, by index_for_math_symbol() ... a bad idea, really).
static char    *symbol_code[NCODES][3] = {
	// name, code in Table E.11, p604 new ps book, char-equivalent}
	// 
	// Organization of list below is as in the tables in 
	// Lamport's Latex book
	//
	// Table 3.3 Greek Letters
	// lowercase
	{"alpha", "\\141", "a"},
	{"beta", "\\142", "b"},
	{"gamma", "\\147", "g"},
	{"delta", "\\144", "d"},
	{"epsilon", "\\145", "e"},
// varepsilon
	{"zeta", "\\172", "z"},
	{"eta", "\\150", "h"},
	{"theta", "\\161", "q"},
	{"vartheta", "\\112", "q"},
	{"iota", "\\151", "i"},
	{"kappa", "\\153", "k"},
	{"lambda", "\\154", "l"},
	{"mu", "\\155", "m"},
	{"nu", "\\156", "n"},
	{"xi", "\\170", "x"},
// o [not needed, really]
	{"pi", "\\160", "p"},
	{"varpi", "\\166", "p"},	// guess that size is same as pi
	{"rho", "\\162", "r"},
	{"sigma", "\\163", "s"},
	{"varsigma", "\\126", "s"},
	{"tau", "\\164", "t"},
	{"upsilon", "\\165", "u"},
	{"psi", "\\171", "y"},
	{"chi", "\\143", "c"},
	{"phi", "\\146", "f"},
	{"varphi", "\\152", "f"},
	{"omega", "\\167", "w"},
//
// Uppercase
	{"Gamma", "\\107", "G"},
	{"Delta", "\\104", "D"},
	{"Theta", "\\121", "Q"},
	{"Lambda", "\\114", "L"},
	{"Xi", "\\130", "X"},
	{"Pi", "\\120", "P"},
	{"Sigma", "\\123", "S"},
	{"Upsilon", "\\241", "Y"},	// guess that size is same as psi
	{"Phi", "\\106", "F"},
	{"Psi", "\\131", "Y"},
	{"Omega", "\\127", "W"},
// 
// Table 3.4: Binary Operation Symbols
	{"pm", "\\261", "+"}, // guess that size is same as +
// mp
	{"times", "\\264", "x"}, // guess that size is same as x
	{"div", "\\270", "x"},	 // guess that size is same as x
	{"ast", "\\052", "*"},
// star
	{"circ", "\\260", "."},	// guess that size is same as .
	{"bullet", "\\267", "*"}, // guess that size is same as *
	{"cdot", "\\327", ","},
// cap
// cup
// uplus
// sqcap
// sqcup
// vee
	{"wedge", "\\331", "M"}, // guess that size is same as M
// setminus
// wr
// diamond
// bigtriangleup
// bigtriangledown
// triangleleft
// triangleright
// lhd
// rhd
// unlhd
// unrhd
	{"oplus", "\\305", "o"},
// ominus
	{"otimes", "\\304", "o"},
// oslash
// odot
// bigcirc
// dagger
// ddagger
// amalg
//
// Table 3.5: Relation Symbols
	{"leq", "\\243", "<"}, // guess that size is same as <
// prec
// preceq
// ll
	{"subset", "\\314", "<"}, // guess that size is same as <
	{"subseteq", "\\315", "<"}, // guess that size is same as <
// sqsubset
// sqsubseteq
// MOVE 'in' to after 'infty'
// vdash
	{"geq", "\\263", ">"}, // guess that size is same as >
// succ
// succeq
// gg
	{"supset", "\\311", ">"}, // guess that size is same as >
	{"supseteq", "\\312", ">"}, // guess that size is same as >
// sqsupset
// sqsupseteq    
// ni
// dashv
	{"equiv", "\\272", "="}, // guess that size is same as =
	{"sim", "\\176", "~"},
// simeq
// asymp
	{"approx", "\\273", "~"}, // guess that size is same as ~
	{"cong", "\\100", "="},	  // guess that size is same as =
	{"neq", "\\271", "="},	  // guess that size is same as =
// doteq
	{"propto", "\\265", "~"}, // guess that size is same as ~    
// models
	{"perp", "\\136", "M"},
// mid
// parallel
// bowtie
// join
// smile
// frown
// 
// Table 3.6: Arrow Symbols
	{"leftarrow", "\\254", "M"},
	{"Leftarrow", "\\334", "M"},
	{"rightarrow", "\\256", "M"},
	{"Rightarrow", "\\336", "M"},
	{"leftrightarrow", "\\253", "M"},
	{"Leftrightarrow", "\\333", "M"},
// mapsto
// hookleftarrow
// leftharpoonup
// leftharpoondown
// rightleftharpoons
// longleftarrow
// Longleftarrow
// longrightarrow
// Longrightarrow
// longleftrightarrow
// Longleftrightarrow
// longmapsto
// hookrightarrow
// rightharpoonup
// rightharpoon down
// leadsto
	{"uparrow", "\\255", "|"}, // guess that size is same as "|"
	{"Uparrow", "\\335", "|"}, // guess that size is same as "|"
	{"downarrow", "\\257", "|"}, // guess that size is same as "|"
	{"Downarrow", "\\337", "|"}, // guess that size is same as "|"
// updownarrow
// Updownarrow
// neararrow
// searrow
// swarrow
// nwarrow
//
// Table 3.7: Miscellaneous Symbols
	{"aleph", "\\300", "M"},
// hbar
// imath
// jmath
// ell
	{"wp", "\\303", "M"},
	{"Re", "\\302", "R"},		// guess that size is same as R
	{"Im", "\\301", "M"},		// guess that size is same as M
// mho
	{"prime", "\\242", "'"},
	{"emptyset", "\\306", "M"},
	{"nabla", "\\321", "M"},	// guess that size is same as M
	{"surd", "\\326", "M"},		// guess that size is same as M
	{"sqrt", "\\326", "M"},		// not in this table, but what the heck
// top
	{"bot", "\\136", "M"},
// |
	{"angle", "\\320", "M"},
	{"forall", "\\042", "M"},	// guess that size is same as M
	{"exists", "\\044", "M"},	// guess that size is same as M
	{"neg", "\\330", "M"},
// flat
// natural
// sharp
// backslash
	{"partial", "\\266", "d"}, // guess that size is same as d
	{"infty", "\\245", "M"},   // guess that size is same as M
// Interpose 'int' and 'in' here to avoid clashes with 'infty'
	{"int", "\\362", "M"}, // guess that size is same as M
	{"in", "\\316", "<"},  // guess that size is same as <
// Box
// Diamond
// triangle
	{"clubsuit", "\\247", "M"},
	{"diamondsuit", "\\340", "M"},
// heartsuit
	{"spadesuit", "\\252", "M"},
//
// Table 3.8 Variable-sized symbols
	{"sum", "\\345", "M"}, // guess that size is same as M
	{"prod", "\\325", "M"},	// guess that size is same as M
// int -- moved up to avoid name clashes
// oint
// bigcap
// bigcup
// bigsqcup
// bigvee
// bigwedge
// bigodot
// bigotimes
// bigoplus
// biguplus
//
// Table 3.10
// (
// [
// {
	{"lfloor", "\\353", "M"},
	{"lceil", "\\351", "M"},
	{"langle", "\\341", "<"},
// /
// |
// )
// ]
// }
	{"rfloor", "\\373", "M"},
	{"rceil", "\\371", "M"},
	{"rangle", "\\361", ">"}
// backslash SEE ABOVE
// \|
// uparrow SEE ABOVE
// downarrow SEE ABOVE
// updownarrow SEE ABOVE
// Uparrow SEE ABOVE
// Downarrow SEE ABOVE
// Updownarrow SEE ABOVE
};
double gr_current_descender(void);
std::vector<std::string> part_string(const std::string &s)
{
	using namespace std;
	string::size_type i, lasti = 0, len = s.size();
	bool inmath = false;
	vector<string> parts;
	for (i = 0; i < len; i++) {
		if (s[i] == '$') {
			// \$ escapes but not \\$
			if (i > 0 && s[i-1] == '\\') {
				if (!(i > 1 && s[i-2] == '\\'))
					continue;
			}
			if (inmath) i++; // keep the $ at the end
			parts.push_back(s.substr(lasti, i-lasti));
			inmath = !inmath;
			lasti = i;
		}
	}
	parts.push_back(s.substr(lasti, len-lasti));
#if 0
	printf("\n\"%s\"\n", s.c_str());
	for (unsigned int l = 0; l < parts.size(); l++)
		printf("    \"%s\"\n", parts[l].c_str());
#endif
	return parts;
}

char* gr_fontname_from_id(int id);

#define	default_fontID		gr_font_Helvetica
#define default_encoding        font_encoding_isolatin1
#define	default_fontsize_pt	12.0

static gr_font  CurrentFont = {
	default_fontID,
	default_encoding,				
	default_fontsize_pt
};

// Q: should this be done in Moveup() routine? [then what about $N$N though]
#define START_NEW_TEXT {\
    if (_output_file_type == postscript && _grWritePS) { \
	fprintf(_grPS, "(");\
	check_psfile();\
    }\
}

#define STOP_OLD_TEXT {\
    if (_output_file_type == postscript && _grWritePS) {\
        fprintf(_grPS, ") sh\n");\
        check_psfile();\
    }\
}


enum position {Superscript, Subscript, Inline};	// Baseline indicator
static std::stack<position> pstack;   // baseline position stack

// Use spacing patterned on results of a TeX example (using Large font). All
// quantities are multiples of Mspace.
static const double SubSize = 0.75; // relative height of subscripts = 6/8
static const double SuperSize = 0.75; // relative height of superscripts = 6/8 
static const double SuperMoveUp =0.625;	// Move up for super = 5/8
static const double SubMoveDown =0.375;	// Move down for sub = 3/8 



#define PS_showpage     "grestore\nshowpage\n"
#define PS_stroke       "s\n"

extern FILE    *_grPS;
extern FILE    *_grSVG;
extern bool     _grNeedBegin;
extern bool     _grPathExists;
extern bool     _grWritePS;

static void     gr_drawstring(const char *s);
static void     gr_drawstring_svg(const char *s, double xcm, double ycm, double angle);
static int      index_for_math_symbol(const char *s);	// base routine
static double   gr_charwidth_cm(int c, int font, double fontsize_pt);
static void     gr_DrawChar(const char *c);
static void     gr_setfont_fontsize(gr_fontID newID, bool force = false);
static void     pstack_erase();
static void     MoveDown(void);
static void     MoveUp(void);
static void     MoveUp_svg(double *xcm, double *ycm);
static void     MoveDn_svg(double *xcm, double *ycm);
static void     MoveHorizontally(double em_distance);
static char    *symbol_in_math(const char *sPtr, int *inc);

gr_font_info    font_list[] =
{
	{gr_font_Courier, "Courier"},
	{gr_font_CourierOblique, "Courier-Oblique"},
	{gr_font_CourierBold, "Courier-Bold"},
	{gr_font_CourierBoldOblique, "Courier-BoldOblique"},
	{gr_font_Helvetica, "Helvetica"},
	{gr_font_HelveticaBold, "Helvetica-Bold"},
	{gr_font_HelveticaOblique, "Helvetica-Oblique"},
	{gr_font_PalatinoRoman, "Palatino-Roman"},
	{gr_font_PalatinoItalic, "Palatino-Italic"},
	{gr_font_PalatinoBold, "Palatino-Bold"},
	{gr_font_PalatinoBoldItalic, "Palatino-BoldItalic"},
	{gr_font_Symbol, "Symbol"},
	{gr_font_TimesRoman, "Times-Roman"},
	{gr_font_TimesItalic, "Times-Italic"},
	{gr_font_TimesBold, "Times-Bold"},
	{gr_font_TimesBoldItalic, "Times-BoldItalic"},
	{gr_font_Century, "Century"},
	{gr_font_end_of_list, ""}
};

// Draw text at specified location.
void
gr_show_at(/*const*/ char *s, double xcm, double ycm, gr_textStyle style, double angle_deg)
{
#ifdef DEBUG
        printf("DEBUG %s:%d gr_show_at(\"%s\",xcm,ycm,style,%f)\n",__FILE__,__LINE__,s,angle_deg);
#endif
	if (0.0 == gr_currentfontsize_pt() || !strlen(s)) {
		return;
	}
	double          oldfontsize_pt = gr_currentfontsize_pt();
	gr_fontID       oldfontID = gr_currentfont();
	double          width_cm, ascent_cm, descent_cm;
	rectangle box;
	extern bool _warn_offpage;
	if (_warn_offpage 
	    && ( xcm < OFFPAGE_LEFT 
		 || xcm > OFFPAGE_RIGHT
		 || ycm < OFFPAGE_BOTTOM
		 || ycm > OFFPAGE_TOP)) {
		warning("Drawing text at a location that is offpage.");
	}
	char *fn_svg = NULL;
	double r, g, b;
	_griState.color_text().getRGB(&r, &g, &b);
	switch (_output_file_type) {
	case  postscript:
		break;
	case svg: 
		switch (CurrentFont.id) {
		case gr_font_Courier:            fn_svg = "Courier";            break;
		case gr_font_CourierOblique:     fn_svg = "Courier-Italic";     break;
		case gr_font_CourierBold:        fn_svg = "Courier-Bold";       break;
		case gr_font_CourierBoldOblique: fn_svg = "Courier-BoldItalic"; break;
		case gr_font_Helvetica:          fn_svg = "Helvetica";          break;
		case gr_font_HelveticaOblique:   fn_svg = "Helvetica-Italic";   break;
		case gr_font_HelveticaBold:      fn_svg = "Helvetica-Bold";     break;
		case gr_font_PalatinoRoman: 
		case gr_font_PalatinoItalic:
		case gr_font_PalatinoBold:
		case gr_font_PalatinoBoldItalic:
			fn_svg = "Times";
			warning("SVG cannot handle Palatino font yet");
			break;
		case gr_font_Symbol:               fn_svg = "Symbol";               break;
		case gr_font_TimesRoman:           fn_svg = "Times";                break;
		case gr_font_TimesItalic:          fn_svg = "Times-Italic";         break;
		case gr_font_TimesBold:            fn_svg = "Times-Bold";           break;
		case gr_font_TimesBoldItalic:      fn_svg = "Times-BoldItalic";     break;
		case gr_font_Century:     	   fn_svg = "Century";  	    break;
		default: 
			fn_svg = "Times";
			warning("SVG defaulting to Times font");
			break;
		}
		break;
	case  gif:
		fprintf(stderr, "INTERNAL error at %s:%d -- nothing known for GIF\n\n", __FILE__, __LINE__);
		exit(99);
		break;
	}

//	if (_output_file_type == svg) {
//		fprintf(stderr, "%s:%d approximating drawing of '%s' NOTE: subscripts, etc won't work\n", __FILE__, __LINE__, s);
//	}

	void set_ps_color(char what);
	set_ps_color('t');
	gr_setfontsize_pt(oldfontsize_pt);
	gr_setfont(oldfontID);
	gr_stringwidth(s, &width_cm, &ascent_cm, &descent_cm);
	switch (style) {
	case TEXT_LJUST:
		gr_moveto_cm(xcm, ycm);
		if (_output_file_type == postscript) {
			if (_grWritePS) {
				if (fabs(angle_deg) > 0.1)
					fprintf(_grPS, "%.2f rotate ", angle_deg);
				gr_drawstring(s);
			}
		} else if (_output_file_type == svg) {
			gr_drawstring_svg(s, xcm, ycm, angle_deg);
		} else {
			fprintf(stderr, "%s:%d unknown file output type\n",__FILE__,__LINE__);
		}
		// This box not tested specifically
		box.set(0, -descent_cm, width_cm, ascent_cm);
		box.rotate(angle_deg);
		box.shift_x(xcm);
		box.shift_y(ycm);
		break;
	case TEXT_RJUST:
		if (_output_file_type == postscript) {
			if (_grWritePS) {
				fprintf(_grPS, "%.1f %.1f m ",
					PT_PER_CM * (xcm - width_cm * cos(angle_deg / DEG_PER_RAD)),
					PT_PER_CM * (ycm - width_cm * sin(angle_deg / DEG_PER_RAD)));
				if (fabs(angle_deg) > 0.1)
					fprintf(_grPS, "%.2f rotate ", angle_deg);
				gr_drawstring(s);
			}
		} else if (_output_file_type == svg) {
			if (_grWritePS) {
				gr_drawstring_svg(s, 
						  xcm - width_cm * cos(angle_deg / DEG_PER_RAD),
						  ycm - width_cm * sin(angle_deg / DEG_PER_RAD),
						  angle_deg);
			}
		} else {
			fprintf(stderr, "%s:%d unknown file output type\n",__FILE__,__LINE__);
		}

		// This box not tested specifically
		box.set(-width_cm, -descent_cm, 0.0, ascent_cm);
		box.rotate(angle_deg);
		box.shift_x(xcm);
		box.shift_y(ycm);
		break;
	case TEXT_CENTERED:
		if (_output_file_type == postscript) {
			if (_grWritePS) {
#ifdef DEBUG
			        fprintf(_grPS, "%% DEBUG %s:%d '%s' at angle %f\n",__FILE__,__LINE__,s,angle_deg);
#endif
				fprintf(_grPS, "%.1f %.1f m ",
					PT_PER_CM * (xcm - 0.5 * width_cm * cos(angle_deg / DEG_PER_RAD)),
					PT_PER_CM * (ycm - 0.5 * width_cm * sin(angle_deg / DEG_PER_RAD)));
				if (fabs(angle_deg) > 0.1)
					fprintf(_grPS, "%.2f rotate ", angle_deg);
				gr_drawstring(s);
			}
		} else if (_output_file_type == svg) {
			if (_grWritePS) {
				gr_drawstring_svg(s, 
						  xcm - 0.5 * width_cm * cos(angle_deg / DEG_PER_RAD),
						  ycm - 0.5 * width_cm * sin(angle_deg / DEG_PER_RAD),
						  angle_deg);
			}
		} else {
			fprintf(stderr, "%s:%d unknown file output type\n",__FILE__,__LINE__);
		}
		box.set(-width_cm/2, -descent_cm, width_cm/2, ascent_cm);
		box.rotate(angle_deg);
		box.shift_x(xcm);
		box.shift_y(ycm);
		break;
	default:
		warning("gr_show_at type is UNKNOWN\n");
	}
	switch (_output_file_type) {
	case  postscript:
		if (_grWritePS) {
			if (fabs(angle_deg) > 0.1)
				fprintf(_grPS, "%.2f rotate ", -angle_deg);
			check_psfile();
			//fprintf(_grPS, "%% gr_show_at() END\n");
		}
		break;
	case svg:
		//fprintf(_grSVG, "</text>\n");
		break;
	case gif:
		fprintf(stderr, "INTERNAL error at %s:%d -- nothing known for GIF\n\n", __FILE__, __LINE__);
		exit(99);
		break;
	default:
		fprintf(stderr, "%s:%d unknown file output type\n",__FILE__,__LINE__);
		break;		// BUG: should check filetype here
	}
	// Update bounding box
	bounding_box_update(box);
	_drawingstarted = true;
}

// gr_drawstring() -- draw string, including font changes &super/subscripts
static void
gr_drawstring(const char *s)
{
        char slast = '\0';
	int             slen = strlen(s);
	bool            inmath = false;
	gr_fontID       original_font = gr_currentfont();
	gr_fontID       current_font = original_font;
	gr_fontID       slant_font = original_font;	// prevent compiler warning
	double          original_fontsize = gr_currentfontsize_pt();
	bool            know_slant_font = false;
#ifdef DEBUG
	printf("DEBUG %s:%d gr_drawstring(\"%s\")\n",__FILE__,__LINE__,s);
#endif
	if (slen <= 0)
		return;
	if (0.0 == gr_currentfontsize_pt())
		return;
	// Figure out slant font, if there is an appropriate one
	switch (original_font) {
	case gr_font_TimesRoman:
		slant_font = gr_font_TimesItalic;
		know_slant_font = true;
		break;
	case gr_font_TimesBold:
		slant_font = gr_font_TimesBoldItalic;
		know_slant_font = true;
		break;
	case gr_font_Helvetica:
		slant_font = gr_font_HelveticaOblique;
		know_slant_font = true;
		break;
	case gr_font_HelveticaBold:
		slant_font = gr_font_HelveticaBoldOblique;
		know_slant_font = true;
		break;
	case gr_font_Courier:
		slant_font = gr_font_CourierOblique;
		know_slant_font = true;
		break;
	case gr_font_CourierBold:
		slant_font = gr_font_CourierBoldOblique;
		know_slant_font = true;
		break;
	case gr_font_PalatinoRoman:
		slant_font = gr_font_PalatinoItalic;
		know_slant_font = true;
		break;
	case gr_font_PalatinoBold:
		slant_font = gr_font_PalatinoBoldItalic;
		know_slant_font = true;
		break;
        case gr_font_Century:
                slant_font = gr_font_TimesItalic; // BUG: should be Century Italic
		know_slant_font = true;
		break;
	default:
		know_slant_font = false;
	}
	// Scan through whole string.
	START_NEW_TEXT;
	while (*s != '\0') {
#ifdef DEBUG
//		printf("DEBUG(%s:%d) *s= '%c'\n",__FILE__,__LINE__,*s);
#endif
		if (*s == '-' && CurrentFont.encoding == font_encoding_isolatin1) {
			// Use a different character to avoid looking like underscore.
			if (_grWritePS) {
				STOP_OLD_TEXT;
				fprintf(_grPS, "(\\255) sh\n");	// endash
				check_psfile();
				START_NEW_TEXT;
			}
			s++;
			continue;
		}
		// Figure out whether entering or leaving math mode; enter/leave if
		// find $ without preceeding \.  Thus a$b$ has math but a\$b\$ does
		// not.
		if (*s == '$' && slast != '\\') {
			if (inmath) {
#ifdef DEBUG
				printf("DEBUG(%s:%d) got $ so leave math mode\n",__FILE__,__LINE__);
				printf("DEBUG(%s:%d) pstack size is %d\n",__FILE__,__LINE__,int(pstack.size()));
#endif
					
				// Were in math; now go back to original font.
				inmath = false;
				if (current_font != original_font) {
					current_font = original_font;
					STOP_OLD_TEXT;
					gr_setfont(current_font);
					START_NEW_TEXT;
				}
				if (!pstack.empty()) {
					warning("a text string ended without completing a mathematical grouping (superscript, subscript, or {block})");
					pstack_erase();
				}
			} else {
#ifdef DEBUG
				printf("DEBUG(%s:%d) got $ so enter math mode\n",__FILE__,__LINE__);
#endif
				// Go to Italic/Oblique font, as case may be.  Unfortunately,
				// PostScript uses different names for this slanted font.
				inmath = true;
				if (know_slant_font) {
					current_font = slant_font;
					STOP_OLD_TEXT;
					gr_setfont(current_font);
					START_NEW_TEXT;
				}
				if (!pstack.empty()) {
					warning("a text string started without an empty mathematical grouping (superscript, subscript, or {block})");
					pstack_erase();
				}
			}
			slast = *s++;
			continue;
		}
		// Handle math mode.  This code is a little kludgy, so be carefull.
		if (inmath) {
			if (*s == '^') {
				// Handle superscripts
				slast = *s++;
				if (*s == '\0') {
					// Odd -- nothing to superscript
					if (current_font != original_font) {
						STOP_OLD_TEXT;
						gr_setfontsize_pt(original_fontsize);
						gr_setfont(original_font);
					}
					return;
				} else if (*s == '{') {
					// Several characters to superscript
					pstack.push(Superscript);
#ifdef DEBUG
					printf("DEBUG(%s:%d) pushed superscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Superscript, int(pstack.size()));
#endif
					MoveUp();
				} else if (*s == '\\') {
					// Math character to superscript
					int             inc;
					char *            insert;
					insert = symbol_in_math(s, &inc);
					if (inc) {
						gr_fontID       oldfontID = gr_currentfont();
						pstack.push(Superscript);
#ifdef DEBUG
						printf("DEBUG(%s:%d) pushed subscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Superscript, int(pstack.size()));
#endif
						MoveUp();
						STOP_OLD_TEXT;
						gr_setfont(gr_font_Symbol);
						if (_grWritePS) {
							fprintf(_grPS, "(%s) sh\n", insert);
							check_psfile();
						}
						gr_setfont(oldfontID);
						START_NEW_TEXT;
						s += inc;
#ifdef DEBUG
						printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
						MoveDown();
						pstack.pop();
					}
				} else {
					// Single character to superscript
					pstack.push(Superscript);
#ifdef DEBUG
					printf("DEBUG(%s:%d) pushed subscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Subscript, int(pstack.size()));
#endif
					MoveUp();
					// Draw single character in math mode.  If it's a digit,
					// do not do in italics!
					if (isdigit(*s) || ispunct(*s)) {
						if (*s == '/' && !isdigit(slast)) {
							gr_DrawChar(s);
						} else {
							STOP_OLD_TEXT;
							gr_setfont(original_font);
							START_NEW_TEXT;
							gr_DrawChar(s);
							STOP_OLD_TEXT;
							gr_setfont(slant_font);
							START_NEW_TEXT;
						}
					} else {
						gr_DrawChar(s);
					}
					MoveDown();
#ifdef DEBUG
					printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
					pstack.pop();
				}
			} else if (*s == '_') {
				// Handle subscript
				slast = *s++;
				if (*s == '\0') {
					// Odd -- nothing to subscript
					if (current_font != original_font) {
						STOP_OLD_TEXT;
						gr_setfontsize_pt(original_fontsize);
						gr_setfont(original_font);
					}
					return;
				} else if (*s == '{') {
					// Several characters to subscript
					pstack.push(Subscript);
#ifdef DEBUG
					printf("DEBUG(%s:%d) pushed subscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Subscript, int(pstack.size()));
#endif
					MoveDown();
				} else if (*s == '\\') {
					// Math character to subscript
					int             inc;
					char *            insert;
					insert = symbol_in_math(s, &inc);
					if (inc) {
						gr_fontID       oldfontID = gr_currentfont();
						pstack.push(Subscript);
#ifdef DEBUG
						printf("DEBUG(%s:%d) pushed subscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Subscript, int(pstack.size()));
#endif
						MoveDown();
						STOP_OLD_TEXT;
						gr_setfont(gr_font_Symbol);
						if (_grWritePS) {
							fprintf(_grPS, "(%s) sh\n", insert);
							check_psfile();
						}
						gr_setfont(oldfontID);
						START_NEW_TEXT;
						s += inc;
						MoveUp();
#ifdef DEBUG
						printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
						pstack.pop();
					}
				} else {
					// Single character to subscript
					pstack.push(Subscript);
#ifdef DEBUG
					printf("DEBUG(%s:%d) pushed subscript=%d onto stack to make length %d\n",__FILE__,__LINE__,Subscript, int(pstack.size()));
#endif
					MoveDown();
					// Draw single character in math mode.  If it's a digit,
					// do not do in italics!
					if (isdigit(*s) || ispunct(*s)) {
						if (*s == '/' && !isdigit(slast)) {
							gr_DrawChar(s);
						} else {
							STOP_OLD_TEXT;
							gr_setfont(original_font);
							START_NEW_TEXT;
							gr_DrawChar(s);
							STOP_OLD_TEXT;
							gr_setfont(slant_font);
							START_NEW_TEXT;
						}
					} else {
						gr_DrawChar(s);
					}
					MoveUp();
#ifdef DEBUG
					printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
					pstack.pop();
				}
			} else if (*s == '{') { // just a grouping, not a baseline shift
				pstack.push(Inline);
#ifdef DEBUG
				printf("DEBUG(%s:%d) pushed Inline=%d onto stack to make length %d\n",__FILE__,__LINE__,Inline,int(pstack.size()));
#endif

			} else if (*s == '}') {	// finished with Superscript/Subscript/Inline
				if (pstack.size() > 0) {
					position p = pstack.top();
					if (p == Superscript) {
						MoveDown();
					} else if (p == Subscript) {
						MoveUp();
					} // ignore inline
#ifdef DEBUG
					printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
					pstack.pop();
				} else {
					warning("unmatched \"}\" in a mathematicsal string");
				}
			} else if (*s == '\\') {
				// Substitute math symbol, unless it's
				// an escaped string
				int inc;
				char *insert;
				if (*(s + 1) == '$') {
					slast = *s++;
				} else if (*(s + 1) == ',') {
					slast = *s++;
					MoveHorizontally(0.1666666);	// thinspace
				} else if (*(s + 1) == '!') {
					slast = *s++;
					MoveHorizontally(-0.1666666);	// neg thinspace
				} else if (*(s + 1) == '"') {
					slast = *s++;
				} else if (*(s + 1) == '\\') {
					slast = *s++;
				} else if (*(s + 1) == '{' || *(s + 1) == '}') {
					STOP_OLD_TEXT;
					gr_setfont(original_font);
					START_NEW_TEXT;
					gr_DrawChar(s + 1);
					STOP_OLD_TEXT;
					gr_setfont(slant_font);
					START_NEW_TEXT;
					slast = *s++;
				} else {
					insert = symbol_in_math(s, &inc);
					if (inc) {
						// math symbol in symbol font
						gr_fontID oldfontID = gr_currentfont();
						STOP_OLD_TEXT;
						gr_setfont(gr_font_Symbol);
						if (_grWritePS) {
							fprintf(_grPS, "(%s) sh\n", insert);
							check_psfile();
						}
						gr_setfont(oldfontID);
						START_NEW_TEXT;
						s += inc;
					} else {
						// Not a known math-mode symbol, so just 
						// draw it.  Is this the right thing to do?
						gr_DrawChar(s + 1);
					}
				}
			} else {
				// Draw single character in math mode.
				// If it's a digit, do not use italics.
				if (isdigit(*s) || ispunct(*s)) {
					if (*s == '/' && !isdigit(slast)) {
						gr_DrawChar(s);
					} else {
						STOP_OLD_TEXT;
						gr_setfont(original_font);
						START_NEW_TEXT;
						gr_DrawChar(s);
						STOP_OLD_TEXT;
						gr_setfont(slant_font);
						START_NEW_TEXT;
					}
				} else {
					gr_DrawChar(s);
				}
			}
		} else {
			// draw simple character outside math mode
			if (*s == '\\') {
				if (*(s + 1) == '$') {
					slast = *s++;
				} else if (*(s + 1) == '"') {
					slast = *s++;
				} else if (*(s + 1) == '\\') {
					slast = *s++;
				}
			}
			gr_DrawChar(s);
		}
		slast = *s++;
	}
	STOP_OLD_TEXT;
	gr_setfontsize_pt(original_fontsize);
	gr_setfont(original_font);
	_drawingstarted = true;
	if (!pstack.empty()) {
		warning("a text string ended without completing a mathematical grouping (superscript, subscript, or {block})");
		pstack_erase();
	}
	return;
}

char* gr_fontname_from_id(int id)
{
	switch (id) {
	case gr_font_Courier:            return("Courier");
	case gr_font_CourierOblique:     return("Courier-Italic");
	case gr_font_CourierBold:        return("Courier-Bold");
	case gr_font_CourierBoldOblique: return("Courier-BoldItalic");
	case gr_font_Helvetica:          return("Helvetica");
	case gr_font_HelveticaOblique:   return("Helvetica-Italic");
	case gr_font_HelveticaBold:      return("Helvetica-Bold");
	case gr_font_PalatinoRoman: 
	case gr_font_PalatinoItalic:
	case gr_font_PalatinoBold:
	case gr_font_PalatinoBoldItalic:
		warning("SVG cannot handle Palatino font yet");
		return("Times");
	case gr_font_Symbol:               return("Symbol");
	case gr_font_TimesRoman:           return("Times");
	case gr_font_TimesItalic:          return("Times-Italic");
	case gr_font_TimesBold:            return("Times-Bold");
	case gr_font_TimesBoldItalic:      return("Times-BoldItalic");
	case gr_font_Century:     	   return("Century");
	}
	warning("SVG defaulting to Times font");
	return("Times");
}

static void gr_drawchar_svg(char c, double *xcm, double *ycm, gr_fontID font_id)
{
	char st[2];
	double size = gr_currentfontsize_pt();
	char *font_style;
	if (pstack.size() > 0)
		size *= SuperSize;
	if (isdigit(c) || font_id == gr_font_Symbol)
		font_style = "normal";
	else
		font_style = "italic";
	const char *fill = _griState.color_text().get_hexcolor().c_str();
	double transparency = _griState.color_text().getT();
	fprintf(_grSVG, "<g><text x=\"%.1f\" y=\"%.1f\" font-family=\"%s\" font-size=\"%.1f\" font-style=\"%s\" fill=\"%s\" opacity=\"%.2f\" style=\"fill:%s\">%c</text></g>\n",
		(*xcm) * PT_PER_CM, 
		/*gr_page_height_pt() -*/ -(*ycm) * PT_PER_CM,
		gr_fontname_from_id(font_id), size, font_style,
		fill,
		1.0 - transparency,
		fill,
		c);
	gr_setfont(font_id);
	st[0] = c; 
	st[1] = '\0';
	double w, a, d;
	double oldfontsize = gr_currentfontsize_pt();
	gr_setfontsize_pt(size);
	gr_stringwidth(st, &w, &a, &d); // BUG: NEED TO SET FONT FIRST
	gr_setfontsize_pt(oldfontsize);
#ifdef DEBUG
	printf("'%s' has width %f cm\n", st, w);
#endif
	*xcm += w;
}
static void
gr_drawstring_svg(const char *s, double xcm, double ycm, double angle)
{
#ifdef DEBUG
	printf("DEBUG %s:%d gr_drawstring_svg(s=\"%s\", xcm=%f, ycm=%f, angle=%f)\n", __FILE__,__LINE__, s, xcm, ycm, angle);
#endif
	int             slen = strlen(s);
	gr_fontID       original_font = gr_currentfont();
	gr_fontID       slant_font = original_font;	// prevent compiler warning
	double          original_fontsize = gr_currentfontsize_pt();
	bool            know_slant_font = false;
	if (slen <= 0)
		return;
	if (0.0 == gr_currentfontsize_pt())
		return;
	// Figure out slant font, if there is an appropriate one
	switch (original_font) {
	case gr_font_TimesRoman:
		slant_font = gr_font_TimesItalic;
		know_slant_font = true;
		break;
	case gr_font_TimesBold:
		slant_font = gr_font_TimesBoldItalic;
		know_slant_font = true;
		break;
	case gr_font_Helvetica:
		slant_font = gr_font_HelveticaOblique;
		know_slant_font = true;
		break;
	case gr_font_HelveticaBold:
		slant_font = gr_font_HelveticaBoldOblique;
		know_slant_font = true;
		break;
	case gr_font_Courier:
		slant_font = gr_font_CourierOblique;
		know_slant_font = true;
		break;
	case gr_font_CourierBold:
		slant_font = gr_font_CourierBoldOblique;
		know_slant_font = true;
		break;
	case gr_font_PalatinoRoman:
		slant_font = gr_font_PalatinoItalic;
		know_slant_font = true;
		break;
	case gr_font_PalatinoBold:
		slant_font = gr_font_PalatinoBoldItalic;
		know_slant_font = true;
		break;
        case gr_font_Century:
                slant_font = gr_font_TimesItalic; // BUG: should be Century Italic
		know_slant_font = true;
		break;
	default:
		know_slant_font = false;
	}
	std::vector<std::string> parts;
	parts = part_string(s);
	int nparts = parts.size();
	double xxcm = xcm, yycm = ycm;
	fprintf(_grSVG, "<g transform=\"translate(%.2f,%.2f)\">\n", xxcm*PT_PER_CM,
		gr_page_height_pt() - yycm*PT_PER_CM);
	xxcm = yycm = 0.0;
	fprintf(_grSVG, "<g transform=\"rotate(%.1f)\">\n", -angle);
	angle = 0.0;		// BUG: not sure if this is the best method
	//printf("DEBUG (%s)\n", _griState.color_text().get_hexcolor().c_str());
	for (int i = 0; i < nparts; i++) {
		double w, a, d;
		std::string p = parts[i];
		gr_stringwidth(p.c_str(), &w, &a, &d);
		//printf("    \"%s\" w=%f a=%f d=%f\n", parts[i].c_str(),w,a,d);
		if (p[0] != '$') {
			// Normal text
			fprintf(_grSVG, "<g><text x=\"%.1f\" y=\"%.1f\" font-family=\"%s\" font-size=\"%.1f\" font-style=\"normal\" fill=\"%s\" opacity=\"%.2f\" style=\"fill:%s\">%s</text></g>\n",
				xxcm * PT_PER_CM,
				/*gr_page_height_pt() -*/ -yycm * PT_PER_CM,
				gr_fontname_from_id(original_font),
				original_fontsize,
				_griState.color_text().get_hexcolor().c_str(),
				1.0 - _griState.color_text().getT(),
				_griState.color_text().get_hexcolor().c_str(),
				p.c_str());
			xxcm += w;
			//FIXME: should s/\$/$/ first
		} else {
			// Math text
			unsigned int ic, nc = p.size();
//			w = gr_thinspace_cm() / 1.0; // put a bit of space before math
//			xxcm += w;
			if (p[nc-1] == '$') nc--;
			fprintf(_grSVG, "<g>\n");
			for (ic = 1; ic < nc; ic++) {
#ifdef DEBUG
				printf(" -- %d [%c] --\n", ic, p[ic]);
#endif
				if (p[ic] == '\\') { // HEREHEREHERE
					if (p[ic+1] == '\\') {
						gr_drawchar_svg('\\', &xxcm, &yycm, original_font);
						ic += 1;
						continue;
					}
					char *insert;
					int inc;
					insert = symbol_in_math(p.c_str() + ic, &inc);
					int symbol_letter = index_for_math_symbol(insert);
					if (inc) {
						gr_drawchar_svg(symbol_letter, &xxcm, &yycm, gr_font_Symbol);
						ic += inc;
					} else {
						gr_drawchar_svg('?', &xxcm, &yycm, gr_font_Symbol);
					}
				} else if (p[ic] == '{') {
					// ignore
					pstack.push(Inline);
					ic++;
					continue;
				} else if (p[ic] == '}') {
					if (pstack.size() > 0) {
						position p = pstack.top();
						if (p == Superscript) {
							MoveDn_svg(&xxcm, &yycm);
						} else if (p == Subscript) {
							MoveUp_svg(&xxcm, &yycm);
						}
#ifdef DEBUG
						printf("DEBUG(%s:%d) about to pop stack (was %d) to make length %d\n",__FILE__,__LINE__,pstack.top(), int(pstack.size())-1);
#endif
						pstack.pop();
					} else {
						warning("unmatched \"}\" in a mathematical string");
					}
				} else if (p[ic] == '_') {
					pstack.push(Subscript);
					MoveDn_svg(&xxcm, &yycm);
					if (p[ic+1] != '{') { // BUG: should check for symbol
						gr_drawchar_svg(p[ic+1], &xxcm, &yycm, original_font);
						MoveUp_svg(&xxcm, &yycm);
						pstack.pop();
					}
					ic++;
					continue;
				} else if (p[ic] == '^') {
					pstack.push(Superscript);
					MoveUp_svg(&xxcm, &yycm);
					if (p[ic+1] != '{') { // BUG: should check for symbol
						gr_drawchar_svg(p[ic+1], &xxcm, &yycm, original_font);
						MoveDn_svg(&xxcm, &yycm);
						pstack.pop();
					}
					ic++;
					continue;
				} else {
					gr_drawchar_svg(p[ic], &xxcm, &yycm, original_font);
				}
			}
			fprintf(_grSVG, "</g>\n");
		}
	}
	fprintf(_grSVG, "</g>\n"); // rotate
	fprintf(_grSVG, "</g>\n"); // translate
	return;
}


// set fontsize in points
void
gr_setfontsize_pt(double fontsize_pt)
{
	if (fontsize_pt < 0.0)
		CurrentFont.size_pt = default_fontsize_pt;
	else
		CurrentFont.size_pt = fontsize_pt;
	gr_setfont_fontsize(CurrentFont.id);
}

// Set font encoding
void
gr_set_font_encoding(gr_font_encoding encoding)
{
	CurrentFont.encoding = encoding;
}
// Get font encoding
gr_font_encoding
gr_current_font_encoding()
{
	return CurrentFont.encoding;
}

/*
 * gr_currentfont() -- find current font synopsis int gr_currentfont()
 * description:  gets the current font,as set by gr_setfont(). return value:
 * current font number.
 */
gr_fontID
gr_currentfont()
{
	return CurrentFont.id;
}

/*
 * gr_currentfontsize_pt() -- return current fontsize in points
 */
double
gr_currentfontsize_pt()
{
	return CurrentFont.size_pt;
}

/*
 * gr_setfont() --  set new font. SYNOPSIS void	gr_setfont(int new_font)
 * DESCRIPTION:  Sets the font for future string drawing to 'new_font'. These
 * fonts are predefined: TimesRoman Helvetica Courier Symbol Palatino-Roman
 * Palatino-Italic.
 * 
 */
void
gr_setfont(gr_fontID newID, bool force /* default false */)
{
	gr_setfont_fontsize(newID, force);
}

static void
gr_setfont_fontsize(gr_fontID newID, bool force)
{
	int             i = 0;
	static bool     have_set_font = false;
	static gr_font  last_font;
	/* Search the font list */
	while (font_list[i].id != gr_font_end_of_list) {
		if (newID == font_list[i].id) {
			/* Found the font, but ignore request if no change */
			if (force
			    || (!have_set_font
				|| newID != last_font.id
				|| CurrentFont.encoding != last_font.encoding
				|| CurrentFont.size_pt != last_font.size_pt)) {
				CurrentFont.id = newID;
				if (!_grNeedBegin) {
					/*
					 * Don't try to write if haven't done gr_begin() yet,
					 * since then will ruin things like
					 * gr_setup_ps_filename();
					 */
					if (_grWritePS) {
						switch (CurrentFont.encoding) {
						case font_encoding_standard:
							fprintf(_grPS, "/%s findfont ", font_list[i].name);
							break;
						case font_encoding_isolatin1:
							if (CurrentFont.id == gr_font_Symbol) 
								fprintf(_grPS, "/%s findfont ", font_list[i].name);
							else
								fprintf(_grPS, "/%s-ISOLatin1 findfont ", font_list[i].name);
							break;
						}
						fprintf(_grPS, "%.2f sc sf\n", CurrentFont.size_pt);
					}
					have_set_font = true;
					last_font.id = newID;
					last_font.encoding = CurrentFont.encoding;
					last_font.size_pt = CurrentFont.size_pt;
				}
			}
			return;
		}
		i++;
	}
	warning("Ignoring request for unknown font.");
}

//here

static char *symbol_in_math(const char *sPtr, int *inc)
{
	/* handle greek letter or symbol in math mode */
	sPtr++;
	*inc = 0;
	for (int i = 0; i < NCODES; i++) {
		int len = strlen(symbol_code[i][0]);
		if (!strncmp(sPtr, symbol_code[i][0], len)) {
			*inc = len;
			return symbol_code[i][1];
		}
	}
	return NULL;
}

// Clear the position stack (doesn't STL do this??)
static void
pstack_erase()
{
	while (!pstack.empty()) {
		position p = pstack.top();
		if (p == Superscript) {
			MoveDown();
		} else if (p == Subscript) {
			MoveUp();
		} // ignore inline
#ifdef DEBUG
		if (p == Superscript)
			printf("\tcleared Superscript from position stack\n");
		else if (p == Subscript)
			printf("\tcleared Subscript from position stack\n");
#endif
		pstack.pop();
	}
}
// Move left/right by indicated number of M spaces
static void
MoveHorizontally(double em_distance)
{
	double          w, a, d;
	gr_stringwidth("M", &w, &a, &d);
	STOP_OLD_TEXT;
	gr_rmoveto_cm(em_distance * w, 0.0);
	START_NEW_TEXT;
}

// MoveUp() -- move up, shifting to smaller/larger size if necessary
static void MoveUp_svg(double *xcm, double *ycm)
{
#ifdef DEBUG
	printf("DEBUG(%s:%d) moving text position up one level.  Stack size on entry = %d\n", __FILE__,__LINE__,(int)pstack.size());
#endif
	if (!pstack.size())
		return;
	double dy;
	// See if already in subscript.
	position p = pstack.top();
	if (p == Subscript) {
		// Moving up from subscript, so enlarge font, then undo last move
		// down.
		gr_setfontsize_pt(gr_currentfontsize_pt() / SubSize);
		dy = SubMoveDown * gr_currentCapHeight_cm();
	} else {
		// Moving up from inline or superscript, so move up, then reduce font.
		dy = SuperMoveUp * gr_currentCapHeight_cm();
		gr_setfontsize_pt(gr_currentfontsize_pt() * SuperSize);
	} // ignore Inline
	*ycm += dy;
}
// MoveDown() -- move down, shifting to smaller/larger size if necessary
static void MoveDn_svg(double *xcm, double *ycm)
{
#ifdef DEBUG
	printf("DEBUG(%s:%d) moving text position down one level\n", __FILE__,__LINE__);
#endif
	if (!pstack.size())
		return;
	double dy;
	position p = pstack.top();
	// See if already in superscript.
	if (p == Superscript) {
		// Moving down from superscript, so enlarge font, then undo last move up.
		gr_setfontsize_pt(gr_currentfontsize_pt() / SuperSize);
		dy = -SuperMoveUp * gr_currentCapHeight_cm();
	} else {
		// Moving down from inline or subscript, so move down, then reduce font.
		dy = -SubMoveDown * gr_currentCapHeight_cm();
		gr_setfontsize_pt(gr_currentfontsize_pt() * SubSize);
	}
	*ycm += dy;
}

static void
MoveUp()
{
#ifdef DEBUG
	printf("DEBUG(%s:%d) moving text position up one level\n", __FILE__,__LINE__);
#endif
	STOP_OLD_TEXT;
	// See if already in subscript.
	position p = pstack.top();
	if (p == Subscript) {
		// Moving up from subscript, so enlarge font, then undo last move
		// down.
		gr_setfontsize_pt(gr_currentfontsize_pt() / SubSize);
		gr_rmoveto_pt(0.0, SubMoveDown * gr_currentCapHeight_cm() * PT_PER_CM);
	} else {
		// Moving up from inline or superscript, so move up, then reduce font.
		gr_rmoveto_pt(0.0, SuperMoveUp * gr_currentCapHeight_cm() * PT_PER_CM);
		gr_setfontsize_pt(gr_currentfontsize_pt() * SuperSize);
	} // ignore Inline
	START_NEW_TEXT;
}

// MoveDown() -- move down, shifting to smaller/larger size if necessary
static void
MoveDown()
{
#ifdef DEBUG
	printf("DEBUG(%s:%d) moving text position down one level\n", __FILE__,__LINE__);
#endif
	STOP_OLD_TEXT;
	position p = pstack.top();
	// See if already in superscript.
	if (p == Superscript) {
		// Moving down from superscript, so enlarge font, then undo last move up.
		gr_setfontsize_pt(gr_currentfontsize_pt() / SuperSize);
		gr_rmoveto_pt(0.0, -SuperMoveUp * gr_currentCapHeight_cm() * PT_PER_CM);
	} else {
		// Moving down from inline or subscript, so move down, then reduce font.
		gr_rmoveto_pt(0.0, -SubMoveDown * gr_currentCapHeight_cm() * PT_PER_CM);
		gr_setfontsize_pt(gr_currentfontsize_pt() * SubSize);
	}
	START_NEW_TEXT;
}

static void
gr_DrawChar(const char *c)
{
	extern bool     _grWritePS;
	if (_grWritePS) {
		extern FILE *_grPS;
		switch (*c) {
		case '\\':
			fprintf(_grPS, "\\\\");
			break;
		case '(':
			fprintf(_grPS, "\\(");
			break;
		case ')':
			fprintf(_grPS, "\\)");
			break;
		default:
			fprintf(_grPS, "%c", *c);
			break;
		}
		check_psfile();
	}
	_drawingstarted = true;
}

// Draw indicated text in a "whiteout" box of indicated color, left-right
// centered at the indicated (x,y) locn specified in user-units.  The text
// and box will be rotated by gr_currenttextangle_deg() degrees, measured
// counterclockwise from the horizontal.
void
gr_show_in_box(/*const*/GriString &s,
	       const GriColor& text_color,
	       const GriColor& box_color,
	       double x,	// cm units
	       double y,
	       double angle_deg)
{
	GriColor old_text_color = _griState.color_text();
	GriColor old_line_color = _griState.color_line();

	double          width, ascent, descent;
	double          x0, y0, dx, dy, dx_rot, dy_rot;
	double          thin_space = gr_thinspace_cm();
	if (0.0 == gr_currentfontsize_pt())
		return;
	gr_stringwidth(s.getValue(), &width, &ascent, &descent);
	x0 = x;			// save
	y0 = y;

	// White out below text.
	dx = -0.5 * width - thin_space;
	dy = -thin_space;
	gr_rotate_xy(dx, dy, angle_deg, &dx_rot, &dy_rot);

	static GriPath p(5);
	p.clear();
	p.push_back(x0 + dx_rot, y0 + dy_rot, 'm');

	dx = -dx;
	gr_rotate_xy(dx, dy, angle_deg, &dx_rot, &dy_rot);
	p.push_back(x0 + dx_rot, y0 + dy_rot, 'l');

	dx = 0.5 * width + thin_space;
	dy = ascent + thin_space;
	gr_rotate_xy(dx, dy, angle_deg, &dx_rot, &dy_rot);
	p.push_back(x0 + dx_rot, y0 + dy_rot, 'l');

	dx = -dx;
	gr_rotate_xy(dx, dy, angle_deg, &dx_rot, &dy_rot);
	p.push_back(x0 + dx_rot, y0 + dy_rot, 'l');

	p.push_back(x0 + dx_rot, y0 + dy_rot, 'l');
    
	_griState.set_color_line(box_color);
	p.fill(units_cm);

	bounding_box_update(p.bounding_box(units_cm));

	// Draw text
	_griState.set_color_text(text_color);
	dx = -0.5 * width;
	dy = 0.0;
	gr_rotate_xy(dx, dy, angle_deg, &dx_rot, &dy_rot);
	gr_show_at(s.getValue(),
		   x0 + dx_rot,
		   y0 + dy_rot,
		   TEXT_LJUST,
		   angle_deg);
	_griState.set_color_line(old_line_color);
	_griState.set_color_text(old_text_color);
	_drawingstarted = true;
}

// Rotate (x,y) into (xx,yy), through `angle' degrees counterclockwise.
void
gr_rotate_xy(double x, double y, double angle, double *xx, double *yy)
{
	angle /= DEG_PER_RAD;	// convert to radians
	double c = cos(angle);
	double s = sin(angle);
	*xx = c * x - s * y;
	*yy = s * x + c * y;
}

// Get the width, ascent and descent of string s, in current font.
// BUG: Ascent and descent are inaccurate.
// BUG: Smaller size of super/subscripts not accounted for.
void
gr_stringwidth(const char *s, double *w, double *a, double *d)
{
	*w = *a = *d = 0.0;
	if (strlen(s) == 0)
		return;
	bool            used_supers = false;
	bool            used_subs = false;
	bool            inmath = false;
	bool            oldWritePS = _grWritePS;
	double          oldfontsize_pt = gr_currentfontsize_pt();
	_grWritePS = false;
	while (*s != '\0') {
		// figure out whether entering or leaving math mode
		if (*s == '$' && *(s - 1) != '\\') {
#ifdef DEBUG
			printf("DEBUG %s:%d toggling inmath; rest of string is \"%s\"\n",__FILE__,__LINE__,s);
#endif
			inmath = (inmath ? false : true);
			s++;
			continue;
		}
		// handle math mode differently
		// ?? BUG Superscripts and subscripts are printed smaller, but
		// ?? BUG their size is assumed to be the same as normal chars.
		if (inmath) {
			if (*s == '^')	// handle superscript
				used_supers = true;
			else if (*s == '_')	// handle subscript
				used_subs = true;
			else if (*s == '{')	// ignore groups while computing string length
				;		// EMPTY
			else if (*s == '}')	// ignore groups
				;		// EMPTY
			else if (*s == '\\') {	// handle synonym
				int             skip;
				char *            ss;
				// First catch thinspace commands
				if (*(s + 1) == ',') {
					// Thinspace = Mwidth/6
					*w += gr_charwidth_cm((int)'M', CurrentFont.id, CurrentFont.size_pt) / 6.0;
					s += 1;
				} else if (*(s + 1) == '!') {
					// Negative thinspace = -Mwidth/6
					*w -= gr_charwidth_cm((int)'M', CurrentFont.id, CurrentFont.size_pt) / 6.0;
					s += 1;
				} else {
					ss = symbol_in_math(s, &skip); // NULL if can't find
					if (ss != NULL) {
						gr_fontID       oldfontID = CurrentFont.id;
						int             C = index_for_math_symbol(ss);
						s += skip;
						*w += gr_charwidth_cm(C, gr_font_Symbol, CurrentFont.size_pt);
						CurrentFont.id = oldfontID;
					} else {
						// it's not a known math symbol
						*w += gr_charwidth_cm('\\', CurrentFont.id, CurrentFont.size_pt);
					}
				}
			} else {
				// We are in math-mode, but it's not a special character. Add
				// appropriate amount for either super/subscript or normal
				// character.
				*w += gr_charwidth_cm((int) *s, CurrentFont.id, CurrentFont.size_pt);
			}
		} else {
			// not inmath
			*w += gr_charwidth_cm((int) *s, CurrentFont.id, CurrentFont.size_pt);
		}
		s++;
	}
	// Calculate ascent/descent.  BUG: doesn't take math chars into acct
	*a = gr_currentCapHeight_cm() * (1 + (used_supers ? 1 : 0) *
					 (SuperSize + SuperMoveUp - 1));
#if 0				// before 2.054
	*d = gr_currentCapHeight_cm() * (1.0 + 0.5 * (int) used_subs);
#endif
	*d = gr_current_descender() * (1 + (used_subs ? 1.0 : 0.0) *
				       (SubSize + SubMoveDown - 1));
#ifdef DEBUG
	printf("DEBUG %s:%d gr_stringwidth(s=\"%s\",...) RETURNING w=%f, a=%f d=%f\n", __FILE__,__LINE__,s,*w,*a,*d);
#endif
	// reset fontsize ... can't do with gr_setfontsize_pt() 
	// because that would call this function in infinite recursion.
	CurrentFont.size_pt = oldfontsize_pt;
	_grWritePS = oldWritePS;
}

// return index (for size-table) for a character (given as integer)
static int
index_for_math_symbol(const char *s)
{
	//printf("index_for_math_symbol(%s)\n",s);
	if (!s) return (int)'?';
	for (int i = 0; i < NCODES; i++) {
		//printf("  %3d (%s) (%s)\n", i, symbol_code[i][1],symbol_code[i][2]);
		if (!strncmp(s, symbol_code[i][1], strlen(symbol_code[i][1]))) {
			//printf(" match\n");
			return (int) *symbol_code[i][2];
		}
		//printf("  no match\n");
	}
	//printf("index_for_math_symbol(%s) cannot find a match\n", s);
	return (int) 'M';	// a guess, since we have no clue
}

// Return thinspace (=1/6 of width of "M" in current font), in cm
double
gr_thinspace_cm()
{
	return (gr_charwidth_cm(int('M'), CurrentFont.id, CurrentFont.size_pt) / 6.0);
}

// Return width of quad (= width of "M" in current font), in cm
double
gr_quad_cm()
{
	return (gr_charwidth_cm((int) 'M', CurrentFont.id, CurrentFont.size_pt));
}

// Following page should substituted as output from
// ~kelley/src/gri/src/get_font_metrics

struct font_metric {
	float XHeight;
	float CapHeight;
	float Ascender;
	float Descender;
	float width[128];
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Courier.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//

// Created by Perl script get_font_metrics.pl
struct font_metric CenturyRoman = {
    0.016369, // XHeight
    0.025471, // CapHeight
    0.026000, // Ascender
    -0.007232, // Descender
    { // Widths of first 128 characters
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0098072, 0.0104422, 0.0137231, 
        0.0196144, 0.0196144, 0.0293864, 0.0287514, 0.0071967, 
        0.0117475, 0.0117475, 0.0176389, 0.0213783, 0.0098072, 
        0.0117475, 0.0098072, 0.0098072, 0.0196144, 0.0196144, 
        0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0196144, 
        0.0196144, 0.0196144, 0.0196144, 0.0098072, 0.0098072, 
        0.0213783, 0.0213783, 0.0213783, 0.0156633, 0.0259997, 
        0.0254706, 0.0254706, 0.0254706, 0.0274461, 0.0254706, 
        0.0235303, 0.0274461, 0.0293864, 0.0143581, 0.0196144, 
        0.0274461, 0.0235303, 0.0333022, 0.0287514, 0.0274461, 
        0.0235303, 0.0274461, 0.0254706, 0.0222250, 0.0235303, 
        0.0287514, 0.0254706, 0.0346075, 0.0248356, 0.0248356, 
        0.0215547, 0.0117475, 0.0213783, 0.0117475, 0.0213783, 
        0.0176389, 0.0071967, 0.0196144, 0.0196144, 0.0156633, 
        0.0202494, 0.0176389, 0.0117475, 0.0189442, 0.0215547, 
        0.0111125, 0.0104422, 0.0209197, 0.0111125, 0.0313619, 
        0.0215547, 0.0176389, 0.0202494, 0.0196144, 0.0156633, 
        0.0163336, 0.0137231, 0.0215547, 0.0189442, 0.0274461, 
        0.0189442, 0.0189442, 0.0169686, 0.0117475, 0.0213783, 
        0.0117475, 0.0213783, 0.0000000
    }
};

struct font_metric Courier = {
	0.015028,	// XHeight   
	0.019826,	// CapHeight 
	0.022190,	// Ascender  
	-0.005539,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0211667, 0.0211667, 0.0211667, 
		0.0211667, 0.0211667, 0.0000000
	}
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Helvetica.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric Helvetica = {
	0.018450,	// XHeight   
	0.025329,	// CapHeight 
	0.025329,	// Ascender  
	-0.007302,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0098072, 0.0098072, 0.0125236, 
		0.0196144, 0.0196144, 0.0313619, 0.0235303, 0.0078317, 
		0.0117475, 0.0117475, 0.0137231, 0.0206022, 0.0098072, 
		0.0117475, 0.0098072, 0.0098072, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0098072, 0.0098072, 
		0.0206022, 0.0206022, 0.0206022, 0.0196144, 0.0358069, 
		0.0235303, 0.0235303, 0.0254706, 0.0254706, 0.0235303, 
		0.0215547, 0.0274461, 0.0254706, 0.0098072, 0.0176389, 
		0.0235303, 0.0196144, 0.0293864, 0.0254706, 0.0274461, 
		0.0235303, 0.0274461, 0.0254706, 0.0235303, 0.0215547, 
		0.0254706, 0.0235303, 0.0333022, 0.0235303, 0.0235303, 
		0.0215547, 0.0098072, 0.0098072, 0.0098072, 0.0165453, 
		0.0196144, 0.0078317, 0.0196144, 0.0196144, 0.0176389, 
		0.0196144, 0.0196144, 0.0098072, 0.0196144, 0.0196144, 
		0.0078317, 0.0078317, 0.0176389, 0.0078317, 0.0293864, 
		0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0117475, 
		0.0176389, 0.0098072, 0.0196144, 0.0176389, 0.0254706, 
		0.0176389, 0.0176389, 0.0176389, 0.0117828, 0.0091722, 
		0.0117828, 0.0206022, 0.0000000
	}
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Helvetica-Oblique.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric Helvetica_Oblique = {
	0.018450,	// XHeight   
	0.025329,	// CapHeight 
	0.025329,	// Ascender  
	-0.007302,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0098072, 0.0098072, 0.0125236, 
		0.0196144, 0.0196144, 0.0313619, 0.0235303, 0.0078317, 
		0.0117475, 0.0117475, 0.0137231, 0.0206022, 0.0098072, 
		0.0117475, 0.0098072, 0.0098072, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0098072, 0.0098072, 
		0.0206022, 0.0206022, 0.0206022, 0.0196144, 0.0358069, 
		0.0235303, 0.0235303, 0.0254706, 0.0254706, 0.0235303, 
		0.0215547, 0.0274461, 0.0254706, 0.0098072, 0.0176389, 
		0.0235303, 0.0196144, 0.0293864, 0.0254706, 0.0274461, 
		0.0235303, 0.0274461, 0.0254706, 0.0235303, 0.0215547, 
		0.0254706, 0.0235303, 0.0333022, 0.0235303, 0.0235303, 
		0.0215547, 0.0098072, 0.0098072, 0.0098072, 0.0165453, 
		0.0196144, 0.0078317, 0.0196144, 0.0196144, 0.0176389, 
		0.0196144, 0.0196144, 0.0098072, 0.0196144, 0.0196144, 
		0.0078317, 0.0078317, 0.0176389, 0.0078317, 0.0293864, 
		0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0117475, 
		0.0176389, 0.0098072, 0.0196144, 0.0176389, 0.0254706, 
		0.0176389, 0.0176389, 0.0176389, 0.0117828, 0.0091722, 
		0.0117828, 0.0206022, 0.0000000
	}
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Helvetica-Bold.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric Helvetica_Bold = {
	0.018768,	// XHeight   
	0.025329,	// CapHeight 
	0.025329,	// Ascender  
	-0.007302,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0098072, 0.0117475, 0.0167217, 
		0.0196144, 0.0196144, 0.0313619, 0.0254706, 0.0098072, 
		0.0117475, 0.0117475, 0.0137231, 0.0206022, 0.0098072, 
		0.0117475, 0.0098072, 0.0098072, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0196144, 0.0196144, 
		0.0196144, 0.0196144, 0.0196144, 0.0117475, 0.0117475, 
		0.0206022, 0.0206022, 0.0206022, 0.0215547, 0.0343958, 
		0.0254706, 0.0254706, 0.0254706, 0.0254706, 0.0235303, 
		0.0215547, 0.0274461, 0.0254706, 0.0098072, 0.0196144, 
		0.0254706, 0.0215547, 0.0293864, 0.0254706, 0.0274461, 
		0.0235303, 0.0274461, 0.0254706, 0.0235303, 0.0215547, 
		0.0254706, 0.0235303, 0.0333022, 0.0235303, 0.0235303, 
		0.0215547, 0.0117475, 0.0098072, 0.0117475, 0.0206022, 
		0.0196144, 0.0098072, 0.0196144, 0.0215547, 0.0196144, 
		0.0215547, 0.0196144, 0.0117475, 0.0215547, 0.0215547, 
		0.0098072, 0.0098072, 0.0196144, 0.0098072, 0.0313619, 
		0.0215547, 0.0215547, 0.0215547, 0.0215547, 0.0137231, 
		0.0196144, 0.0117475, 0.0215547, 0.0196144, 0.0274461, 
		0.0196144, 0.0196144, 0.0176389, 0.0137231, 0.0098778, 
		0.0137231, 0.0206022, 0.0000000
	}
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Palatino-Roman.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric PalatinoRoman = {
	0.016545,	// XHeight   
	0.024412,	// CapHeight 
	0.025612,	// Ascender  
	-0.009913,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0088194, 0.0098072, 0.0130881, 
		0.0176389, 0.0176389, 0.0296333, 0.0274461, 0.0098072, 
		0.0117475, 0.0117475, 0.0137231, 0.0213783, 0.0088194, 
		0.0117475, 0.0088194, 0.0213783, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0088194, 0.0088194, 
		0.0213783, 0.0213783, 0.0213783, 0.0156633, 0.0263525, 
		0.0274461, 0.0215547, 0.0250119, 0.0273050, 0.0215547, 
		0.0196144, 0.0269169, 0.0293511, 0.0118886, 0.0117475, 
		0.0256117, 0.0215547, 0.0333728, 0.0293158, 0.0277283, 
		0.0213078, 0.0277283, 0.0235656, 0.0185208, 0.0216253, 
		0.0274461, 0.0254706, 0.0352778, 0.0235303, 0.0235303, 
		0.0235303, 0.0117475, 0.0213783, 0.0117475, 0.0213783, 
		0.0176389, 0.0098072, 0.0176389, 0.0195086, 0.0156633, 
		0.0215547, 0.0168981, 0.0117475, 0.0196144, 0.0205317, 
		0.0102658, 0.0082550, 0.0196144, 0.0102658, 0.0311503, 
		0.0205317, 0.0192617, 0.0212019, 0.0197556, 0.0139347, 
		0.0149578, 0.0115006, 0.0212725, 0.0199319, 0.0294217, 
		0.0182033, 0.0196144, 0.0176389, 0.0117475, 0.0213783, 
		0.0117475, 0.0213783, 0.0000000
	}
};
// Created by Perl script get_font_metrics.pl
struct font_metric PalatinoItalic = {
    0.017004, // XHeight
    0.024412, // CapHeight
    0.025859, // Ascender
    -0.009737, // Descender
    { // Widths of first 128 characters
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0088194, 0.0117475, 0.0176389, 
        0.0176389, 0.0176389, 0.0313619, 0.0274461, 0.0098072, 
        0.0117475, 0.0117475, 0.0137231, 0.0213783, 0.0088194, 
        0.0117475, 0.0088194, 0.0104422, 0.0176389, 0.0176389, 
        0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0176389, 
        0.0176389, 0.0176389, 0.0176389, 0.0088194, 0.0088194, 
        0.0213783, 0.0213783, 0.0213783, 0.0176389, 0.0263525, 
        0.0254706, 0.0215547, 0.0235303, 0.0274461, 0.0215547, 
        0.0196144, 0.0254706, 0.0274461, 0.0117475, 0.0117475, 
        0.0235303, 0.0196144, 0.0333022, 0.0274461, 0.0274461, 
        0.0215547, 0.0274461, 0.0235303, 0.0196144, 0.0215547, 
        0.0274461, 0.0254706, 0.0333022, 0.0254706, 0.0235303, 
        0.0235303, 0.0117475, 0.0213783, 0.0117475, 0.0213783, 
        0.0176389, 0.0098072, 0.0156633, 0.0163336, 0.0143581, 
        0.0176389, 0.0137231, 0.0098072, 0.0176389, 0.0176389, 
        0.0098072, 0.0098072, 0.0156633, 0.0098072, 0.0274461, 
        0.0196144, 0.0156633, 0.0176389, 0.0163336, 0.0137231, 
        0.0137231, 0.0117475, 0.0196144, 0.0176389, 0.0254706, 
        0.0176389, 0.0176389, 0.0156633, 0.0117475, 0.0213783, 
        0.0117475, 0.0213783, 0.0000000
    }
};



// Created by Perl script get_font_metrics.pl
struct font_metric PalatinoBold = {
    0.016616, // XHeight
    0.024024, // CapHeight
    0.025400, // Ascender
    -0.009102, // Descender
    { // Widths of first 128 characters
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
        0.0000000, 0.0000000, 0.0088194, 0.0098072, 0.0141817, 
        0.0176389, 0.0176389, 0.0313619, 0.0293864, 0.0098072, 
        0.0117475, 0.0117475, 0.0156633, 0.0213783, 0.0088194, 
        0.0117475, 0.0088194, 0.0104422, 0.0176389, 0.0176389, 
        0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0176389, 
        0.0176389, 0.0176389, 0.0176389, 0.0088194, 0.0088194, 
        0.0213783, 0.0213783, 0.0213783, 0.0156633, 0.0263525, 
        0.0274461, 0.0235303, 0.0254706, 0.0293864, 0.0215547, 
        0.0196144, 0.0293864, 0.0293864, 0.0137231, 0.0137231, 
        0.0274461, 0.0215547, 0.0352778, 0.0293864, 0.0293864, 
        0.0215547, 0.0293864, 0.0254706, 0.0215547, 0.0235303, 
        0.0274461, 0.0274461, 0.0352778, 0.0235303, 0.0235303, 
        0.0235303, 0.0117475, 0.0213783, 0.0117475, 0.0213783, 
        0.0176389, 0.0098072, 0.0176389, 0.0215547, 0.0156633, 
        0.0215547, 0.0176389, 0.0137231, 0.0196144, 0.0215547, 
        0.0117475, 0.0117475, 0.0215547, 0.0117475, 0.0313619, 
        0.0215547, 0.0196144, 0.0215547, 0.0215547, 0.0137231, 
        0.0156633, 0.0117475, 0.0215547, 0.0196144, 0.0293864, 
        0.0176389, 0.0196144, 0.0176389, 0.0109361, 0.0213783, 
        0.0109361, 0.0213783, 0.0000000
    }
};


//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Symbol.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric Symbol = {
	0.000000,	// XHeight   
	0.000000,	// CapHeight 
	0.000000,	// Ascender  
	0.000000,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0088194, 0.0117475, 0.0251531, 
		0.0176389, 0.0193675, 0.0293864, 0.0274461, 0.0154869, 
		0.0117475, 0.0117475, 0.0176389, 0.0193675, 0.0088194, 
		0.0193675, 0.0088194, 0.0098072, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0098072, 0.0098072, 
		0.0193675, 0.0193675, 0.0193675, 0.0156633, 0.0193675, 
		0.0254706, 0.0235303, 0.0254706, 0.0215900, 0.0215547, 
		0.0269169, 0.0212725, 0.0254706, 0.0117475, 0.0222603, 
		0.0254706, 0.0242006, 0.0313619, 0.0254706, 0.0254706, 
		0.0270933, 0.0261408, 0.0196144, 0.0208844, 0.0215547, 
		0.0243417, 0.0154869, 0.0270933, 0.0227542, 0.0280458, 
		0.0215547, 0.0117475, 0.0304447, 0.0117475, 0.0232128, 
		0.0176389, 0.0176389, 0.0222603, 0.0193675, 0.0193675, 
		0.0174272, 0.0154869, 0.0183797, 0.0144992, 0.0212725, 
		0.0116064, 0.0212725, 0.0193675, 0.0193675, 0.0203200, 
		0.0183797, 0.0193675, 0.0193675, 0.0183797, 0.0193675, 
		0.0212725, 0.0154869, 0.0203200, 0.0251531, 0.0242006, 
		0.0173919, 0.0242006, 0.0174272, 0.0169333, 0.0070556, 
		0.0169333, 0.0193675, 0.0000000
	}
};
//
// Following font metric generated by `get_font_metrics'
// perlscript from Font Metric file `/usr/openwin/lib/X11/fonts/F3/afm/Times-Roman.afm'.
// All measurement in centimetres, given a pointsize of 1.0
//
struct font_metric TimesRoman = {
	0.015875,	// XHeight   
	0.023354,	// CapHeight 
	0.024095,	// Ascender  
	-0.007655,	// Descender 
	{	// Widths of first 128 characters
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 
		0.0000000, 0.0000000, 0.0088194, 0.0117475, 0.0143933, 
		0.0176389, 0.0176389, 0.0293864, 0.0274461, 0.0117475, 
		0.0117475, 0.0117475, 0.0176389, 0.0198967, 0.0088194, 
		0.0117475, 0.0088194, 0.0098072, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0176389, 
		0.0176389, 0.0176389, 0.0176389, 0.0098072, 0.0098072, 
		0.0198967, 0.0198967, 0.0198967, 0.0156633, 0.0324908, 
		0.0254706, 0.0235303, 0.0235303, 0.0254706, 0.0215547, 
		0.0196144, 0.0254706, 0.0254706, 0.0117475, 0.0137231, 
		0.0254706, 0.0215547, 0.0313619, 0.0254706, 0.0254706, 
		0.0196144, 0.0254706, 0.0235303, 0.0196144, 0.0215547, 
		0.0254706, 0.0254706, 0.0333022, 0.0254706, 0.0254706, 
		0.0215547, 0.0117475, 0.0098072, 0.0117475, 0.0165453, 
		0.0176389, 0.0117475, 0.0156633, 0.0176389, 0.0156633, 
		0.0176389, 0.0156633, 0.0117475, 0.0176389, 0.0176389, 
		0.0098072, 0.0098072, 0.0176389, 0.0098072, 0.0274461, 
		0.0176389, 0.0176389, 0.0176389, 0.0176389, 0.0117475, 
		0.0137231, 0.0098072, 0.0176389, 0.0176389, 0.0254706, 
		0.0176389, 0.0176389, 0.0156633, 0.0169333, 0.0070556, 
		0.0169333, 0.0190853, 0.0000000
	}
};

/*
 * gr_charwidth_cm(char c, int font, double fontsize_pt)
 * 
 * RETURN VALUE the width of the character, in centimetres
 * 
 * Font info created by the `get_font_metrics' perlscript, in the Gri src
 * directory.  This looks in the OpenWindows font metrics files to figure the
 * pertintent stuff out.  (You might have to edit this for different
 * machines).
 */
static double
gr_charwidth_cm(int c, int font, double fontsize_pt)
{
	unsigned char   i = (int) c;
	if (i > 127)
		return fontsize_pt * 0.0211663;	/* error, but guess Courier size
						 * anyway */
	switch (font) {
	case gr_font_TimesRoman:
		return fontsize_pt * TimesRoman.width[i];
	case gr_font_Courier:
		return fontsize_pt * 0.0211663;	/* Courier has fixed width */
	case gr_font_Symbol:
		return fontsize_pt * Symbol.width[i];
	case gr_font_Helvetica:
		return fontsize_pt * Helvetica.width[i];
	case gr_font_HelveticaBold:
		return fontsize_pt * Helvetica_Bold.width[i];
	case gr_font_PalatinoRoman:
		return fontsize_pt * PalatinoRoman.width[i];
	case gr_font_Century:
                return fontsize_pt * CenturyRoman.width[i];
	default:
		break;
		/* Guess similar size to Helvetica */
	}
	return (fontsize_pt * Helvetica.width[i]);
}

double
gr_current_descender()		// descender, in positive cm
{
	switch (CurrentFont.id) {
	case gr_font_Courier:
		return (-CurrentFont.size_pt * Courier.Descender);
	case gr_font_Helvetica:
		return(-CurrentFont.size_pt * Helvetica.Descender);
	case gr_font_Symbol:
		return(-CurrentFont.size_pt * Symbol.Descender);
	case gr_font_TimesRoman:
		return(-CurrentFont.size_pt * TimesRoman.Descender);
	case gr_font_Century:
		return(-CurrentFont.size_pt * CenturyRoman.Descender);
	case gr_font_PalatinoRoman:
		return(-CurrentFont.size_pt * PalatinoRoman.Descender);
	case gr_font_PalatinoItalic:
		return(-CurrentFont.size_pt * PalatinoItalic.Descender);
	case gr_font_PalatinoBold:
		return(-CurrentFont.size_pt * PalatinoBold.Descender);
	default:
		break;
	}
	return CurrentFont.size_pt * 0.025329; // Guess similar to Helvetica
}


// gr_currentCapHeight_cm() -- find height of capital characters
double
gr_currentCapHeight_cm()
{
	double          height_cm = 0.0;
	switch (CurrentFont.id) {
	case gr_font_Courier:
		height_cm = (CurrentFont.size_pt * Courier.CapHeight);
		break;
	case gr_font_Helvetica:
		height_cm = (CurrentFont.size_pt * Helvetica.CapHeight);
		break;
	case gr_font_Symbol:
		height_cm = (CurrentFont.size_pt * Symbol.CapHeight);
		break;
	case gr_font_TimesRoman:
		height_cm = (CurrentFont.size_pt * TimesRoman.CapHeight);
		break;
	case gr_font_Century:
	        height_cm = (CurrentFont.size_pt * CenturyRoman.CapHeight);
		break;
	case gr_font_PalatinoRoman:
	        height_cm = (CurrentFont.size_pt * PalatinoRoman.CapHeight);
		break;
	case gr_font_PalatinoItalic:
	        height_cm = (CurrentFont.size_pt * PalatinoItalic.CapHeight);
		break;
	case gr_font_PalatinoBold:
	        height_cm = (CurrentFont.size_pt * PalatinoBold.CapHeight);
		break;
	default:
		/*
		 * Don't know this font.
		 */
		break;
	}
	if (height_cm > 0.0)
		return height_cm;
	else
		return CurrentFont.size_pt * 0.025329;	/* Guess Helvetica */
}
