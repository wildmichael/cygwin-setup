/*
 * Copyright (c) 2001, Robert Collins.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     A copy of the GNU General Public License can be found at
 *     http://www.gnu.org/
 *
 * Written by Robert Collins  <rbtcollins@hotmail.com>
 *
 */

/* this is the parent class for all package source (not source code - installation
 * source as in http/ftp/disk file) operations. 
 */

#if 0
static const char *cvsid =
  "\n%%% $Id$\n";
#endif

#include <stdlib.h>
#include <strings.h>
#include "package_source.h"

void
packagesource::set_canonical (char const *fn)
{
canonical = new char[strlen (fn) + 1];
  strcpy (canonical,fn);

  /* The base is from the last '/' to the first following '.' */
  char const *bstart = strchr (fn, '/');
  char const *tmp;
  while (bstart && (tmp = strchr (bstart, '/')))
    bstart = tmp;
  
  if (!bstart)
    bstart = fn;
  char const *bend = strchr (bstart, '.');
  if (!bend)
    bend = strchr (bstart, '\0');
  char const *end = strchr (fn ,'\0');
  base = new char [bend-bstart+1];
  memcpy (base,bstart, bend-bstart);
  base [bend-bstart] = '\0';

  filename = new char [end - bstart+1];
  memcpy (filename, bstart, end-bstart);
  filename[end-bstart] = '\0';
 
 if (cached)
  delete cached; 
  cached = 0;
}
