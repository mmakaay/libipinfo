/* tokenize.h --- header file for the ipinfo_tokenize() function
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

#ifndef _IPINFO_TOKENIZE_H
#define _IPINFO_TOKENIZE_H

#include "gettext_setup.h"

#define IPINFO_MAX_TOKEN_LEN         4

#define IPINFO_TOKEN_END             1
#define IPINFO_TOKEN_COLON           2
#define IPINFO_TOKEN_DOT             3
#define IPINFO_TOKEN_SLASH           4
#define IPINFO_TOKEN_HEX             5
#define IPINFO_TOKEN_DEC             6
#define IPINFO_TOKEN_ILLEGAL         7

int ipinfo_tokenize(char *, char *, char **);
const char *ipinfo_tokendescription(int);

#endif /* tokenize.h */
