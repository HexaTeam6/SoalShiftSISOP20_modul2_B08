# SoalShiftSISOP20_modul2_B08

Soal Shift Modul 2 yang berfokus pada penggunaan exec, fork, wait, dan daemon.

## SOAL 1

Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut:
  1.  Program menerima 4 argumen berupa:
      1.1.    Detik: 0-59 atau * (any value)
      1.2.   Menit: 0-59 atau * (any value)
      1.3.  Jam: 0-23 atau * (any value)
      1.4.   Path file .sh
  2.  Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
      sesuai.
  3.  Program hanya menerima 1 config cron.
  4.  Program berjalan di background (daemon).
  5.  Tidak boleh menggunakan fungsi system().

Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap
detik pada jam 07:34.

### Penyelesaian

## SOAL 2

Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.
  1.  Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat
      sebuah program C yang per 30 detik membuat sebuah folder dengan nama
      timestamp [YYYY-mm-dd_HH:ii:ss].
  2.  Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari
      https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap
      gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah
      detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYYmm-
      dd_HH:ii:ss].
  3.  Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan
      folder akan di delete(sehingga hanya menyisakan .zip).
  4.  Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut mengenerate
      sebuah program "killer" yang siap di run(executable) untuk
      menterminasi semua operasi program tersebut. Setelah di run, program yang
      menterminasi ini lalu akan mendelete dirinya sendiri.
  5.  Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu
      MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan
      dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen
      -b. Ketika dijalankan dalam MODE_A, program utama akan langsung
      menghentikan semua operasinya ketika program killer dijalankan. Untuk
      MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi
      membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua
      folder terisi gambar, terzip lalu di delete).
      
Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi.
Buatlah program dalam mimpi Kiwa jadi nyata!

Catatan:
- Tidak boleh memakai system().
- Program utama harus ter-detach dari terminal.
Hint:
- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran
tertentu.
- Epoch Unix bisa didapatkan dari time().

### Penyelesaian

## SOAL 3

Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia
memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan
(multiprocessing).
  1.  Program buatan jaya harus bisa membuat dua direktori di
      “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
      lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
  2.  Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori
      “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak
      hanya itu tugasnya.
  3.  Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di
      dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan
      pengelompokan, semua file harus dipindahkan ke
      “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke
      “/home/[USER]/modul2/indomie/”.
  4.  Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/”
      harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu
      3 detik kemudian membuat file bernama “coba2.txt”.
      (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”).
      
Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut.

Catatan :
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork.
- Direktori “.” dan “..” tidak termasuk.

### Penyelesaian
