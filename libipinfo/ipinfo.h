/* ipinfo.h --- header file for the libipinfo library
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

#ifndef _IPINFO_H
#define _IPINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Communication domains.
 */
#define IPINFO_IPV4                4
#define IPINFO_IPV6                6

/* The maximum possible string length for representing an IP address.
 * This number is based on an IPv6 address plus a prefix length:
 * "xxxx:" * 6 + "123.123.123.123" + "/128" + \0
 */
#define IPINFO_MAX_ADDRLEN         50

/**
 * The number of bytes in an IPv4 address.
 */ 
#define IPV4_BYTELEN               4

/**
 * The number of bytes in an IPv6 address.
 */ 
#define IPV6_BYTELEN               16

/**
 * The maximum length for an ipinfo parsing error.
 */
#define IPINFO_MAX_ERROR_LEN       255

/**
 * Defines used by ipinfo_format().
 */
#define IPINFO_FORMAT_IP           1
#define IPINFO_FORMAT_PREFIX       2
#define IPINFO_FORMAT_NETMASK      3
#define IPINFO_FORMAT_NETWORK      4
#define IPINFO_FORMAT_BROADCAST    5
#define IPINFO_FORMAT_HOSTCOUNT    6
#define IPINFO_FORMAT_DOMAIN       7

/**
 * This struct is used by ipinfo_parse() to store the parsing result.
 */
typedef struct
{
    unsigned char addr_bytes[IPV6_BYTELEN]; /* Storage for address bytes.    */

    unsigned char mask_bytes[IPV6_BYTELEN]; /* Storage for netmask bytes.    */

    int prefixlen;                          /* The prefix length or -1 if no */
                                            /* prefix length or netmask was  */
                                            /* used in the address.          */

    int domain;                             /* Communication domain, one of: */
                                            /* - 0 = no parsing done         */
                                            /* - 4 = IPv4                    */
                                            /* - 6 = IPv6                    */

    char *classification;                   /* Classification of the address.*/

    int status;                             /* The parsing status            */
                                            /*  0 = parsing successful       */
                                            /* >0 = parsing failed           */

    char error[IPINFO_MAX_ERROR_LEN];       /* Storage for parsing errors.   */
}
ipinfo;

/**
 * Library interface functions.
 */
int   ipinfo_parse(char *, ipinfo *);
int   ipinfo_format(ipinfo *, int, char *);
const char *ipinfo_domaindescription(int);

#endif /* ipinfo.h */
