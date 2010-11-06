/* ipv4_netmask.c --- IPv4 netmask processing
 * Copyright (c) 2010
 * Maurice Makaay
 *
 * This file is part of the IPinfo package.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the MIT license. You should have received a copy
 * of the license along with this program. If not, see the website
 * <http://www.opensource.org/licenses/mit-license.php>
 */

#include "ipinfo.h"
#include "config.h"
#include "gettext_setup.h"

/**
 * Convert IPv4 netmask bytes to a prefix length.
 *
 * @param unsigned char* netmask_bytes
 *   A pointer to the start of an array of 4 bytes that hold the
 *   netmask bytes.
 * @param char* msg
 *   A pointer to a char array that will be used to report errors.
 * @param size_t msglen
 *   The maximum string length that can be written to msg.
 * @return integer prefixlen
 *   The prefix length (0 - 32) or -1 if there was an error
 *   in the netmask. In case of an error, an error message is
 *   written to the msg parameter.
 */
int
ipinfo_ipv4_netmask2prefixlen(netmask_bytes, msg, msglen)
    unsigned char *netmask_bytes;
    char          *msg;
    size_t         msglen;
{
    int byte, pos, bit, bits, rest_is_zero = 0, prefixlen = 0;

    for (pos = 0; pos < 4; pos++)
    {
        byte = netmask_bytes[pos];

        if (rest_is_zero)
        {
            if (byte != 0)
            {
                snprintf(
                    msg, msglen,
                    _("Byte %d '%d' in the netmask is invalid "
                      "(expected zero)"), pos+1, byte);
                return -1;
            }
            continue;
        }

        switch (byte)
        {
        case   0: prefixlen += 0; rest_is_zero = 1; break; /* 00000000 */
        case 128: prefixlen += 1; rest_is_zero = 1; break; /* 10000000 */
        case 192: prefixlen += 2; rest_is_zero = 1; break; /* 11000000 */
        case 224: prefixlen += 3; rest_is_zero = 1; break; /* 11100000 */
        case 240: prefixlen += 4; rest_is_zero = 1; break; /* 11110000 */ 
        case 248: prefixlen += 5; rest_is_zero = 1; break; /* 11111000 */ 
        case 252: prefixlen += 6; rest_is_zero = 1; break; /* 11111100 */ 
        case 254: prefixlen += 7; rest_is_zero = 1; break; /* 11111110 */ 
        case 255: prefixlen += 8; rest_is_zero = 0; break; /* 11111111 */ 

        default:
            snprintf(
                msg, msglen,
                _("Byte %d '%d' in the netmask is invalid"), pos+1, byte);
            return -1;
        }
    }

    return prefixlen;
}

/**
 * Convert a prefix length to IPv4 netmask bytes.
 *
 * @return integer prefixlen
 *   The prefix length (0 - 32) to convert to a netmask.
 * @param unsigned char* netmask_bytes
 *   A pointer to the start of an array of 4 bytes that will hold the
 *   netmask bytes.
 */
void
ipinfo_ipv4_prefixlen2netmask(prefixlen, netmask_bytes)
    int            prefixlen;
    unsigned char *netmask_bytes;
{
    int pos;

    for (pos = 0; pos < 4; pos++)
    {
        if (prefixlen == 0) {
            netmask_bytes[pos] = 0;
        }
        else if (prefixlen >= 8) {
            netmask_bytes[pos] = 255;
            prefixlen -= 8;
        }
        else {
            netmask_bytes[pos] = 255 << (8 - prefixlen);
            prefixlen = 0; 
        }
    }
}

