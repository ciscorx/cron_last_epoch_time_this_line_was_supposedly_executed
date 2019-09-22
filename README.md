# cron_last_epoch_time_this_line_was_supposedly_executed
Outputs to stdio the epoch time at which a given user crontab expression was supposedly last executed.

## Description:
This program outputs to stdio the epoch time at which a given user
  crontab expression was supposedly last executed, in addition to
  the disposition element of the cron expression, if present.  The
  expression should be comprised of one line from a user crontab
  delimited by double quotes, passed as an argument.  Optionally, a
  second argument may be supplied specifying the epoch time from
  which to start.

## Example:
     ./cron_last_epoch_time_this_line_was_supposedly_executed "0 22 * * mon,tue,wed,thu,fri disable_wifi.sh" 1569016800<br/>
     This outputs: 1568930400 disable_wifi.sh

## Dependencies: 
      ccronexpr.c borrowed from https://github.com/staticlibs/ccronexpr

## To compile under linux:  
      gcc -o cron_last_epoch_time_this_line_was_supposedly_executed cron_last_epoch_time_this_line_was_supposedly_executed.c ccronexpr.c