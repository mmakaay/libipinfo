/* format.c --- IP / network formatting functionality.
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
#include "ipv4_format.h"
#include "ipv6_format.h"

/**
 * Generate a string representation for IP address or network related info.
 * 
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an ipinfo_parse() call.
 * @param int type
 *     The type of formatting. One of:
 *     - IPINFO_FORMAT_IP
 *     - IPINFO_FORMAT_PREFIX
 *     - IPINFO_FORMAT_NETMASK
 *     - IPINFO_FORMAT_NETWORK
 *     - IPINFO_FORMAT_BROADCAST (only applicable for IPv4)
 *     - IPINFO_FORMAT_HOSTCOUNT (only applicable for IPv4)
 *     - IPINFO_FORMAT_DOMAIN
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
int
ipinfo_format(parsed, type, str)
    ipinfo *parsed;
    int     type;
    char   *str;
{
    if (parsed->domain == IPINFO_IPV4)
    {
        switch (type)
        {
        case IPINFO_FORMAT_IP:
            ipinfo_ipv4_format_ip(parsed, str);
            break;
        case IPINFO_FORMAT_PREFIX:
            ipinfo_ipv4_format_network_prefix(parsed, str);
            break;
        case IPINFO_FORMAT_NETMASK:
            ipinfo_ipv4_format_netmask(parsed, str);
            break;
        case IPINFO_FORMAT_NETWORK:
            ipinfo_ipv4_format_network_address(parsed, str);
            break;
        case IPINFO_FORMAT_BROADCAST:
            ipinfo_ipv4_format_broadcast_address(parsed, str);
            break;
        case IPINFO_FORMAT_HOSTCOUNT:
            ipinfo_ipv4_format_hostcount(parsed, str);
            break;
        case IPINFO_FORMAT_DOMAIN:
            sprintf(str, ipinfo_domaindescription(parsed->domain));
            break;

        default:
            // TODO
            break;
        }

        return 1;
    }
    else if (parsed->domain == IPINFO_IPV6)
    {
        switch (type)
        {
        case IPINFO_FORMAT_IP:
            ipinfo_ipv6_format_ip(parsed, str);
            break;
        case IPINFO_FORMAT_PREFIX:
            ipinfo_ipv6_format_network_prefix(parsed, str);
            break;
        case IPINFO_FORMAT_NETMASK:
            ipinfo_ipv6_format_netmask(parsed, str);
            break;
        case IPINFO_FORMAT_NETWORK:
            ipinfo_ipv6_format_network_address(parsed, str);
            break;
        case IPINFO_FORMAT_DOMAIN:
            sprintf(str, ipinfo_domaindescription(parsed->domain));
            break;

        default:
            // TODO
            break;
        }

        return 1;
    }
    else
    {
        snprintf(
            str, IPINFO_MAX_ADDRLEN,
            "[IPinfo error: no address parsed!]"
        );
        return 0;
    }
}

