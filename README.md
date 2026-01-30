# RP2040_MUSIC_PLAYER
A music player made using Raspberry Pi Pico W
It supports Wave files only as of now.

# Features
    Supports playback of uncompressed **PCM WAV audio files** from SD card
    16-bit, 44.1 kHz, mono audio output via I2S interface
    Audio output through external I2S amplifier (MAX98357A)
    Hardware volume control using rotary encoder
    Dedicated Play / Pause, Next, and Previous push buttons
    Automatic track switching when a song ends 
    
# User Interface
    Display shows following information
       Current song name 
       Playback status (PLAY / PAUSE)
       Playback progress bar
       Volume level

# Rotary Encoder
    The rotary encoder is used for real-time volume control.
       Clockwise - Increases Volume
       Anticlockwise - Decrease Volume
       Push Button is used to select song 
               
# Libraries Required
               #include <SPI.h>
               #include <SD.h>
               #include <I2S.h>
               #include <Adafruit_GFX.h>
               #include <Adafruit_ST7789.h>
               
# Wave File Format
               Bit Rate               705kbps
               Channels               Mono
               Sample_rate            44.100kHz
               Sample_size            16bit



