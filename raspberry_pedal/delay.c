#include <stdlib.h>
#include <stdio.h>
#include <bcm2835.h>

#define FOOT_SWITCH       RPI_GPIO_P1_11
#define LED               RPI_GPIO_P1_07
#define DELAY_MAX 800000
#define DELAY_MIN 0

uint32_t Delay_Buffer[DELAY_MAX];
uint32_t DelayCounter = 0;

uint32_t read_timer=0;
uint32_t output_signal=0;
uint32_t input_signal=0;

int delay_multiplier = 0;
int gain_splitter = 0;

uint8_t FOOT_SWITCH_val;

//main program 
int main(int argc, char *argv[]){
	for(int i = 0;i<argc;i++){
		printf("%s\n",argv[i]);
	}
	if (argc == 3) {
		
		
		gain_splitter = atoi(argv[1]);
		printf("arg1=%d\n", gain_splitter);
		delay_multiplier = atoi(argv[2]);
		printf("arg2=%d\n", delay_multiplier);
		
	}


	uint32_t Delay_Depth = 10000 + (delay_multiplier * 5000);

	// Start the BCM2835 Library to access GPIO.
	if (!bcm2835_init())
	{printf("bcm2835_init failed. Are you running as root??\n"); return 1;}
 
 
    	// Start the SPI BUS.
	if (!bcm2835_spi_begin())
	{printf("bcm2835_spi_begin failed. Are you running as root??\n"); return 1;}
  
    	//define PWM mode
    	bcm2835_gpio_fsel(12,BCM2835_GPIO_FSEL_ALT5 ); //PWM0 signal on GPIO12
   	    bcm2835_gpio_fsel(13,BCM2835_GPIO_FSEL_ALT0 ); //PWM1 signal on GPIO13
    	bcm2835_pwm_set_clock(2);                      // Max clk frequency (19.2MHz/2 = 9.6MHz)
   	    bcm2835_pwm_set_mode(0, 1, 1);                 //channel 0, markspace mode, PWM enabled. 
    	bcm2835_pwm_set_range(0,64);                   //channel 0, 64 is max range (6bits): 9.6MHz/64=150KHz PWM freq.
    	bcm2835_pwm_set_mode(1, 1, 1);                 //channel 1, markspace mode, PWM enabled.
    	bcm2835_pwm_set_range(1,64);                   //channel 0, 64 is max range (6bits): 9.6MHz/64=150KHz PWM freq.
  
    	//define SPI bus configuration
    	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);    // 4MHz clock with _64 
    	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    	uint8_t mosi[10] = { 0x01, 0x00, 0x00 }; //12 bit ADC read channel 0. 
    	uint8_t miso[10] = { 0 };

    	bcm2835_gpio_fsel(FOOT_SWITCH, BCM2835_GPIO_FSEL_INPT);     //FOOT_SWITCH as input
    	bcm2835_gpio_fsel(LED, BCM2835_GPIO_FSEL_OUTP);             //LED as output

    	bcm2835_gpio_set_pud(FOOT_SWITCH, BCM2835_GPIO_PUD_UP);     //FOOT_SWITCH pull-up enabled 

    	while(1) {
            
   		    bcm2835_spi_transfernb(mosi, miso, 3);
            input_signal = (miso[2] + ((miso[1] & 0x0F) << 8));

            Delay_Buffer[DelayCounter]  = input_signal;
            DelayCounter++;

            if(DelayCounter >= Delay_Depth) DelayCounter = 0; 

		    output_signal = input_signal + (Delay_Buffer[DelayCounter])>>1;

    		

		    output_signal = output_signal / gain_splitter;

            if (gain_splitter == 100) {
                
                output_signal = 0;

            }

    	bcm2835_pwm_set_data(1,output_signal & 0x3F);
   		bcm2835_pwm_set_data(0,output_signal >> 6);

    	}
    bcm2835_spi_end();
    bcm2835_close();
   	return 0;
}
