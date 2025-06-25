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

## Kelompok x

Nama | NRP
--- | ---
Adiwidya Budi Pratama | 5027241012
Rayka Dharma Pranandita | 5027241039
Ni`mah Fauziyyah Atok | 5027241103
Muhammad Farrel Rafli Al Fasya | 5027241075

## Deskripsi Soal
Buat program yang mengukur waktu eksekusi sebuah child process. Parent process harus mencatat waktu sebelum melakukan fork(), lalu membuat child yang menjalankan tugas sederhana seperti sleep selama N detik, dan setelah child selesai (wait), parent kembali mencatat waktu akhir. Program harus menghitung dan menampilkan selisih waktu tersebut dalam satuan milidetik sebagai durasi eksekusi child process. Tes program ini dengan beberapa nilai sleep yang berbeda untuk memastikan hasilnya akurat.
### Catatan

> Insert catatan dari pengerjaan kalian... (contoh dibawah) // hapus line ini

Struktur repository:
```
.
..
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

**Solusi**
```
else {
        printf("PARENT: Berhasil membuat child process dengan PID: %d.\n", pid);
        printf("PARENT: Menunggu child process selesai...\n");

        // Tunggu hingga child process selesai
        wait(NULL);
```
Pada percabangan untuk parent process, diberlakukan wait hingga child process berhasil diterminasi.

> Simulasi Kerja Proses dengan sleep()

**Teori**
Fungsi sleep() adalah panggilan sistem yang menangguhkan (blok) eksekusi proses selama sejumlah detik yang ditentukan, melepaskan CPU untuk dijalankan oleh proses lain sehingga tidak terjadi busy-waiting pada CPU (Stevens & Rago, 2013, hlm. 127)¹. Selama periode tidur, proses tidak mengonsumsi waktu CPU, melainkan hanya menunggu secara pasif hingga waktu yang ditetapkan habis (Silberschatz, Galvin, & Gagne, 2018, hlm. 67)². Teknik ini sering dipakai untuk mensimulasikan beban kerja ringan atau pengaturan waktu antar proses dalam pengujian, tanpa membebani CPU dengan perhitungan berlebih.

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



**Video Menjalankan Program**
...

## Daftar Pustaka
- Bovet, D. P., & Cesati, M. (2006). Understanding the Linux kernel (3rd ed). O’Reilly.
- Tanenbaum, A. S. (2015). Modern operating systems (Fourth edition). Pearson.
- Stevens, W. R., & Rago, S. A. (2013). Advanced Programming in the UNIX Environment (3rd ed.). Addison-Wesley Professional.
- Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley.
