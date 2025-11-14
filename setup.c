#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread1_createFolder(void *arg) {
    printf("[Thread 1] Creating NewGit2.0 setup...\n");

    system("mkdir -p ~/NewGit2.0");

    system("touch ~/NewGit2.0/InitInfo.txt");
    system("touch ~/NewGit2.0/configUser.txt");

    printf("[Thread 1] Folder & files created.\n");
    return NULL;
}

void *thread2_compile(void *arg) {
    printf("[Thread 2] Compiling newgit...\n");

    const char *cmd =
        "gcc -o newgit "
        "main.c "
        "Config/MakeConfig.c "
        "Errors/errors.c "
        "init/initialization.c "
        "Staging/staging.c "
        "Commit/commit.c "
        "Revert/revert.c "
        "CommitInfo/info.c "
        "CheckingFirst/CheckingFirst.c";

    int result = system(cmd);

    if (result == 0)
        printf("[Thread 2] Compilation successful.\n");
    else
        printf("[Thread 2] Compilation failed!\n");

    return NULL;
}

void *thread3_copyBin(void *arg) {
    printf("[Thread 3] Copying newgit to /usr/local/bin/... (sudo required)\n");
    printf("Please enter your password if asked.\n");

    system("sudo cp newgit /usr/local/bin/");

    printf("[Thread 3] Installation complete.\n");
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    printf("=== Starting NewGit Installer ===\n");

    // Thread 1: Make folder and files
    pthread_create(&t1, NULL, thread1_createFolder, NULL);
    pthread_join(t1, NULL);      // Wait until folder created

    printf("Waiting 1 second...\n");
    sleep(1);

    // Thread 2: Compile program
    pthread_create(&t2, NULL, thread2_compile, NULL);
    pthread_join(t2, NULL);      // Wait until compilation done

    printf("Waiting 1 second...\n");
    sleep(1);

    // Thread 3: Copy binary (sudo)
    pthread_create(&t3, NULL, thread3_copyBin, NULL);
    pthread_join(t3, NULL);

    printf("=== NewGit Installation Completed ===\n");

    return 0;
}
