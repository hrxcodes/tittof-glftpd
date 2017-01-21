/*   __  ____  __       ___
    / /_/ / /_/ /____  / _/
   / __/ / __/ __/ _ \/ _/
   \__/ /\__/\__/\___/_/
     /_/
    /
   / Only allow sane chars in user tagline.
  / cscript SITE[:space:]TAGLINE pre /bin/pre_tagline
*/
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int i = 0;
    regex_t tagline;
    int result;
    if ( argc != 4 || strcmp(argv[2], getenv("USER")) != 0) return EXIT_FAILURE;
    result = regcomp(&tagline, "^[][A-Za-z0-9^_ -]*$", REG_EXTENDED);
    result = regexec(&tagline, argv[1], 0, NULL, 0);
    if ( result == REG_NOMATCH || strlen(argv[1]) > 50 ) { result = REG_NOMATCH; }
    regfree(&tagline);
    if ( !result ) return EXIT_SUCCESS;
    else if ( result == REG_NOMATCH ) {
        openlog("BAD_TAGLINE", LOG_NDELAY, LOG_DAEMON);
        syslog(LOG_ERR, "{%s}", getenv("USER"));
        closelog();
        return EXIT_FAILURE;
    } else return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
