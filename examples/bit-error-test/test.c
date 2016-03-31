#include "contiki.h"
#include "dev/radio.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "net/netstack.h"
#include "sys/node-id.h"
#include "rime/rime.h"

#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------- */

#define SEND_PERIOD (CLOCK_SECOND / 10)

/* -------------------------------------------------------------------- */
PROCESS(test_process, "Test process");
AUTOSTART_PROCESSES(&test_process);
/* -------------------------------------------------------------------- */
struct packet_s {
  uint8_t number;
  uint8_t filling[31];
};
/* -------------------------------------------------------------------- */
static void input_packet(void)
{
  static struct packet_s reference_packet;
  static uint16_t last_number;
  static uint16_t rx_ok;
  static uint16_t rx_corrupt;

  struct packet_s *p = (struct packet_s *) packetbuf_hdrptr();

  if(memcmp(p->filling, reference_packet.filling, sizeof(p->filling))) {
    /* corrupt packet detected */
    rx_corrupt++;
    return;
  }

  if(last_number > p->number) {
    printf("Out of 100 packets %u seen, %u good, %u corrupt\n",
        rx_ok + rx_corrupt + (uint16_t)rimestats.badcrc,
        rx_ok, rx_corrupt + (uint16_t)rimestats.badcrc);
    rx_ok = 0;
    rx_corrupt = 0;
    rimestats.badcrc = 0;
  }

  rx_ok++;
  last_number = p->number;
}
/* -------------------------------------------------------------------- */
RIME_SNIFFER(print_sniffer, input_packet, NULL);
/* -------------------------------------------------------------------- */
PROCESS_THREAD(test_process, ev, data)
{
  static struct etimer timer;
  static struct packet_s packet;

  PROCESS_BEGIN();

  rime_sniffer_add(&print_sniffer);

  if(node_id == 1) {
    printf("Receiving...\n");
    /* receiver */
    for(;;) {
      PROCESS_YIELD();
    }
  } else {
    printf("Sending...\n");
    /* sender */
    etimer_set(&timer, CLOCK_SECOND);
    for(;;) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

      leds_on(LEDS_ALL);

      /* send to radio */
      NETSTACK_RADIO.send(&packet, sizeof(packet));

      packet.number++;
      if(packet.number >= 100) {
        packet.number = 0;
        etimer_set(&timer, SEND_PERIOD * 10);
        printf("100 packets sent\n");
      } else {
        etimer_set(&timer, SEND_PERIOD);
      }

      leds_off(LEDS_ALL);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
