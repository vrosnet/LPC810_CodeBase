/**************************************************************************/
/*!
    @file     main.c

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, K. Townsend (microBuilder.eu)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include <stdio.h>
#include "LPC8xx.h"
#include "gpio.h"
#include "mrt.h"
#include "uart.h"

#if defined(__CODE_RED)
  #include <cr_section_macros.h>
  #include <NXP/crp.h>
  __CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
#endif

#define LED_LOCATION    (2)

void configurePins()
{
  /* Enable SWM clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);

  /* Pin setup generated via Switch Matrix Tool
     ------------------------------------------
     PIO0_5 = RESET
     PIO0_4 = U0_TXD
     PIO0_3 = GPIO
     PIO0_2 = GPIO (User LED)
     PIO0_1 = GPIO
     PIO0_0 = U0_RXD
     ------------------------------------------
     NOTE: SWD is disabled to free GPIO pins!
     ------------------------------------------ */

  /* Pin Assign 8 bit Configuration */
  /* U0_TXD */
  /* U0_RXD */
  LPC_SWM->PINASSIGN0 = 0xffff0004UL;

  /* Pin Assign 1 bit Configuration */
  LPC_SWM->PINENABLE0 = 0xffffffffUL;
}

int main(void)
{
  /* Configure the core clock/PLL via CMSIS */
  SystemCoreClockUpdate();

  /* Initialise the GPIO block */
  gpioInit();

  /* Initialise the UART0 block for printf output */
  // uart0Init(115200);

  /* Configure the multi-rate timer for 1ms ticks */
  mrtInit(SystemCoreClock/1000);

  /* Configure the switch matrix (setup pins for UART0 and GPIO) */
  configurePins();

  /* Set the LED pin to output (1 = output, 0 = input) */
  LPC_GPIO_PORT->DIR0 |= (1 << LED_LOCATION);

  while(1)
  {
    /* Turn LED Off by setting the GPIO pin high */
    LPC_GPIO_PORT->SET0 = 1 << LED_LOCATION;
    mrtDelay(500);

    /* Turn LED On by setting the GPIO pin low */
    LPC_GPIO_PORT->CLR0 = 1 << LED_LOCATION;
    mrtDelay(500);

    /* Send some text (printf is redirected to UART0) */
    // printf("Hello, LPC810!\n");
  }
}