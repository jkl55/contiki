// -*- mode: contiki-c-mode; c-basic-offset: 4; c-indent-level: 4 -*-
#include "contiki.h"
#include "sys/process.h"
#include "dev/serial-line.h"
#include "dev/watchdog.h"
#include "lib/random.h"
#include "sys/node-id.h"
#include "netstack.h"
#include "cc2420.h"
#include "dev/leds.h"
#include "rime/rime.h"
#include <stdio.h>

#define TEST_PACKET_SIZE 124

PROCESS(example_process, "Example");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data)
{
    static struct etimer et;
    static uint8_t sendBuffer[TEST_PACKET_SIZE];

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    while (RTIMER_CLOCK_LT(RTIMER_NOW(), 0xffff));
    
    if (node_id == 1) {
        while (RTIMER_CLOCK_LT(RTIMER_NOW(), 0x10000 + 1000ul));

        NETSTACK_RADIO.send(sendBuffer, TEST_PACKET_SIZE);

        clock_wait(20);

        NETSTACK_RADIO.send(sendBuffer, TEST_PACKET_SIZE);

    } else {

        // output: "packet NOT seen"
        cc2420_set_channel(25);
        while (RTIMER_CLOCK_LT(RTIMER_NOW(), 0x10000 + 1050ul));
        cc2420_set_channel(26);

        if (!NETSTACK_RADIO.channel_clear()) {
            printf("packet seen\n");
        } else {
            printf("packet NOT seen\n");
        }

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
