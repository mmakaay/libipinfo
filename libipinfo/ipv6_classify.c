/* ipv6_classify.c --- Classify IPv6 addresses
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

/**
 * A list of IPv6 prefixes and a description of the class of address that
 * is specified using that prefix.
 */
const char *ipinfo_ipv6_class_prefixes[][2] = {
  {"0000:0000:0000:0000:0000:0000:0000:0000", "Unspecified address"          },
  {"0000:0000:0000:0000:0000:0000:0000:0001", "Loopback address"             },

  {"0000:0000:0000:0000:0000:0000",           "IPv4-compatible (RFC2373)"    },
  {"0000:0000:0000:0000:0000:ffff",           "IPv4-mapped (RFC2373)"        },
  {"0000:0000:0000:0000:ffff:0000",           "IPv4-translated (RFC2765)"    },

  {"fe80",                                    "Link-Local Unicast"           },
  {"fec0",                                    "Site-local Address"           },
  {"ff01:0000:0000:0000:0000:0000:0000:0001", "All Interface-Local Nodes"    },
  {"ff01:0000:0000:0000:0000:0000:0000:0002", "All Interface-Local Routers"  },
  {"ff01",                                    "Interface-Local Multicast"    },
  {"ff02:0000:0000:0000:0000:0000:0000:0001", "All Link-Local Nodes"         },
  {"ff02:0000:0000:0000:0000:0000:0000:0002", "All Link-Local Routers"       },
  {"ff02",                                    "Link-Local Multicast"         },
  {"ff02:0000:0000:0000:0000:0001:ff",        "Solicited-Node Address"       },
  {"ff04",                                    "Admin-Local Multicast"        },
  {"ff05:0000:0000:0000:0000:0000:0000:0002", "All Site-Local Routers"       },
  {"ff05",                                    "Site-Local Multicast"         },
  {"ff08",                                    "Organization-Local Multicast" },
  {"ff0e",                                    "Global Scope Multicast"       },
  {"ff0",                                     "Multicast"                    },
  {"",                                        "Global Unicast"               },
  {NULL}
};

char *
ipinfo_ipv6_classify(parsed)
    ipinfo *parsed;
{
    int  i;
    char canonical[IPINFO_MAX_ADDRLEN];

    ipinfo_ipv6_format_canonical(parsed, (char *)&canonical);

    for (i = 0; ipinfo_ipv6_class_prefixes[i][0] != NULL; i++)
    {
      char *prefix          = (char *)ipinfo_ipv6_class_prefixes[i][0];
      char *classification  = (char *)ipinfo_ipv6_class_prefixes[i][1];

      if (strncmp(canonical, prefix, strlen(prefix)) == 0) {
        return classification;
      }
    }

    return "Unknown Class";
}

