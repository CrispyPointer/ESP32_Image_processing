#include"Low_pass_filter.h"

uint8_t img_array [HEIGHT][WIDTH] = {0}; // grayscale goes from 0 to 255. fits in 8bitss

uint8_t filter_array_mean [3][3] ={
        { 1, 1, 1},
        { 1, 1, 1},
        { 1, 1, 1}
};

uint8_t filter_array_gaussian [3][3] ={
        { 1, 2, 1},
        { 2, 4, 2},
        { 1, 2, 1}
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
   Serial.println("End process!!!");
   delay(3000);
}

void loop() {
}

bool capture_image() {
   camera_fb_t * frame = NULL;
   frame = esp_camera_fb_get();
   Serial.println("Transfer frame to 2d array");
   delay(2000);
   frame_to_2d_array(frame);

   Serial.println("Sending raw data: ");
   delay(1000);
   transmit_1d_array(frame);
   Serial.println("End transmitting");
   delay(2000);
   
   Serial.println("Sending low_filter data");
   delay(1000);
   mean_filter();
   Serial.println("End transmitting");
   delay(2000);

   Serial.println("Sending gaussian_filter data");
   delay(1000);
   gaussian_filter();
   Serial.println("End transmitting");
   delay(2000);
   
   Serial.println("Sending laplacian_filter data");
   delay(1000);   
   laplacian_filter();
   Serial.println("End transmitting");
   delay(2000);
   
   esp_camera_fb_return(frame);
   if (!frame)
       return false;
   return true;
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
  Serial.println();
}

void gaussian_filter(){
  uint8_t img_array_new[HEIGHT][WIDTH] = {0};
  for(uint16_t i = 0; i < HEIGHT-1; i++){
    for(uint16_t j = 0; j < WIDTH-2; j++){
      double temp =
      {
         img_array[i][j]*filter_array_gaussian[0][0] + img_array[i][j+1]*filter_array_gaussian[0][1] + img_array[i][j+2]*filter_array_gaussian[0][2] 
       + img_array[i+1][j]*filter_array_gaussian[1][0] + img_array[i+1][j+1]*filter_array_gaussian[1][1] + img_array[i+1][j+2]*filter_array_gaussian[1][2] 
       + img_array[i+2][j]*filter_array_gaussian[2][0] + img_array[i+2][j+1]*filter_array_gaussian[2][1] + img_array[i+2][j+2]*filter_array_gaussian[2][2]
      };
      temp = (temp/9) + 0.5 - (temp<0);
      Serial.print(img_array_new[i+1][j+1] = temp);
      Serial.print(",");
    }
  }
  Serial.println();
}

void laplacian_filter(){
  uint8_t img_array_new[HEIGHT][WIDTH] = {0};
  for(uint16_t i = 0; i < HEIGHT-1; i++){
    for(uint16_t j = 0; j < WIDTH-2; j++){
      double temp =
      {
         img_array[i][j]*filter_array_laplacian[0][0] + img_array[i][j+1]*filter_array_laplacian[0][1] + img_array[i][j+2]*filter_array_laplacian[0][2] 
       + img_array[i+1][j]*filter_array_laplacian[1][0] + img_array[i+1][j+1]*filter_array_laplacian[1][1] + img_array[i+1][j+2]*filter_array_laplacian[1][2] 
       + img_array[i+2][j]*filter_array_laplacian[2][0] + img_array[i+2][j+1]*filter_array_laplacian[2][1] + img_array[i+2][j+2]*filter_array_laplacian[2][2]
      };
      temp = (temp+255/8) + 0.5 - (temp<0);
      Serial.print(img_array_new[i+1][j+1] = temp);
      Serial.print(",");
    }
  }
  Serial.println();
}

void transmit_1d_array(camera_fb_t * frame){
  for(uint32_t i = 0; i < HEIGHT*WIDTH; i++){
    Serial.print(frame->buf[i]);
    Serial.print(",");
  }
  Serial.println();
}
