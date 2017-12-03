/**
 *  @filename   :   epd1in54-demo.ino
 *  @brief      :   1.54inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 5 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//#include <SPI.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <epd1in54.h>
#include <epdpaint.h>
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1


/* Can run 'make menuconfig' to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

void blink_task(void *pvParameter)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(static_cast<gpio_num_t>(BLINK_GPIO));
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(static_cast<gpio_num_t>(BLINK_GPIO), GPIO_MODE_OUTPUT);
    while(1) {
        /* Blink off (output low) */
        gpio_set_level(static_cast<gpio_num_t>(BLINK_GPIO), 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(static_cast<gpio_num_t>(BLINK_GPIO), 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}




/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;

void display_test(void *pvParameter)
 {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //printf("Start\n");
  //while(true) {vTaskDelay(1000 / portTICK_PERIOD_MS);}
  if (epd.Init(lut_full_update) != 0) {
      printf("e-paper init failed\n");
      return;
  }
      //printf("e-paper init success\n");

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
  //printf("Clear \n");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  //printf("Display \n");
  epd.DisplayFrame();
  vTaskDelay(100 / portTICK_PERIOD_MS);
  //printf("Clear Frame memory\n");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  //while(true){vTaskDelay(1000 / portTICK_PERIOD_MS);}
//printf("Rotating \n"); 
  //paint.SetRotate(ROTATE_0); 
  paint.SetRotate(ROTATE_0); 
  paint.SetWidth(200); 
  paint.SetHeight(24);
  //paint.SetHeight(24);
  //vTaskDelay(1000 / portTICK_PERIOD_MS);

  vTaskDelay(100 / portTICK_PERIOD_MS);
  //while(true) {vTaskDelay(1000 / portTICK_PERIOD_MS);}
  paint.Clear(COLORED);
  paint.DrawStringAt(30, 4, "Hello world!", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
  //epd.DisplayFrame();
  printf("done\n");

  //vTaskDelay(5000 / portTICK_PERIOD_MS);
  printf("e-paper Demo\n");
  //while(true) {vTaskDelay(1000 / portTICK_PERIOD_MS);}
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(30, 4, "Hoi Evelin", &Font16, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());
  printf("done\n");
  //vTaskDelay(5000 / portTICK_PERIOD_MS);
  //epd.DisplayFrame();

  paint.SetWidth(64);
  paint.SetHeight(64);
  vTaskDelay(100 / portTICK_PERIOD_MS);
  //while(true) {vTaskDelay(1000 / portTICK_PERIOD_MS);}
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 16, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 120, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 16, 130, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 120, 130, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  printf("Done...\n");
  while(true) {vTaskDelay(1000 / portTICK_PERIOD_MS);}

  if (epd.Init(lut_partial_update) != 0) {
      printf("e-Paper init failed");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();

  //time_start_ms = millis();


while(true) {
  // put your main code here, to run repeatedly:
  //time_now_s = (millis() - time_start_ms) / 1000;
    time_now_s = 500;
  char time_string[] = {'0', '0', ':', '0', '0', '\0'};
  time_string[0] = time_now_s / 60 / 10 + '0';
  time_string[1] = time_now_s / 60 % 10 + '0';
  time_string[3] = time_now_s % 60 / 10 + '0';
  time_string[4] = time_now_s % 60 % 10 + '0';

  paint.SetWidth(32);
  paint.SetHeight(96);
  paint.SetRotate(ROTATE_270);

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 80, 72, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();

  vTaskDelay(500 / portTICK_PERIOD_MS);
}
}

extern "C" void app_main()
{
    //xTaskCreate(&blink_task, "display_test", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    //xTaskCreate(&display_test, "display_test", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    xTaskCreate(&display_test, "display_test", 8000, NULL, 5, NULL);
}

