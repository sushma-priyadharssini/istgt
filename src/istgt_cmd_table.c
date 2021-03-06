/*-
 * Copyright (c) 2003, 2004, 2005, 2009 Silicon Graphics International Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * $Id: //depot/users/kenm/FreeBSD-test2/sys/cam/ctl/ctl_cmd_table.c#4 $
 * $FreeBSD: stable/9/sys/cam/ctl/ctl_cmd_table.c 237824 2012-06-29 21:27:37Z ken $
 */
/*
 * CAM Target Layer command table.
 *
 * Author: Ken Merry <ken@FreeBSD.org>, Kim Le
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <stdint.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#ifdef HAVE_PTHREAD_NP_H
#include <pthread_np.h>
#endif
#include <unistd.h>

#ifdef __FreeBSD__
#include <sys/param.h>
#endif

#include "istgt.h"
#include "istgt_ver.h"
#include "istgt_log.h"
#include "istgt_sock.h"
#include "istgt_misc.h"
#include "istgt_md5.h"
#include "istgt_lu.h"
#include "istgt_iscsi.h"
#include "istgt_proto.h"

#if !defined(__GNUC__)
#undef __attribute__
#define __attribute__(x)
#endif

/*
 * Whenever support for a new command is added, it should be added to this
 * table.
 */
struct istgt_cmd_entry istgt_cmd_table[] =
{
/* 00 TEST UNIT READY */
{ ISTGT_SERIDX_TUR, ISTGT_CMD_FLAG_OK_ON_BOTH |
			  ISTGT_FLAG_DATA_NONE |
			  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
			  __TUR
 },

/* 01 REWIND */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE , __UNK},

/* 02 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE , __UNK},

/* 03 REQUEST SENSE */
{ ISTGT_SERIDX_RQ_SNS, ISTGT_FLAG_DATA_IN |
				       ISTGT_CMD_FLAG_OK_ON_ALL_LUNS |
				       ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				       ISTGT_CMD_FLAG_NO_SENSE |
				       ISTGT_CMD_FLAG_OK_ON_STOPPED |
				       ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				       ISTGT_CMD_FLAG_OK_ON_OFFLINE |
				       ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				       ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
					   __RQSNS
 },

/* 04 FORMAT UNIT */
{ ISTGT_SERIDX_FORMAT, ISTGT_CMD_FLAG_OK_ON_SLUN |
				ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				ISTGT_FLAG_DATA_OUT,
				__FMT
 },

/* 05 READ BLOCK LIMITS */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __RBL},

/* 06 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 07 REASSIGN BLOCKS */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 08 READ(6) */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_SLUN |
				  ISTGT_FLAG_DATA_IN |
				  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
				  __RD6
 },

/* 09 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK },

/* 0A WRITE(6) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN |
				   ISTGT_FLAG_DATA_OUT,
				   __WR6
},

/* 0B SEEK(6) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 0C */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 0D */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 0E */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 0F READ REVERSE(6) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 10 WRITE FILEMARKS(6) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 11 SPACE(6) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 12 INQUIRY */
{ ISTGT_SERIDX_INQ, ISTGT_CMD_FLAG_OK_ON_ALL_LUNS |
			      ISTGT_CMD_FLAG_ALLOW_ON_RESV |
			      ISTGT_CMD_FLAG_NO_SENSE |
			      ISTGT_CMD_FLAG_OK_ON_STOPPED |
			      ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
			      ISTGT_CMD_FLAG_OK_ON_OFFLINE |
			      ISTGT_CMD_FLAG_OK_ON_SECONDARY |
			      ISTGT_FLAG_DATA_IN |
			      ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
				  __INQ
 },

/* 13 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 14 RECOVER BUFFERED DATA */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 15 MODE SELECT(6) */
{ ISTGT_SERIDX_MD_SEL, ISTGT_CMD_FLAG_OK_ON_BOTH |
				     ISTGT_CMD_FLAG_OK_ON_STOPPED |
				     ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				     ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				     ISTGT_FLAG_DATA_OUT,
					 __MSEL
 },

/* 16 RESERVE(6) */
{ ISTGT_SERIDX_RESV, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				    ISTGT_CMD_FLAG_OK_ON_BOTH |
				    ISTGT_CMD_FLAG_OK_ON_STOPPED |
				    ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				    ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				    ISTGT_FLAG_DATA_OUT,
					 __RSV6
 },

/* 17 RELEASE(6) */
{ ISTGT_SERIDX_REL, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				   ISTGT_CMD_FLAG_OK_ON_BOTH |
				   ISTGT_CMD_FLAG_OK_ON_STOPPED |
				   ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				   ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				   ISTGT_FLAG_DATA_NONE,
					__REL6
 },

/* 18 COPY */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 19 ERASE(6) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 1A MODE SENSE(6) */
{ ISTGT_SERIDX_MD_SNS, ISTGT_CMD_FLAG_OK_ON_BOTH |
				    ISTGT_CMD_FLAG_OK_ON_STOPPED |
				    ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				    ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				    ISTGT_FLAG_DATA_IN,
					__MSN6
 },

/* 1B START STOP UNIT */
{ ISTGT_SERIDX_START, ISTGT_CMD_FLAG_OK_ON_SLUN |
				   ISTGT_CMD_FLAG_OK_ON_STOPPED |
				   ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				   ISTGT_CMD_FLAG_OK_ON_OFFLINE |
				   ISTGT_FLAG_DATA_NONE |
				   ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
					__START
 },

/* 1C RECEIVE DIAGNOSTIC RESULTS */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 1D SEND DIAGNOSTIC */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 1E PREVENT ALLOW MEDIUM REMOVAL */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 1F */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 20 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 21 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 22 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 23 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 24 SET WINDOW */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 25 READ CAPACITY(10) */
{ ISTGT_SERIDX_RD_CAP, ISTGT_CMD_FLAG_OK_ON_SLUN|
				       ISTGT_CMD_FLAG_OK_ON_STOPPED |
				       ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				       ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				       ISTGT_FLAG_DATA_IN |
				       ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
						__RCAP10
 },

/* 26 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 27 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 28 READ(10) */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_SLUN |
				  ISTGT_FLAG_DATA_IN |
				  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
				  __RD10
 },

/* 29 READ GENERATION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 2A WRITE(10) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
	              __WR10
 },

/* 2B SEEK(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 2C ERASE(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 2D READ UPDATED BLOCK */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 2E WRITE AND VERIFY(10) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
	              __WV10
 },

/* 2F VERIFY(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 30 SEARCH DATA HIGH(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 31 SEARCH DATA EQUAL(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 32 SEARCH DATA LOW(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 33 SET LIMITS(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 34 PRE-FETCH(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __PREFTCH10},

/* 35 SYNCHRONIZE CACHE(10) */
{ ISTGT_SERIDX_START, ISTGT_CMD_FLAG_OK_ON_SLUN |
				   ISTGT_FLAG_DATA_NONE,
				   __SYC10
 },

/* 36 LOCK UNLOCK CACHE(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 37 READ DEFECT DATA(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __RDDEF},

/* 38 MEDIUM SCAN */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 39 COMPARE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 3A COPY AND VERIFY */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 3B WRITE BUFFER */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_PROC |
				     ISTGT_FLAG_DATA_OUT,
					 __WBF
 },

/* 3C READ BUFFER */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 3D UPDATE BLOCK */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 3E READ LONG */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 3F WRITE LONG */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 40 CHANGE DEFINITION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 41 WRITE SAME(10) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN | ISTGT_FLAG_DATA_OUT, __WS10},

/* 42 READ SUB-CHANNEL */
{ ISTGT_SERIDX_UNMAP, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT, __UNMAP},

/* 43 READ TOC/PMA/ATIP */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 44 REPORT DENSITY SUPPORT */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 45 PLAY AUDIO(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 46 GET CONFIGURATION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 47 PLAY AUDIO MSF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 48 PLAY AUDIO TRACK INDEX */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 49 PLAY TRACK RELATIVE(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4A GET EVENT STATUS NOTIFICATION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4B PAUSE/RESUME */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4C LOG SELECT */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4D LOG SENSE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4E STOP PLAY/SCAN */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 4F */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 50 XDWRITE(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __XDW},

/* 51 XPWRITE(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __XDW},

/* 52 XDREAD(10) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __XDW},

/* 53 RESERVE TRACK */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 54 SEND OPC INFORMATION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 55 MODE SELECT(10) */
{ ISTGT_SERIDX_MD_SEL, ISTGT_CMD_FLAG_OK_ON_BOTH |
				     ISTGT_CMD_FLAG_OK_ON_STOPPED |
				     ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				     ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				     ISTGT_FLAG_DATA_OUT,
						  __MSEL10
 },

/* 56 RESERVE(10) */
{ ISTGT_SERIDX_RESV, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				    ISTGT_CMD_FLAG_OK_ON_BOTH |
				    ISTGT_CMD_FLAG_OK_ON_STOPPED |
				    ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				    ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				    ISTGT_FLAG_DATA_OUT,
						  __RSV10
 },

/* 57 RELEASE(10) */
{ ISTGT_SERIDX_REL, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				   ISTGT_CMD_FLAG_OK_ON_BOTH |
				   ISTGT_CMD_FLAG_OK_ON_STOPPED |
				   ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				   ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				   ISTGT_FLAG_DATA_OUT,
						  __REL10
 },

/* 58 REPAIR TRACK */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 59 READ MASTER CUE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 5A MODE SENSE(10) */
{ ISTGT_SERIDX_MD_SNS, ISTGT_CMD_FLAG_OK_ON_BOTH |
				    ISTGT_CMD_FLAG_OK_ON_STOPPED |
				    ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				    ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				    ISTGT_FLAG_DATA_IN,
						  __MSN10
 },

/* 5B CLOSE TRACK/SESSION */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 5C READ BUFFER CAPACITY */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 5D SEND CUE SHEET */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 5E PERSISTENT RESERVE IN */
{ ISTGT_SERIDX_PRES_IN, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
						ISTGT_CMD_FLAG_OK_ON_BOTH |
						ISTGT_CMD_FLAG_OK_ON_STOPPED |
						ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
						ISTGT_CMD_FLAG_OK_ON_SECONDARY |
						ISTGT_FLAG_DATA_IN |
						ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
						  __PRIN
 },

//{ ISTGT_SERIDX_INVLD, __UNK},

/* 5F PERSISTENT RESERVE OUT */
{ ISTGT_SERIDX_PRES_OUT, ISTGT_CMD_FLAG_ALLOW_ON_RESV |
						  ISTGT_CMD_FLAG_OK_ON_BOTH |
						  ISTGT_CMD_FLAG_OK_ON_STOPPED |
						  ISTGT_CMD_FLAG_OK_ON_INOPERABLE|
						  ISTGT_CMD_FLAG_OK_ON_SECONDARY |
						  ISTGT_FLAG_DATA_OUT |
						  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
						  __PROUT
 },

//{ ISTGT_SERIDX_INVLD, __UNK},

/* 60 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 61 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 62 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 63 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 64 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 65 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 66 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 67 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 68 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 69 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6A */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6B */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6C */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6D */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6E */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 6F */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 70 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 71 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 72 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 73 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 74 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 75 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 76 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 77 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 78 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 79 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7A */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7B */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7C */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7D */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7E */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 7F */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 80 XDWRITE EXTENDED(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __XDW},

/* 81 REBUILD(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 82 REGENERATE(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 83 EXTENDED COPY */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_ALL_LUNS | ISTGT_FLAG_DATA_OUT, __XCPY},

/* 84 RECEIVE COPY RESULTS */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_ALL_LUNS | ISTGT_FLAG_DATA_IN, __RCVCPY},

/* 85 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 86 ACCESS CONTROL IN */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 87 ACCESS CONTROL OUT */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 88 READ(16) */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_SLUN | ISTGT_FLAG_DATA_IN |
                                  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
				   __RD16
 },

/* 89 COMPARE AND WRITE */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN | ISTGT_FLAG_DATA_OUT,
				   __ATS
 },

/* 8A WRITE(16) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
				   __WR16
 },

/* 8B */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 8C READ ATTRIBUTE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 8D WRITE ATTRIBUTE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 8E WRITE AND VERIFY(16) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
				   __WV16
 },

/* 8F VERIFY(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 90 PRE-FETCH(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 91 SYNCHRONIZE CACHE(16) */
{ ISTGT_SERIDX_START, ISTGT_CMD_FLAG_OK_ON_SLUN |
				   ISTGT_FLAG_DATA_NONE,
				   __SYC16
 },

/* 92 LOCK UNLOCK CACHE(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 93 WRITE SAME(16) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN | ISTGT_FLAG_DATA_OUT,  __WS16},

/* 94 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 95 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 96 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 97 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 98 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 99 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 9A */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 9B */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 9C */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 9D */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* 9E SERVICE ACTION IN(16) */
/* XXX KDM not all service actions will be read capacity!! */
{ ISTGT_SERIDX_RD_CAP, ISTGT_CMD_FLAG_OK_ON_SLUN |
					   ISTGT_CMD_FLAG_OK_ON_STOPPED |
					   ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
					   ISTGT_CMD_FLAG_OK_ON_SECONDARY |
					   ISTGT_FLAG_DATA_IN |
					   ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
	 __RDCAP
 },

/* 9F SERVICE ACTION OUT(16) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A0 REPORT LUNS */
{ ISTGT_SERIDX_INQ, ISTGT_CMD_FLAG_OK_ON_ALL_LUNS |
				  ISTGT_CMD_FLAG_ALLOW_ON_RESV |
				  ISTGT_CMD_FLAG_NO_SENSE |
				  ISTGT_CMD_FLAG_OK_ON_STOPPED |
				  ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				  ISTGT_CMD_FLAG_OK_ON_OFFLINE |
				  ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				  ISTGT_FLAG_DATA_IN |
				  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
	 __RLUN
 },

/* A1 BLANK */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A2 SEND EVENT */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A3 MAINTENANCE (IN) Service Action - (0A) REPORT TARGET PORT GROUP */
{ ISTGT_SERIDX_MAIN_IN, ISTGT_CMD_FLAG_OK_ON_BOTH |
				         ISTGT_CMD_FLAG_OK_ON_STOPPED |
				         ISTGT_CMD_FLAG_OK_ON_INOPERABLE |
				         ISTGT_CMD_FLAG_OK_ON_SECONDARY |
				         ISTGT_FLAG_DATA_IN,
	 __M_IN
 },

/* A4 MAINTENANCE (OUT) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A5 MOVE MEDIUM */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A6 EXCHANGE MEDIUM */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A7 MOVE MEDIUM ATTACHED */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* A8 READ(12) */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_SLUN | ISTGT_FLAG_DATA_IN |
                                  ISTGT_CMD_FLAG_ALLOW_ON_PR_RESV,
	 __RD12
 },

/* A9 PLAY TRACK RELATIVE(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* AA WRITE(12) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
	 __WR12
 },

/* AB SERVICE ACTION IN(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* AC ERASE(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* AD READ DVD STRUCTURE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* AE WRITE AND VERIFY(12) */
{ ISTGT_SERIDX_WRITE, ISTGT_CMD_FLAG_OK_ON_SLUN| ISTGT_FLAG_DATA_OUT,
	 __WRV12
 },

/* AF VERIFY(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B0 SEARCH DATA HIGH(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B1 SEARCH DATA EQUAL(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B2 SEARCH DATA LOW(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B3 SET LIMITS(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B4 READ ELEMENT STATUS ATTACHED */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B5 REQUEST VOLUME ELEMENT ADDRESS */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B6 SEND VOLUME TAG */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B7 READ DEFECT DATA(12) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B8 READ ELEMENT STATUS */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* B9 READ CD MSF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BA REDUNDANCY GROUP (IN) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BB REDUNDANCY GROUP (OUT) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BC SPARE (IN) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BD SPARE (OUT) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BE VOLUME SET (IN) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* BF VOLUME SET (OUT) */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C0 - ISC_SEND_MSG_SHORT */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_PROC | ISTGT_FLAG_DATA_NONE,
	          __ISMx
 },

/* C1 - ISC_SEND_MSG */
{ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_PROC | ISTGT_FLAG_DATA_OUT,
	          __ISMx
 },

/* C2 - ISC_WRITE */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_PROC | ISTGT_FLAG_DATA_OUT,
	          __ISMx
 },

/* C3 - ISC_READ */
{ ISTGT_SERIDX_READ, ISTGT_CMD_FLAG_OK_ON_PROC | ISTGT_FLAG_DATA_IN,
	          __ISMx
 },

/* C4 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C5 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C6 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C7 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C8 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* C9 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CA */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CB */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CC */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CD */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* CF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D0 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D1 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D2 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D3 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D4 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D5 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D6 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D7 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D8 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* D9 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DA */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DB */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DC */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DD */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* DF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E0 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E1 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E2 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E3 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E4 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E5 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E6 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E7 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E8 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* E9 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* EA */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* EB */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* EC */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* ED */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* EE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* EF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F0 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F1 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F2 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F3 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F4 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F5 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F6 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F7 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F8 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* F9 */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FA */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FB */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FC */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FD */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FE */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK},

/* FF */
{ ISTGT_SERIDX_INVLD, ISTGT_CMD_FLAG_NONE, __UNK}

};

int iscsi_ops_indx_table[256] = {
	NOPOUT, SCSI, TASK, LOGIN, TXT, DATAOUT, LOGOUT, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	SNACK, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, VEN_1C, VEN_1D, VEN_1E, UNDEF,
	NOPIN, SCSI_RSP, TASK_RSP, LOGIN_RSP, TXT_RSP, DATAIN, LOGOUT_RSP, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, R2T,  ASYNC, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, VEN_3C, VEN_3D, VEN_3E, REJECT,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF,
	UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF, UNDEF
};

char iscsi_ops[ISCSI_ARYSZ + 1] [15] = {
	"NOPOUT", "SCSI", "TASK", "LOGIN", "TXT", "DATAOUT", "LOGOUT", "SNACK", "VEN_1C", "VEN_1D", "VEN_1E",
	"NOPIN", "SCSI_RSP", "TASK_RSP", "LOGIN_RSP", "TXT_RSP", "DATAIN", "LOGOUT_RSP", "R2T",
	"ASYNC", "VEN_3C", "VEN_3D", "VEN_3E", "REJECT",
	"UNDEF",
};
char scsi_ops[SCSI_ARYSZ + 1][20] = {
	"TUR",
	"REQ_SENSE",
	"FMT_UNIT",
	"RD_BL",
	"RD6",
	"WR6",
	"INQ",
	"MODE_SEL",
	"RSV6",
	"REL6",
	"MODE_SENSE6",
	"START_STOP_UNIT",
	"RDCAP10",
	"RD10",
	"WR10",
	"WR_V10",
	"PREFETCH10",
	"SYNC10",
	"RD_DEFECT_DATA",
	"WR_BUFFER",
	"WR_SAME10",
	"UNMAP",
	"XDxx",
	"MODE_SEL10",
	"RSV10",
	"REL10",
	"MODE_SENSE10",
	"PR_IN",
	"PR_OUT",
	"XCOPY",
	"RCV_CPY_RSLTS",
	"RD16",
	"ATS",
	"WR16",
	"WR_V16",
	"SYNC16",
	"WR_SAME16",
	"RDCAP_SAIN16",
	"REPORT_LUNS",
	"MAINT_IN",
	"RD12",
	"WR12",
	"WR_V12",
	"ISMXX",
	"UNKNOWN"
};
