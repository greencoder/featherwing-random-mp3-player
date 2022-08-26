// Specifically for use with the Adafruit Feather, the pins are pre-set here!

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// These are the pins used for the Feather ESP8266
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS      16     // VS1053 chip select pin (output)
#define VS1053_DCS     15     // VS1053 Data/command select pin (output)
#define CARDCS          2     // Card chip select pin
#define VS1053_DREQ     0     // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

File sdCardRoot;
int numberOfMp3Files = 0;

void setup() {
  Serial.begin(115200);

  if (!musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }

  musicPlayer.setVolume(0, 0);

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  sdCardRoot = SD.open("/");
  numberOfMp3Files = countMp3Files(sdCardRoot);
  Serial.println("Found " + String(numberOfMp3Files) + " MP3 files");
}

void loop() {
  while (true) {
    playRandomMp3File();
  }
}

bool isValidMp3FileName(String fileName) {
  bool endsWithMp3Suffix = fileName.endsWith(String(".mp3"));
  bool isNotHiddenFile = !fileName.startsWith(String("."));
  return isNotHiddenFile && endsWithMp3Suffix;
}

int countMp3Files(File dir) {
  int numberOfMp3Files = 0;

  while (true) {
    File entry =  dir.openNextFile();
    String fileName;

    if (!entry) {
      break;
    }
    else {
      fileName = String(entry.name());
    }

    if (!entry.isDirectory() && isValidMp3FileName(fileName)) {
      numberOfMp3Files++;
    }

    entry.close();
  }

  return numberOfMp3Files;
}

String getMp3FilenameByIndexNumber(int targetIndex, File dir) {
  int mp3FileIndex = -1;
  String fileName;

  while (true) {
    File entry =  dir.openNextFile();

    if (!entry) {
      break;
    }
    else {
      fileName = String(entry.name());
    }

    if (!entry.isDirectory() && isValidMp3FileName(fileName)) {
      mp3FileIndex++;
    }

    if (mp3FileIndex == targetIndex) {
      String filePath = "/" + String(entry.name());
      entry.close();
      return filePath;
    }
    else {
      entry.close();
    }
  }
}

void playRandomMp3File() {
  int randomFileNumber = random(0, numberOfMp3Files);
  String filename = getMp3FilenameByIndexNumber(randomFileNumber, sdCardRoot);

  // You can't play a string name, it must be converted to char array first
  int strLength = filename.length() + 1;
  char charBuf[strLength];
  filename.toCharArray(charBuf, strLength);

  Serial.println("Playing file: " + filename);
  musicPlayer.playFullFile(charBuf);
}
