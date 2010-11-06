/* ipv6_format.c --- RFC5952 compatible formatting of IPv6 addresses
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
 * Format an IPv6 address string using the rules from RFC5952.
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an
 *     ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
void
ipinfo_ipv6_format_ip(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    char *p  = str; 
    char  field[5];
    char *class = parsed->classification;
    int   work_pos = -1, /* The start of a block of zeroes */
          work_len = -1, /* The length of a block of zeroes */
          max_pos  = -1, /* The start of the largest block of zeroes */
          max_len  = -1, /* The length of the largest block of zeroes */
          ipv4, i, j;

    /* - 4.2.1.  Shorten as Much as Possible
     *   The use of the symbol "::" MUST be used to its maximum capability.
     *   For example, 2001:db8:0:0:0:0:2:1 must be shortened to 2001:db8::2:1.
     *   Likewise, 2001:db8::0:1 is not acceptable, because the symbol "::"
     *   could have been used to produce a shorter representation 2001:db8::1.
     *
     * - 4.2.3.  Choice in Placement of "::"
     *   When there is an alternative choice in the placement of a "::", the
     *   longest run of consecutive 16-bit 0 fields MUST be shortened (i.e.,
     *   the sequence with three consecutive zero fields is shortened in 2001:
     *   0:0:1:0:0:0:1).  When the length of the consecutive 16-bit 0 fields
     *   are equal (i.e., 2001:db8:0:0:1:0:0:1), the first sequence of zero
     *   bits MUST be shortened.  For example, 2001:db8::1:0:0:1 is correct
     *   representation.
     *
     * To handle these rules, we have to find the longest block of
     * consecutive zero fields in the address.
     */
    for (i = 0; i < IPV6_BYTELEN; i += 2)
    {
        if (parsed->addr_bytes[i] == 0 && parsed->addr_bytes[i + 1] == 0)
        {
            if (work_pos == -1) {
                work_pos = i;
                work_len = 2;
            } else {
                work_len += 2;
            }
        }
        else
        {
            work_len = -1;
            work_pos = -1;
        }

        if (work_len > max_len) {
            max_len = work_len;
            max_pos = work_pos;
        }
    }

    /* - 4.2.2.  Handling One 16-Bit 0 Field
     *   The symbol "::" MUST NOT be used to shorten just one 16-bit 0 field.
     *   For example, the representation 2001:db8:0:1:1:1:1:1 is correct, but
     *   2001:db8::1:1:1:1:1 is not correct.
     */
    if (max_len == 2) {
      max_pos = -1;
      max_len = -1;
    }

    /**
     * Check for the need of an IPv4 representation for the last 32 bits
     * of the IPv6 address. Masks for this:
     *
     * 0000:0000:0000:0000:0000:0000:xxxx:xxxx
     * 0000:0000:0000:0000:0000:ffff:xxxx:xxxx
     * 0000:0000:0000:0000:ffff:0000:xxxx:xxxx
     *
     * The address ::1 is reserved for IPv6 localhost and should not be
     * represented using the IPv4 dotted-quad format.
     */
    ipv4 = 0;
    if (max_pos == 0)
    {
        if (max_len == 12) {
            ipv4 = 1;
        } else if (max_len == 14 &&
                   (parsed->addr_bytes[14] != 0 || 
                    parsed->addr_bytes[15] != 1)) {
            ipv4 = 1;
        } else if (max_len == 10 &&
                   parsed->addr_bytes[10] == 255 && 
                   parsed->addr_bytes[11] == 255) {
            ipv4 = 1;
        } else if (max_len == 8 && 
                   parsed->addr_bytes[8]  == 255 && 
                   parsed->addr_bytes[9]  == 255 &&
                   parsed->addr_bytes[10] == 0   && 
                   parsed->addr_bytes[11] == 0) {
            ipv4 = 1;
        }
    }


    /* For IPv4 compatible formatting, we might have something like
     * 0000:xxxx for the last two fields. In that case, we don't
     * include the 0000 in the compression, so we can render the
     * IPv6 part as 0.0.xx.xx later on.
     */
    if (ipv4 && (max_pos + max_len) > 12 && (max_pos + max_len) != 16) {
      max_len = 12 - max_pos;
    }

    memset(str, 0, IPINFO_MAX_ADDRLEN);

    /* Add the address fields. */
    for (i = 0; i < IPV6_BYTELEN; i++)
    {
        /* Handle colons Inside a compressed zero block. */
        if (i == max_pos) {
            strcat(p, "::");
            p += 2;
            i += max_len - 1;
            continue;
        /* Handle regular colons. */ 
        } else if (i != max_pos + max_len) {
            if (i != 0 && i % 2 == 0) strcat(p++, ":");
        }


        /* - 5. Text Representation of Special Addresses
         *   Addresses such as IPv4-Mapped IPv6 addresses, ISATAP [RFC5214],
         *   and IPv4-translatable addresses [ADDR-FORMAT] have IPv4 addresses
         *   embedded in the low-order 32 bits of the address.  These addresses
         *   have a special representation that may mix hexadecimal and dot
         *   decimal notations.  The decimal notation may be used only for the
         *   last 32 bits of the address.
         */
        if (ipv4 && i == 12)
        {
          ipinfo tmp;
          tmp.domain       = IPINFO_IPV4;
          tmp.addr_bytes[0] = parsed->addr_bytes[12];
          tmp.addr_bytes[1] = parsed->addr_bytes[13];
          tmp.addr_bytes[2] = parsed->addr_bytes[14];
          tmp.addr_bytes[3] = parsed->addr_bytes[15];
          ipinfo_ipv4_format_ip(&tmp, p);
          break;
        }

        /* - 4.1. Handling Leading Zeros in a 16-Bit Field
         *   Leading zeros MUST be suppressed.  For example, 2001:0db8::0001 is
         *   not acceptable and must be represented as 2001:db8::1. A single
         *   16-bit 0000 field MUST be represented as 0.
         *
         * - 4.3.  Lowercase
         *   The characters "a", "b", "c", "d", "e", and "f" in an IPv6 address
         *   MUST be represented in lowercase.
         *
         * Format %x automatically takes care of lower case output.
         */
        snprintf(
            field, 5, "%x",
            parsed->addr_bytes[i] * 256 +
            parsed->addr_bytes[i + 1]
        );
        sprintf(p, "%s", field);
        p += strlen(field);
        i++;
    }
}

/**
 * Format an IPv6 network prefix.
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an
 *     ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
void
ipinfo_ipv6_format_network_prefix(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    ipinfo_ipv6_format_network_address(parsed, str);
    snprintf(&(str[strlen(str)]), 5, "/%d", parsed->prefixlen);
} /**
 * Format an IPv6 network address.
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an
 *     ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
void
ipinfo_ipv6_format_network_address(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    ipinfo network_address;
    int pos, prefixlen = parsed->prefixlen;

    for (pos = 0; pos < IPV6_BYTELEN; pos++)
    {
        if (prefixlen == 0) {
            network_address.addr_bytes[pos] = 0;
        } else if (prefixlen >= 8) {
            network_address.addr_bytes[pos] = parsed->addr_bytes[pos];
            prefixlen -= 8;
        } else {
            unsigned char mask = 255 << (8 - prefixlen);
            prefixlen = 0;
            network_address.addr_bytes[pos] = parsed->addr_bytes[pos] & mask;
        }
    }

    ipinfo_ipv6_format_ip(&network_address, str);
}

/**
 * Format an IPv6 netmask.
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an
 *     ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
void
ipinfo_ipv6_format_netmask(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    ipinfo netmask;
    int pos, prefixlen = parsed->prefixlen;

    for (pos = 0; pos < IPV6_BYTELEN; pos++)
    {
        if (prefixlen == 0) {
            netmask.addr_bytes[pos] = 0;
        } else if (prefixlen >= 8) {
            netmask.addr_bytes[pos] = 255;
            prefixlen -= 8;
        } else {
            netmask.addr_bytes[pos] = 255 << (8 - prefixlen);
            prefixlen = 0;
        }
    }

    ipinfo_ipv6_format_ip(&netmask, str);
}

/**
 * Format the canonical form of an IPv6 address.
 * The canonical form is: xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxxx
 *
 * @param ipinfo* parsed
 *     An ipinfo struct, holding the results of an
 *     ipinfo_parse() call.
 * @param char* str
 *     Storage for the formatted string. This character array must be
 *     at least IPINFO_MAX_ADDRLEN bytes long.
 */
void
ipinfo_ipv6_format_canonical(parsed, str)
    ipinfo *parsed;
    char   *str;
{
    char *p = str; 
    int i;

    memset(str, 0, IPINFO_MAX_ADDRLEN);

    /* Add the address fields. */
    for (i = 0; i < IPV6_BYTELEN; i++)
    {
        if (i != 0 && i % 2 == 0) strcat(p++, ":");
        snprintf(p, 3, "%02x", parsed->addr_bytes[i]);
        p += 2;
    }
}
