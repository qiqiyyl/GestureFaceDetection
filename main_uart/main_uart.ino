
#include "DFRobot_GestureFaceDetection.h"
#include <Wire.h>
#include "gestureImg.h"
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Adafruit_NeoPixel.h>
// #define PIN 12
#define PIN 9
#define LED_COUNT 180

#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  12
#define A   A0
#define B   A1
#define C   A2

#define clear()  fillRect(0, 0, 16, 22, matrix.Color333(0, 0, 0))
#define clear_face()  fillRect(0, 22, 16, 32, matrix.Color333(0, 0, 0))
// Define the device ID for the GestureFaceDetection sensor
#define DEVICE_ID  0x72 
// Create an instance of DFRobot_GestureFaceDetection_I2C with the specified device ID
DFRobot_GestureFaceDetection_UART gfd(&Serial1, DEVICE_ID);
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
    // Initialize URAT communication
    Serial1.begin(9600);
    // Initialize serial communication for debugging purposes
    Serial.begin(115200);
    Serial.println("Start");
    // Retrieve and print the Product ID (PID) of the sensor
    Serial.println(gfd.getPid());
    // Retrieve and print the Vendor ID (VID) of the sensor
    Serial.println(gfd.getVid());
    gfd.setGestureDetectThres(50);
    
    matrix.begin();
    matrix.setRotation(3);
    matrix.fillScreen(0);
    strip.begin();
}

uint16_t flagType = 0, flagFace = -1;

void loop(){
  uint16_t faceNumber = gfd.getFaceNumber();
  Serial.print("人脸: ");
  Serial.println(faceNumber);
  if(faceNumber > 0) {
    uint16_t gestureType = gfd.getGestureType();
    // Serial.println(gestureType);
    if(flagType != gestureType){
      changPanelImg(gestureType);
    }
    flagType = gestureType;
  }
  else if(flagFace != 0) {
    setAllLEDs(strip.Color(0, 0, 0));
    matrix.clear();
  }
  if(faceNumber < 100 && faceNumber != flagFace){
      matrix.clear_face();
      if(faceNumber<10){ 
        matrix.setCursor(5, 22);
      }
      else{
        matrix.setCursor(2, 22);
      }
      matrix.print(faceNumber);
  }
  flagFace = faceNumber;
}

// 根据传入的手势类型进行变化矩阵板的图像，灯带变化
void changPanelImg(uint16_t gestureType)
{
  matrix.clear();
  switch(gestureType) {
    case 1:
      setAllLEDs(strip.Color(0, 0, 20));
      matrix.drawRGBBitmap(0, 2, (const uint16_t *)bitmap1, 16, 16);
      break;
    case 2:
      setAllLEDs(strip.Color(0, 20, 0));
      matrix.drawRGBBitmap(0, 2, (const uint16_t *)bitmap2, 16, 16);
      break;
    case 3:
      setAllLEDs(strip.Color(20, 0, 0));
      matrix.drawRGBBitmap(0, 2, (const uint16_t *)bitmap3, 16, 19);
      break;
    case 4:
      setAllLEDs(strip.Color(20, 20, 0));
      matrix.drawRGBBitmap(0, 2, (const uint16_t *)bitmap4, 16, 20);
      break;
    case 5:
      setAllLEDs(strip.Color(20, 0, 20));
      matrix.drawRGBBitmap(0, 2, (const uint16_t *)bitmap5, 16, 18);
      break;
    default:
      setAllLEDs(strip.Color(0, 0, 0));
      // matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
      break;
  }

}

// 改变灯带颜色
void setAllLEDs(uint32_t c){
  for(int i = 0; i < strip.numPixels(); ++i){
    strip.setPixelColor(i, c);
  }
  strip.show();
}
