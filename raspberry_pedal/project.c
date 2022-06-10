#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <math.h>
#include <bcm2835.h>
/*
#define C2	65
#define CS2	69
#define D2	73
#define Eb2	77
#define E2	82
#define F2	87
#define FS2	92
#define G2	98
#define GS2	103
#define A2	110
#define Bb2	116
#define B2	123

#define C3	130
#define CS3	138
#define D3	146
#define Eb3	155
#define E3	164
#define F3	174
#define FS3	185
#define G3	196
#define GS3	207
#define A3	220
#define Bb3	233lkj
#define B3	246

*/

int main(void) {
/*
	int arrayB[] = {E2, F2, B2, E2, F2, C3, E2, F2, CS3, E2, F2, C3, E2, F2, B2, B2};
	int arrayA[] = {E2, E2, E3, E2, E2, Eb3, E2, E2, E2, E2, E2, B2, E2, E2, Bb2, E2, E2, A2,
			E2, GS2, E2, G2, E2, FS2, F2};
*/
	/*wiringPiSetup();
	pinMode (26, PWM_OUTPUT);*/
  if (!bcm2835_init())
    {printf("bcm2835_init failed. Are you running as root??\n"); return 1;}
 
 
    // Start the SPI BUS.
    if (!bcm2835_spi_begin())
    {printf("bcm2835_spi_begin failed. Are you running as root??\n"); return 1;}
  
    //define PWM mode   
    bcm2835_gpio_fsel(12,BCM2835_GPIO_FSEL_ALT0 ); //PWM0 signal on GPIO18    
    bcm2835_gpio_fsel(13,BCM2835_GPIO_FSEL_ALT0 ); //PWM1 signal on GPIO13    
    bcm2835_pwm_set_clock(2);                      // Max clk frequency (19.2MHz/2 = 9.6MHz)
    bcm2835_pwm_set_mode(0,1 , 1);                 //channel 0, markspace mode, PWM enabled. 
    bcm2835_pwm_set_range(0,64);                   //channel 0, 64 is max range (6bits): 9.6MHz/64=150KHz PWM freq.
    bcm2835_pwm_set_mode(1, 1, 1);                 //channel 1, markspace mode, PWM enabled.
    bcm2835_pwm_set_range(1,64);                   //channel 0, 64 is max range (6bits): 9.6MHz/64=150KHz PWM freq.
  

	float i = 0;
	for(;;){
		float a = sin(i) + 1;
		//printf("%f\t", a);
		a = a * 32;
		int b = a;
		//printf("%d\n", b);

		bcm2835_pwm_set_data(1, b);
		i+= 0.1;
		if (i > 1000) {
			i = 0;
		}
		delay(1);

		/*for (int i = 0; i < sizeof(arrayA)/sizeof(int); i++){
			softToneCreate(26);
			softToneWrite(26, arrayA[i]);
			if (i == 8){
			softToneWrite(26, D3);
			delay(300);
			softToneWrite(26, CS3);
			delay(300);
			softToneWrite(26, C3);
			delay(450);
			}
		delay(150);
		}
		int a = 0;
		while (a < 4) {
			for (int j = 0; j < sizeof(arrayB)/sizeof(int); j++){
				softToneCreate(26);
				softToneWrite(26, arrayB[j]);
				delay(150);
			}
			a++;
		}*/
	}
	return 0;
}
