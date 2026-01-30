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

# Future Updates
      1.MP3 audio format support will be added to reduce storage usage and allow larger music libraries. 
      2.The user interface will be improved with better layout, smoother rendering, and a clearer song selection screen using the rotary encoder. 
      3.Separate SPI buses will be used for the SD card and TFT display to avoid bus contention and improve audio stability. 
      4.Button handling will be made non-blocking for faster and more responsive control. 
      5.The file system will be enhanced to support automatic scanning, sorting, and handling of a large number of audio files.

