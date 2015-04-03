#ifndef PDRTEST_PROJECT_CONF_H
#define PDRTEST_PROJECT_CONF_H

//#include "common.h"

#undef   NETSTACK_CONF_MAC
#define  NETSTACK_CONF_MAC     nullmac_driver
#undef   NETSTACK_CONF_RDC
#define  NETSTACK_CONF_RDC     nullrdc_noframer_driver
// #undef   NETSTACK_CONF_FRAMER
// #define  NETSTACK_CONF_FRAMER  framer_nullmac

#define TEST_CHANNEL 26

// radio channel to use
#undef CC2420_CONF_CHANNEL
#define CC2420_CONF_CHANNEL  TEST_CHANNEL

#define RF_CHANNEL           TEST_CHANNEL

#define ALLOW_BAD_CRC 1

// #undef CC2420_CONF_AUTOACK
// #define CC2420_CONF_AUTOACK              0

// #undef RF230_CONF_AUTOACK
// #define RF230_CONF_AUTOACK               0

// if zero, then do NOT turn perform the CCA check when sending
//#define WITH_SEND_CCA                    0

// default 8MHz on Zolertia Z1
#define USE_FAST_CPU  1

#endif
