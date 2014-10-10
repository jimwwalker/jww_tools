#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv, char** envp) {

    int sleep_time_secs = 20;
    char* prog = getenv("JWW_SLEEP_EXEC_TARGET");
    char* sleep_time = getenv("JWW_SLEEP_EXEC_SEC");

    if (prog == NULL) {
        return 1;
    }

    if (sleep_time != NULL) {
        sleep_time_secs = atoi(sleep_time);
    }

    sleep(sleep_time_secs);
    execve(prog, argv, envp);

    return 0;
}