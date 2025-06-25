#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Penggunaan: %s <durasi_sleep_detik>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (argv[1][i] < '0' || argv[1][i] > '9') {
            fprintf(stderr, "Input harus berupa angka bulat positif saja.\n");
            exit(EXIT_FAILURE);
        }
    }

    int sleep_duration = atoi(argv[1]);
    if (sleep_duration <= 0) {
        fprintf(stderr, "Durasi sleep harus angka positif.\n");
        exit(EXIT_FAILURE);
    }

    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);
    printf("PARENT: Waktu mulai dicatat sebelum fork().\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork() gagal");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) {
        printf("  CHILD: Proses child (PID: %d) mulai berjalan.\n", getpid());
        printf("  CHILD: Akan sleep selama %d detik.\n", sleep_duration);
        
        sleep(sleep_duration);
        
        printf("  CHILD: Selesai sleep, proses child akan keluar.\n");
        exit(EXIT_SUCCESS);
    }
    else {
        printf("PARENT: Berhasil membuat child process dengan PID: %d.\n", pid);
        printf("PARENT: Menunggu child process selesai...\n");

        wait(NULL);

        gettimeofday(&end_time, NULL);
        printf("PARENT: Child process telah selesai. Waktu akhir dicatat.\n\n");

        long seconds_diff = end_time.tv_sec - start_time.tv_sec;
        long micros_diff = end_time.tv_usec - start_time.tv_usec;
        
        // Konversi total selisih ke milidetik
        double elapsed_ms = (seconds_diff * 1000.0) + (micros_diff / 1000.0);

        printf("====================================================\n");
        printf("Hasil Pengukuran Waktu Eksekusi Child Process\n");
        printf("----------------------------------------------------\n");
        printf("Durasi eksekusi terukur: %.3f milidetik\n", elapsed_ms);
        printf("====================================================\n");
    }

    return 0;
}
