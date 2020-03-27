#include "mbed.h"

Serial pc (USBTX,USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalOut greenLED(LED2);
DigitalOut redLED(LED1);
DigitalIn Switch(SW3);
BusOut display(D6,D7,D9,D10,D11,D5,D4,D8);
char table[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,0x00};
char table_fp[11] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF,0x00};
int sample = 1024;
float ADCdata[1024];

int main(){
    for(int i=0;i<sample ;i++){
        ADCdata[i] = Ain;
        wait(1./(sample));
    }
    for (int i=0;i<sample;i++){
        pc.printf("%1.3f\r\n",ADCdata[i]);
        wait(0.01);
    }
    float maxpos[200];
    float max = 0;
    float maxfront = 0;
    float maxnext = 0;
    int pos = 0;
    for(int i=1;i<sample-1;i++){
        max = ADCdata[i];
        maxfront = ADCdata[i-1];
        maxnext = ADCdata[i+1];
        if(max>maxfront&&max>maxnext){
            maxpos[pos] = i*(1./(sample)); 
            pos++;
        }
    }
    float sum = maxpos[pos-1]-maxpos[0];
    float avg = sum*1./(pos-1);
   // pc.printf("%1.8f\r\n",avg);
    float avg_freq = 1./avg;
  //  pc.printf("%1.8f\r\n",avg_freq);
    int round_freq = (int)(avg_freq+0.5);
    int freq_display_on_ssd[3];
   // pc.printf("%d\r\n",round_freq);
    int tmp_round_freq =round_freq; 
    for(int i=0;i<3;i++){
        freq_display_on_ssd[i] = tmp_round_freq%10;
        tmp_round_freq/=10;
    }
    //for(int i=0;i<3;i++){
    //    pc.printf("%d\r\n",freq_display_on_ssd[i]);
    //}
    float delay = 1./(round_freq*25);
    while(1){
        if(Switch == 0){
            greenLED = 1;
            redLED = 0;
            while(1){
                display = table[freq_display_on_ssd[2]];
                wait (1);
                if(Switch == 1)break;
                display = table[freq_display_on_ssd[1]];
                wait (1);
                if(Switch == 1)break;
                display = table_fp[freq_display_on_ssd[0]];
                wait (1);
                if(Switch == 1)break;
            }
        }
        else{
            greenLED = 0;
            redLED = 1;
            display = table[10];
            while(1){
                for(float i=0; i<2; i+=0.08 ){
                    Aout = 0.5 + 0.5*sin(i*3.14159);
                    wait(delay);
                }
                if (Switch==0)
                    break;
            }
        }
    }
}
