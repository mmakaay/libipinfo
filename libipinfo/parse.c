/* parse.c --- Parse an IP-address
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
#include "gettext_setup.h"
#include "tokenize.h"
#include "ipv4_netmask.h"
#include "ipv6_classify.h"

/* #define IPINFO_DEBUG */

#define IPINFO_STATE_START           0 
#define IPINFO_STATE_FIELD           1 
#define IPINFO_STATE_COMPRESSED      2 
#define IPINFO_STATE_AFTERHEX        3 
#define IPINFO_STATE_AFTERDEC        4 
#define IPINFO_STATE_IPV4BYTE        5 
#define IPINFO_STATE_IPV4DOT         6 
#define IPINFO_STATE_AFTERCOLON      7 
#define IPINFO_STATE_PREFIXSLASH     8 
#define IPINFO_STATE_IPV4NETMASK     9 
#define IPINFO_STATE_IPV4NETMASKDOT  10
#define IPINFO_STATE_IPV6PREFIXLEN   11
#define IPINFO_STATE_END             12 

#define UNEXPECTED(msg,code) \
    ({ \
        char fmt[IPINFO_MAX_ERROR_LEN]; \
        snprintf( \
            fmt, \
            IPINFO_MAX_ERROR_LEN, \
            "%s (%s: %s)", \
            _("Unexpected: %s at position %d in address '%s'"), \
            _("expected"), \
            msg \
        ); \
        snprintf( \
            parsed->error, \
            IPINFO_MAX_ERROR_LEN, \
            fmt, ipinfo_tokendescription(tokentype), tokenpos, address \
         ); \
        parsed->status = code; \
        return 0; \
    })

/**
 * Retrieve the description for a given domain.
 *
 * @param int domain
 *   The id of the domain for which to lookup the description.
 *   This is either 4 (for IPv4) or 6 (for IPv6).
 * @return char* description
 *   The description for the domain or NULL if the domain id
 *   was invalid.
 */
const char *
ipinfo_domaindescription(domain)
  int domain;
{
    switch (domain) {
    case 4  : return _("IPv4"); break;
    case 6  : return _("IPv6"); break;
    default : return NULL;
    }
}

/**
 * Parse an IP address.
 * 
 * This will parse a textual IPv4 or IPv6 address.
 * The results are stored in the provided ipinfo struct.
 * 
 * When parsing fails, then the error field in the struct is updated
 * with an error message describing the parsing issue, the status field
 * gets a status code that is unique for the issue and 0 is returned
 * by the function.
 * 
 * @param char* address
 *   The IP address string to parse.
 * @param ipinfo* parsed
 *   Parsing results are stored in this struct.
 * @return integer
 *   1 is returned when parsing was successful, 0 otherwise.
 */
int
ipinfo_parse(address, parsed)
    char   *address;
    ipinfo *parsed;
{
    char  token[IPINFO_MAX_TOKEN_LEN + 1];
    char *tokenptr      =  address;
    int   tokentype;
    int   tokenpos      =  0;
    int   prevtokenlen  = -1;
    int   state         =  IPINFO_STATE_START;
    int   pos           = -1;
    int   compress_pos  = -1;
    int   ipv4_pos;
    int   ipv4_byte;
    int   prefixlen;
    int   i;

    /* Initialize the result data */
    memset(parsed, 0, sizeof(*parsed));
    parsed->prefixlen = -1;

    /* A quick check to see if the input string isn't too large. */
    if (strlen(address) > (IPINFO_MAX_ADDRLEN - 1)) {
        snprintf(
            parsed->error, IPINFO_MAX_ERROR_LEN,
            _("The IP address is too long (at most %d bytes are expected)"),
            IPINFO_MAX_ADDRLEN - 1, strlen(address));
        parsed->status = 1;
        return 0;
    }

    for (;;)
    {
        /* Keep track of the length of the previous token.
         * This is used for rolling back the last token in case
         * a dot is found, in which case IPv4 address parsing is
         * started at the previous token.
         */
        if (prevtokenlen == -1) {
          prevtokenlen = 0;
        } else {
          prevtokenlen = strlen(token);
        }

        /* Keep track of the start position of the token that we are at. */
        tokenpos = tokenptr - address + 1;

        /* Fetch the next token. */
#ifdef IPINFO_DEBUG
        printf("ipinfo_parse(): enter with state = %d\n", state);
        printf("ipinfo_parse(): parse token from '%s'\n", tokenptr);
#endif
        tokentype = ipinfo_tokenize(tokenptr, (char *)&token, &tokenptr);
#ifdef IPINFO_DEBUG
        printf(
            "ipinfo_parse(): parsed %s token '%s'\n",
            ipinfo_tokendescription(tokentype), token
        );
#endif

        /* Check for illegal characters in the input. */
        if (tokentype == IPINFO_TOKEN_ILLEGAL) {
            snprintf(
                parsed->error, IPINFO_MAX_ERROR_LEN,
                _("Illegal character '%c' at position %d in address '%s'"),
                *token, tokenpos, address);
            parsed->status = 2;
            return 0;
        }

        /* START: expect the start of an IP address: either a
         * colon or a field.
         */
        if (state == IPINFO_STATE_START)
        {
            /* A colon is found. This must be the start of a compressed
             * group of zeroes.
             */
            if (tokentype == IPINFO_TOKEN_COLON) {
                state = IPINFO_STATE_COMPRESSED;
                continue;
            }

            /* At this point, we expect an IP field. */
            if (tokentype != IPINFO_TOKEN_HEX &&
                tokentype != IPINFO_TOKEN_DEC) {
                UNEXPECTED(_("address field or colon"),3);
            }

            /* The IP field will be handled by the FIELD state */
            state = IPINFO_STATE_FIELD;
        }

        /* AFTERCOLON: expect either another colon for indicating a
         * compressed group of zeroes or an IPv6 field.
         */
        if (state == IPINFO_STATE_AFTERCOLON)
        {
          if (tokentype == IPINFO_TOKEN_COLON) {
            state = IPINFO_STATE_COMPRESSED;
          } else {
            state = IPINFO_STATE_FIELD;
          }
        }

        /* COMPRESSED: expect second colon of a compressed group of zeroes */
        if (state == IPINFO_STATE_COMPRESSED)
        {
            if (tokentype == IPINFO_TOKEN_COLON)
            {
                /* Only one compressed zero block can exist. */
                if (compress_pos != -1) {
                    snprintf(
                        parsed->error, IPINFO_MAX_ERROR_LEN,
                        _("Duplicate '::' block at position %d "
                          "in address '%s'"),
                        tokenpos, address);
                  parsed->status = 4;
                  return 0;
                }

                compress_pos = pos == -1 ? 0 : pos + 1;
                state = IPINFO_STATE_FIELD;
                continue;
            } else {
                UNEXPECTED(_("colon"),5);
            }
        }

        /* FIELD: expect an IP address field. */
        if (state == IPINFO_STATE_FIELD)
        {
            unsigned int field;
            int byte;

            /* End of string is only valid after a zero compression
             * block "::".
             */ 
            if (tokentype == IPINFO_TOKEN_END && pos + 1 == compress_pos) {
                break;
            }

            if (tokentype == IPINFO_TOKEN_HEX) {
                state = IPINFO_STATE_AFTERHEX;
            } else if (tokentype == IPINFO_TOKEN_DEC) {
                state = IPINFO_STATE_AFTERDEC;
            } else if (tokentype == IPINFO_TOKEN_SLASH) {
                state = IPINFO_STATE_IPV6PREFIXLEN;
                continue;
            } else {
                UNEXPECTED(_("address field"),6);
            }

            /* Check for too many address bytes. */
            if (pos + 3 > IPV6_BYTELEN) {
                pos += 2;
                break;
            }

            /* Add the IPv6 field bytes. */
            sscanf(token, "%x", &field);
            parsed->addr_bytes[++pos] = (field & 0xff00) >> 8;
            parsed->addr_bytes[++pos] =  field & 0x00ff;
            continue;
        }

        /* AFTERHEX: the previous token was a hexadecimal number. Expect a
         * colon, which starts the next IPv6 field.
         */
        if (state == IPINFO_STATE_AFTERHEX)
        {
            if (tokentype == IPINFO_TOKEN_END) {
                break;
            } else if (tokentype == IPINFO_TOKEN_COLON) {
                state = IPINFO_STATE_AFTERCOLON;
                continue;
            } else if (tokentype == IPINFO_TOKEN_SLASH) {
                state = IPINFO_STATE_IPV6PREFIXLEN;
                continue;
            } else {
                UNEXPECTED(_("colon or slash"),7);
            }
        }

        /* AFTERDEC: the previous token was a decimal number. Expect a
         * colon, which starts the next IPv6 field, or a dot, which
         * indicates the start of an IPv4 style address.
         */
        if (state == IPINFO_STATE_AFTERDEC)
        {
            if (tokentype == IPINFO_TOKEN_END) {
                break;
            } else if (tokentype == IPINFO_TOKEN_COLON) {
                state = IPINFO_STATE_AFTERCOLON;
                continue;
            } else if (tokentype == IPINFO_TOKEN_SLASH) {
                state = IPINFO_STATE_IPV6PREFIXLEN;
                continue;
            } else if (tokentype == IPINFO_TOKEN_DOT) {
                /* Rollback bytes that we assumed to be an IPv6 hex field */
                parsed->addr_bytes[pos] = parsed->addr_bytes[pos - 1] = 0;
                pos -= 2;
                tokenptr -= (prevtokenlen + 1);

                /* Continue with IPv4 address parsing. */
                state = IPINFO_STATE_IPV4BYTE;
                ipv4_pos = 0;
                continue;
            }
            else {
                UNEXPECTED(_("colon, slash or dot"),8);
            }
        }

        /* IPV4BYTE: we're parsing an IPv4 style address representation
         * and are expecting the next byte for it (0-255). */
        if (state == IPINFO_STATE_IPV4BYTE)
        {
            if (tokentype == IPINFO_TOKEN_DEC)
            {
                ipv4_byte = atoi(token);

                if (strlen(token) > 1 && token[0] == '0') {
                  snprintf(
                      parsed->error, IPINFO_MAX_ERROR_LEN,
                      _("Illegal IPv4 byte '%s' at position %d "
                        "in address '%s' (no leading zeroes are allowed)"),
                      token, tokenpos, address);
                  parsed->status = 9;
                  return 0;
                }

                if (ipv4_byte < 0 || ipv4_byte > 255) {
                  snprintf(
                      parsed->error, IPINFO_MAX_ERROR_LEN,
                      _("Illegal IPv4 byte value '%d' at position %d "
                        "in address '%s' (expected: 0 - 255)"),
                      ipv4_byte, tokenpos, address);
                  parsed->status = 10;
                  return 0;
                }

                /* Check for too many address bytes. */
                if (pos + 2 > IPV6_BYTELEN) {
                    pos++;
                    break;
                }

                /* Add the IPv4 byte. */
                ipv4_pos ++;
                parsed->addr_bytes[++pos] = ipv4_byte;

                /* After 4 bytes, our IPv4 address is complete. */
                if (ipv4_pos == IPV4_BYTELEN)
                {
                    /* When we have seen 4 bytes and if we only have four
                     * bytes in the parsed address so far, then this must be
                     * an IPv4 address.
                     */
                    if (compress_pos == -1 && pos + 1 == IPV4_BYTELEN) {
                      parsed->domain = IPINFO_IPV4;
                    }

                    state = IPINFO_STATE_PREFIXSLASH;
                }
                else {
                    state = IPINFO_STATE_IPV4DOT;
                }
                continue;
            }
            else {
                UNEXPECTED(_("IPv4 address byte 0 - 255"),11);
            }
        }

        /* IPV4DOT: we're parsing an IPv4 style address representation
         * and are expecting the next dot for it. */
        if (state == IPINFO_STATE_IPV4DOT)
        {
            if (tokentype == IPINFO_TOKEN_DOT) {
                state = IPINFO_STATE_IPV4BYTE;
                continue;
            }
            else {
                UNEXPECTED(_("dot"),12);
            }
        }

        /* PREFIXSLASH: expect a slash that starts an IPv6 prefix length
         * or an IPv4 subnet mask. */
        if (state == IPINFO_STATE_PREFIXSLASH)
        {
            if (tokentype == IPINFO_TOKEN_END) {
                break;
            } else if (tokentype == IPINFO_TOKEN_SLASH) {
                if (parsed->domain == IPINFO_IPV4) {
                    ipv4_pos = 0;
                    state = IPINFO_STATE_IPV4NETMASK;
                } else {
                    state = IPINFO_STATE_IPV6PREFIXLEN;
                }
                continue;
            } else {
                UNEXPECTED(_("prefix length slash"),13);
            }
        }

        /* PREFIXSLEN: expect an IPv6 prefix length (0 - 128) */
        if (state == IPINFO_STATE_IPV6PREFIXLEN)
        {
            if (tokentype == IPINFO_TOKEN_DEC)
            {
                prefixlen = atoi(token);
                if (prefixlen < 0 || prefixlen > 128) {
                  snprintf(
                      parsed->error, IPINFO_MAX_ERROR_LEN,
                      _("Illegal prefix length '%d' used at position %d "
                        "in address '%s' (expected: IPv6 prefix "
                        "length 0 - 128)"),
                      prefixlen, tokenpos, address);
                  parsed->status = 14;
                  return 0;
                }
                parsed->prefixlen = prefixlen;
                state = IPINFO_STATE_END;
                continue;
            }
            else {
                UNEXPECTED(_("IPv6 prefix length 0 - 128"),15);
            }
        }

        /* SUBNETMASK: expect an IPv4 subnet mask. */
        if (state == IPINFO_STATE_IPV4NETMASK)
        {
            if (tokentype == IPINFO_TOKEN_DEC)
            {
                ipv4_byte = atoi(token);

                if (strlen(token) > 1 && token[0] == '0') {
                  snprintf(
                      parsed->error, IPINFO_MAX_ERROR_LEN,
                      _("Illegal IPv4 netmask byte '%s' at position %d "
                        "in address '%s' (no leading zeroes are allowed)"),
                      token, tokenpos, address);
                  parsed->status = 16;
                  return 0;
                }

                if (ipv4_byte < 0 || ipv4_byte > 255)
                {
                  snprintf(
                      parsed->error, IPINFO_MAX_ERROR_LEN,
                      _("Illegal IPv4 netmask byte value '%d' at position %d "
                        "in address '%s' (expected: 0 - 255)"),
                      ipv4_byte, tokenpos, address);
                  parsed->status = 17;
                  return 0;
                }

                ipv4_pos++;
                parsed->addr_bytes[++pos] = ipv4_byte;

                /* We now have a full netmask in dot-decimal notation. */
                if (ipv4_pos == 4)
                {
                    /* Convert the netmask to a prefix length. */
                    int prefixlen = ipinfo_ipv4_netmask2prefixlen(
                        &(parsed->addr_bytes[pos - 3]),
                        parsed->error, IPINFO_MAX_ERROR_LEN
                    );
                    if (prefixlen == -1) {
                      parsed->status = 18;
                      return 0;
                    } else {
                      parsed->prefixlen = prefixlen;
                    }

                    state = IPINFO_STATE_END;
                }
                else {
                    state = IPINFO_STATE_IPV4NETMASKDOT;
                }
                continue;
            }
            else if (ipv4_pos == 0) {
                UNEXPECTED(_("IPv4 prefix 0 - 32 or a dot-decimal netmask"),19);
            } else {
                UNEXPECTED(_("IPv4 netmask byte 0-255"),20);
            }
        }

        /* SUBNETMASKDOT: expect an IPv4 subnet mask byte separation dot. */
        if (state == IPINFO_STATE_IPV4NETMASKDOT)
        {
            if (tokentype == IPINFO_TOKEN_DOT)
            {
                state = IPINFO_STATE_IPV4NETMASK; 
                continue;
            }
            /* The netmask might be in CIDR notation, in which case
             * there is one decimal 0 - 32 after the slash.
             */
            else if (tokentype == IPINFO_TOKEN_END && ipv4_pos == 1)
            {
                if (ipv4_byte < 0 || ipv4_byte > 32) {
                    snprintf(
                        parsed->error, IPINFO_MAX_ERROR_LEN,
                        _("Invalid IPv4 prefix length %d in address '%s' "
                          "(expected: IPv4 prefix length 0 - 32)"),
                        ipv4_byte, address);
                    parsed->status = 21;
                    return 0;
                }

                parsed->prefixlen = ipv4_byte;
                state = IPINFO_STATE_END;
            }
            else {
                UNEXPECTED("dot",22);
            }
        }

        /* END: we've seen the end of the IP address, no more tokens
         * are expected.
         */
        if (state == IPINFO_STATE_END)
        {
            if (tokentype == IPINFO_TOKEN_END) {
                break;
            } else {
                UNEXPECTED(_("end of address"),23);
            }
        }
    }

    /* All tokenizing and analyzing is done.
     * The parser might already have decided that the parsed address
     * was an IPv4 address. Since the parser does check the byte values
     * and the length of the address already, we don't have to do
     * extensive checking on the address anymore at this point.
     */
    if (parsed->domain == IPINFO_IPV4)
    {
        // NOOP
    }
    /* If we have encountered compressed IPv6 zeroes, then we inflate
     * the IPv6 address up to its full byte size here.
     */
    else if (compress_pos != -1)
    {
#ifdef IPINFO_DEBUG
        printf("ipinfo_parse(): handle compressed zero decompression\n");
        printf("ipinfo_parse(): parse position = %d\n", pos);
        printf("ipinfo_parse(): compression position = %d\n", compress_pos);
#endif
        /* Compression is in use, but there is no space for decompression
         * in the parsed byte array.
         */
        if (pos + 1 >= IPV6_BYTELEN) {
            snprintf(
                parsed->error, IPINFO_MAX_ERROR_LEN,
                _("Address '%s' contains a compressed zeroes block '::', "
                  "but the address already contains %d bytes or more"),
                address, IPV6_BYTELEN);
            parsed->status = 24;
            return 0;
        }

        /* If compression is at the end of the address. We already have
         * zeroes setup for those fields, so no extra work is needed.
         */
        pos ++;
        if (pos != compress_pos)
        {
            int i;
            int compress_len = IPV6_BYTELEN - pos;
            int move_len     = pos - compress_pos;

            /* Move bytes after the compression position to the end and
             * fill the old byte positions with zeroes. */
            for (i = 0; i < move_len; i++) {
                int src_pos            = compress_pos + move_len - i - 1;
                int dst_pos            = IPV6_BYTELEN - i - 1;
                parsed->addr_bytes[dst_pos] = parsed->addr_bytes[src_pos];
                parsed->addr_bytes[src_pos] = 0;
            }
        }

        parsed->domain = IPINFO_IPV6;
    }
    /* When there are no compressed zeroes, then the address should be
     * at the required length already.
     */
    else
    {
#ifdef IPINFO_DEBUG
        printf("ipinfo_parse(): no compressed zero decompression needed\n");
#endif
        /* Check if there are enough bytes. */
        if (pos + 1 < IPV6_BYTELEN) {
            snprintf(
                parsed->error, IPINFO_MAX_ERROR_LEN,
                _("Address '%s' contains only %d bytes "
                  "(%d bytes are expected)"),
                address, pos + 1, IPV6_BYTELEN);
            parsed->status = 25;
            return 0;
        }

        /* Check if the number of bytes does not exceed the maximum. */
        if (pos + 1 > IPV6_BYTELEN) {
            snprintf(
                parsed->error, IPINFO_MAX_ERROR_LEN,
                _("Address '%s' exceeds the maximum IPv6 byte length "
                  "(%d bytes are expected)"),
                address, IPV6_BYTELEN, (pos + 1));
            parsed->status = 26;
            return 0;
        }

        parsed->domain = IPINFO_IPV6;
    }

    return 1;
}

