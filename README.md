# RP2040 Music Player

A portable music player powered by the **Raspberry Pi Pico W**.  
Plays audio files stored on an SD card through an I2S amplifier, controlled via rotary encoder and push buttons, with a TFT display showing playback info in real time.

Built for simplicity — no OS, no overhead, just music.

---

## Features

#### Playback
- WAV (16-bit PCM uncompressed) and MP3 audio support
- 44.1 kHz mono output via I2S (MAX98357A amplifier)
- Auto advances to next track when song ends

#### Controls
- Rotary encoder for volume adjustment and track browsing
- Dedicated Push buttons — Play/Pause, Next, Previous
- Press encoder to open/close track selection menu

#### Display
- Current song name
- Playback status — PLAYING / PAUSED
- Real-time progress bar
- Volume level bar

---
