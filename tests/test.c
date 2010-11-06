/* test.c --- Tests for libipinfo
 * Copyright (c) 2010
 * Maurice Makaay
 *
 * This file is part of the IPinfo package.
 *
 * -----------------------------------------------------------------------
 *
 * This file contains a lot of IPv6 address tests that were
 * created by Stephen Ryan from Dartware and others for testing a
 * regular expression that was written for checking IPv6 addresses
 * (http://forums.dartware.com/viewtopic.php?t=452).
 *
 * Many thanks to all those involved in creating this test data.
 * It has proven very useful for testing the ipinfo library.
 *
 * -----------------------------------------------------------------------
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the MIT license. You should have received a copy
 * of the license along with this program. If not, see the website
 * <http://www.opensource.org/licenses/mit-license.php>
 */

#include "ipinfo.h"
#include "config.h"

void
ipinfo_test(expected, address)
    int   expected;
    char *address;
{
    ipinfo parsed;
    int res;

    res = ipinfo_parse(address, &parsed);
    if (res != expected) {
        printf("fail: %s\n", address);
    }
}

int
main()
{
    ipinfo_test(!1,"");
    ipinfo_test(1,"2001:0000:1234:0000:0000:C1C0:ABCD:0876");
    ipinfo_test(1,"3ffe:0b00:0000:0000:0001:0000:0000:000a");
    ipinfo_test(1,"FF02:0000:0000:0000:0000:0000:0000:0001");
    ipinfo_test(1,"0000:0000:0000:0000:0000:0000:0000:0001");
    ipinfo_test(1,"0000:0000:0000:0000:0000:0000:0000:0000");
    ipinfo_test(1,"::ffff:192.168.1.26");
    /* extra 0 not allowed! */ 
    ipinfo_test(!1,"02001:0000:1234:0000:0000:C1C0:ABCD:0876");
    /* extra 0 not allowed! */
    ipinfo_test(!1,"2001:0000:1234:0000:00001:C1C0:ABCD:0876");
    /* leading space */
    ipinfo_test(!1," 2001:0000:1234:0000:0000:C1C0:ABCD:0876");
    /* trailing space */
    ipinfo_test(!1,"2001:0000:1234:0000:0000:C1C0:ABCD:0876 ");
    /* leading and trailing space */
    ipinfo_test(!1," 2001:0000:1234:0000:0000:C1C0:ABCD:0876  ");
    /* junk after valid address */
    ipinfo_test(!1," 2001:0000:1234:0000:0000:C1C0:ABCD:0876  0");
    /* internal space */
    ipinfo_test(!1,"2001:0000:1234: 0000:0000:C1C0:ABCD:0876");
    /* garbage instead of "." in IPv4 */
    ipinfo_test(!1,"2001:1:1:1:1:1:255Z255X255Y255");
    ipinfo_test(!1,"::ffff:192x168.1.26");
    /* seven segments */
    ipinfo_test(!1,"3ffe:0b00:0000:0001:0000:0000:000a");
    /* nine segments */
    ipinfo_test(!1,"FF02:0000:0000:0000:0000:0000:0000:0000:0001");
    /* double "::" */
    ipinfo_test(!1,"3ffe:b00::1::a");
    /* double "::" */
    ipinfo_test(!1,"::1111:2222:3333:4444:5555:6666::");
    ipinfo_test(1,"2::10");
    ipinfo_test(1,"ff02::1");
    ipinfo_test(1,"fe80::");
    ipinfo_test(1,"2002::");
    ipinfo_test(1,"2001:db8::");
    ipinfo_test(1,"2001:0db8:1234::");
    ipinfo_test(1,"::ffff:0:0");
    ipinfo_test(1,"::1");
    ipinfo_test(1,"::ffff:192.168.1.1");
    ipinfo_test(1,"1:2:3:4:5:6:7:8");
    ipinfo_test(1,"1:2:3:4:5:6::8");
    ipinfo_test(1,"1:2:3:4:5::8");
    ipinfo_test(1,"1:2:3:4::8");
    ipinfo_test(1,"1:2:3::8");
    ipinfo_test(1,"1:2::8");
    ipinfo_test(1,"1::8");
    ipinfo_test(1,"1::2:3:4:5:6:7");
    ipinfo_test(1,"1::2:3:4:5:6");
    ipinfo_test(1,"1::2:3:4:5");
    ipinfo_test(1,"1::2:3:4");
    ipinfo_test(1,"1::2:3");
    ipinfo_test(1,"1::8");
    ipinfo_test(1,"::2:3:4:5:6:7:8");
    ipinfo_test(1,"::2:3:4:5:6:7");
    ipinfo_test(1,"::2:3:4:5:6");
    ipinfo_test(1,"::2:3:4:5");
    ipinfo_test(1,"::2:3:4");
    ipinfo_test(1,"::2:3");
    ipinfo_test(1,"::8");
    ipinfo_test(1,"1:2:3:4:5:6::");
    ipinfo_test(1,"1:2:3:4:5::");
    ipinfo_test(1,"1:2:3:4::");
    ipinfo_test(1,"1:2:3::");
    ipinfo_test(1,"1:2::");
    ipinfo_test(1,"1::");
    ipinfo_test(1,"1:2:3:4:5::7:8");
    /* double "::" */
    ipinfo_test(!1,"1:2:3::4:5::7:8");
    ipinfo_test(!1,"12345::6:7:8");
    ipinfo_test(1,"1:2:3:4::7:8");
    ipinfo_test(1,"1:2:3::7:8");
    ipinfo_test(1,"1:2::7:8");
    ipinfo_test(1,"1::7:8");
    ipinfo_test(1,"1:2:3:4:5:6:1.2.3.4");
    ipinfo_test(1,"1:2:3:4:5::1.2.3.4");
    ipinfo_test(1,"1:2:3:4::1.2.3.4");
    ipinfo_test(1,"1:2:3::1.2.3.4");
    ipinfo_test(1,"1:2::1.2.3.4");
    ipinfo_test(1,"1::1.2.3.4");
    ipinfo_test(1,"1:2:3:4::5:1.2.3.4");
    ipinfo_test(1,"1:2:3::5:1.2.3.4");
    ipinfo_test(1,"1:2::5:1.2.3.4");
    ipinfo_test(1,"1::5:1.2.3.4");
    ipinfo_test(1,"1::5:11.22.33.44");
    ipinfo_test(!1,"1::5:400.2.3.4");
    ipinfo_test(!1,"1::5:260.2.3.4");
    ipinfo_test(!1,"1::5:256.2.3.4");
    ipinfo_test(!1,"1::5:1.256.3.4");
    ipinfo_test(!1,"1::5:1.2.256.4");
    ipinfo_test(!1,"1::5:1.2.3.256");
    ipinfo_test(!1,"1::5:300.2.3.4");
    ipinfo_test(!1,"1::5:1.300.3.4");
    ipinfo_test(!1,"1::5:1.2.300.4");
    ipinfo_test(!1,"1::5:1.2.3.300");
    ipinfo_test(!1,"1::5:900.2.3.4");
    ipinfo_test(!1,"1::5:1.900.3.4");
    ipinfo_test(!1,"1::5:1.2.900.4");
    ipinfo_test(!1,"1::5:1.2.3.900");
    ipinfo_test(!1,"1::5:300.300.300.300");
    ipinfo_test(!1,"1::5:3000.30.30.30");
    ipinfo_test(!1,"1::400.2.3.4");
    ipinfo_test(!1,"1::260.2.3.4");
    ipinfo_test(!1,"1::256.2.3.4");
    ipinfo_test(!1,"1::1.256.3.4");
    ipinfo_test(!1,"1::1.2.256.4");
    ipinfo_test(!1,"1::1.2.3.256");
    ipinfo_test(!1,"1::300.2.3.4");
    ipinfo_test(!1,"1::1.300.3.4");
    ipinfo_test(!1,"1::1.2.300.4");
    ipinfo_test(!1,"1::1.2.3.300");
    ipinfo_test(!1,"1::900.2.3.4");
    ipinfo_test(!1,"1::1.900.3.4");
    ipinfo_test(!1,"1::1.2.900.4");
    ipinfo_test(!1,"1::1.2.3.900");
    ipinfo_test(!1,"1::300.300.300.300");
    ipinfo_test(!1,"1::3000.30.30.30");
    ipinfo_test(!1,"::400.2.3.4");
    ipinfo_test(!1,"::260.2.3.4");
    ipinfo_test(!1,"::256.2.3.4");
    ipinfo_test(!1,"::1.256.3.4");
    ipinfo_test(!1,"::1.2.256.4");
    ipinfo_test(!1,"::1.2.3.256");
    ipinfo_test(!1,"::300.2.3.4");
    ipinfo_test(!1,"::1.300.3.4");
    ipinfo_test(!1,"::1.2.300.4");
    ipinfo_test(!1,"::1.2.3.300");
    ipinfo_test(!1,"::900.2.3.4");
    ipinfo_test(!1,"::1.900.3.4");
    ipinfo_test(!1,"::1.2.900.4");
    ipinfo_test(!1,"::1.2.3.900");
    ipinfo_test(!1,"::300.300.300.300");
    ipinfo_test(!1,"::3000.30.30.30");
    ipinfo_test(1,"fe80::217:f2ff:254.7.237.98");
    ipinfo_test(1,"fe80::217:f2ff:fe07:ed62");
    /* unicast, full */
    ipinfo_test(1,"2001:DB8:0:0:8:800:200C:417A");
    /* multicast, full */
    ipinfo_test(1,"FF01:0:0:0:0:0:0:101");
    /* loopback, full */
    ipinfo_test(1,"0:0:0:0:0:0:0:1");
    /* unspecified, full */
    ipinfo_test(1,"0:0:0:0:0:0:0:0");
    /* unicast, compressed */
    ipinfo_test(1,"2001:DB8::8:800:200C:417A");
    /* multicast, compressed */
    ipinfo_test(1,"FF01::101");
    /* loopback, compressed, non-routable */
    ipinfo_test(1,"::1");
    /* unspecified, compressed, non-routable */
    ipinfo_test(1,"::");
    /* IPv4-compatible IPv6 address, full, deprecated */
    ipinfo_test(1,"0:0:0:0:0:0:13.1.68.3");
    /* IPv4-mapped IPv6 address, full */
    ipinfo_test(1,"0:0:0:0:0:FFFF:129.144.52.38");
    /* IPv4-compatible IPv6 address, compressed, deprecated */
    ipinfo_test(1,"::13.1.68.3");
    /* IPv4-mapped IPv6 address, compressed */
    ipinfo_test(1,"::FFFF:129.144.52.38");
    /* unicast, full */
    ipinfo_test(!1,"2001:DB8:0:0:8:800:200C:417A:221");
    ipinfo_test(!1,"FF01::101::2");
    /* multicast, compressed */
    ipinfo_test(!1,"");
    /* nothing */

    ipinfo_test(1,"fe80:0000:0000:0000:0204:61ff:fe9d:f156");
    ipinfo_test(1,"fe80:0:0:0:204:61ff:fe9d:f156");
    ipinfo_test(1,"fe80::204:61ff:fe9d:f156");

    /* Some systems treat the leading "0" in ".086" as the start
       of an octal number, but others don't. To prevent ambiguous
       interpretation, we don't allow this */
    ipinfo_test(!1,"fe80:0000:0000:0000:0204:61ff:254.157.241.086");

    ipinfo_test(1,"fe80:0:0:0:204:61ff:254.157.241.86");
    ipinfo_test(1,"fe80::204:61ff:254.157.241.86");
    ipinfo_test(1,"::1");
    ipinfo_test(1,"fe80::");
    ipinfo_test(1,"fe80::1");
    ipinfo_test(!1,":");

    /* Aeron supplied these test cases. */

    ipinfo_test(!1,"1111:2222:3333:4444::5555:");
    ipinfo_test(!1,"1111:2222:3333::5555:");
    ipinfo_test(!1,"1111:2222::5555:");
    ipinfo_test(!1,"1111::5555:");
    ipinfo_test(!1,"::5555:");
    ipinfo_test(!1,":::");
    ipinfo_test(!1,"1111:");
    ipinfo_test(!1,":");

    ipinfo_test(!1,":1111:2222:3333:4444::5555");
    ipinfo_test(!1,":1111:2222:3333::5555");
    ipinfo_test(!1,":1111:2222::5555");
    ipinfo_test(!1,":1111::5555");
    ipinfo_test(!1,":::5555");
    ipinfo_test(!1,":::");

    ipinfo_test(!1,"1.2.3.4:1111:2222:3333:4444::5555");
    ipinfo_test(!1,"1.2.3.4:1111:2222:3333::5555");
    ipinfo_test(!1,"1.2.3.4:1111:2222::5555");
    ipinfo_test(!1,"1.2.3.4:1111::5555");
    ipinfo_test(!1,"1.2.3.4::5555");
    ipinfo_test(!1,"1.2.3.4::");

    /* Additional test cases
       from http://rt.cpan.org/Public/Bug/Display.html?id=50693 */

    ipinfo_test(1,"2001:0db8:85a3:0000:0000:8a2e:0370:7334");
    ipinfo_test(1,"2001:db8:85a3:0:0:8a2e:370:7334");
    ipinfo_test(1,"2001:db8:85a3::8a2e:370:7334");
    ipinfo_test(1,"2001:0db8:0000:0000:0000:0000:1428:57ab");
    ipinfo_test(1,"2001:0db8:0000:0000:0000::1428:57ab");
    ipinfo_test(1,"2001:0db8:0:0:0:0:1428:57ab");
    ipinfo_test(1,"2001:0db8:0:0::1428:57ab");
    ipinfo_test(1,"2001:0db8::1428:57ab");
    ipinfo_test(1,"2001:db8::1428:57ab");
    ipinfo_test(1,"0000:0000:0000:0000:0000:0000:0000:0001");
    ipinfo_test(1,"::1");
    ipinfo_test(1,"::ffff:12.34.56.78");
    ipinfo_test(1,"::ffff:0c22:384e");
    ipinfo_test(1,"2001:0db8:1234:0000:0000:0000:0000:0000");
    ipinfo_test(1,"2001:0db8:1234:ffff:ffff:ffff:ffff:ffff");
    ipinfo_test(1,"2001:db8:a::123");
    ipinfo_test(1,"fe80::");
    ipinfo_test(1,"::ffff:192.0.2.128");
    ipinfo_test(1,"::ffff:c000:280");

    ipinfo_test(!1,"123");
    ipinfo_test(!1,"ldkfj");
    ipinfo_test(!1,"2001::FFD3::57ab");
    ipinfo_test(!1,"2001:db8:85a3::8a2e:37023:7334");
    ipinfo_test(!1,"2001:db8:85a3::8a2e:370k:7334");
    ipinfo_test(!1,"1:2:3:4:5:6:7:8:9");
    ipinfo_test(!1,"1::2::3");
    ipinfo_test(!1,"1:::3:4:5");
    ipinfo_test(!1,"1:2:3::4:5:6:7:8:9");
    ipinfo_test(!1,"::ffff:2.3.4");
    ipinfo_test(!1,"::ffff:257.1.2.3");

    /* New from Aeron */

    ipinfo_test(1,"1111:2222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(1,"1111:2222:3333:4444:5555:6666:7777::");
    ipinfo_test(1,"1111:2222:3333:4444:5555:6666::");
    ipinfo_test(1,"1111:2222:3333:4444:5555::");
    ipinfo_test(1,"1111:2222:3333:4444::");
    ipinfo_test(1,"1111:2222:3333::");
    ipinfo_test(1,"1111:2222::");
    ipinfo_test(1,"1111::");
    ipinfo_test(1,"::");
    ipinfo_test(1,"1111:2222:3333:4444:5555:6666::8888");
    ipinfo_test(1,"1111:2222:3333:4444:5555::8888");
    ipinfo_test(1,"1111:2222:3333:4444::8888");
    ipinfo_test(1,"1111:2222:3333::8888");
    ipinfo_test(1,"1111:2222::8888");
    ipinfo_test(1,"1111::8888");
    ipinfo_test(1,"::8888");
    ipinfo_test(1,"1111:2222:3333:4444:5555::7777:8888");
    ipinfo_test(1,"1111:2222:3333:4444::7777:8888");
    ipinfo_test(1,"1111:2222:3333::7777:8888");
    ipinfo_test(1,"1111:2222::7777:8888");
    ipinfo_test(1,"1111::7777:8888");
    ipinfo_test(1,"::7777:8888");
    ipinfo_test(1,"1111:2222:3333:4444::6666:7777:8888");
    ipinfo_test(1,"1111:2222:3333::6666:7777:8888");
    ipinfo_test(1,"1111:2222::6666:7777:8888");
    ipinfo_test(1,"1111::6666:7777:8888");
    ipinfo_test(1,"::6666:7777:8888");
    ipinfo_test(1,"1111:2222:3333::5555:6666:7777:8888");
    ipinfo_test(1,"1111:2222::5555:6666:7777:8888");
    ipinfo_test(1,"1111::5555:6666:7777:8888");
    ipinfo_test(1,"::5555:6666:7777:8888");
    ipinfo_test(1,"1111:2222::4444:5555:6666:7777:8888");
    ipinfo_test(1,"1111::4444:5555:6666:7777:8888");
    ipinfo_test(1,"::4444:5555:6666:7777:8888");
    ipinfo_test(1,"1111::3333:4444:5555:6666:7777:8888");
    ipinfo_test(1,"::3333:4444:5555:6666:7777:8888");
    ipinfo_test(1,"::2222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(1,"1111:2222:3333:4444:5555:6666:123.123.123.123");
    ipinfo_test(1,"1111:2222:3333:4444:5555::123.123.123.123");
    ipinfo_test(1,"1111:2222:3333:4444::123.123.123.123");
    ipinfo_test(1,"1111:2222:3333::123.123.123.123");
    ipinfo_test(1,"1111:2222::123.123.123.123");
    ipinfo_test(1,"1111::123.123.123.123");
    ipinfo_test(1,"::123.123.123.123");
    ipinfo_test(1,"1111:2222:3333:4444::6666:123.123.123.123");
    ipinfo_test(1,"1111:2222:3333::6666:123.123.123.123");
    ipinfo_test(1,"1111:2222::6666:123.123.123.123");
    ipinfo_test(1,"1111::6666:123.123.123.123");
    ipinfo_test(1,"::6666:123.123.123.123");
    ipinfo_test(1,"1111:2222:3333::5555:6666:123.123.123.123");
    ipinfo_test(1,"1111:2222::5555:6666:123.123.123.123");
    ipinfo_test(1,"1111::5555:6666:123.123.123.123");
    ipinfo_test(1,"::5555:6666:123.123.123.123");
    ipinfo_test(1,"1111:2222::4444:5555:6666:123.123.123.123");
    ipinfo_test(1,"1111::4444:5555:6666:123.123.123.123");
    ipinfo_test(1,"::4444:5555:6666:123.123.123.123");
    ipinfo_test(1,"1111::3333:4444:5555:6666:123.123.123.123");
    ipinfo_test(1,"::2222:3333:4444:5555:6666:123.123.123.123");

    /* New invalid from Aeron */
    ipinfo_test(!1,"XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX");

    /* Too many components */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:8888:9999");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:8888::");
    ipinfo_test(!1,"::2222:3333:4444:5555:6666:7777:8888:9999");

    /* Too few components */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666");
    ipinfo_test(!1,"1111:2222:3333:4444:5555");
    ipinfo_test(!1,"1111:2222:3333:4444");
    ipinfo_test(!1,"1111:2222:3333");
    ipinfo_test(!1,"1111:2222");
    ipinfo_test(!1,"1111");

    /* Missing */
    ipinfo_test(!1,"11112222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:22223333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:33334444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:44445555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:55556666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:66667777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:77778888");

    /* Missing : intended for :: */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:");
    ipinfo_test(!1,"1111:2222:3333:4444:");
    ipinfo_test(!1,"1111:2222:3333:");
    ipinfo_test(!1,"1111:2222:");
    ipinfo_test(!1,"1111:");
    ipinfo_test(!1,":");
    ipinfo_test(!1,":8888");
    ipinfo_test(!1,":7777:8888");
    ipinfo_test(!1,":6666:7777:8888");
    ipinfo_test(!1,":5555:6666:7777:8888");
    ipinfo_test(!1,":4444:5555:6666:7777:8888");
    ipinfo_test(!1,":3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,":2222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666:7777:8888");

    /* ::: */
    ipinfo_test(!1,":::2222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:::3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:::4444:5555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:::5555:6666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:::6666:7777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:::7777:8888");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:::8888");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:::");

    /* Double :: */
    ipinfo_test(!1,"::2222::4444:5555:6666:7777:8888");
    ipinfo_test(!1,"::2222:3333::5555:6666:7777:8888");
    ipinfo_test(!1,"::2222:3333:4444::6666:7777:8888");
    ipinfo_test(!1,"::2222:3333:4444:5555::7777:8888");
    ipinfo_test(!1,"::2222:3333:4444:5555:7777::8888");
    ipinfo_test(!1,"::2222:3333:4444:5555:7777:8888::");

    ipinfo_test(!1,"1111::3333::5555:6666:7777:8888");
    ipinfo_test(!1,"1111::3333:4444::6666:7777:8888");
    ipinfo_test(!1,"1111::3333:4444:5555::7777:8888");
    ipinfo_test(!1,"1111::3333:4444:5555:6666::8888");
    ipinfo_test(!1,"1111::3333:4444:5555:6666:7777::");

    ipinfo_test(!1,"1111:2222::4444::6666:7777:8888");
    ipinfo_test(!1,"1111:2222::4444:5555::7777:8888");
    ipinfo_test(!1,"1111:2222::4444:5555:6666::8888");
    ipinfo_test(!1,"1111:2222::4444:5555:6666:7777::");

    ipinfo_test(!1,"1111:2222:3333::5555::7777:8888");
    ipinfo_test(!1,"1111:2222:3333::5555:6666::8888");
    ipinfo_test(!1,"1111:2222:3333::5555:6666:7777::");

    ipinfo_test(!1,"1111:2222:3333:4444::6666::8888");
    ipinfo_test(!1,"1111:2222:3333:4444::6666:7777::");

    ipinfo_test(!1,"1111:2222:3333:4444:5555::7777::");

    /* Invalid data */
    ipinfo_test(!1,"XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:00.00.00.00");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:000.000.000.000");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:256.256.256.256");

    /* Too many components */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:8888:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666::1.2.3.4");
    ipinfo_test(!1,"::2222:3333:4444:5555:6666:7777:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:1.2.3.4.5");

    /* Too few components */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:1.2.3.4");
    ipinfo_test(!1,"1111:2222:1.2.3.4");
    ipinfo_test(!1,"1111:1.2.3.4");

    /* Missing : */
    ipinfo_test(!1,"11112222:3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:22223333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:33334444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:44445555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:55556666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:66661.2.3.4");

    /* Missing . */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:255255.255.255");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:255.255255.255");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:255.255.255255");

    /* Missing : intended for :: */
    ipinfo_test(!1,":1.2.3.4");
    ipinfo_test(!1,":6666:1.2.3.4");
    ipinfo_test(!1,":5555:6666:1.2.3.4");
    ipinfo_test(!1,":4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":2222:3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666:1.2.3.4");

    /* ::: */
    ipinfo_test(!1,":::2222:3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:::3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:::4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:::5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:::6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:::1.2.3.4");

    /* Double :: */
    ipinfo_test(!1,"::2222::4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,"::2222:3333::5555:6666:1.2.3.4");
    ipinfo_test(!1,"::2222:3333:4444::6666:1.2.3.4");
    ipinfo_test(!1,"::2222:3333:4444:5555::1.2.3.4");

    ipinfo_test(!1,"1111::3333::5555:6666:1.2.3.4");
    ipinfo_test(!1,"1111::3333:4444::6666:1.2.3.4");
    ipinfo_test(!1,"1111::3333:4444:5555::1.2.3.4");

    ipinfo_test(!1,"1111:2222::4444::6666:1.2.3.4");
    ipinfo_test(!1,"1111:2222::4444:5555::1.2.3.4");

    ipinfo_test(!1,"1111:2222:3333::5555::1.2.3.4");

    /* Missing parts */
    ipinfo_test(!1,"::.");
    ipinfo_test(!1,"::..");
    ipinfo_test(!1,"::...");
    ipinfo_test(!1,"::1...");
    ipinfo_test(!1,"::1.2..");
    ipinfo_test(!1,"::1.2.3.");
    ipinfo_test(!1,"::.2..");
    ipinfo_test(!1,"::.2.3.");
    ipinfo_test(!1,"::.2.3.4");
    ipinfo_test(!1,"::..3.");
    ipinfo_test(!1,"::..3.4");
    ipinfo_test(!1,"::...4");

    /* Extra : in front */
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666:7777::");
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666::");
    ipinfo_test(!1,":1111:2222:3333:4444:5555::");
    ipinfo_test(!1,":1111:2222:3333:4444::");
    ipinfo_test(!1,":1111:2222:3333::");
    ipinfo_test(!1,":1111:2222::");
    ipinfo_test(!1,":1111::");
    ipinfo_test(!1,":::");
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666::8888");
    ipinfo_test(!1,":1111:2222:3333:4444:5555::8888");
    ipinfo_test(!1,":1111:2222:3333:4444::8888");
    ipinfo_test(!1,":1111:2222:3333::8888");
    ipinfo_test(!1,":1111:2222::8888");
    ipinfo_test(!1,":1111::8888");
    ipinfo_test(!1,":::8888");
    ipinfo_test(!1,":1111:2222:3333:4444:5555::7777:8888");
    ipinfo_test(!1,":1111:2222:3333:4444::7777:8888");
    ipinfo_test(!1,":1111:2222:3333::7777:8888");
    ipinfo_test(!1,":1111:2222::7777:8888");
    ipinfo_test(!1,":1111::7777:8888");
    ipinfo_test(!1,":::7777:8888");
    ipinfo_test(!1,":1111:2222:3333:4444::6666:7777:8888");
    ipinfo_test(!1,":1111:2222:3333::6666:7777:8888");
    ipinfo_test(!1,":1111:2222::6666:7777:8888");
    ipinfo_test(!1,":1111::6666:7777:8888");
    ipinfo_test(!1,":::6666:7777:8888");
    ipinfo_test(!1,":1111:2222:3333::5555:6666:7777:8888");
    ipinfo_test(!1,":1111:2222::5555:6666:7777:8888");
    ipinfo_test(!1,":1111::5555:6666:7777:8888");
    ipinfo_test(!1,":::5555:6666:7777:8888");
    ipinfo_test(!1,":1111:2222::4444:5555:6666:7777:8888");
    ipinfo_test(!1,":1111::4444:5555:6666:7777:8888");
    ipinfo_test(!1,":::4444:5555:6666:7777:8888");
    ipinfo_test(!1,":1111::3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,":::3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,":::2222:3333:4444:5555:6666:7777:8888");
    ipinfo_test(!1,":1111:2222:3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333:4444:5555::1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333:4444::1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333::1.2.3.4");
    ipinfo_test(!1,":1111:2222::1.2.3.4");
    ipinfo_test(!1,":1111::1.2.3.4");
    ipinfo_test(!1,":::1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333:4444::6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333::6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222::6666:1.2.3.4");
    ipinfo_test(!1,":1111::6666:1.2.3.4");
    ipinfo_test(!1,":::6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222:3333::5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222::5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111::5555:6666:1.2.3.4");
    ipinfo_test(!1,":::5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111:2222::4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111::4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":::4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":1111::3333:4444:5555:6666:1.2.3.4");
    ipinfo_test(!1,":::2222:3333:4444:5555:6666:1.2.3.4");

    /* Extra : at end */
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:7777:::");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666:::");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:::");
    ipinfo_test(!1,"1111:2222:3333:4444:::");
    ipinfo_test(!1,"1111:2222:3333:::");
    ipinfo_test(!1,"1111:2222:::");
    ipinfo_test(!1,"1111:::");
    ipinfo_test(!1,":::");
    ipinfo_test(!1,"1111:2222:3333:4444:5555:6666::8888:");
    ipinfo_test(!1,"1111:2222:3333:4444:5555::8888:");
    ipinfo_test(!1,"1111:2222:3333:4444::8888:");
    ipinfo_test(!1,"1111:2222:3333::8888:");
    ipinfo_test(!1,"1111:2222::8888:");
    ipinfo_test(!1,"1111::8888:");
    ipinfo_test(!1,"::8888:");
    ipinfo_test(!1,"1111:2222:3333:4444:5555::7777:8888:");
    ipinfo_test(!1,"1111:2222:3333:4444::7777:8888:");
    ipinfo_test(!1,"1111:2222:3333::7777:8888:");
    ipinfo_test(!1,"1111:2222::7777:8888:");
    ipinfo_test(!1,"1111::7777:8888:");
    ipinfo_test(!1,"::7777:8888:");
    ipinfo_test(!1,"1111:2222:3333:4444::6666:7777:8888:");
    ipinfo_test(!1,"1111:2222:3333::6666:7777:8888:");
    ipinfo_test(!1,"1111:2222::6666:7777:8888:");
    ipinfo_test(!1,"1111::6666:7777:8888:");
    ipinfo_test(!1,"::6666:7777:8888:");
    ipinfo_test(!1,"1111:2222:3333::5555:6666:7777:8888:");
    ipinfo_test(!1,"1111:2222::5555:6666:7777:8888:");
    ipinfo_test(!1,"1111::5555:6666:7777:8888:");
    ipinfo_test(!1,"::5555:6666:7777:8888:");
    ipinfo_test(!1,"1111:2222::4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"1111::4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"::4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"1111::3333:4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"::3333:4444:5555:6666:7777:8888:");
    ipinfo_test(!1,"::2222:3333:4444:5555:6666:7777:8888:");

    /* From here on, I added my own test cases for ipinfo. */
    ipinfo_test(1, "ABCD:EF01:2345:6789:ABCD:EF01:2345:6789");
    ipinfo_test(1, "abcd:ef01:2345:6789:abcd:ef01:2345:6789");
    ipinfo_test(1, "2001:DB8:0:0:8:800:200C:417A");
    ipinfo_test(1, "2001:DB8:0:0:8:800:200C:417A");
    ipinfo_test(1, "FF01:0:0:0:0:0:0:101");
    ipinfo_test(1, "0:0:0:0:0:0:0:1");
    ipinfo_test(1, "0:0:0:0:0:0:0:0");
    ipinfo_test(1, "2001:DB8::8:800:200C:417A");
    ipinfo_test(1, "FF01::101");
    ipinfo_test(1, "::1");
    ipinfo_test(1, "::");
    ipinfo_test(1, "0:0:0:0:0:0:13.1.68.3");
    ipinfo_test(1, "0:0:0:0:0:FFFF:129.144.52.38");
    ipinfo_test(1, "::13.1.68.3");
    ipinfo_test(1, "::FFFF:129.144.52.38");
    ipinfo_test(1, "2001:0DB8:0000:CD30:0000:0000:0000:0000/60");
    ipinfo_test(1, "2001:0DB8::CD30:0:0:0:0/60");
    ipinfo_test(1, "2001:0DB8:0:CD30::/60");
    ipinfo_test(1, "2001:0DB8:0:CD30:123:4567:89AB:CDEF");
    ipinfo_test(1, "2001:0DB8:0:CD30::/60");
    ipinfo_test(1, "2001:0DB8:0:CD30:123:4567:89AB:CDEF/60");
    ipinfo_test(1, "2001:0DB8:0:CD30::/60");
}

