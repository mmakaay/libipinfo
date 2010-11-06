/* ipv4_format.c --- IPv4 IP address and network formatting functions
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

#include "config.h"
#include "ipinfo.h"
#include "math.h"

/**
 * Format the IPv4 address in dotted quad format (e.g. 192.168.0.20)
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 16 bytes long (123.123.123.123 + \0).
 */
void
ipinfo_ipv4_format_ip(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    snprintf(
        str, 16, "%d.%d.%d.%d",
        parsed->addr_bytes[0],
        parsed->addr_bytes[1],
        parsed->addr_bytes[2],
        parsed->addr_bytes[3]
    );
}

/**
 * Format the IPv4 netmask (e.g. 255.255.255.0)
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 16 bytes long (123.123.123.123 + \0).
 */
void
ipinfo_ipv4_format_netmask(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    unsigned char netmask[4];
    ipinfo_ipv4_prefixlen2netmask(parsed->prefixlen, (char *)&netmask);
    snprintf(
        str, 20, "%d.%d.%d.%d",
        netmask[0],
        netmask[1],
        netmask[2],
        netmask[3]
    );
}


/**
 * Format the IPv4 network prefix (e.g. 192.168.0.0/24).
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 20 bytes long (123.123.123.123/123 + \0).
 */
void
ipinfo_ipv4_format_network_prefix(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    unsigned char netmask[4];
    ipinfo_ipv4_prefixlen2netmask(parsed->prefixlen, (char *)&netmask);

    snprintf(
        str, 20, "%d.%d.%d.%d/%d",
        parsed->addr_bytes[0] & netmask[0],
        parsed->addr_bytes[1] & netmask[1],
        parsed->addr_bytes[2] & netmask[2],
        parsed->addr_bytes[3] & netmask[3],
        parsed->prefixlen
    );
}

/**
 * Format the IPv4 network address (e.g. 192.168.0.0).
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 16 bytes long (123.123.123.123 + \0).
 */
void
ipinfo_ipv4_format_network_address(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    unsigned char netmask[4];
    ipinfo_ipv4_prefixlen2netmask(parsed->prefixlen, (char *)&netmask);

    snprintf(
        str, 16, "%d.%d.%d.%d",
        parsed->addr_bytes[0] & netmask[0],
        parsed->addr_bytes[1] & netmask[1],
        parsed->addr_bytes[2] & netmask[2],
        parsed->addr_bytes[3] & netmask[3]
    );
}

/**
 * Format the IPv4 broadcast address (e.g. 192.168.0.255).
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 16 bytes long (123.123.123.123 + \0).
 */
void
ipinfo_ipv4_format_broadcast_address(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    unsigned char netmask[4];
    ipinfo_ipv4_prefixlen2netmask(parsed->prefixlen, (char *)&netmask);

    netmask[0] = ~netmask[0];
    netmask[1] = ~netmask[1];
    netmask[2] = ~netmask[2];
    netmask[3] = ~netmask[3];

    snprintf(
        str, 16, "%d.%d.%d.%d",
        parsed->addr_bytes[0] | netmask[0],
        parsed->addr_bytes[1] | netmask[1],
        parsed->addr_bytes[2] | netmask[2],
        parsed->addr_bytes[3] | netmask[3]
    );
}

/**
 * Format the IPv4 host count (e.g. 255).
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least 11 bytes long (highest possible number 4294967294 + \0).
 */
void
ipinfo_ipv4_format_hostcount(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    unsigned int hostcount = parsed->prefixlen >= 31
                  ? 0 : pow(2, (32 - parsed->prefixlen)) - 2;

    snprintf(str, 11, "%u", hostcount);
}
