#include <SPI.h>
#include <SD.h>
#include <I2S.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

//  Configuration & Pins 
namespace Config {
    const int TFT_DC = 8, TFT_RST = 9, TFT_CS = 13;
    const int SD_CS = 14;
    const int BTN_PLAY = 2, BTN_NEXT = 3, BTN_BACK = 4;
    const int ENC_CLK = 26, ENC_DT = 27, ENC_SW = 28;
    const int SAMPLE_RATE = 44100;
    const int MAX_FILES = 200;
}


Adafruit_ST7789 tft(&SPI1, Config::TFT_CS, Config::TFT_DC, Config::TFT_RST);
I2S i2s;
File audioFile;

String fileList[Config::MAX_FILES];
int fileCount = 0, currentTrack = 0, selectedTrack = 0;
int volume = 1500; // Start at 50%
uint32_t fileSize = 0;
bool isPlaying = false;
bool isMenuOpen = false;


void updateProgressBar() {
    if (!audioFile || isMenuOpen) return;

    static uint32_t lastPos = 0;
    uint32_t currentPos = audioFile.position();
    
    if (abs((int)(currentPos - lastPos)) < 10000) return; 
    lastPos = currentPos;

    int progressWidth = map(currentPos, 44, fileSize, 0, 200);
    progressWidth = constrain(progressWidth, 0, 200);

    tft.drawRect(10, 190, 202, 10, ST77XX_WHITE);
    tft.fillRect(11, 191, progressWidth, 8, ST77XX_CYAN);
    tft.fillRect(11 + progressWidth, 191, 200 - progressWidth, 8, ST77XX_BLACK);
}

void renderMainUI() {
    tft.fillScreen(ST77XX_BLACK);
    tft.fillRect(0, 0, 240, 32, ST77XX_BLUE);
    tft.setCursor(10, 8);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.print("Now Playing");

    tft.setCursor(10, 55);
    tft.setTextColor(ST77XX_CYAN);
    String title = fileList[currentTrack];
    if (title.startsWith("/")) title = title.substring(1); 
    if (title.length() > 18) title = title.substring(0, 15) + "...";
    tft.println(title);

    tft.setCursor(10, 90);
    tft.setTextColor(isPlaying ? ST77XX_GREEN : ST77XX_RED);
    tft.print(isPlaying ? ">> PLAYING" : "|| PAUSED");

    tft.setCursor(10, 130);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);
    tft.print("VOLUME");


    int volBar = map(volume, 0, 3000, 0, 200);
    tft.drawRect(10, 145, 202, 12, ST77XX_WHITE);
    tft.fillRect(11, 146, volBar, 10, ST77XX_MAGENTA);

    tft.setCursor(10, 175);
    tft.print("PROGRESS");
    
    updateProgressBar();
}

void renderList() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("SD Library");

    for (int i = -2; i <= 2; i++) {
        int idx = selectedTrack + i;
        if (idx < 0 || idx >= fileCount) continue;

        int yPos = 60 + (i + 2) * 35;
        if (i == 0) {
            tft.fillRect(0, yPos - 8, 240, 30, 0x3186); 
            tft.setTextColor(ST77XX_GREEN);
        } else {
            tft.setTextColor(ST77XX_WHITE);
        }

        tft.setCursor(15, yPos);
        String name = fileList[idx];
        if (name.startsWith("/")) name = name.substring(1);
        tft.print(name.substring(0, 16));
    }
}

void loadTrack(int index) {
    if (audioFile) audioFile.close();
    
    audioFile = SD.open(fileList[index]);
    if (audioFile) {
        fileSize = audioFile.size();
        audioFile.seek(44); 
        currentTrack = index;
        isPlaying = true;
    }
    renderMainUI();
}

void setup() {
    pinMode(Config::BTN_PLAY, INPUT_PULLUP);
    pinMode(Config::BTN_NEXT, INPUT_PULLUP);
    pinMode(Config::BTN_BACK, INPUT_PULLUP);
    pinMode(Config::ENC_CLK, INPUT_PULLUP);
    pinMode(Config::ENC_DT, INPUT_PULLUP);
    pinMode(Config::ENC_SW, INPUT_PULLUP);

    SPI1.setSCK(10); SPI1.setTX(11); SPI1.setRX(12);
    SPI1.begin();

    tft.init(240, 320);
    tft.setRotation(1);
    
    if (!SD.begin(Config::SD_CS, SPI1)) {
        tft.fillScreen(ST77XX_RED);
        tft.setCursor(10, 10);
        tft.print("SD FAIL");
        while(1);
    }

    File root = SD.open("/");
    while (File entry = root.openNextFile()) {
        String n = String(entry.name());
        n.toLowerCase();
        if (!entry.isDirectory() && n.endsWith(".wav") && fileCount < Config::MAX_FILES) {
            fileList[fileCount++] = "/" + String(entry.name());
        }
        entry.close();
    }

    if (fileCount > 0) loadTrack(0);
    else {
        tft.println("No WAV files found");
        while(1);
    }

    i2s.setBCLK(18); i2s.setDATA(20);
    i2s.setFrequency(Config::SAMPLE_RATE);
    i2s.setBitsPerSample(16);
    i2s.begin();
}

void loop() { 
    static uint32_t lastBtn = 0;
    if (millis() - lastBtn > 250) {
        if (!digitalRead(Config::BTN_PLAY)) { isPlaying = !isPlaying; renderMainUI(); lastBtn = millis(); }
        if (!digitalRead(Config::BTN_NEXT)) { loadTrack((currentTrack + 1) % fileCount); lastBtn = millis(); }
        if (!digitalRead(Config::BTN_BACK)) { loadTrack((currentTrack - 1 + fileCount) % fileCount); lastBtn = millis(); }
    }

    static int lastClk = digitalRead(Config::ENC_CLK);
    int currentClk = digitalRead(Config::ENC_CLK);
    if (currentClk != lastClk) {
        if (digitalRead(Config::ENC_DT) != currentClk) {
            isMenuOpen ? selectedTrack++ : volume += 150;
        } else {
            isMenuOpen ? selectedTrack-- : volume -= 150;
        }
        volume = constrain(volume, 0, 3000);
        selectedTrack = constrain(selectedTrack, 0, fileCount - 1);
        
        isMenuOpen ? renderList() : renderMainUI();
        lastClk = currentClk;
    }
 
    static bool lastSw = HIGH;
    bool sw = digitalRead(Config::ENC_SW);
    if (lastSw == HIGH && sw == LOW) {
        if (!isMenuOpen) {
            isMenuOpen = true;
            selectedTrack = currentTrack;
            renderList();
        } else {
            isMenuOpen = false;
            if (selectedTrack != currentTrack) loadTrack(selectedTrack);
            else renderMainUI();
        }
        delay(250);
    }
    lastSw = sw;

    if (isPlaying && audioFile.available() >= 2) {
        int16_t sample;
        audioFile.read((uint8_t*)&sample, 2);
        sample = (sample * volume) / 3000;
        i2s.write(sample);
        i2s.write(sample);
    } else if (isPlaying && !audioFile.available()) {
        loadTrack((currentTrack + 1) % fileCount);
    }

    updateProgressBar();
}
