/* ipv4_format.h --- header file for the IPv4 formatting functions
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

#ifndef _IPV4_FORMAT_H
#define _IPV4_FORMAT_H

void ipinfo_ipv4_format_ip(ipinfo *, char *);
void ipinfo_ipv4_format_network_prefix(ipinfo *, char *);
void ipinfo_ipv4_format_netmask(ipinfo *, char *);
void ipinfo_ipv4_format_network_address(ipinfo *, char *);
void ipinfo_ipv4_format_broadcast_address(ipinfo *, char *);
void ipinfo_ipv4_format_hostcount(ipinfo *, char *);

#endif /* ipv4_format.h */
