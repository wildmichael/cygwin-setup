/*
 * Copyright (c) 2000, Red Hat, Inc.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     A copy of the GNU General Public License can be found at
 *     http://www.gnu.org/
 *
 * Written by DJ Delorie <dj@cygnus.com>
 *
 */

/* The purpose of this file is to handle the case where we're
   installing from files that already exist in the current directory.
   If a setup.ini file is present, we set the mirror site to "." and
   pretend we're installing from the `internet' ;-) else we have to
   find all the .tar.gz files, deduce their versions, and try to
   compare versions in the case where the current directory contains
   multiple versions of any given package.  We do *not* try to compare
   versions with already installed packages; we always choose a
   package in the current directory over one that's already installed
   (otherwise, why would you have asked to install it?).  Note
   that we search recursively. */

static char *cvsid = "\n%%% $Id$\n";

#include "win32.h"

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <unistd.h>
#include <ctype.h>

#include "ini.h"
#include "resource.h"
#include "concat.h"
#include "state.h"
#include "dialog.h"
#include "msg.h"
#include "find.h"

#include "port.h"

static int
is_test_version (char *v)
{
  int i;
  for (i=0; v[i] && isdigit (v[i]); i++) ;
  return (i >= 6) ? 1 : 0;
}

static char *canonicalize_version (char *v)
{
  static char nv[100];
  char *np=nv, *dp, *ov = v;
  int i;

  while (*v)
    {
      if (isdigit (*v))
	{
	  for (dp=v; *dp && isdigit (*dp); dp++) ;
	  for (i=dp-v; i<12; i++)
	    *np++ = '0';
	  while (v < dp)
	    *np++ = *v++;
	}
      else
	*np++ = *v++;
    }
  *np++ = 0;
  return nv;
}

static void
found_file (char *path, unsigned int fsize)
{
  char base[_MAX_PATH], *ver;

  int l = strlen (path);

  if (strcmp (path + l - 7, ".tar.gz") != 0)
    return;
  if (strstr (path, "-src."))
    return;
  if (strstr (path, "-patch."))
    return;

  char *sl = strrchr (path, '/');
  if (sl)
    sl ++;
  else
    sl = path;
  strcpy (base, sl);
  base[strlen (base) - 7] = 0; /* remove .tar.gz */
  for (ver=base; *ver; ver++)
    if ((*ver == '-' || *ver == '_') && isdigit (ver[1]))
      {
	*ver++ = 0;
	break;
      }

  Package *p = 0;
  int i;

  for (i=0; i<npackages; i++)
    if (strcmp (package[i].name, base) == 0)
      {
	p = package + i;
	break;
      }
  if (p == 0)
      p = new_package (strdup (base));

  int trust = is_test_version (ver) ? TRUST_TEST : TRUST_CURR;
  if (!*ver)
    trust = TRUST_PREV;

  /* See if this version is older than what we have */
  if (p->info[trust].version)
    {
      char *ov = canonicalize_version (p->info[trust].version);
      char *nv = canonicalize_version (ver);
      if (strcmp (ov, nv) > 0)
	return;
    }

  p->info[trust].version = _strdup (ver);
  p->info[trust].install = _strdup (path);
  p->info[trust].install_size = fsize;
}

void
do_fromcwd (HINSTANCE h)
{
  if (_access ("./setup.ini", 0) == 0)
    {
      mirror_site = ".";
      next_dialog = IDD_S_LOAD_INI;
      return;
    }

  next_dialog = IDD_CHOOSE;

  find (".", found_file);

  return;
}
