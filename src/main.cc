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
  hw.Led1().Set();



    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    while(1){
      
    }

}
