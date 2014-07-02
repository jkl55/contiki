#include "contiki.h"
#include "lib/random.h"
#include "sys/etimer.h"
#include "sys/node-id.h"
#include "lib/assert.h"
#include "isr_compat.h"

#include <stdio.h> /* For printf() */

#define PERIOD (10)

/*---------------------------------------------------------------------------*/
volatile uint32_t seconds_counter;
/*---------------------------------------------------------------------------*/
PROCESS(test_process, "Test process");
AUTOSTART_PROCESSES(&test_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  random_init(4);

  printf("starting...\n");

  // start timer B
  TBCTL = TBSSEL_1 + MC_2 + TBIE; // ACLK, contmode, interrupt

  etimer_set(&timer, PERIOD);
  for(;;) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // disable interrupts and wait for a short time;
    // simulates the behavior of some long executing interrupt handlers
    dint();
#if CONTIKI_TARGET_SKY
    volatile uint16_t r = (random_rand() & 0xfff) + 0x800; // sky @ 3.9 MHz
#elif CONTIKI_TARGET_Z1
    volatile uint16_t r = (random_rand() & 0x1fff) + 0x1000; // z1 @ 8 MHz
#else
#error
#endif
    //r = 0x2fff; // a larger delay value does NOT cause a problem (tested @ tmote sky)!
    for(; r > 0; --r) {
      asm("nop");
    }
    eint(); // enable back interrupts

    etimer_restart(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
ISR(TIMERB1, timerb_interurpt)
{
  if(TBIV == 14) { // timer overflow?
    if(seconds_counter == 0) {
      // initialize
      seconds_counter = clock_seconds();
    } else {
      // increase
      seconds_counter += 2;
    }
    printf("B: %lu %lu\n", clock_seconds(), seconds_counter);
    assert(clock_seconds() == seconds_counter);
  }
}
/*---------------------------------------------------------------------------*/
