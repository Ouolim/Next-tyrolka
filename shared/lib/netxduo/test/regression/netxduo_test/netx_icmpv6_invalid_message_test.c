/* Test processing of too big message with invalid length. */

#include    "nx_api.h"   

extern void    test_control_return(UINT status);

#if defined(FEATURE_NX_IPV6) && !defined(NX_DISABLE_RX_SIZE_CHECKING)
#include    "nx_icmpv6.h"
 
#define     DEMO_STACK_SIZE    2048

/* Define the ThreadX and NetX object control blocks...  */

static TX_THREAD               thread_0;  
static NX_PACKET_POOL          pool_0;
static NX_IP                   ip_0;

/* Define the counters used in the demo application...  */

static ULONG                   error_counter;  

/* Define thread prototypes.  */
static VOID    thread_0_entry(ULONG thread_input);
extern VOID    test_control_return(UINT status);       
extern VOID    _nx_ram_network_driver_1500(struct NX_IP_DRIVER_STRUCT *driver_req);

/* Invalid too big message. */
static char too_big_pkt[] = {
0x33, 0x33, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
0x00, 0x00, 0xa0, 0xa0, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x08, 0x3a, 0xff, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0xa0, 0xa0, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x02, 0x00, 
0xf2, 0x80, 0x00, 0x00, 0x05, 0x00
};

static char echo_request_pkt[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x3a, 0x40, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x80, 0x00, 
0x19, 0x22, 0x00, 0x00, 0x00
};

static char echo_reply_pkt[] = {
0x00, 0x11, 0x22, 0x33, 0x44, 0x56, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x00, 0x86, 0xdd, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x3a, 0x40, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
0x00, 0xff, 0xfe, 0x00, 0x01, 0x00, 0xfe, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, 
0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0x81, 0x00, 
0x18, 0x22, 0x00, 0x00, 0x00
};

typedef struct TEST_PACKET_STRUCT
{
    CHAR *test_packet_ptr;
    UINT  test_packet_length;

}TEST_PACKET;

static TEST_PACKET test_packet[] = 
{
#ifdef NX_ENABLE_IPV6_PATH_MTU_DISCOVERY
    {
        too_big_pkt,
        sizeof(too_big_pkt),
    },
#endif
    {
        echo_request_pkt,
        sizeof(echo_request_pkt),
    },
    {
        echo_reply_pkt,
        sizeof(echo_reply_pkt),
    },
};

/* Define what the initial system looks like.  */

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void           netx_icmpv6_invalid_message_test_application_define(void *first_unused_memory)
#endif
{
CHAR       *pointer;
UINT       status;

    /* Setup the working pointer.  */
    pointer = (CHAR *) first_unused_memory;

    /* Initialize the value.  */
    error_counter = 0;

    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,  
        pointer, DEMO_STACK_SIZE, 
        4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    pointer = pointer + DEMO_STACK_SIZE;

    /* Initialize the NetX system.  */
    nx_system_initialize();

    /* Create a packet pool.  */
    status = nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", 1536, pointer, 1536*16);
    pointer = pointer + 1536*16;

    if(status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&ip_0, "NetX IP Instance 0", IP_ADDRESS(1,2,3,4), 0xFFFFFF00UL, &pool_0, _nx_ram_network_driver_1500,
        pointer, 2048, 1);
    pointer = pointer + 2048;
  
    /* Enable IPv6 */
    status += nxd_ipv6_enable(&ip_0); 

    /* Check IPv6 enable status.  */
    if(status)
        error_counter++;        

    /* Enable IPv6 ICMP  */
    status += nxd_icmp_enable(&ip_0); 

    /* Check IPv6 ICMP enable status.  */
    if(status)
        error_counter++;
}

/* Define the test threads.  */

static void    thread_0_entry(ULONG thread_input)
{
UINT                    status;   
UINT                    address_index;
NXD_ADDRESS             ipv6_address;
ULONG                   prefix_length;
UINT                    interface_index;
NX_PACKET              *packet_ptr;
INT                     i;

    /* Print out test information banner.  */
    printf("NetX Test:   ICMPv6 Invalid Message Test..............................."); 

    /* Check for earlier error.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }                     

    /* Set the linklocal address.  */
    status = nxd_ipv6_address_set(&ip_0, 0, NX_NULL, 10, &address_index); 

    /* Check the status.  */
    if(status)
        error_counter++;  

    /* Sleep 5 seconds for linklocal address DAD.  */
    tx_thread_sleep(5 * NX_IP_PERIODIC_RATE);

    /* Get the linklocal address.  */
    status = nxd_ipv6_address_get(&ip_0, address_index, &ipv6_address, &prefix_length, &interface_index);
                       
    /* Check the status.  */
    if((status) || (prefix_length != 10) || (interface_index != 0))
        error_counter++;  

    /* Inject the test packets.   */
    for (i = 0; i < (sizeof(test_packet) / sizeof(TEST_PACKET)); i++)
    {

        /* Allocate packet. */
        status = nx_packet_allocate(&pool_0, &packet_ptr, NX_PHYSICAL_HEADER, NX_WAIT_FOREVER);

        /* Check status */
        if (status)
            error_counter++;

        /* Fill in the packet with data. Skip the MAC header.  */
        memcpy(packet_ptr->nx_packet_prepend_ptr, test_packet[i].test_packet_ptr + 14, test_packet[i].test_packet_length - 14);
        packet_ptr->nx_packet_length = test_packet[i].test_packet_length - 14;
        packet_ptr->nx_packet_append_ptr = packet_ptr->nx_packet_prepend_ptr + packet_ptr->nx_packet_length;

        /* Directly receive the packet.  */
        _nx_ip_packet_deferred_receive(&ip_0, packet_ptr);
    }

#ifndef NX_DISABLE_ICMP_INFO
    if (ip_0.nx_ip_icmp_invalid_packets != (sizeof(test_packet) / sizeof(TEST_PACKET)))
        error_counter++;
#endif /* NX_DISABLE_ICMP_INFO */

    /* Check the error.  */
    if(error_counter)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }  
    else
    {
        printf("SUCCESS!\n");      
        test_control_return(0);
    }
}                     
#else

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void           netx_icmpv6_invalid_message_test_application_define(void *first_unused_memory)
#endif
{

    /* Print out test information banner.  */
    printf("NetX Test:   ICMPv6 Invalid Message Test...............................N/A\n"); 
    test_control_return(3);

}
#endif /* FEATURE_NX_IPV6 */
