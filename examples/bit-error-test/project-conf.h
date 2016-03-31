#undef   NETSTACK_CONF_MAC
#define  NETSTACK_CONF_MAC nullmac_driver
#undef   NETSTACK_CONF_RDC
#define  NETSTACK_CONF_RDC nullrdc_driver
#undef   NETSTACK_CONF_FRAMER
#define  NETSTACK_CONF_FRAMER framer_802154

#undef CC2420_CONF_AUTOACK
#define CC2420_CONF_AUTOACK              0

#define RIMESTATS_CONF_ENABLED 1
