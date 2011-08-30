#ifndef MONITORDEFINITIONS_H
#define MONITORDEFINITIONS_H

#include <e32def.h>
#include "KeypadBuddyUids.hrh"

_LIT(KMonitorServerNameSearch, "KeyPadBuddyMonitor*");
_LIT(KMonitorServerName, "KeyPadBuddyMonitor");
_LIT(KMonitorServerFileName, "keypadbuddymonitor.exe");

enum TMonitorFunctions
    {
    EMonitorDeactivate = 1000,
    EMonitorResetCache
    };

const TInt KSettingsRootStreamUidValue = KKeypadBuddyMonitorUidValue;
const TInt KOwnSettingsStreamUidValue = KKeypadBuddyMonitorUidValue;

const TInt KForceActivateArgumentSlot = 9;

#endif //MONITORDEFINITIONS_H
