/*
 * Copyright (c) 2000, 2001, Red Hat, Inc.
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

/* The purpose of this file is to contain all the global variables
   that define the "state" of the install, that is, all the
   information that the user has provided so far.  These are set by
   the various dialogs and used by the various actions. 
 
   Note that this is deprecated. Persistent settings should be accessed
   via a class that stores them cross-installs, and non-persistent settings
   directly via the appropriate class. One of the reasons for this is that
   non-trivial types would require this file to include appropriate headers,
   making all of setup.exe rebuild for potentially minor changes.
 
 */

#include "String++.h"

extern bool unattended_mode;

extern int source;

extern String local_dir;

extern int root_text;
extern int root_scope;
extern int root_menu;
extern int root_desktop;

extern int net_method;
extern char *net_proxy_host;
extern int net_proxy_port;

extern char *net_user;
extern char *net_passwd;
extern char *net_proxy_user;
extern char *net_proxy_passwd;
extern char *net_ftp_user;
extern char *net_ftp_passwd;

