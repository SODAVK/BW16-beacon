//simple version of deauth on bw16, but it work
//for advanced features (cts inject, disassoc)   see beaconspamv2.ino
//made by SoDaVk, please give me a star on github: https://github.com/SODAVK
// WARNING: version of AmebaSDK must be 3.1.7 for normal compile
#include <WiFi.h>
#include <wifi_conf.h>
#include <wifi_util.h>
#include <wifi_structures.h>

extern uint8_t* rltk_wlan_info;
extern "C" void* alloc_mgtxmitframe(void* ptr);
extern "C" void update_mgntframe_attrib(void* ptr, void* frame_control);
extern "C" int dump_mgntframe(void* ptr, void* frame_control);

void sendWifiRawManagementFrames(void* frame, size_t length) {
  void *ptr = (void *)**(uint32_t **)(rltk_wlan_info + 0x10);
  void *frame_control = alloc_mgtxmitframe(ptr + 0xae0);

  if (frame_control != 0) {
    update_mgntframe_attrib(ptr, frame_control + 8);
    memset((void *)*(uint32_t *)(frame_control + 0x80), 0, 0x68);
    uint8_t *frame_data = (uint8_t *)*(uint32_t *)(frame_control + 0x80) + 0x28;
    memcpy(frame_data, frame, length);
    *(uint32_t *)(frame_control + 0x14) = length;
    *(uint32_t *)(frame_control + 0x18) = length;
    dump_mgntframe(ptr, frame_control);
  }
}

const char* ssid_list[] = {
  "Hello world!",
  "Hola Mundo!",
  "Bonjour le monde!",
  "Ciao mondo!",
  "Olá Mundo!",
  "Hallo Welt!",
  "你好，世界！",
  "こんにちは世界！",
  "안녕하세요 세상!",
  "Привет, мир!",
  "Merhaba Dünya!",
  "Salam Dünya!",
  "Shalom Olam!",
  "Ahlan ya aalam!",
  "Namaste Duniya!",
  "Sawubona Mhlaba!",
  "Mo ki O Ile Aye!",
  "Halo, Dunia!",
  "Mingalarba, ကမ္ဘာ!",
  "Dia duit, domhain!",
  "Halló heimur!",
  "Kaixo mundua!",
  "Bonghjornu mondu!",
  "Saluton mondo!",
  "Dzień dobry świecie!",
  "Ahoj světe!",
  "Pozdravljen svet!",
  "Ahojte svete!",
  "Labas Pasauli!",
  "Sveika, pasaule!",
  "Tere maailm!",
  "Hei maailma!",
  "God dag verden!",
  "Hej världen!",
  "Halló verden!",
  "Sannu Duniya!",
  "Moni dziko lapansi!",
  "Lumela Lefatše!",
  "Yá'át'ééh, Nahasdzáán!",
  "Hola, Mondu!",
  "Ave, Terra!",
  "Helo Byd!",
  "E kaa ro, ụwa!",
  "Aloha Honua!",
  "Terveh, muailma!",
  "Barev Dzez Ashkharh!",
  "გამარჯობა სამყარო!",
  "Салом Ҷаҳон!",
  "Саламатсызбы, дүйнө!",
  "හෙලෝ ලෝකය!"
};

const int ssid_count = sizeof(ssid_list) / sizeof(ssid_list[0]);
uint8_t beacon_template[] = {
    //non fixed
    0x80, 0x00,                   
    0x00, 0x00,
    0xff,0xff,0xff,0xff,0xff,0xff,
    0x12,0x34,0x56,0x78,0x9a,0xbc, 
    0x12,0x34,0x56,0x78,0x9a,0xbc, 
    0x00, 0x00,//seqfrag
    //fixed
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x64, 0x00,//interval
    0x31, 0x04,//cap
    //t.t.sk.
    0x00, 0x00,
    //r
    0x01, 0x08, 0x82,0x84,0x8b,0x96,0x0c,0x12,0x18,0x24,
    //canal
    0x03, 0x01, 0x01
};

uint16_t sequence_number = 0;
void setup() {
  Serial.begin(115200);
  delay(1000);
  wifi_on(RTW_MODE_STA);
  wifi_disable_powersave();
  pmu_set_sysactive_time(0xFFFFFFFF);
  wext_set_channel(WLAN0_NAME, 1);
  Serial.println("Beacon started");
}

uint8_t channels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
size_t channel_index = 0;
void loop() {
  for (int i = 0; i < ssid_count; i++) {
    const char* ssid = ssid_list[i];
    int ssid_len = strlen(ssid);
    uint8_t frame[256];
    memcpy(frame, beacon_template, sizeof(beacon_template));
    frame[22] = sequence_number & 0xFF;
    frame[23] = (sequence_number >> 8) & 0x0F;
    sequence_number++;
    frame[36] = 0x00;
    frame[37] = ssid_len;
    memcpy(&frame[38], ssid, ssid_len);
    size_t frame_len = 38 + ssid_len + 2 + 10 + 3;
    sendWifiRawManagementFrames(frame, frame_len);
    channel_index = (channel_index + 1) % 3;
    wext_set_channel(WLAN0_NAME, channels[channel_index]);
    delay(1); //or delayMicroseconds(100);
  }
}
