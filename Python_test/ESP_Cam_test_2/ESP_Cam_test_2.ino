#include"Low_pass_filter.h"

uint8_t img_array [HEIGHT][WIDTH] = {0}; // grayscale goes from 0 to 255. fits in 8bitss

uint8_t filter_array_mean [3][3] ={
        { 1, 1, 1},
        { 1, 1, 1},
        { 1, 1, 1}
};

int8_t filter_array_laplacian [3][3] ={
        { 0, -1,  0},
        { -1, 4, -1},
        { 0, -1,  0}
};

void setup() {
   //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
   uint8_t img_array_new [HEIGHT][WIDTH] = {0};
   Serial.begin(115200);
   Serial.println("BEGIN");
   Serial.println(setup_camera(FRAME_SIZE) ? "OK" : "ERR INIT");
   Serial.println("Wait for Camera becomes stable?!");
   delay(5000);
   Serial.println("Start capturing:");
   if (!capture_image()) {
    Serial.println("Failed capture");
    return;
   };
   //print_features();
   Serial.println("End capturing");
   delay(3000);
}

void loop() {
}

bool capture_image() {
   camera_fb_t * frame = NULL;
   frame = esp_camera_fb_get();
   Serial.println("Transfer frame to 2d array");
   delay(500);
   frame_to_2d_array(frame);
   Serial.println("Applying mean filter: ");
   delay(500);   
   //mean_filter();
   laplacian_filter();
   
   //transmit_1d_array(frame);
   //transmit_pixel_raw();
   //transmit_pixel_after();
   esp_camera_fb_return(frame);
   if (!frame)
       return false;
   return true;
}

void mean_filter(){
  uint8_t img_array_new[HEIGHT][WIDTH] = {0};
  for(uint16_t i = 0; i < HEIGHT-1; i++){
    for(uint16_t j = 0; j < WIDTH-2; j++){
      double temp =
      {
         img_array[i][j]*filter_array_mean[0][0] + img_array[i][j+1]*filter_array_mean[0][1] + img_array[i][j+2]*filter_array_mean[0][2] 
       + img_array[i+1][j]*filter_array_mean[1][0] + img_array[i+1][j+1]*filter_array_mean[1][1] + img_array[i+1][j+2]*filter_array_mean[1][2] 
       + img_array[i+2][j]*filter_array_mean[2][0] + img_array[i+2][j+1]*filter_array_mean[2][1] + img_array[i+2][j+2]*filter_array_mean[2][2]
      };
      temp = (temp/9) + 0.5 - (temp<0);
      Serial.print(img_array_new[i+1][j+1] = temp);
      Serial.print(",");
    }
  }
}

void laplacian_filter(){
  uint8_t img_array_new[HEIGHT][WIDTH] = {0};
  for(uint16_t i = 0; i < HEIGHT-1; i++){
    for(uint16_t j = 0; j < WIDTH-2; j++){
      double temp =
      {
         img_array[i][j]*filter_array_mean[0][0] + img_array[i][j+1]*filter_array_mean[0][1] + img_array[i][j+2]*filter_array_mean[0][2] 
       + img_array[i+1][j]*filter_array_mean[1][0] + img_array[i+1][j+1]*filter_array_mean[1][1] + img_array[i+1][j+2]*filter_array_mean[1][2] 
       + img_array[i+2][j]*filter_array_mean[2][0] + img_array[i+2][j+1]*filter_array_mean[2][1] + img_array[i+2][j+2]*filter_array_mean[2][2]
      };
      temp = (temp+255/8) + 0.5 - (temp<0);
      Serial.print(img_array_new[i+1][j+1] = temp);
      Serial.print(",");
    }
  }
}

void frame_to_2d_array(camera_fb_t * frame){
   uint32_t frame_index = 0;
   for(uint16_t i = 0; i < HEIGHT; i++){
      for(uint16_t j = 0; j < WIDTH; j++){
        img_array[i][j] = frame->buf[frame_index];
        frame_index++;
      }
   }
}

void transmit_1d_array(camera_fb_t * frame){
  for(uint32_t i = 0; i < HEIGHT*WIDTH; i++){
    Serial.print(frame->buf[i]);
    Serial.print(",");
  }
}
