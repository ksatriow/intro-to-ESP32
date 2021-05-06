#include "deklarasi.h"

void tampil(int a) {
  // berikut ini program supaya jam tampil lebih bagus jika nilainya kurang dari 10
  // seharusnya tampil 1:12:23 >> 01:12:23
  timeClient.update();
  switch (a) {
    case 0:
      sprintf (newMessage, "Made In Indramayu, anakkendali.com");
      break;
    case 1:
      if (jam  < 10 && menit < 10) {
        sprintf (newMessage, "0%d:0%d", jam, menit);
      }
      if (jam  < 10 && menit >= 10) {
        sprintf (newMessage, "0%d:%d", jam, menit);
      }
      if (jam  >= 10 && menit >= 10) {
        sprintf (newMessage, "%d:%d", jam, menit);
      }
      if (jam  >= 10 && menit < 10) {
        sprintf (newMessage, "%d:0%d", jam, menit);
      }
      break;
    case 2:
      sprintf (newMessage, "%s  %d-%d-%d", hari, tanggal, bulan, tahun);
      break;
    case 3:
    sprintf (newMessage, "Alarm Next Tutorial");
      break;
    case 4:
      sprintf (newMessage, "For The Fairest");
      break;
    case 5:
    sprintf (newMessage, "Devi");
      break;
  }
}
