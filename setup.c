#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "Errors/errors.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define END "\033[0m"

// Spinner animation
void spinner(int sec) {
    char spin[] = {'|', '/', '-', '\\'};
    int i = 0;
    int t = sec * 10; // 10 updates per second

    while (t--) {
        printf("\r   %c Processing...", spin[i]);
        fflush(stdout);
        i = (i + 1) % 4;
        usleep(100000); // 0.1 sec
    }
    printf("\r   ✔ Done!                \n");
}

// Progress bar animation
void progressBar(int sec) {
    int total = 20;
    for (int i = 0; i <= total; i++) {
        printf("\r   [");
        for (int j = 0; j < i; j++) printf("#");
        for (int j = i; j < total; j++) printf("-");
        printf("] %d%%", (i * 100) / total);
        fflush(stdout);
        usleep(sec * 50000);
    }
    printf("\n");
}

// ---------------- THREAD FUNCTIONS ----------------

void *thread1_createFolder(void *arg) {
    printf("\n[Thread 1] 🚀 Starting NewGit2.0 setup...\n");
    spinner(2);

    system("mkdir -p ~/NewGit2.0");
    system("touch ~/NewGit2.0/InitInfo.txt");
    system("touch ~/NewGit2.0/configUser.txt");

    progressBar(2);

    printf("[Thread 1] 📁 Folder & files created successfully.\n");
    return NULL;
}

void *thread2_compile(void *arg) {
    printf("\n[Thread 2] 🔧 Compiling newgit...\n");
    spinner(3);

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

    progressBar(2);

    if (result == 0)
        printf("[Thread 2] ✅ Compilation successful.\n");
    else
        printf("[Thread 2] ❌ Compilation failed!\n");

    return NULL;
}

void *thread3_copyBin(void *arg) {
    printf("\n[Thread 3] 📦 Installing newgit to /usr/local/bin...\n");
    printf("🔑 Please enter your password if asked.\n");

    spinner(2);

    system("sudo cp newgit /usr/local/bin/");

    progressBar(2);

    printf("[Thread 3] 🟢 Installation complete.\n");
    return NULL;
}



// ---------------- MAIN ----------------

int main() {
    pthread_t t1, t2, t3;

    printf("=====================================\n");
    printf("       🛠️  NewGit Installer 2.0       \n");
    printf("=====================================\n");

    pthread_create(&t1, NULL, thread1_createFolder, NULL);
    pthread_join(t1, NULL);

    printf("\n⏳ Waiting 1 second...\n");
    sleep(1);

    pthread_create(&t2, NULL, thread2_compile, NULL);
    pthread_join(t2, NULL);

    printf("\n⏳ Waiting 1 second...\n");
    sleep(1);

    pthread_create(&t3, NULL, thread3_copyBin, NULL);
    pthread_join(t3, NULL);


    printf("\n=====================================\n");
    printf("   🎉 NewGit Installation Completed! 🎉\n");
    printf("=====================================\n");

    return 0;
}
