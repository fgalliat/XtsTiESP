# XtsTiESP

Xtase - fgalliat @Aug 2020



- try tu reuse a Ti92(model  1-no asm) as Keyboard for an ESP32 + ILI9341 + SDCard

- it uses my other project : https://github.com/fgalliat/XtsTiLink_gh.git (for the calc & link part)

- **libraries :** 

  - SdFat
  - TFT_eSPI

- **hardware :**
  - ESP32 mini
  - Adafruit ProtoShield (for UNO)
  - ILI9341
  - SDCard reader
  - Buzzer / LED
  - -- Ti92 / Arduino ProMini 5v (328P) --

- **devboard :**
  - **TODO:**
    - [x] Board wiring diagram
    - [ ] Soldering
      - [x] SCL & SDA
      - [x] maybe Aref to Vcc (SCL,SDA,Vcc,GND ~kinda GROVE)
      - [x] 8pins headers for Screen on lower-board
      - [x] SDCard Reader on upper board / cut pins @ bottom
      - [x] SDCard to SPI SD (**code wrote / works!**)
      - [x] Screen SPI missing solders (MISO,MOSI, SCK)
      - [x] some solders between lower & upper boards
      - [x] SPI Screen just next SPI 8pins headers (**test it**)
      - [ ] Additional Button (PULL_UP)
    - [x] test SDCard custom SPI config
    - [x] test ILI custom SPI config
    - [x] test all UARTs (0, 1, x) - fixed UART1
    - [x] protect SerialX from 5v logic (switchable)
    - [x] BUZZER / LED

  - **Diagram :**

    - DevBoard **!! NOT UNO Compatible !! (even if same form factor)**

    ![DevBoard !! NOT UNO Compatible !!](./pictures/board.png)

    - ESP32 mini (HW665) (Arduino IDE : **WEMOS D1 MINI ESP32**)

      ![ESP32 mini wiring](./pictures/esp32_wiring.png)

      !! BEWARE **RX1** is on **GPIO16** now

- **ideas :**

  - webServer w/ DnD + UPLOAD
    - secure POST request w/ a token (~AUTH form) (done)
    - emit onERROR u_send() message & lock process (done)
    - add SoftAP / STA mode switcher (w/ SoftAP fallback)
    - add SoftAP / STA  - SSID:psk file support + API (? /etc/wifi.psk ?)
  - BT Server / HID Server
  - connected to XtsTiLink/Ti92 system (of my own)