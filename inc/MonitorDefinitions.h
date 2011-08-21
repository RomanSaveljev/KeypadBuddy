#ifndef MONITORDEFINITIONS_H
#define MONITORDEFINITIONS_H

#include <e32def.h>

_LIT(KMonitorServerNameSearch, "KeyPadBuddyMonitor*");
_LIT(KMonitorServerName, "KeyPadBuddyMonitor");
_LIT(KMonitorServerFileName, "keypadbuddymonitor.exe");

enum TMonitorFunctions
    {
    EMonitorDeactivate = 1000,
    EMonitorResetCache
    };

const TInt KArgumentSlot = 8;
_LIT8(KLanguageRestore, "-languagerestore");

#endif //MONITORDEFINITIONS_H
