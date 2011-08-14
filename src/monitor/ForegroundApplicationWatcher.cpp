#include "ForegroundApplicationWatcher.h"
#include <APGWGNAM.H>
#include "KeypadBuddyServer.h"

CForegroundApplicationWatcher* CForegroundApplicationWatcher::NewL(CKeypadBuddyServer& aServer)
    {
    CForegroundApplicationWatcher* self = new(ELeave) CForegroundApplicationWatcher(aServer);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CForegroundApplicationWatcher::CForegroundApplicationWatcher(CKeypadBuddyServer& aServer) : CActive(CActive::EPriorityStandard),
    iServer(aServer)
    {
    CActiveScheduler::Add(this);
    }

void CForegroundApplicationWatcher::ConstructL()
    {
    User::LeaveIfError(iWsSession.Connect());
    iWg = RWindowGroup(iWsSession);
    User::LeaveIfError(iWg.Construct((TUint32)&iWg, EFalse));
    iWg.SetOrdinalPosition(-1);
    iWg.EnableReceiptOfFocus(EFalse);
    CApaWindowGroupName* wn = CApaWindowGroupName::NewLC(iWsSession);
    wn->SetHidden(ETrue);
    wn->SetWindowGroupName(iWg);
    CleanupStack::PopAndDestroy();
    User::LeaveIfError(iWg.EnableGroupChangeEvents());
    }

CForegroundApplicationWatcher::~CForegroundApplicationWatcher()
    {
    iWg.Close();
    iWsSession.Close();
    }

void CForegroundApplicationWatcher::Watch()
    {
    iStatus = KRequestPending;
    iWsSession.EventReady(&iStatus);
    SetActive();
    }

void CForegroundApplicationWatcher::RunL()
    {
    if (iStatus != KErrCancel)
        {
        Watch();
        }
    if (iStatus == KErrNone)
        {
        TWsEvent e;
        iWsSession.GetEvent(e);
        }
    iServer.ForegroundApplicationChanged();
    }

TUid CForegroundApplicationWatcher::ForegroundAppL() const
    {
    TInt wgid = iWsSession.GetFocusWindowGroup();
    CApaWindowGroupName* gn;
    gn = CApaWindowGroupName::NewLC(iWsSession, wgid);
    TUid ret = gn->AppUid();
    CleanupStack::PopAndDestroy(gn);
    return ret;
    }

void CForegroundApplicationWatcher::DoCancel()
    {
    iWsSession.EventReadyCancel();
    }

