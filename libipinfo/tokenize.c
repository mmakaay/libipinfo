/* tokenize.c --- Parse a single IP-address token from a string
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
#include "tokenize.h"

/**
 * Retrieve the description for a given tokentype.
 * 
 * @param int tokentype
 *   The id of the tokentype for which to lookup the description.
 * @return char* description
 *   The description for the tokentype or NULL if the tokentype
 *   was invalid.
 */
const char *
ipinfo_tokendescription(tokentype)
  int tokentype;
{
    switch (tokentype) {
    case 1  : return _("end of address");     break;
    case 2  : return _("colon");              break;
    case 3  : return _("dot");                break;
    case 4  : return _("slash");              break;
    case 5  : return _("hex address field;"); break;
    case 6  : return _("address field");      break;
    case 7  : return _("illegal character");  break;
    default : return NULL;
    }
}

/**
 * Parse a single token from a string.
 *
 * @param char* parse_string
 *     A pointer to the string to parse the token from.
 * @param char* token
 *     Storage for the token that was found. This character array must be
 *     at least IPINFO_MAX_TOKEN_LEN + 1 bytes long.
 * @param char** next_token
 *     This pointer will be filled with a pointer to the start of
 *     the next token.
 * @return int tokentype
 *   The token type id, corresponding with the IPINFO_TOKEN_* defines.
 */
int
ipinfo_tokenize(parse_string, token, next_token)
    char  *parse_string;
    char  *token;
    char **next_token;
{
    int tokentype;

    /* Initialize the token */
    memset(token, 0, IPINFO_MAX_TOKEN_LEN + 1);

    /* Handle number and hex tokens. */
    if ((*parse_string >= '0' && *parse_string <= '9') ||
        (*parse_string >= 'a' && *parse_string <= 'f') ||
        (*parse_string >= 'A' && *parse_string <= 'F'))
    {
        int pos    = 0;
        int is_hex = 0;
        int i;

        while ((*parse_string >= '0' && *parse_string <= '9') ||
               (*parse_string >= 'a' && *parse_string <= 'f') ||
               (*parse_string >= 'A' && *parse_string <= 'F'))
        {
            /* To be able to differentiate between numbers and hex. */
            if (!(*parse_string >= '0' && *parse_string <= '9')) is_hex = 1;

            /* Lower case, since IPv6 addresses are case insensitive. */
            token[pos++] = tolower(*parse_string++);

            /* The maximum length for a number or a hex string is 4. */
            if (pos == 4) break;
        }

        if (is_hex) {
            tokentype = IPINFO_TOKEN_HEX;
        } else {
            tokentype = IPINFO_TOKEN_DEC;
        }
    }
    /* Handle other token types. */
    else
    {
        if (*parse_string == '\0') {
            tokentype = IPINFO_TOKEN_END;
        } else if (*parse_string == ':') {
            tokentype = IPINFO_TOKEN_COLON;
        } else if (*parse_string == '.') {
            tokentype = IPINFO_TOKEN_DOT;
        } else if (*parse_string == '/') {
            tokentype = IPINFO_TOKEN_SLASH;
        } else {
            tokentype = IPINFO_TOKEN_ILLEGAL;
        }

        token[0] = *parse_string;
        parse_string ++;
    }

    *next_token = parse_string;

    return tokentype;
}
