/*
 * Copyright (c) 2001, Gary R. Van Sickle.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     A copy of the GNU General Public License can be found at
 *     http://www.gnu.org/
 *
 * Written by Gary R. Van Sickle <g.r.vansickle@worldnet.att.net>
 *
 */

// This is the implementation of the ThreeBarProgressPage class.  It is a fairly generic
// progress indicator property page with three progress bars.

#include "win32.h"
#include "commctrl.h"
#include "resource.h"

#include "dialog.h"
#include "site.h"

#include "propsheet.h"
#include "threebar.h"
#include "cistring.h"

bool ThreeBarProgressPage::Create ()
{
  return PropertyPage::Create (IDD_INSTATUS);
}

void
ThreeBarProgressPage::OnInit ()
{
  // Get HWNDs to the dialog controls
  ins_action = GetDlgItem (IDC_INS_ACTION);
  ins_pkgname = GetDlgItem (IDC_INS_PKG);
  ins_filename = GetDlgItem (IDC_INS_FILE);
  // Bars
  ins_pprogress = GetDlgItem (IDC_INS_PPROGRESS);
  ins_iprogress = GetDlgItem (IDC_INS_IPROGRESS);
  ins_diskfull = GetDlgItem (IDC_INS_DISKFULL);
  // Bar labels
  ins_bl_package = GetDlgItem (IDC_INS_BL_PACKAGE);
  ins_bl_total = GetDlgItem (IDC_INS_BL_TOTAL);
  ins_bl_disk = GetDlgItem (IDC_INS_BL_DISK);
}

void
ThreeBarProgressPage::SetText1 (const TCHAR * t)
{
  ::SetWindowText (ins_action, t);
}

void
ThreeBarProgressPage::SetText2 (const TCHAR * t)
{
  ::SetWindowText (ins_pkgname, t);
}

void
ThreeBarProgressPage::SetText3 (const TCHAR * t)
{
  ::SetWindowText (ins_filename, t);
}

void
ThreeBarProgressPage::SetBar1 (long progress, long max)
{
  int percent = (int) (100.0 * ((double) progress) / (double) max);
  SendMessage (ins_pprogress, PBM_SETPOS, (WPARAM) percent, 0);
}

void
ThreeBarProgressPage::SetBar2 (long progress, long max)
{
  int percent = (int) (100.0 * ((double) progress) / (double) max);
  SendMessage (ins_iprogress, PBM_SETPOS, (WPARAM) percent, 0);
  cistring s;
  s.Format (IDS_CYGWIN_SETUP_WITH_PROGRESS, percent);
  GetOwner ()->SetWindowText (s.c_str ());
}

void
ThreeBarProgressPage::SetBar3 (long progress, long max)
{
  int percent = (int) (100.0 * ((double) progress) / (double) max);
  SendMessage (ins_diskfull, PBM_SETPOS, (WPARAM) percent, 0);
}

void
ThreeBarProgressPage::EnableSingleBar (bool enable)
{
  // Switch to/from single bar mode
  ShowWindow (ins_bl_total, enable ? SW_HIDE : SW_SHOW);
  ShowWindow (ins_bl_disk, enable ? SW_HIDE : SW_SHOW);
  ShowWindow (ins_iprogress, enable ? SW_HIDE : SW_SHOW);
  ShowWindow (ins_diskfull, enable ? SW_HIDE : SW_SHOW);
}

void
ThreeBarProgressPage::OnActivate ()
{
  // Disable back and next buttons
  GetOwner ()->SetButtons (0);

  // Set all bars to 0
  SetBar1 (0);
  SetBar2 (0);
  SetBar3 (0);

  switch (task)
    {
    case WM_APP_START_SITE_INFO_DOWNLOAD:
    case WM_APP_START_SETUP_INI_DOWNLOAD:
      // For these tasks, show only a single progress bar.
      EnableSingleBar ();
      break;
    default:
      // Show the normal 3-bar view by default
      EnableSingleBar (false);
      break;
    }

  Window::PostMessage (task);
}

bool
  ThreeBarProgressPage::OnMessageApp (UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
    {
    case WM_APP_START_DOWNLOAD:
      {
	// Start the package download thread.
	do_download (GetInstance (), GetHWND ());
	break;
      }
    case WM_APP_DOWNLOAD_THREAD_COMPLETE:
      {
	if (lParam == IDD_S_INSTALL)
	  {
	    // Download is complete and we want to go on to the install.
	    Window::PostMessage (WM_APP_START_INSTALL);
	  }
	else if (lParam != 0)
	  {
	    // Download failed for some reason, go back to site selection page
	    GetOwner ()->SetActivePageByID (lParam);
	  }
	else
	  {
	    // Was a download-only, and is complete or failed.
	    GetOwner ()->PressButton (PSBTN_CANCEL);
	  }
	break;
      }
    case WM_APP_START_INSTALL:
      {
	// Start the install thread.
	do_install (GetInstance (), GetHWND ());
	break;
      }
    case WM_APP_INSTALL_THREAD_COMPLETE:
      {
	// Re-enable and "Push" the Next button
	GetOwner ()->SetButtons (PSWIZB_NEXT);
	GetOwner ()->PressButton (PSBTN_NEXT);
	break;
      }
    case WM_APP_START_SITE_INFO_DOWNLOAD:
      {
	do_download_site_info (GetInstance (), GetHWND ());
	break;
      }
    case WM_APP_SITE_INFO_DOWNLOAD_COMPLETE:
      {
	GetOwner ()->SetActivePageByID (lParam);
	break;
      }
    case WM_APP_START_SETUP_INI_DOWNLOAD:
      {
	do_ini (GetInstance (), GetHWND ());
	break;
      }
    case WM_APP_SETUP_INI_DOWNLOAD_COMPLETE:
      {
	if (lParam == IDD_S_FROM_CWD)
	  {
	    // There isn't actually a dialog template named this
	    do_fromcwd (GetInstance (), GetHWND ());
	  }
	else
	  {
	    GetOwner ()->SetActivePageByID (lParam);
	  }
	break;
      }
    default:
      {
	// Not handled
	return false;
      }
    }

  return true;
}