#include <stdlib.h>
#include <stdio.h>
#include <bcm2835.h>

int output_signal = 0;
int input_signal = 0;

int fuzz_on = 0;
int gain_splitter = 0;

//main program 
int main(int argc, char *argv[]){
	
    for(int i = 0;i<argc;i++){
		
        printf("%s\n",argv[i]);
	
    }
	
    if (argc == 3) {
	
		gain_splitter = atoi(argv[1]);
		printf("arg1=%d\n", gain_splitter);
		fuzz_on = atoi(argv[2]);
		printf("arg2=%d\n", fuzz_on);
	}

	if (!bcm2835_init()) {
        
        printf("bcm2835_init failed. Are you running as root??\n"); 
        return 1;
    
    }
 
	if (!bcm2835_spi_begin()) {

        printf("bcm2835_spi_begin failed. Are you running as root??\n"); 
        return 1;
        
    }
  
    bcm2835_gpio_fsel(12,BCM2835_GPIO_FSEL_ALT5 ); 
   	bcm2835_gpio_fsel(13,BCM2835_GPIO_FSEL_ALT0 ); 
    bcm2835_pwm_set_clock(2);                    
   	bcm2835_pwm_set_mode(0, 1, 1);                
    bcm2835_pwm_set_range(0,64);                  
    bcm2835_pwm_set_mode(1, 1, 1);             
    bcm2835_pwm_set_range(1,64);              

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);    
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);              
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); 
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                  
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    uint8_t mosi[10] = { 0x01, 0x00, 0x00 };
    uint8_t miso[10] = { 0 };

    while(1) {


            
   		bcm2835_spi_transfernb(mosi, miso, 3);
        input_signal = (miso[2] + ((miso[1] & 0x0F) << 8));

        

        output_signal = input_signal / ((gain_splitter - 20) * -1);

        if (gain_splitter == 20) {
                
            output_signal = 0;

        }

        if (fuzz_on == 1) {
            output_signal = output_signal << 5;
        }

    	bcm2835_pwm_set_data(1,output_signal & 0x3F);
   		bcm2835_pwm_set_data(0,output_signal >> 6);

    }
    bcm2835_spi_end();
    bcm2835_close();
   	return 0;
}
