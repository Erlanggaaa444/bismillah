# bismillah

projek abdimas

# desc

Sebuah program yang digunakan untuk mengatur penyiraman otomatis menggunakaan dua mode yaitu sistem penyiraman menggunakan sensor dan sistem penyiraman menggunakan timer. Sistem penyiraman menggunakan sensor merupakan sistem cycle turn on/off solenoid melalui relay dengan batasan nilai threshold dari pembacaan nilai sensor. Sistem penyiraman menggunakan timer merupaan sistem cycle turn on/off solenoid melalui relay dengan batasan nilai waktu yang ditentukan oleh pengguna melalui keypad. Pada sistem ini ditampilkan menu berupa mode yang dijalankan dan nilai kelembapan tanah.

# komponen yang digunakan

Software

- Arduino IDE (upload & verify)
- Visual Studio Code (code editor)

Hardware

- Arduino Mega 2560
- LCD I2C 20x4
- Keypad Matrix 4x4
- Relay 8 channel
- Sensor Kelembapan Tanah

# on going task

- membuat kode non blocking dan interrupt (placeholder keypad)
- membuat sistem timer oncall func (placeholder relay)

# finished task

- membuat tampilan menu pada lcd

# future task

- integrasi keypad & lcd
- membuat kode keluaran output relay
