/* ipinfo.c --- a command line utility for displaying IP info
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

int
main(argc, argv)
    int   argc;
    char *argv[];
{
    ipinfo parsed;
    char   buf[IPINFO_MAX_ADDRLEN];
    int    i;

    /* Initialize gettext */
    setlocale(LC_MESSAGES, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    if (argc != 2) {
      printf("Usage: ipinfo <ip address>\n", argv[0]);
      return 1;
    }

    if (ipinfo_parse(argv[1], &parsed))
    {
        ipinfo_format(&parsed, IPINFO_FORMAT_DOMAIN, (char *)&buf);
        printf(_("Domain             = %s\n"), buf);

        ipinfo_format(&parsed, IPINFO_FORMAT_IP, (char *)&buf);
        printf(_("IP Address         = %s\n"), buf);

        if (parsed.prefixlen != -1)
        {
            ipinfo_format(&parsed, IPINFO_FORMAT_PREFIX, (char *)&buf);
            printf(_("Network prefix     = %s\n"), buf);

            ipinfo_format(&parsed, IPINFO_FORMAT_NETWORK, (char *)&buf);
            printf(_("Network address    = %s\n"), buf);

            ipinfo_format(&parsed, IPINFO_FORMAT_NETMASK, (char *)&buf);
            printf(_("Netmask            = %s\n"), buf);

            if (parsed.domain == IPINFO_IPV4)
            {
                ipinfo_format(&parsed, IPINFO_FORMAT_BROADCAST, (char *)&buf);
                printf(_("Broadcast address  = %s\n"), buf);

                ipinfo_format(&parsed, IPINFO_FORMAT_HOSTCOUNT, (char *)&buf);
                printf(_("Number of hosts    = %s\n"), buf);
            }
        }
    }
    else
    {
        printf(_("Error: %s (%d)\n"), parsed.error, parsed.status);
        return(parsed.status);
    }
}

