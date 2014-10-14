#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARGV_EXTRA (7)

int main(int argc, char** argv, char** envp) {

    int ii  = 0, jj = ARGV_EXTRA, newargv_c = argc + ARGV_EXTRA + 1;
    char** newargv = calloc(newargv_c, sizeof(char*)); /* NULL at end hence + 1*/
    char* sleep_time = getenv("JWW_SLEEP_EXEC_SEC");
    char* optional_output_filename = getenv("JWW_STRACE_EXEC_OUT");
    char* strace_target = getenv("JWW_STRACE_TARGET");
    char* suffix = ".real";

    newargv[0] = "/usr/bin/strace";
    newargv[1] = "-t";
    newargv[2] = "-f";
    newargv[3] = "-etrace=!futex,clock_gettime,gettimeofday";
    newargv[4] = "-o";
    newargv[5] = "/tmp/strace_exec.out";

    if (strace_target == NULL) {
        /* in this case we will try and strace this program, but with a different suffix */
        int len = strlen(argv[0]) + strlen(suffix) + 1;
        strace_target = malloc(len);
        strncpy(strace_target, argv[0], len);
        strncat(strace_target, suffix, len - strlen(argv[0]) - 1);
    }
    
    newargv[6] = strace_target;

    if (optional_output_filename != NULL) {
        newargv[5] = optional_output_filename;
    }

    if (sleep_time != NULL) {
        sleep(atoi(sleep_time));
    }

    for(ii = 1; ii < argc; ii++, jj++) {
        newargv[jj] = argv[ii];
    }

    newargv[newargv_c - 1] = NULL;

    execve("/usr/bin/strace", newargv, envp);

    return 0;
}