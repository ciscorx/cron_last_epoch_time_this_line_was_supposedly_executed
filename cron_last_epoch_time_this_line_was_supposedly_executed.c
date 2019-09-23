/**********************************************************************
**  cron_last_epoch_time_this_line_was_supposedly_executed.c         **
**                                                                   **
** 
**  This program outputs to stdio the epoch time at which a given user
**  crontab expression was supposedly last executed, in addition to
**  the disposition element of the cron expression, if present.  The
**  expression should be comprised of one line from a user crontab
**  delimited by double quotes, passed as an argument.  Optionally, a
**  second argument may be supplied specifying the epoch time from
**  which to start.
**
**  Example:
**     ./cron_last_epoch_time_this_line_was_supposedly_executed "0 22 * * mon,tue,wed,thu,fri disable_wifi.sh" 1569016800
**     This outputs: 1568930400 disable_wifi.sh
**
**  Dependencies: 
**      ccronexpr.c borrowed from https://github.com/staticlibs/ccronexpr
**
**  To compile under linux:  
**      gcc -o cron_last_epoch_time_this_line_was_supposedly_executed cron_last_epoch_time_this_line_was_supposedly_executed.c ccronexpr.c
***********************************************************************/
#include "ccronexpr.h"
#include <stdlib.h>

int position_of_beginning_of_next_word_boundary(char* string, int initial_pos) {
    int pos;
    pos = initial_pos;
    if (string == NULL)
	return -1;
    while( string[pos] != NULL && string[pos] != ' ' )       
	pos = pos + 1;
    if (string[pos] == NULL)
	return -1;

    while( string[pos] == ' ' )	    
	pos = pos + 1;
       
    return pos;
}

int position_of_end_of_next_word_boundary(char* string, int initial_pos) {
    int pos;
    pos = initial_pos;
    if (string == NULL)
	return -1;
    while( string[pos] != NULL && string[pos] != ' ' )       
	pos = pos + 1;
    return pos;
}

int main(int argc, char *argv[]) {
    cron_expr expr;
    const char* err;
    char* cron_line;
    char* cron_line_disposition;
    char* cron_line_schedule;
    int i;
    int word_boundary_pos;
    int last_word_boundary_pos;
    int beginning_of_next_word_boundary;    
    time_t cur;
    struct tm  ts;

    last_word_boundary_pos = 0;
    beginning_of_next_word_boundary = 0;
    word_boundary_pos = 0;
    err = NULL;
    if (argc == 2 && *argv[1] != NULL && (argv[1][0] == '-' || argv[1][0] == 'h') ) {
	printf("\n");
	printf("     This program outputs to stdio the epoch time at which a given user\n");
	printf("     crontab expression was supposedly last executed, in addition to\n");
	printf("     the disposition element of the cron expression, if present.  The\n");
	printf("     expression should be comprised of one line from a user crontab\n");
	printf("     delimited by double quotes, passed as an argument.  Optionally, a\n");
	printf("     second argument may be supplied specifying the epoch time from\n");
	printf("     which to start.  BTW user crontab expressions dont have seconds fields.\n");
	printf("\n");
	printf("     Example:\n");
	printf("        ./cron_last_epoch_time_this_line_was_supposedly_executed \"0 22 * * mon,tue,wed,thu,fri disable_wifi.sh\" 1569016800\n");
	printf("        This outputs: 1568930400 disable_wifi.sh\n");
	exit(0);
    }


    if (argc == 2 || argc == 3) {
	cron_line_disposition = (char*) malloc(sizeof(char)*1000);
	cron_line_schedule = (char*) malloc(sizeof(char)*1000);
	strcpy(cron_line_schedule,"0 ");    /* user crontabs only have 5 schedule fields, so we must add one to represent seconds*/
	strcat(cron_line_schedule,argv[1]);
	for (i=0;i<6;i++) {
	    beginning_of_next_word_boundary = position_of_beginning_of_next_word_boundary(cron_line_schedule, word_boundary_pos);
	    
	    if (beginning_of_next_word_boundary != -1) {
		last_word_boundary_pos = word_boundary_pos;
		word_boundary_pos = beginning_of_next_word_boundary;
	
	    } else if (i<5) {
		free(cron_line_disposition);
		free(cron_line_schedule);
		exit(0);
	    }		     
	} 
	if (beginning_of_next_word_boundary != -1) {
	    last_word_boundary_pos = position_of_end_of_next_word_boundary(cron_line_schedule,last_word_boundary_pos);
	    strcpy(cron_line_disposition, cron_line_schedule + last_word_boundary_pos+1);
	    cron_line_schedule[last_word_boundary_pos] = NULL;	    
	}

	if (argc == 3) 
	    cur = (time_t) atof(argv[2]);
	else {
	    /* truncate seconds to 0 if current time*/
	    cur = time(NULL);   
	    ts = *localtime(&cur);
	    ts.tm_sec = 0;
/*	    ts.tm_isdst = -1; */       
	    cur = mktime(&ts);
	}
    } else {
	printf("A single cron expression is required: one line from a user crontab delimited by double quotes.  Optionally, a second argument may be supplied specifying the epoch time from which to start.\nFor help see the -h option.\n");
	exit(0);
    }
    memset(&expr, 0, sizeof(expr));

    cron_parse_expr(cron_line_schedule, &expr, &err);

    time_t prev = cron_prev(&expr, cur);  /* if you want next epoch time instead of previous, simply change cron_prev to cron_next  */
    if (beginning_of_next_word_boundary != -1)
	printf("%lld %s\n", (long long) prev, cron_line_disposition);
    else
	printf("%lld\n", (long long) prev);   
    free(cron_line_disposition);
    free(cron_line_schedule);
}


