/*
 * Copyright (c) 2001, Jan Nieuwenhuizen.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     A copy of the GNU General Public License can be found at
 *     http://www.gnu.org/
 *
 * Written by Jan Nieuwenhuizen <janneke@gnu.org>
 *
 */
#ifndef SCRIPT_H
#define SCRIPT_H

/* Run the script fname, found in dir.  If fname has suffix .sh, and
   we have a Bourne shell, execute it using sh.  Otherwise, if fname
   has suffix .bat, execute using cmd */
   
void run_script (char const *dir, char const *fname);

/* Initialisation stuff for run_script: sh, cmd, CYGWINROOT and PATH */
void init_run_script ();

/* Run the scripts fname.sh and fname.bat, found in dir. */
void try_run_script (char const *dir, char const *fname);

#endif /* SCRIPT_H */
