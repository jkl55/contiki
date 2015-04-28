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

#define INTERFERENCE_PERIOD (RTIMER_ARCH_SECOND / 256 + 10)

static uint8_t sendBuffer[TEST_PACKET_SIZE] __attribute__((aligned (2)));

void generate_traffic(void)
{
    watchdog_stop();

    // disable interrupts
    dint();
    // disable CCA on Tx
    cc2420_set_cca_threshold(127);

    rtimer_clock_t next = RTIMER_NOW() + INTERFERENCE_PERIOD;

    for (;;) {
        while (RTIMER_CLOCK_LT(RTIMER_NOW(), next));
        next = RTIMER_NOW() + INTERFERENCE_PERIOD;

        NETSTACK_RADIO.send(sendBuffer, TEST_PACKET_SIZE / 2);
    }
}

static void input_packet(void)
{
    printf("input %d bytes\n", packetbuf_totlen());
}

RIME_SNIFFER(printsniffer, input_packet, NULL);

static void *send_timer_callback(struct rtimer *rt, void *x)
{
    printf("send the packet\n");

    NETSTACK_RADIO.send(sendBuffer, TEST_PACKET_SIZE);
}

PROCESS(example_process, "Example");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data)
{
    static struct etimer et;
    static struct rtimer rt;
    static uint8_t sendBuffer[TEST_PACKET_SIZE];

    PROCESS_BEGIN();

    cc2420_set_channel(26);

    etimer_set(&et, CLOCK_SECOND);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if (node_id == 3) generate_traffic();

    rime_sniffer_add(&printsniffer);
    
    while (RTIMER_CLOCK_LT(RTIMER_NOW(), 0xffff));

    cc2420_set_channel(25);

    if (node_id == 1) {
        rtimer_set(&rt, RTIMER_ARCH_SECOND / 8, 1,
                send_timer_callback, &rt);
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
