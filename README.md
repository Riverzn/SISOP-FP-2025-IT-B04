# SISOP-FP-2025-IT-B04

# Final Project Sistem Operasi IT

## Peraturan
1. Waktu pengerjaan dimulai hari Kamis (19 Juni 2025) setelah soal dibagikan hingga hari Rabu (25 Juni 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-FP-2025-IT-[Kelas][Kelompok]” (contoh:Sisop-FP-2025-IT-A01).
4. Setelah pengerjaan selesai, seluruh source code dan semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github dikumpulkan pada form yang disediakan. Pastikan github di setting ke publik.
5. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
6. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
7. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
8. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
9. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal final project, maka nilai dianggap 0.
10. Pengerjaan soal final project sesuai dengan modul yang telah diajarkan.

## Kelompok B04

Nama | NRP
--- | ---
Adiwidya Budi Pratama | 5027241012
Rayka Dharma Pranandita | 5027241039
Ni`mah Fauziyyah Atok | 5027241103
Muhammad Farrel Rafli Al Fasya | 5027241075

## Deskripsi Soal
Buat program yang mengukur waktu eksekusi sebuah child process. Parent process harus mencatat waktu sebelum melakukan fork(), lalu membuat child yang menjalankan tugas sederhana seperti sleep selama N detik, dan setelah child selesai (wait), parent kembali mencatat waktu akhir. Program harus menghitung dan menampilkan selisih waktu tersebut dalam satuan milidetik sebagai durasi eksekusi child process. Tes program ini dengan beberapa nilai sleep yang berbeda untuk memastikan hasilnya akurat.   

### Catatan
Struktur repository:
#### README.md  
Menjelaskan mengenai teori teori dari fungsi dan cara kerja program process timer, meliputi parent-child process, wait(), Time Measurement, sleep() serta argumen baris perintah 'argc' serta validasi input.    

#### ukurwaktu.c
Merupakan program yang mengukur waktu eksekusi sebuah child process, dimana di dalamnya digunakan fork() untuk membuat child process, melakukan sleep pada child process selama waktu yang diminta user (parent process melakukan wait hingga childnya selesai), dengan hasil akhir program berupa waktu yang dihabiskan oleh ekseskusi child process
```
.
├── README.md # Penjelasan mengenai teori dari program yang telah ditulis
└── ukurwaktu.c # Program process timer
```

## Pengerjaan

> Proses hubungan Parent-Child

**Teori** <br>
Sebuah proses adalah sebuah program yang sedang dieksekusi, lengkap dengan state dan sumber dayanya. Di lingkungan Unix-like, mekanisme utama untuk membuat proses baru adalah system call fork(). fork() menciptakan salinan hampir identik dari proses yang memanggilnya. Proses asli disebut parent process, dan duplikatnya disebut child process. Setelah panggilan fork(), kedua proses ini berjalan secara independen, tetapi melanjutkan eksekusi dari titik yang sama dalam kode. Nilai kembalian dari fork() digunakan untuk membedakan antara parent (mengembalikan PID child) dan child (mengembalikan 0).

**Solusi**
```
pid_t pid = fork();

if (pid < 0) {
    perror("fork() gagal");
    exit(EXIT_FAILURE);
}

else if (pid == 0) {
    // Kode ini dijalankan oleh proses CHILD
    printf("  CHILD: Proses child (PID: %d) mulai berjalan.\n", getpid());
    printf("  CHILD: Akan sleep selama %d detik.\n", sleep_duration);
    
    sleep(sleep_duration);
    
    printf("  CHILD: Selesai sleep, proses child akan keluar.\n");
    exit(EXIT_SUCCESS);
}
else {
    // Kode ini dijalankan oleh proses PARENT
    printf("PARENT: Berhasil membuat child process dengan PID: %d.\n", pid);
    printf("PARENT: Menunggu child process selesai...\n");

    wait(NULL);  // Menunggu child selesai
```
#### Penjelasan
- `pid_t pid = fork();` membuat proses baru. **Jika berhasil**, `fork()` menciptakan salinan proses yang identik.
- `pid == 0` menunjukkan bahwa **proses ini adalah child**.
- `pid > 0` → berarti **proses ini adalah parent**, dan `pid` mengandung **PID dari child.**
- `wait(NULL)`; → dipanggil oleh parent untuk **menunggu child selesai**, memperlihatkan **sinkronisasi** antar proses.

Seperti yang dijelaskan dalam teori : 
`fork()` adalah mekanisme utama untuk menciptakan hubungan parent-child. Setelah `fork()`, parent dan child memiliki jalur eksekusi independen yang dimisahkan menggunakan `if-else` berdasarkan nilai return `fork()`.

> Wait()

**Teori**     
Wait() adalah sebuah panggilan sistem (system call) yang digunakan oleh parent process untuk menyinkronkan eksekusinya dengan child process. Secara spesifik, wait() akan menjeda (memblokir) eksekusi parent hingga salah satu dari child process-nya selesai atau diterminasi. Fungsi utamanya adalah untuk memeriksa apakah sebuah tugas yang didelegasikan ke child telah selesai. Seperti yang dijelaskan oleh Bovet & Cesati (2005, h. 129), bahwa "A process may, for instance, create a child process to perform a specific task and then invoke some wait( )-like library function to check whether the child has terminated. If the child has terminated, its termination code will tell the parent process if the task has been carried out successfully."       

Pola penggunaan wait() ini merupakan inti dari cara kerja banyak program fundamental, salah satunya adalah command shell (terminal). Tanenbaum dan Bos (2015) memberikan contoh praktis mengenai siklus ini. Sebuah shell akan membaca perintah, membuat child process melalui fork(), kemudian "To wait for the child to finish, the parent executes a waitpid system call, which just waits until the child terminates (any child if more than one exists)." (Tanenbaum & Bos, 2015, h. 55). Dengan demikian, wait() tidak hanya menjadi alat sinkronisasi, tetapi juga komponen kunci dalam arsitektur eksekusi perintah di sistem operasi modern.     

Dalam konteks eksperimen sistem modern, Zhao, Gong, dan Fonseca (2021, h. 6) juga menyatakan bahwa "The fault handling measurement code makes the parent wait for the child exit before starting the next iteration because tearing down the child virtual memory has non-negligible costs that would add measurement noise". Ini menunjukkan bahwa penggunaan wait() tidak hanya penting untuk sinkronisasi logika program, tetapi juga untuk menjaga keakuratan dalam pengukuran performa proses di tingkat sistem operasi

**Solusi**
```
else {
        printf("PARENT: Berhasil membuat child process dengan PID: %d.\n", pid);
        printf("PARENT: Menunggu child process selesai...\n");

        // Tunggu hingga child process selesai
        wait(NULL);
```
Pada percabangan untuk parent process, diberlakukan wait hingga child process berhasil diterminasi.    

> Time Measurement

Pada Linux terdapat dua jenis utama perhitungan waktu di kernel Linux, yang pertama mencatat waktu dan tanggal saat ini yang digunakan untuk dikembalikan ke program pengguna
melalui API time(), ftime(), dan gettimeofday(), selanjutnya ada maintining timer, yang digunakan untuk memberi tahu kernel. Penjelasan mengenai gettimeofday() dijelaskan oleh Bovet & Cesati (2005, h. 252) "gettimeofday( ) Returns, in a data structure named timeval, the number of elapsed seconds since midnight of January 1, 1970 (UTC) and the number of elapsed microseconds in the last second (a second data structure named timezone is not currently used)".    

Penjelasan tentang bagaimana gettimeofday() digunakan dalam konteks benchmarking sistem juga diulas oleh Beckman et al. (2008). Mereka menyoroti bahwa "the commonly used POSIX gettimeofday() system call is not quite good enough:even if its precision matches its resolution (which is not guaranteed), it will still have a precision of only 1 µs. Besides, as we will show later, on some systems invoking ittakes several microseconds, simply because of the system call overhead"    

Ini menunjukkan bahwa meskipun gettimeofday() populer digunakan dalam pengukuran waktu, ada batasan akurasi dan performa ketika digunakan dalam sistem skala besar atau program yang sangat sensitif terhadap waktu.   

**Solusi**
```c

    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);
    printf("PARENT: Waktu mulai dicatat sebelum fork().\n");

    pid_t pid = fork();
    ...
    ...
    else {
        ...
        gettimeofday(&end_time, NULL);
    }
```
Digunakan gettimeofday untuk mencatat start time dari sebelum child process hingga child process berhasil di-terminasi
> Simulasi Kerja Proses dengan sleep()

**Teori**
Fungsi sleep() adalah panggilan sistem yang menangguhkan (blok) eksekusi proses selama sejumlah detik yang ditentukan, melepaskan CPU untuk dijalankan oleh proses lain sehingga tidak terjadi busy-waiting pada CPU (Stevens & Rago, 2013, hlm. 127)¹. Selama periode tidur, proses tidak mengonsumsi waktu CPU, melainkan hanya menunggu secara pasif hingga waktu yang ditetapkan habis (Silberschatz, Galvin, & Gagne, 2018, hlm. 67)². Teknik ini sering dipakai untuk mensimulasikan beban kerja ringan atau pengaturan waktu antar proses dalam pengujian, tanpa membebani CPU dengan perhitungan berlebih.

Panggilan sistem sleep() membuat proses masuk ke state blocked hingga interval waktu yang ditentukan habis, kemudian di‐enqueue kembali oleh scheduler untuk eksekusi—sebuah mekanisme yang lebih efisien dibanding busy‐waiting (Liu & Layland, 1973)¹ DOI: 10.1145/321738.321744. Implementasi low‐level sleep() bergantung pada infrastruktur timer kernel, seperti struktur timing wheel yang dioptimalkan untuk manajemen ribuan timer secara efisien (Varghese & Lauck, 1997)² DOI: 10.1145/263867.263885.

**Solusi**
```
pid_t pid = fork();

if (pid < 0) {
    perror("fork() gagal");
    exit(EXIT_FAILURE);
}
else if (pid == 0) {
    // Kode ini dijalankan oleh proses CHILD
    printf("  CHILD: Proses child (PID: %d) mulai bekerja.\n", getpid());
    printf("  CHILD: Mensimulasikan kerja—akan sleep %d detik.\n", sleep_duration);

    sleep(sleep_duration);

    printf("  CHILD: Selesai sleep, proses child akan keluar.\n");
    exit(EXIT_SUCCESS);
}
else {
    // Kode ini dijalankan oleh proses PARENT
    printf("PARENT: Berhasil membuat child process (PID: %d).\n", pid);
    printf("PARENT: Menunggu child process selesai...\n");

    wait(NULL);  // Menunggu child selesai
    printf("PARENT: Child process telah selesai.\n");
}
```
#### Penjelasan
fork() membagi program menjadi dua proses:
Parent (pid > 0)
Child (pid == 0)
Di jalur child, sleep(sleep_duration) menunda exit selama N detik untuk mensimulasikan “kerja”.
Di jalur parent, wait(NULL) memblok eksekusi hingga child selesai, memastikan sinkronisasi.
Dengan teknik ini, kita dapat menguji struktur parent–child dan mekanisme penjadwalan tanpa beban CPU yang berat.

> Argumen Baris Perintah `argc` dan Validasi Input

**Teori**     
Dalam pemrograman C, interaksi antara pengguna dan program sering dilakukan melalui argumen baris perintah. Fungsi `main()` secara konvensional menerima dua parameter: int argc dan char *argv[], yang masing-masing mewakili jumlah argumen dan array dari string argumen yang diketik pengguna saat menjalankan program.

Validasi terhadap argumen sangat penting untuk menjamin keandalan program, mencegah error akibat input yang salah, dan memastikan bahwa input yang diterima sesuai dengan yang diharapkan.
"A robust program must verify that its inputs are within expected parameters and handle exceptions or errors appropriately."
(Silberschatz et al., 2018, p. 98)
"Programs that take arguments from the command line should always check the number and type of arguments and provide helpful usage messages if they are incorrect."
(Stevens & Rago, 2013, p. 29)
"Many command-line utilities in UNIX use argc and argv to parse command-line options. It is crucial to validate these inputs to prevent unintended behavior."
(Tanenbaum, 2015, p. 264)   

Tanpa validasi, jika pengguna memasukkan nilai seperti "abc" atau tidak memasukkan argumen sama sekali, program bisa mengalami crash atau melakukan perilaku tak terduga. Menurut penelitian oleh Ebad (2023) “Improper validation of command-line inputs can lead to buffer overflows and incorrect logic execution, especially in systems programmed in low-level languages like C.”
(Ebad, 2023, https://doi.org/10.14569/IJACSA.2023.0140117)   
Jena (2021) juga menekankan bahwa “Command-line arguments must be parsed and validated carefully to ensure they conform to expected types and ranges, particularly when numeric values are expected.”
(Jena, 2021, https://doi.org/10.1201/9781003188254-16)

**Solusi**
```
if (argc != 2) {
    fprintf(stderr, "Penggunaan: %s <durasi_sleep_detik>\n", argv[0]);
    exit(EXIT_FAILURE);
}

int sleep_duration = atoi(argv[1]);
if (sleep_duration <= 0) {
    fprintf(stderr, "Durasi sleep harus angka positif.\n");
    exit(EXIT_FAILURE);
}
```
Penjelasan:
- `argc != 2` → memastikan pengguna memberikan satu argumen tambahan selain nama program.
- `atoi(argv[1])` → mengubah argumen string menjadi integer.
- Validasi `sleep_duration <= 0` → mencegah nilai tidak valid seperti nol, negatif, atau input bukan angka.


Tanpa validasi ini, program bisa mengalami kesalahan logika atau keluar dengan error runtime, misalnya jika input "abc" diberikan, `atoi` akan menghasilkan 0.

**Video Menjalankan Program**
https://github.com/user-attachments/assets/28a07e59-6dfd-4920-9d98-320844dc7448



## Daftar Pustaka     
Beckman, P., Iskra, K., Yoshii, K., Coghlan, S., Nataraj, A., 2008. Benchmarking the effects of operating system interference on extreme-scale parallel machines. Cluster Comput 11, 3–16. https://doi.org/10.1007/s10586-007-0047-2     
Bovet, D. P., & Cesati, M. (2006). Understanding the Linux kernel (3rd ed). O’Reilly.    
Ebad, S. A. (2023). Investigating the Input Validation Vulnerabilities in C Programs. International Journal of Advanced Computer Science and Applications, 14(1), 153–160. https://doi.org/10.14569/IJACSA.2023.0140117   
Jena, S. (2021). Command Line Arguments. in C Programming (page. 403–414). CRC Press. https://doi.org/10.1201/9781003188254-16      
Liu, C. L., & Layland, J. W. (1973). Scheduling Algorithms for Multiprogramming in a Hard‐Real‐Time Environment. Journal of the ACM, 20(1), 46–61.    
Stevens, W. R., & Rago, S. A. (2013). Advanced Programming in the UNIX Environment (3rd ed.). Addison-Wesley Professional.    
Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley.    
Tanenbaum, A. S. (2015). Modern operating systems (Fourth edition). Pearson.   
Varghese, G., & Lauck, R. (1997). Hashed and hierarchical timing wheels: data structures for the efficient implementation of a timer facility. ACM Transactions on Computer Systems, 15(1), 36–70.   
Zhao, K., Gong, S. and Fonseca, P., 2021. On-demand-fork: A microsecond fork for memory-intensive and latency-sensitive applications. In: Proceedings of the Sixteenth European Conference on Computer Systems (EuroSys ’21). Online: ACM. pp.1–16. https://doi.org/10.1145/3447786.3456258
