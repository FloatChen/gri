// Private definitions for gri. See also extern.hh

#if !defined(_private_)
#define		_private_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>		// part of STL
#include "gr.hh"
#include "gr_coll.hh"
#include "errors.hh"
#include "files.hh"
#include "GriTimer.hh"
#include "DataFile.hh"



#define         cmd_being_done_LEN       100	// cmd stack
#define		LineLength	       32768	// max length of line
#define		LineLength_1	       32767	// -1 + max length of line
#define		MAX_cmd_word		8192	// words/command
#define		MAX_nword		8192	// words/line
#define		_num_dstackMAX		  50	// length of dstack
#define		_imageBLANK		 255	// blank images with white

// Image storage.
typedef struct {
	int             ras_magic;	// magic number
	unsigned int    ras_width;	// width (pixels) of image
	unsigned int    ras_height;	// height (pixels) of image
	unsigned int    ras_depth;	// depth (1, 8, or 24 bits) of pixel
	unsigned int    ras_length;	// length (bytes) of image
	int             ras_type;	// type of file; see RT_* below
	int             ras_maptype;// type of colormap; see RMT_* below
	unsigned int    ras_maplength; // length (bytes) of following map
	unsigned char  *map;	   // map
	unsigned char  *image;	   // image
}               IMAGE;

// From Sun's /usr/include/rasterfile.h
#define	RAS_MAGIC	0x59a66a95
#define RT_STANDARD	1
#define RMT_NONE        0
#define RMT_EQUAL_RGB   1
#define RMT_RAW         2




bool            allocate_grid_storage(int nx, int ny);
bool            allocate_image_storage(int nx, int ny);
bool            allocate_imageMask_storage(int nx, int ny);
bool            allocate_xmatrix_storage(int n);
bool            allocate_ymatrix_storage(int n);
bool            assign_synonym(void);
bool            batch(void);
void            beep_terminal(void);
bool            blank_image(void);
bool            blank_imageMask(void);
int             block_level(void);
char           *block_source_file(void);
unsigned int    block_source_line(void);
unsigned int    block_offset_line(void);
void            bounding_box_display(const char *msg);
void            bounding_box_update(const rectangle& box);

bool locate_i_j(double xx, double yy, int *ii, int *jj);
#if defined(OLD_IMAGE_INTERPOLATION)
bool value_i_j(unsigned int ii, unsigned int jj, double xx, double yy, double *value);
#else
bool value_i_j(unsigned int ii, unsigned int jj, double xx, double yy, double *value);
#endif

bool            calculate_image_histogram(void);
void            check_psfile(void);
bool            chop_into_words(char *s, char **w, int *nw, int max);
bool            chop_into_data_words(char *s, char **w, int *nw, int max);
void            clear_eof_flag_on_data_file(void);
void            close_data_files();
int             cmd_being_done(void);
bool            create_commands(const char *filename);
bool            create_color(const char *name, double r, double g, double b);
bool            create_new_command(FILE * fp, char *line);
bool            create_synonym(const char *name, const char *value);
bool            create_variable(const char *name, double value);
bool            create_x_scale(void);
bool            create_y_scale(void);
char           *cshell_name(const char *s);

int             data_file_index(const char *name);
bool            define_image_scales(double llx, double lly, double urx, double ury);
bool            delete_file(const char *filename);
bool            delete_syn(const char *name);
bool            delete_var(const char *name);
bool            demonstrate_command_usage(void);
void            display_cmd_being_done_stack();
void            display_cmd_stack(const char *s);
void            display_data_stack(const char *s);
void            display_unused_var(void);
void            display_unused_syn(void);
bool            do_command_line(void);
bool            draw_axes(int type, double loc, gr_axis_properties side, bool allow_offset);
bool            draw_axes_if_needed(void);
bool            draw_gri_logo(void);
char           *egetenv(const char *s);
void            end_up(void);
void            expand_blanks(char *cmdline);
int             ExtractQuote(char *sout, const char *s);
bool            extract_help(FILE * fp, char *line);
bool            extract_procedure(FILE * fp, char *line);
bool            extract_syntax(char *line);
char           *file_in_list(const char *name, bool show_nonlocal_files, bool show_local_files);
const char     *filename_sans_dir(const char *fullfilename);
bool            find_min_max(double *data, int num, double *min, double *max);
bool            find_min_max_v(void);
bool            find_min_max_x(void);
bool            find_min_max_y(void);
bool            find_min_max_z(void);
int             get_cmd_values(char **w, int nw, const char *key, int nobjects, double *objects);
bool            get_c_file_name(int old, const char *prompt, const char *name);
bool            get_command_line(void);
bool            getdnum(const char *string, double *number);
bool            get_flag(const char *name);
bool            getinum(const char *string, int *number);
bool            get_var(const char *name, double *value);
bool            get_syn(const char *name, char *value);
void            give_help(void);
void            gri_abort(void);
void            gri_exit(int code);
bool            grid_interp(double xx, double yy, double *value);
bool            grid_exists(void);
bool            gr_missing(double x);
bool            handle_if_block(void);
void            highpass_image(void);
void            histogram_stats(const double *x, unsigned int n, double *q1, double *q2, double *q3);
bool            ignoreCmd(void);
bool            image_exists(void);
bool            image_mapping_exists(void);
bool            image_range_exists(void);
bool            imageMask_exists(void);
bool            image_scales_defined(void);
double          image_to_value(int c);
bool            inside_box(double x, double y);
bool            is_create_new_command(const char *cmdline);
bool            is_punctuation(int c);
bool            is_syn(const char *name);
bool            is_system_command(const char *s);
bool            is_var(const char *w);
char            last_character(const char *s);
bool            look_up_color(const char *name, double *red, double *green, double *blue);
void            lowpass_image(void);
bool            massage_command_line(char *cmdline);
int             match_gri_syntax(const char *cmdline, int flag);
bool            mathCmd(void);
void            matrix_limits(double *min, double *max);
void            moment(double *data, int n, double *ave, double *adev, double *sdev, double *svar, double *skew, double *kurt);
void            more_file_to_terminal(const char *filename);
void            no_scales_error(void);
int             number_good_xyz(double x[], double y[], double f[], int n);
unsigned int    number_missing_cols(void);
int             parse_C_commandCmd(const char *s);
bool            perform_block(const char *s, const char *source_file, int source_line);
bool            perform_command_line(FILE * fp, bool is_which);
int             perform_gri_cmd(int cmd);
bool            perform_gri_program(void);
bool            perform_while_block(const char *buffer, const char *test, int lines);
void            pop_command_word_buffer(void);
void            pop_cmd_being_done_stack(void);
void            push_cmd_being_done_stack(int cmd);
bool            push_cmd_file(const char *fname, bool interactive, bool allow_warning, const char *status);
void            push_command_word_buffer(void);
bool            put_var(const char *name, double value, bool replace_existing);
bool            put_syn(const char *name, const char *value, bool replace_existing);
bool            pop_data_file(int file);
bool            push_data_file(const char *name, DataFile::type the_type, const char *status, bool delete_when_close);
bool            push_data_file_to_top(const char *name);
char           *pwd(void);
double          quantize(double x, int levels, double dx);
bool            quoted(const char *string);
bool            re_compare(const char *string, const char *pattern);
void            remove_comment(char *cmdline);
void            remove_esc_quotes(char *w);
void            remove_trailing_blanks(char *s);
bool            resolve_filename(string& fn, bool trace_path);  
bool            rpn_create_function(char *name, char **w, unsigned int nw);
bool            same_syntax(const char *cmdline, const char *syntax, int flag);
bool            same_word(const char *cp, const char *sp);
bool            scales_defined(void);
bool            set_environment(void);
bool            set_flagCmd(void);
void            set_eof_flag_on_data_file(void);
void            set_line_width_axis(void);
void            set_line_width_curve(void);
void            set_line_width_symbol(void);
void            set_ps_color(char path_or_text);
void            set_up_command_word_buffer(void);
bool            set_x_scale(void);
bool            set_y_scale(void);
bool            show_grid_maskCmd(void);
bool            show_next_lineCmd(void);
void            show_var_stack(void);
void            show_syn_stack();
int             skip_space(const char *s);
int             skip_nonspace(const char *s);
bool            skipping_through_if(void);
bool            start_up(int argc, char ** argv);
bool            stop_replay_if_error(void);
void            strcat_c(char *s, int c);
bool            string_is_blank(const char *s);
bool            substitute_rpn_expressions(const char *cmdline, char *cmdlinecopy);
bool            substitute_synonyms_cmdline(const char *s, char *sout, bool allow_math);
bool            substitute_synonyms(const char *s, char *sout, bool allow_math);
unsigned int    superuser(void);
void            swap(double& a, double& b);
bool            systemCmd(void);
char*           tmp_file_name();
bool            tracing(void);
bool            update_readfrom_file_name(void);
char*           unbackslash(const char *s);
unsigned char   value_to_image(double v);
void		vector_reverse(double *x, int n);
double          vector_min(double *x, unsigned n);
double          vector_max(double *x, unsigned n);

#if defined(VMS)
void
warning(va_dcl va_alist);
#else
void            warning(const char *string, ...);
#endif

bool            warn_if_slow(GriTimer *t, double fraction_done, const char *cmd);
int             what_line(void);
const char     *what_file(void);
bool            well_ordered(double min, double max, double inc);
bool            word_is(int i, const char *word);
void            write_prompt(void);

#endif				// not _private_
