/*
 * Channel Mode +Q.
 * (C) Copyright 2005-2006 The UnrealIRCd team.
 */

#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "proto.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef STRIPBADWORDS
#include "badwords.h"
#endif
#ifdef _WIN32
#include "version.h"
#endif


ModuleHeader MOD_HEADER(nokicks)
  = {
	"nokicks",
	"$Id$",
	"Channel Mode +Q",
	"3.2-b8-1",
	NULL,
    };

ModuleInfo *ModInfo = NULL;

Cmode_t EXTMODE_NOKICKS = 0L;

#define IsNoKicks(x)  (x->mode.extmode & EXTMODE_NOKICKS)


char *nokicks_can_kick(aClient *sptr, aClient *who, aChannel *chptr, char *comment, long sptr_flags, long who_flags);

DLLFUNC int MOD_INIT(nokicks)(ModuleInfo *modinfo)
{
	CmodeInfo req;
	ModuleSetOptions(modinfo->handle, MOD_OPT_PERM);
	MARK_AS_OFFICIAL_MODULE(modinfo);
	memset(&req, 0, sizeof(req));
	ModInfo = modinfo;

	req.flag = 'Q';
	req.paracount = 0;
	req.is_ok = extcmode_default_requirechop;
	CmodeAdd(modinfo->handle, req, &EXTMODE_NOKICKS);

	HookAddPCharEx(modinfo->handle, HOOKTYPE_CAN_KICK, nokicks_can_kick);
	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(nokicks)(int module_load)
{
	return MOD_SUCCESS;
}


DLLFUNC int MOD_UNLOAD(nokicks)(int module_unload)
{
	return MOD_SUCCESS;
}

char *nokicks_can_kick(aClient *sptr, aClient *who, aChannel *chptr, char *comment, long sptr_flags, long who_flags)
{
static char retbuf[512];

	if (IsNoKicks(chptr))
	{
		ircsprintf(retbuf, err_str(ERR_CANNOTDOCOMMAND), me.name, sptr->name, "KICK", "channel is +Q");
		return retbuf;
	}
	return NULL;
}