/* gettext_setup.h --- Setup i18n support for a source file
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
#ifndef _GETTEXT_SETUP_H
#define _GETTEXT_SETUP_H

#define DEFAULT_TEXT_DOMAIN "ipinfo"

#include <locale.h>
#include "gettext.h"
#define _(x) gettext(x)

#endif /* gettext_setup.h */

