# cron_last_epoch_time_this_line_was_supposedly_executed

Outputs to stdio the epoch time at which a given user crontab expression was supposedly last executed.

## Description:
 This program outputs to stdio the epoch time at which a given user
 crontab expression was supposedly last executed, in addition to
 the disposition element of the cron expression, if present.  The
 expression should be comprised of one line from a user crontab
 delimited by double quotes, passed as an argument.  Optionally, a
 second argument may be supplied specifying either the epoch time,
 or the iso8601 time, from which to start.  If this starting time
 is in iso8601 then the output will also be in iso8601 format; if
 the starting time is a malformed iso8601 datetime, such as
 9999-99-99T99:99, then current time is assumed, but the prev time
 outputted to stdio will be in iso8601 format.  All times are local, not GMT.
 For next cron scheduled datetime see cron_next_epoch_time_this_line_should_execute.c

## Example:
     ./cron_last_epoch_time_this_line_was_supposedly_executed "0 22 * * mon,tue,wed,thu,fri disable_wifi.sh" 1569016800
     This outputs: 1568948400 disable_wifi.sh

     ./cron_last_epoch_time_this_line_was_supposedly_executed "0 22 * * mon,tue,wed,thu,fri disable_wifi.sh" 2019-02-08T12:11
     This outputs: 2019-02-07T22:00:00Z disable_wifi.sh

## Dependencies: 
      ccronexpr.c borrowed from https://github.com/staticlibs/ccronexpr

## To compile under linux:  
      gcc -DCRON_USE_LOCAL_TIME -o cron_last_epoch_time_this_line_was_supposedly_executed cron_last_epoch_time_this_line_was_supposedly_executed.c ccronexpr.c