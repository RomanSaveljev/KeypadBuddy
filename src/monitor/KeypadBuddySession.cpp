#include "KeypadBuddySession.h"
#include "MonitorDefinitions.h"
#include <centralrepository.h>
#include "KeypadBuddyServer.h"

CKeypadBuddySession::CKeypadBuddySession(CKeypadBuddyServer& aServer) :
    iServer(aServer)
    {}

void CKeypadBuddySession::ServiceL(const RMessage2& aMessage)
    {
    switch (aMessage.Function())
        {
        case EMonitorDeactivate:
            {
            iServer.DeactivateServerL();
            aMessage.Complete(KErrNone);
            break;
            }
        case EMonitorResetCache:
            {
            iServer.ResetCacheL();
            aMessage.Complete(KErrNone);
            }
            break;
        default:
            {
            aMessage.Panic(_L("KeypadBuddyMonitor"), 1);
            break;
            }
        }
    }
