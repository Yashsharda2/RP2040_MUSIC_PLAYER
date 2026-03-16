## Hardware Connections

### MAX98357A — I2S Amplifier
| Pin | Pico |
|---|---|
| BCLK | GP18 |
| LRCK | GP19 |
| DIN | GP20 |
| VCC | 3.3V |
| SD | 3.3V |
| GAIN | GND |

> Speaker connects to SPK+ and SPK− on the MAX98357A

---

### ST7789 TFT Display
| Pin | Pico |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SCK | GP10 |
| MOSI | GP11 |
| MISO | GP12 |
| CS | GP13 |
| DC | GP8 |
| RST | GP9 |
| LED | 3.3V |

---

### SD Card Module
| Pin | Pico |
|---|---|
| VCC | VBUS |
| GND | GND |
| SCK | GP10 |
| MOSI | GP11 |
| MISO | GP12 |
| CS | GP14 |

> SD card and display share SPI1 (GP10–GP12), separated by CS pins

---

### Rotary Encoder
| Pin | Pico |
|---|---|
| + | 3.3V |
| GND | GND |
| CLK | GP26 |
| DT | GP27 |
| SW | GP28 |

---

### Push Buttons
| Button | Pico |
|---|---|
| Play / Pause | GP2 |
| Next | GP3 |
| Back | GP4 |

---

## MP3 Support — Changed Connections Only

MP3 decoding requires more SPI bandwidth, so move the SD card to **SPI0**.

**Display stays on SPI1 .**

SD card moves to SPI0:

| SD Pin | Pico (WAV) | Pico (MP3) |
|---|---|---|
| SCK | GP10 | GP2 |
| MOSI | GP11 | GP3 |
| MISO | GP12 | GP4 |
| CS | GP14 | GP5 |

> Move the Push buttons also when using MP3 — reassign BTN_PLAY, BTN_NEXT, BTN_BACK to GP6, GP7, GP15 or any free GPIO.
