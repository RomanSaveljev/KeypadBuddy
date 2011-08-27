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

const TInt KArgumentSlot = 8;
_LIT8(KLanguageRestore, "-languagerestore");

const TInt KForceActivateArgumentSlot = 9;

const TUint32 KCachedFepSettingsStream = KKeypadBuddyMonitorUidValue;
const TUint32 KMonitorSettingsStream = KCachedFepSettingsStream + 1;
const TUint32 KActivationEnabledSetting = 1;

#endif //MONITORDEFINITIONS_H
