#ifndef BECOME_DAEMON_H
#define BECOME_DAEMON_H

#define DONT_CHDIR           01    /* Don't chdir("/") */
#define DONT_CLOSE_FILES     02    /* Don't close all open files */
#define DONT_REOPEN_STD_FDS  04    /* Don't reopen stdin, stdout, and stderr to /dev/null */
#define DONT_UMASK0          010   /* Don't do a umask(0) */

#define MAX_CLOSE            8192  /* Maximum file descriptors to close */

int becomeDaemon(int flags);

#endif
