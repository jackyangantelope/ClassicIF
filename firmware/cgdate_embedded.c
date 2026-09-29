#include <string.h>
#include "glk.h"

#define JACK_IF_EPOCH_2026 1767225600u

void glk_current_time(glktimeval_t *time)
{
    time->high_sec = 0;
    time->low_sec = JACK_IF_EPOCH_2026;
    time->microsec = 0;
}

glsi32 glk_current_simple_time(glui32 factor)
{
    return factor ? (glsi32)(JACK_IF_EPOCH_2026 / factor) : 0;
}

static void fixed_date(glkdate_t *date)
{
    memset(date, 0, sizeof(*date));
    date->year = 2026;
    date->month = 1;
    date->day = 1;
    date->weekday = 4;
}

void glk_time_to_date_utc(glktimeval_t *time, glkdate_t *date) { (void)time; fixed_date(date); }
void glk_time_to_date_local(glktimeval_t *time, glkdate_t *date) { (void)time; fixed_date(date); }
void glk_simple_time_to_date_utc(glsi32 time, glui32 factor, glkdate_t *date)
{ (void)time; (void)factor; fixed_date(date); }
void glk_simple_time_to_date_local(glsi32 time, glui32 factor, glkdate_t *date)
{ (void)time; (void)factor; fixed_date(date); }
void glk_date_to_time_utc(glkdate_t *date, glktimeval_t *time)
{ (void)date; glk_current_time(time); }
void glk_date_to_time_local(glkdate_t *date, glktimeval_t *time)
{ (void)date; glk_current_time(time); }
glsi32 glk_date_to_simple_time_utc(glkdate_t *date, glui32 factor)
{ (void)date; return factor ? (glsi32)(JACK_IF_EPOCH_2026 / factor) : 0; }
glsi32 glk_date_to_simple_time_local(glkdate_t *date, glui32 factor)
{ (void)date; return factor ? (glsi32)(JACK_IF_EPOCH_2026 / factor) : 0; }