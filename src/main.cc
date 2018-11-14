/*
#include "stm32f3xx.h"
#include "include/gpio.h"
#include "include/reset_and_clock_control.h"
#include "include/hardware_factory.h"
#include "include/hardware_interface.h"

static ResetAndClockControl rcc;

int main(void)
{
  rcc.InitializeHardware();
  HardwareFactory hardware_factory;
  IHardware& hw = hardware_factory.GetHardware();
  hw.Initialize();
  hw.Led1().Clear();
*/


    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
/*
    while(1){
      
    }

}
*/

#define USE_STDPERIPH_DRIVER
#include "stm32f3xx.h"
#include "math.h"

//Quick hack, approximately 1ms delay
void ms_delay(int ms)
{
   while (ms-- > 0) {
      volatile int x=5971;
      while (x-- > 0)
         __asm("nop");
   }
}



//Flash orange LED at about 1hz
int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  // enable the clock to GPIOD
    __asm("dsb");                         // stall instruction pipeline, until instruction completes, as
                                          //    per Errata 2.1.13, "Delay after an RCC peripheral clock enabling"
    GPIOA->MODER = (1 << 18);             // set pin 13 to be general purpose output

    for (;;) {
       ms_delay(100);
       GPIOA->ODR ^= (1 << 9);           // Toggle the pin 
    }
}