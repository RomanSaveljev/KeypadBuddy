#ifndef FOREGROUNDAPPLICATIONWATCHER_H
#define FOREGROUNDAPPLICATIONWATCHER_H

//#include <e32base.h>
#include <w32std.h>

class CKeypadBuddyServer;

class CForegroundApplicationWatcher : public CActive
    {
public:
    static CForegroundApplicationWatcher* NewL(CKeypadBuddyServer& aServer);
    ~CForegroundApplicationWatcher();
    TUid ForegroundAppL();
    inline RWsSession& WsSession() {return iWsSession;}
    inline RWindowGroup& WindowGroup() {return iWg;}
private:
    CForegroundApplicationWatcher(CKeypadBuddyServer& aServer);
    void ConstructL();
    void RunL();
    void DoCancel();
    void Watch();
private:
    CKeypadBuddyServer& iServer;
    RWsSession iWsSession;
    RWindowGroup iWg;
    };

#endif //FOREGROUNDAPPLICATIONWATCHER_H
