/*
 * ❌ STATUS: NOT WORKING / EXPERIMENTAL
 * This ESP32 code is part of the third approach (ESP32 standalone/Blynk) which did not work.
 * Use the Linux C or Python direct serial connection approaches instead.
 */

/*
 * Silion RFID Reader - ESP32 Arduino Sketch
 * Converted from MicroPython to C++ for ESP32
 * 
 * Hardware Setup:
 *   ESP32 UART1 -> Silion RFID Module
 *   RX = GPIO18,  TX = GPIO17
 *   Baud: 115200,  Timeout: 600ms
 * 
 * Open Serial Monitor at 115200 baud to see output.
 */

#include <HardwareSerial.h>

// ──────────── PIN & UART CONFIG ────────────
#define RFID_RX_PIN   18
#define RFID_TX_PIN   17
#define RFID_BAUD     115200
#define RFID_TIMEOUT  600   // ms

HardwareSerial rfidSerial(1);  // UART1

// ──────────── GLOBALS ────────────
static bool    FASTID = false;
static uint8_t EmbeddedCommandContent[64];
static uint8_t EmbeddedCommandContentLen = 0;

// ──────────── HELPER: print hex array ────────────
void printHex(const uint8_t* buf, int len) {
  for (int i = 0; i < len; i++) {
    if (buf[i] < 0x10) Serial.print('0');
    Serial.print(buf[i], HEX);
    if (i < len - 1) Serial.print(' ');
  }
}

void printHexNoSpace(const uint8_t* buf, int len) {
  for (int i = 0; i < len; i++) {
    if (buf[i] < 0x10) Serial.print('0');
    Serial.print(buf[i], HEX);
  }
}

// ──────────── CRC FUNCTIONS ────────────
uint16_t CalcCRC(const uint8_t* msgbuf, int len) {
  uint16_t calcCrc = 0xFFFF;
  for (int i = 1; i < len; i++) {
    for (int dcdBit = 7; dcdBit >= 0; dcdBit--) {
      uint16_t xorFlag = calcCrc >> 15;
      calcCrc = ((msgbuf[i] >> dcdBit) & 1) | ((calcCrc << 1) & 0xFFFF);
      if (xorFlag) calcCrc ^= 0x1021;
    }
  }
  return calcCrc;
}

uint8_t CalcSubCRC(const uint8_t* buf, int len) {
  uint8_t calcCrc = 0x00;
  for (int i = 0; i < len; i++) {
    calcCrc += buf[i];
  }
  return calcCrc;
}

uint16_t CalcTagCRC(const uint8_t* datas, int len) {
  uint16_t crcVal = 0xFFFF;
  for (int i = 0; i < len; i++) {
    for (int bit = 7; bit >= 0; bit--) {
      if ((crcVal >> 15) != ((datas[i] >> bit) & 1)) {
        crcVal = ((crcVal << 1) ^ 0x1021) & 0xFFFF;
      } else {
        crcVal = (crcVal << 1) & 0xFFFF;
      }
    }
  }
  return crcVal ^ 0xFFFF;
}

// ──────────── DATA TRANSPORT ────────────
int DataTransport(const uint8_t* data, int len) {
  return rfidSerial.write(data, len);
}

// ──────────── DATA RECEIVE ────────────
// Returns length of received message in outBuf. 0 = nothing/error.
int DataRecv(uint8_t* outBuf, int maxLen) {
  // Read first byte (header) — blocks up to RFID_TIMEOUT
  int b = rfidSerial.read();
  if (b < 0) return 0;  // timeout, nothing received

  if (b != 0xFF) {
    Serial.print("Head_ERR:");
    if (b < 0x10) Serial.print('0');
    Serial.println(b, HEX);
    return 0;
  }
  outBuf[0] = 0xFF;

  // Read second byte (length)
  b = rfidSerial.read();
  if (b < 0) return 0;
  outBuf[1] = (uint8_t)b;

  int totalCmd = outBuf[1];
  int expected = totalCmd + 7;  // total frame length
  int idx = 2;

  // Read remaining bytes
  unsigned long startWait = millis();
  while (idx < expected && idx < maxLen) {
    if (rfidSerial.available()) {
      outBuf[idx++] = rfidSerial.read();
      startWait = millis();  // reset timeout on each received byte
    } else {
      if (millis() - startWait > RFID_TIMEOUT) return 0;  // timeout
      delay(1);
    }
  }

  if (idx < expected) return 0;

  // Verify CRC
  uint16_t crc16 = CalcCRC(outBuf, idx - 2);
  uint16_t rxCrc = ((uint16_t)outBuf[idx - 2] << 8) | outBuf[idx - 1];
  if (crc16 == rxCrc) {
    return idx;
  } else {
    Serial.print("CRC_ERR:");
    printHexNoSpace(outBuf, idx);
    Serial.println();
    return 0;
  }
}

// ──────────── GET LAYER ────────────
// Returns 0x12 (APP) or 0x11 (BOOT) or 0x900C (error)
uint16_t GetLayer(bool P = true, int R = 0) {
  const uint8_t cmd[] = {0xFF, 0x00, 0x0C, 0x1D, 0x03};
  DataTransport(cmd, sizeof(cmd));

  uint8_t buf[128];
  int len = DataRecv(buf, sizeof(buf));

  if (len == 0) {
    Serial.println("read_msg is empty when getting the reader's operating phase");
    delay(50);
    if (R >= 2) return 0x900C;
    return GetLayer(P, R + 1);
  }

  if (len >= 7 && buf[2] == 0x0C && buf[3] == 0 && buf[4] == 0) {
    uint8_t layer = buf[len - 3];
    if (P) {
      Serial.print("Get the reader's operating phase: ");
      Serial.print(layer == 0x12 ? "APP layer" : (layer == 0x11 ? "BOOT layer" : "error"));
      Serial.print(" read_msg:");
      printHex(buf, len);
      Serial.println();
    }
    return layer;
  } else {
    Serial.print("Get the reader's operating phase ERROR! R=");
    Serial.print(R);
    Serial.print(" read_msg:");
    printHex(buf, len);
    Serial.println();
    if (R >= 2) return 0x900C;
    delay(50);
    return GetLayer(P, R + 1);
  }
}

// ──────────── SWITCH TO APP LAYER ────────────
uint16_t SwitchToAPPLayer(bool P = true) {
  uint16_t layerFirst = GetLayer(false);

  if (layerFirst == 0x11) {
    const uint8_t cmd[] = {0xFF, 0x00, 0x04, 0x1D, 0x0B};
    DataTransport(cmd, sizeof(cmd));

    uint8_t buf[128];
    int len = DataRecv(buf, sizeof(buf));
    if (P) {
      Serial.print((len >= 7 && buf[3] == 0 && buf[4] == 0) ? "Successful" : "Error");
      Serial.print(" switch to APP read_msg:");
      printHex(buf, len);
      Serial.println();
    }

    uint16_t layer = GetLayer(false);
    if (P) {
      if (layer == 0x12) Serial.println("The current layer is in APP");
      else { Serial.print("The current layer is NOT in APP, layer=0x"); Serial.println(layer, HEX); }
    }
    return layer;
  } else if (layerFirst == 0x12) {
    if (P) Serial.println("The current layer is already in the APP layer");
  } else {
    Serial.print("SwitchToAPPLayer: Error getting layer: 0x");
    Serial.println(layerFirst, HEX);
  }
  return layerFirst;
}

// ──────────── SWITCH TO BOOT LAYER ────────────
uint16_t SwitchToBOOTLayer(bool preparingUpgrade = true, bool P = true) {
  uint16_t layerFirst = GetLayer(false);

  if (layerFirst == 0x12) {
    if (preparingUpgrade) {
      const uint8_t prepCmd[] = {0xFF,0x10,0xAA,0x4D,0x6F,0x64,0x75,0x6C,0x65,0x74,0x65,0x63,0x68,0xAA,0x40,0xAB,0x01,0x96,0xBB,0xBD,0x52};
      DataTransport(prepCmd, sizeof(prepCmd));
      uint8_t buf[128];
      DataRecv(buf, sizeof(buf));
    }
    const uint8_t cmd[] = {0xFF, 0x00, 0x09, 0x1D, 0x06};
    DataTransport(cmd, sizeof(cmd));
    uint8_t buf[128];
    int len = DataRecv(buf, sizeof(buf));
    if (P) {
      Serial.print((len >= 7 && buf[3] == 0 && buf[4] == 0) ? "Successful" : "Error");
      Serial.print(" switch to BOOT read_msg:");
      printHex(buf, len);
      Serial.println();
    }
    delay(250);
    uint16_t layer = GetLayer(false);
    if (P) {
      if (layer == 0x11) Serial.println("The current layer is in BOOT");
      else { Serial.print("The current layer is NOT in BOOT, layer=0x"); Serial.println(layer, HEX); }
    }
    return layer;
  } else if (layerFirst == 0x11) {
    if (P) Serial.println("The current layer is already in the BOOT layer");
  } else {
    Serial.print("SwitchToBOOTLayer: Error getting layer: 0x");
    Serial.println(layerFirst, HEX);
  }
  return layerFirst;
}

// ──────────── GET VERSION ────────────
String GetVersion(bool P = true) {
  uint16_t layerVal = GetLayer(false);
  const char* layerStr = (layerVal == 0x12) ? "APP layer" : ((layerVal == 0x11) ? "BOOT layer" : "error");

  const uint8_t cmd[] = {0xFF, 0x00, 0x03, 0x1D, 0x0C};
  DataTransport(cmd, sizeof(cmd));

  uint8_t buf[128];
  int len = DataRecv(buf, sizeof(buf));

  if (P) {
    Serial.print((len >= 7 && buf[2] == 3 && buf[3] == 0 && buf[4] == 0) ? "Successful " : "Error ");
    Serial.print("Get version at ");
    Serial.print(layerStr);
    Serial.print(", read_msg:");
    printHexNoSpace(buf, len);
    Serial.println();
  }

  if (len >= 7 && buf[2] == 3 && buf[3] == 0 && buf[4] == 0) {
    int hwIndex = 9;

    // Check for Fudan Micro Module
    if (len >= 13 && buf[12] == 0x80) {
      const uint8_t cmd05[] = {0xFF, 0x00, 0x05, 0x1D, 0x0A};
      DataTransport(cmd05, sizeof(cmd05));
      len = DataRecv(buf, sizeof(buf));
      if (len != 0x54 + 7) {
        Serial.print("Fudan Micro Module version length error read_msg:");
        printHex(buf, len);
        Serial.println();
        return "ERROR";
      }
      hwIndex += 4 * 4;
    }

    if (P) {
      Serial.print("Bootloader    Ver:"); printHex(buf + 5, 4); Serial.println();
      Serial.print("Hardware      Ver:"); printHex(buf + hwIndex, 4); Serial.println();
      Serial.print("Firmware     Data:"); printHex(buf + 13, 4); Serial.println();
      Serial.print("Firmware      Ver:"); printHex(buf + 17, 4); Serial.println();
      Serial.print("Supported Protool:"); printHex(buf + 21, 4); Serial.println();
    }

    if (hwIndex + 4 > len) {
      Serial.print("GetVersion error, read_msg:");
      printHex(buf, len);
      Serial.println();
      return "ERROR";
    }

    // Build hardware type string
    String hwType = "";
    uint8_t hwVer[4] = {buf[hwIndex], buf[hwIndex+1], buf[hwIndex+2], buf[hwIndex+3]};

    // Simplified hardware identification
    switch (hwVer[0]) {
      case 0x31: hwType += "Impinj E710,"; break;
      case 0x32: hwType += "Impinj E510,"; break;
      case 0x33: hwType += "Impinj E310,"; break;
      case 0x34: hwType += "Impinj E910,"; break;
      case 0xB1: hwType += "Fudan FM13RD1616 Nat.Std,"; break;
      case 0xB3: hwType += "Fudan FM13RD1616 Mil.Std,"; break;
      default: {
        char tmp[8]; snprintf(tmp, sizeof(tmp), "%02X,", hwVer[0]);
        hwType += tmp;
      }
    }

    switch (hwVer[1]) {
      case 0x00: hwType += "Single antenna,"; break;
      case 0x10: hwType += "28x28 SMD SIMx500,"; break;
      case 0x20: hwType += "21x21 SMD SIMx600,"; break;
      case 0x01: hwType += "Dual antenna,"; break;
      case 0x02: hwType += "Four antennas,"; break;
      case 0x03: hwType += "Eight antennas,"; break;
      case 0x04: hwType += "Sixteen antennas,"; break;
      default: {
        char tmp[8]; snprintf(tmp, sizeof(tmp), "%02X,", hwVer[1]);
        hwType += tmp;
      }
    }

    // Certification area
    const char* certNames[] = {
      "CHINA","FCC","JAPAN(NO_LBT_4FRE)","CE_LOW","KOREA","CE_HIGH","HK",
      "TAIWAN","MALAYSIA","SOUTH_AFRICA","BRAZIL","THAILAND","SINGAPORE",
      "AUSTRALIA","INDIA","URUGUAY","VIETNAM","ISRAEL","PHILIPPINES",
      "INDONESIA","NEW_ZEALAND","PERU","RUSSIA","CE_LOW_AND_HIGH"
    };
    if (hwVer[2] <= 0x17) {
      hwType += "Cert:";
      hwType += certNames[hwVer[2]];
    } else {
      char tmp[16]; snprintf(tmp, sizeof(tmp), "Cert:%02X", hwVer[2]);
      hwType += tmp;
    }

    return hwType;
  } else {
    Serial.print("Failed to get version at ");
    Serial.print(layerStr);
    Serial.print(", read_msg:");
    printHex(buf, len);
    Serial.println();
    return "Failed to get version";
  }
}

// ──────────── GET SERIAL ────────────
String GetSerial(bool P = true) {
  const uint8_t cmd[] = {0xFF, 0x02, 0x10, 0x00, 0x00, 0xF0, 0x93};
  DataTransport(cmd, sizeof(cmd));

  uint8_t buf[128];
  int len = DataRecv(buf, sizeof(buf));

  if (P) {
    Serial.print((len == 0x0C + 7 && buf[3] == 0 && buf[4] == 0) ? "Successful " : "Error ");
    Serial.print("Get serial number read_msg:");
    printHex(buf, len);
    Serial.println();
  }

  if (len == 0x0C + 7 && buf[3] == 0 && buf[4] == 0) {
    if (P) {
      Serial.print("Serial Number all:");
      printHexNoSpace(buf + 5, len - 7);
      Serial.println();
    }

    // Check if old-style serial (all digits < 10)
    bool allSmall = true;
    for (int i = 5; i < len - 2; i++) {
      if (buf[i] >= 10) { allSmall = false; break; }
    }

    String sn = "";
    if (allSmall) {
      for (int i = 5; i < len - 2; i++) sn += String(buf[i]);
      if (P) Serial.println("Serial Number (old):" + sn);
      sn = sn.substring(2);
    } else if (buf[8] == 0x10 || buf[8] == 0x30 || buf[8] == 0x40 || buf[8] == 0x84) {
      char tmp[16];
      snprintf(tmp, sizeof(tmp), "%02X%02X%02X%02X%02X", buf[8], buf[11], buf[12], buf[15], buf[16]);
      sn = String(tmp);
      if (P) Serial.println("Serial Number (new):" + sn);
    } else {
      if (P) Serial.println("Serial number format unknown");
      for (int i = 5; i < len - 2; i++) {
        char tmp[4]; snprintf(tmp, sizeof(tmp), "%02X", buf[i]);
        if (i > 5) sn += ".";
        sn += tmp;
      }
    }
    return sn;
  } else {
    String err = "Unable to retrieve ";
    for (int i = 0; i < len; i++) {
      char tmp[4]; snprintf(tmp, sizeof(tmp), "%02X ", buf[i]);
      err += tmp;
    }
    return err;
  }
}

// ──────────── RESOLUTION (parse tag data) ────────────
void resolution(const uint8_t* msg, int msgLen) {
  // Check heartbeat
  if (msgLen >= 9 && msg[5] == 'X' && msg[6] == 'T' && msg[7] == 'S' && msg[8] == 'J') {
    Serial.print("~~~HeartBeat:");
    printHex(msg, msgLen);
    Serial.println();
    return;
  }

  uint16_t MetadataFlags = ((uint16_t)msg[5] << 8) | msg[6];
  const uint8_t* MetaDataAndTagEPC = msg + 7;
  int metaLen = msgLen - 7 - 2;  // exclude header and CRC
  int index = 0;

  if (MetadataFlags & (1 << 0)) {
    Serial.print("ReadCount=");
    Serial.print(MetaDataAndTagEPC[index]);
    Serial.print(",");
    index += 1;
  }
  if (MetadataFlags & (1 << 1)) {
    Serial.print("RSSI=");
    Serial.print((int)MetaDataAndTagEPC[index] - 256);
    Serial.print("dBm,");
    index += 1;
  }
  if (MetadataFlags & (1 << 2)) {
    Serial.print("AntennaID=");
    if (MetaDataAndTagEPC[index] & 0xF0) {
      Serial.print(MetaDataAndTagEPC[index] >> 4);
      Serial.print("@");
    }
    Serial.print(MetaDataAndTagEPC[index] & 0x0F);
    Serial.print(",");
    index += 1;
  }
  if (MetadataFlags & (1 << 3)) {
    uint32_t freq = ((uint32_t)MetaDataAndTagEPC[index] << 16) |
                    ((uint32_t)MetaDataAndTagEPC[index+1] << 8) |
                    MetaDataAndTagEPC[index+2];
    Serial.print("Frequency=");
    Serial.print(freq);
    Serial.print("kHz,");
    index += 3;
  }
  if (MetadataFlags & (1 << 4)) {
    uint32_t ts = ((uint32_t)MetaDataAndTagEPC[index] << 24) |
                  ((uint32_t)MetaDataAndTagEPC[index+1] << 16) |
                  ((uint32_t)MetaDataAndTagEPC[index+2] << 8) |
                  MetaDataAndTagEPC[index+3];
    Serial.print("Timestamp=");
    Serial.print(ts);
    Serial.print("ms,");
    index += 4;
  }
  if (MetadataFlags & (1 << 5)) {
    uint16_t phase = ((uint16_t)MetaDataAndTagEPC[index] << 8) | MetaDataAndTagEPC[index+1];
    Serial.print("Phase=");
    Serial.print(phase);
    Serial.print(",");
    index += 2;
  }
  if (MetadataFlags & (1 << 6)) {
    Serial.print("ProtocolID=");
    Serial.print(MetaDataAndTagEPC[index]);
    Serial.print(",");
    index += 1;
  }
  if (MetadataFlags & (1 << 7)) {
    uint16_t tagDataLen = ((uint16_t)MetaDataAndTagEPC[index] << 8) | MetaDataAndTagEPC[index+1];
    index += 2;
    if (tagDataLen > 0) {
      Serial.print("MetaData:");
      printHexNoSpace(MetaDataAndTagEPC + index, tagDataLen / 8);
      Serial.print(",");
    }
    index += tagDataLen / 8;
  }

  if (index >= metaLen) return;

  uint8_t epcLength = MetaDataAndTagEPC[index];
  index += 1;

  const uint8_t* TagEPC = MetaDataAndTagEPC + index;
  if (epcLength == 0) {
    Serial.print("Tag EPC is empty, msg=");
    printHexNoSpace(msg, msgLen);
    Serial.println();
    return;
  }

  uint8_t epcWordLen = TagEPC[0] >> 3;
  if (epcLength != 2 + epcWordLen * 2 + 2) {
    Serial.print("!!!EPCLENGTH error ");
    Serial.print(epcLength);
    Serial.print("!=2+");
    Serial.print(epcWordLen);
    Serial.println("*2+2");
    return;
  }

  // Print EPC ID (skip PC[2 bytes] and CRC[2 bytes])
  if (!FASTID) {
    Serial.print("EPCID:");
    printHexNoSpace(TagEPC + 2, epcLength - 4);
    Serial.println();
  } else {
    // FASTID handling
    if (epcWordLen >= 7) {
      const uint8_t* fastidTid = TagEPC + epcLength - 14;  // -12 TID -2 CRC
      uint32_t shortTid = ((uint32_t)fastidTid[0] << 24) | ((uint32_t)fastidTid[1] << 16) |
                          ((uint32_t)fastidTid[2] << 8) | fastidTid[3];

      // Check M4/M5
      if (shortTid == 0xE2801100 || shortTid == 0xE2801104 || shortTid == 0xE2801105 ||
          shortTid == 0xE2801114 || shortTid == 0xE2801130 || shortTid == 0xE2801132) {
        int epc0len = epcWordLen - 7;
        Serial.print("m4m5!! support,EPCID:");
        printHexNoSpace(TagEPC + 2, epc0len * 2);
        Serial.print("~~~~~~TID:");
        printHexNoSpace(fastidTid, 12);
        Serial.println();
      }
      // Check M6+
      else if (shortTid == 0xE2801160 || shortTid == 0xE2801171 || shortTid == 0xE2801170 ||
               shortTid == 0xE2801191 || shortTid == 0xE2801190 || shortTid == 0xE28011A0 ||
               shortTid == 0xE28011A2 || shortTid == 0xE28011C0 || shortTid == 0xE28011C1 ||
               shortTid == 0xE28011B0) {
        Serial.print("new m6!! support, EPCID:");
        printHexNoSpace(TagEPC + 2, epcLength - 4 - 12);
        Serial.print("~~~~~~TID:");
        printHexNoSpace(TagEPC + epcLength - 14, 12);
        Serial.println();
      } else {
        Serial.print(">=7,not support,EPCID:");
        printHexNoSpace(TagEPC + 2, epcLength - 4);
        Serial.println();
      }
    } else {
      Serial.print("<=6,not support,EPCID:");
      printHexNoSpace(TagEPC + 2, epcLength - 4);
      Serial.println();
    }
  }
}

// ──────────── ASYNCHRONOUS INVENTORY ────────────
void AsynchronousInventory(unsigned long timeoutMs) {
  uint16_t FastMode = 0xAA48;

  // Metadata flags
  bool ReadCount     = true;
  bool RSSI_flag     = true;
  bool AntennaID     = true;
  bool Frequency     = true;
  bool Timestamp     = true;
  bool Phase         = true;
  bool ProtocolID    = false;
  bool TagDataLength = true;   // Additional data (read TID)
  FASTID             = false;
  bool XTSJ          = false;

  uint16_t MetadataFlags = 0x0000;
  uint8_t  SearchFlags[2] = {0x00, 0x00};

  if (ReadCount)     MetadataFlags |= (1 << 0);
  if (RSSI_flag)     MetadataFlags |= (1 << 1);
  if (AntennaID)     MetadataFlags |= (1 << 2);
  if (Frequency)     MetadataFlags |= (1 << 3);
  if (Timestamp)     MetadataFlags |= (1 << 4);
  if (Phase)         MetadataFlags |= (1 << 5);
  if (ProtocolID)    MetadataFlags |= (1 << 6);
  if (XTSJ)         SearchFlags[0] |= (1 << 7);

  // Build embedded command content for TID read
  EmbeddedCommandContentLen = 0;
  if (TagDataLength) {
    SearchFlags[1] |= (1 << 2);
    MetadataFlags  |= (1 << 7);

    // Header: [count, subLen, 0x28, 0x00, 0x00, 0x00]
    EmbeddedCommandContent[0] = 0;  // will be set later
    EmbeddedCommandContent[1] = 0;  // will be set later
    EmbeddedCommandContent[2] = 0x28;
    EmbeddedCommandContent[3] = 0x00;
    EmbeddedCommandContent[4] = 0x00;
    EmbeddedCommandContent[5] = 0x00;

    // Read TID bank (bank=2), from block 0, read 6 blocks
    EmbeddedCommandContent[6]  = 2;     // TID bank
    EmbeddedCommandContent[7]  = 0;     // Start block MSB
    EmbeddedCommandContent[8]  = 0;
    EmbeddedCommandContent[9]  = 0;
    EmbeddedCommandContent[10] = 0;     // Start block LSB
    EmbeddedCommandContent[11] = 6;     // Read 6 blocks

    EmbeddedCommandContentLen = 12;
    EmbeddedCommandContent[0] = (EmbeddedCommandContentLen - 6) / 6;  // 1 group
    EmbeddedCommandContent[1] = EmbeddedCommandContentLen - 3;
  }

  uint8_t Option = 0x00;  // No filtering
  if (FASTID) Option |= (1 << 7);

  // Build command packet
  uint8_t wdata[128];
  int wIdx = 0;

  // Header
  wdata[wIdx++] = 0xFF;
  wdata[wIdx++] = 0x00;  // length placeholder
  wdata[wIdx++] = 0xAA;

  // "Moduletech" magic
  const uint8_t magic[] = {0x4D,0x6F,0x64,0x75,0x6C,0x65,0x74,0x65,0x63,0x68};
  memcpy(wdata + wIdx, magic, 10);
  wIdx += 10;

  // FastMode
  wdata[wIdx++] = (FastMode >> 8) & 0xFF;
  wdata[wIdx++] = FastMode & 0xFF;

  // MetadataFlags
  wdata[wIdx++] = (MetadataFlags >> 8) & 0xFF;
  wdata[wIdx++] = MetadataFlags & 0xFF;

  // Option
  wdata[wIdx++] = Option;

  // SearchFlags
  wdata[wIdx++] = SearchFlags[0];
  wdata[wIdx++] = SearchFlags[1];

  // No AccessPassword, SelectAddress, SelectDataLength, SelectData (filtering disabled)

  // EmbeddedCommandContent
  if (EmbeddedCommandContentLen > 0) {
    memcpy(wdata + wIdx, EmbeddedCommandContent, EmbeddedCommandContentLen);
    wIdx += EmbeddedCommandContentLen;
  }

  // SubCRC (from byte 13 onwards)
  wdata[wIdx++] = CalcSubCRC(wdata + 13, wIdx - 13 - 1 + 1);  // Nope, let me fix this
  // Actually: CalcSubCRC from index 13 to current position (exclusive of SubCRC itself)
  // Recalculate properly:
  wIdx--;  // back up
  uint8_t subCrc = CalcSubCRC(wdata + 13, wIdx - 13);
  wdata[wIdx++] = subCrc;
  wdata[wIdx++] = 0xBB;

  // Set length field
  wdata[1] = wIdx - 3;

  // Calculate and append CRC16
  uint16_t crc16 = CalcCRC(wdata, wIdx);
  wdata[wIdx++] = (crc16 >> 8) & 0xFF;
  wdata[wIdx++] = crc16 & 0xFF;

  Serial.print("---start AA48 wdata:");
  printHex(wdata, wIdx);
  Serial.println();

  unsigned long startTime = millis();
  DataTransport(wdata, wIdx);

  // Read activation response
  uint8_t buf[256];
  int len = DataRecv(buf, sizeof(buf));
  if (len >= 7 && buf[3] == 0 && buf[4] == 0) {
    // OK
  } else {
    Serial.print("AA48 Activation failed read_msg:");
    printHex(buf, len);
    Serial.println();
    return;
  }

  Serial.print("---");
  Serial.print(millis() - startTime);
  Serial.print("ms start AA48 read_msg:");
  printHex(buf, len);
  Serial.println();

  // ── Read tags until timeout ──
  int readTimes = 0;
  while (true) {
    len = DataRecv(buf, sizeof(buf));
    if (len >= 9 && buf[3] == 0 && buf[4] == 0) {
      resolution(buf, len);
      readTimes++;
    } else if (len == 0) {
      // No data — continue
    } else {
      Serial.print("AA48 read_msg2:");
      printHexNoSpace(buf, len);
      Serial.println();
    }
    if (millis() - startTime > timeoutMs) break;
  }

  // ── Send STOP command ──
  const uint8_t stopCmd[] = {0xFF,0x0E,0xAA,0x4D,0x6F,0x64,0x75,0x6C,0x65,0x74,0x65,0x63,0x68,0xAA,0x49,0xF3,0xBB,0x03,0x91};
  unsigned long stopTime = millis();
  DataTransport(stopCmd, sizeof(stopCmd));

  Serial.print("---stop AA48 sent:");
  printHex(stopCmd, sizeof(stopCmd));
  Serial.println();

  while (true) {
    len = DataRecv(buf, sizeof(buf));
    if (len == 0x0C + 7) {
      // Check if it's the stop acknowledgment
      bool isStopAck = true;
      for (int i = 0; i < 10; i++) {
        if (buf[5 + i] != stopCmd[3 + i]) { isStopAck = false; break; }
      }
      if (isStopAck) {
        Serial.print("---");
        Serial.print(millis() - stopTime);
        Serial.print("ms stop AA48 read_msg:");
        printHex(buf, len);
        Serial.println();
        break;
      }
    }
    if (len >= 9 && buf[3] == 0 && buf[4] == 0) {
      resolution(buf, len);
      readTimes++;
    } else if (len == 0) {
      // nothing
    } else {
      Serial.print("stop AA48 Err:");
      printHex(buf, len);
      Serial.println();
      break;
    }
    if (millis() - stopTime > 100) {
      Serial.print("---");
      Serial.print(millis() - stopTime);
      Serial.println("ms stop AA48 Stop timeout");
      break;
    }
  }

  Serial.print("read_times=");
  Serial.println(readTimes);
}

// ──────────── CONFIGURE ANTENNA POWER ────────────
void configureAntennaPower(uint16_t readPower, uint16_t writePower,
                           const uint8_t* antennas, int antCount) {
  uint8_t cmd[64];
  int idx = 0;
  cmd[idx++] = 0xFF;
  cmd[idx++] = 0x00;  // length placeholder
  cmd[idx++] = 0x91;
  cmd[idx++] = 0x03;  // option = set read/write power

  for (int a = 0; a < antCount; a++) {
    cmd[idx++] = antennas[a];
    cmd[idx++] = (readPower >> 8) & 0xFF;
    cmd[idx++] = readPower & 0xFF;
    cmd[idx++] = (writePower >> 8) & 0xFF;
    cmd[idx++] = writePower & 0xFF;
  }
  cmd[1] = idx - 3;
  uint16_t crc = CalcCRC(cmd, idx);
  cmd[idx++] = (crc >> 8) & 0xFF;
  cmd[idx++] = crc & 0xFF;

  Serial.print("Configure antenna power cmd:");
  printHex(cmd, idx);
  Serial.println();

  DataTransport(cmd, idx);
  uint8_t buf[128];
  int len = DataRecv(buf, sizeof(buf));
  Serial.print((len >= 7 && buf[3] == 0 && buf[4] == 0) ? "Succeed " : "Failure ");
  Serial.print("Configure antenna power read_msg:");
  printHex(buf, len);
  Serial.println();
}

// ──────────── ENABLE ANTENNA ────────────
void enableAntenna(const uint8_t* antennas, int antCount) {
  uint8_t cmd[64];
  int idx = 0;
  cmd[idx++] = 0xFF;
  cmd[idx++] = 0x00;  // length placeholder
  cmd[idx++] = 0x91;
  cmd[idx++] = 0x02;  // option = enable antenna

  for (int a = 0; a < antCount; a++) {
    cmd[idx++] = antennas[a];
    cmd[idx++] = antennas[a];
  }
  cmd[1] = idx - 3;
  uint16_t crc = CalcCRC(cmd, idx);
  cmd[idx++] = (crc >> 8) & 0xFF;
  cmd[idx++] = crc & 0xFF;

  Serial.print("Enable antenna cmd:");
  printHex(cmd, idx);
  Serial.println();

  DataTransport(cmd, idx);
  uint8_t buf[128];
  int len = DataRecv(buf, sizeof(buf));
  Serial.print((len >= 7 && buf[3] == 0 && buf[4] == 0) ? "Succeed " : "Failure ");
  Serial.print("Enable antenna read_msg:");
  printHex(buf, len);
  Serial.println();
}

// ══════════════════════════════════════════
//                  SETUP
// ══════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n========================================");
  Serial.println("  Silion RFID Reader - ESP32 (C++)");
  Serial.println("========================================");
  Serial.println("START");

  // Initialize RFID UART: baud 115200, RX=18, TX=17
  rfidSerial.begin(RFID_BAUD, SERIAL_8N1, RFID_RX_PIN, RFID_TX_PIN);
  rfidSerial.setTimeout(RFID_TIMEOUT);
  delay(200);

  // 1) Switch to APP layer
  SwitchToAPPLayer(true);

  // 2) Get module info
  Serial.print("Module model: ");
  Serial.println(GetVersion(true));

  Serial.print("Serial number: ");
  Serial.println(GetSerial(true));

  // 3) Configure antenna power (ReadPower=3000, WritePower=3000, Antenna 1)
  uint8_t antennas[] = {1};
  configureAntennaPower(3000, 3000, antennas, 1);

  // 4) Enable antenna
  enableAntenna(antennas, 1);

  // 5) Run asynchronous inventory for 10 seconds
  Serial.println("\n--- Starting RFID Inventory (10 seconds) ---\n");
  AsynchronousInventory(10UL * 1000UL);

  Serial.println("\n========================================");
  Serial.println("END");
  Serial.println("========================================");
}

// ══════════════════════════════════════════
//                  LOOP
// ══════════════════════════════════════════
void loop() {
  // After initial inventory, run another scan every 15 seconds
  delay(15000);
  Serial.println("\n--- Starting RFID Inventory (10 seconds) ---\n");
  AsynchronousInventory(10UL * 1000UL);
  Serial.println("--- Inventory complete ---\n");
}
