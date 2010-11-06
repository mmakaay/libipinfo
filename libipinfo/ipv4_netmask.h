/* ipv4_netmask.h --- header file for the IPv4 netmask handling
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

#ifndef _IPV4_NETMASK2PREFIXLEN_H
#define _IPV4_NETMASK2PREFIXLEN_H

int ipinfo_ipv4_netmask2prefixlen(unsigned char *, char *, size_t);
void ipinfo_ipv4_prefixlen2netmask(int, unsigned char *);

#endif /* netmask.h */
