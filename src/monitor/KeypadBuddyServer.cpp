#include "KeypadBuddyServer.h"
#include "KeypadBuddySession.h"
#include "ForegroundApplicationWatcher.h"
#include "InputMethodWatcher.h"
#include "MonitorDefinitions.h"

const TUid KRepositoryUid = {0x20057507};

const TUid KCRUidAknFep = { 0x101F876D };
const TUint32 KAknFepChineseInputMode = 0x00000004;
const TUint32 KAknFepInputTxtLang = 0x00000005;
const TUint32 KAknFepJapanesePredTxtFlag = 0x00000006;
const TUint32 KAknFepPredTxtFlag = 0x00000007;
const TUint32 KAknFepCangJieMode = 0x00000009;

typedef TBuf8<40> TInputMethodSettingsBuffer;

struct TInputMethodSettings
    {
    TInt iChineseInputMode;
    TInt iInputTxtLang;
    TInt iJapanesePredTxtFlag;
    TInt iPredTxtFlag;
    TInt iCangJieMode;
    void Format(TInputMethodSettingsBuffer& aBuffer)
        {
        _LIT8(KFormat, "%08x");
        aBuffer.Zero();
        aBuffer.AppendFormat(KFormat, iChineseInputMode);
        aBuffer.AppendFormat(KFormat, iInputTxtLang);
        aBuffer.AppendFormat(KFormat, iJapanesePredTxtFlag);
        aBuffer.AppendFormat(KFormat, iPredTxtFlag);
        aBuffer.AppendFormat(KFormat, iCangJieMode);
        }
    TInt Parse(const TInputMethodSettingsBuffer& aBuffer)
        {
        TInt ret = KErrNone;
        TLex8 lex(aBuffer);
        TUint chineseInputMode, inputTxtLang, japanesePredTxtFlag, predTxtFlag, cangJieMode;
        if (KErrNone != lex.Val(chineseInputMode, EHex) ||
            KErrNone != lex.Val(inputTxtLang, EHex) ||
            KErrNone != lex.Val(japanesePredTxtFlag, EHex) ||
            KErrNone != lex.Val(predTxtFlag, EHex) ||
            KErrNone != lex.Val(cangJieMode, EHex))
            {
            ret = KErrCorrupt;
            }
        else
            {
            iChineseInputMode = chineseInputMode;
            iInputTxtLang = inputTxtLang;
            iJapanesePredTxtFlag = japanesePredTxtFlag;
            iPredTxtFlag = predTxtFlag;
            iCangJieMode = cangJieMode;
            }
        return ret;
        }
    };

CKeypadBuddyServer* CKeypadBuddyServer::NewL()
    {
    CKeypadBuddyServer* self = new(ELeave) CKeypadBuddyServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CKeypadBuddyServer::CKeypadBuddyServer() : CServer2(CActive::EPriorityStandard)
    {}

void CKeypadBuddyServer::ConstructL()
    {
    StartL(KMonitorServerName);
    iRepository = CRepository::NewL(KRepositoryUid);
    iFepRepository = CRepository::NewL(KCRUidAknFep);
    iForegroundAppWatcher = CForegroundApplicationWatcher::NewL(*this);
    iChineseInputModeWatcher = new(ELeave) CInputMethodWatcher(*this);
    iInputTxtLangWatcher = new(ELeave) CInputMethodWatcher(*this);
    iJapanesePredTxtFlagWatcher = new(ELeave) CInputMethodWatcher(*this);
    iPredTxtFlagWatcher = new(ELeave) CInputMethodWatcher(*this);
    iCangJieModeWatcher = new(ELeave) CInputMethodWatcher(*this);
    StartWatchingFepKeysL();
    }

CKeypadBuddyServer::~CKeypadBuddyServer()
    {
    delete iForegroundAppWatcher;
    delete iChineseInputModeWatcher;
    delete iInputTxtLangWatcher;
    delete iJapanesePredTxtFlagWatcher;
    delete iPredTxtFlagWatcher;
    delete iCangJieModeWatcher;
    delete iRepository;
    delete iFepRepository;
    }

CSession2* CKeypadBuddyServer::NewSessionL( const TVersion& /*aVersion*/, const RMessage2& /*aMessage*/ ) const
    {
    return new(ELeave) CKeypadBuddySession(const_cast<CKeypadBuddyServer&>(*this));
    }

void CKeypadBuddyServer::ForegroundApplicationChanged()
    {
    TRAP(iLastError, ForegroundApplicationChangedL());
    }

void CKeypadBuddyServer::ForegroundApplicationChangedL()
    {
    TUid uid = iForegroundAppWatcher->ForegroundAppL();
    if (uid != TUid::Null())
        {
        TInputMethodSettingsBuffer buffer;
        if (KErrNone == iRepository->Get(uid.iUid, buffer))
            {
            TInputMethodSettings settings;
            if (KErrNone == settings.Parse(buffer))
                {
                CancelWatchingFepKeys();
                TInt err = KErrNone;
                do
                    {
                    User::LeaveIfError(iFepRepository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));
                    User::LeaveIfError(iFepRepository->Set(KAknFepChineseInputMode, settings.iChineseInputMode));
                    User::LeaveIfError(iFepRepository->Set(KAknFepInputTxtLang, settings.iInputTxtLang));
                    User::LeaveIfError(iFepRepository->Set(KAknFepJapanesePredTxtFlag, settings.iJapanesePredTxtFlag));
                    User::LeaveIfError(iFepRepository->Set(KAknFepPredTxtFlag, settings.iPredTxtFlag));
                    User::LeaveIfError(iFepRepository->Set(KAknFepCangJieMode, settings.iCangJieMode));
                    TUint32 keyInfo;
                    err = iFepRepository->CommitTransaction(keyInfo);
                    __ASSERT_ALWAYS(err == KErrNone || err == KErrLocked, User::Leave(err));
                    }
                while(err == KErrLocked);
                StartWatchingFepKeysL();
                }
            else
                {
                // remove corrupted value
                User::LeaveIfError(iRepository->Delete(uid.iUid));
                }
            }
        // no specific settings setup by the user
        }
    // no else, no tracking of applications without UID3
    }

void CKeypadBuddyServer::InputMethodSettingsChanged()
    {
    TRAP(iLastError, InputMethodSettingsChangedL());
    }

void CKeypadBuddyServer::InputMethodSettingsChangedL()
    {
    TUid uid = iForegroundAppWatcher->ForegroundAppL();
    if (uid != TUid::Null())
        {
        TInputMethodSettings settings;
        User::LeaveIfError(iFepRepository->Get(KAknFepChineseInputMode, settings.iChineseInputMode));
        User::LeaveIfError(iFepRepository->Get(KAknFepInputTxtLang, settings.iInputTxtLang));
        User::LeaveIfError(iFepRepository->Get(KAknFepJapanesePredTxtFlag, settings.iJapanesePredTxtFlag));
        User::LeaveIfError(iFepRepository->Get(KAknFepPredTxtFlag, settings.iPredTxtFlag));
        User::LeaveIfError(iFepRepository->Get(KAknFepCangJieMode, settings.iCangJieMode));
        TInputMethodSettingsBuffer buffer;
        settings.Format(buffer);
        User::LeaveIfError(iRepository->Set(uid.iUid, buffer));
        }
    // no else, no tracking for applications without UID3
    }

CRepository& CKeypadBuddyServer::Repository()
    {
    return *iRepository;
    }

CRepository& CKeypadBuddyServer::FepRepository()
    {
    return *iFepRepository;
    }

void CKeypadBuddyServer::StartWatchingFepKeysL()
    {
    iChineseInputModeWatcher->WatchKeyL(KAknFepChineseInputMode);
    iInputTxtLangWatcher->WatchKeyL(KAknFepInputTxtLang);
    iJapanesePredTxtFlagWatcher->WatchKeyL(KAknFepJapanesePredTxtFlag);
    iPredTxtFlagWatcher->WatchKeyL(KAknFepPredTxtFlag);
    iCangJieModeWatcher->WatchKeyL(KAknFepCangJieMode);
    }

void CKeypadBuddyServer::CancelWatchingFepKeys()
    {
    iChineseInputModeWatcher->Cancel();
    iInputTxtLangWatcher->Cancel();
    iJapanesePredTxtFlagWatcher->Cancel();
    iPredTxtFlagWatcher->Cancel();
    iCangJieModeWatcher->Cancel();
    }




void RunServerL()
    {
    CActiveScheduler* scheduler = new ( ELeave ) CActiveScheduler();
    CleanupStack::PushL( scheduler );
    CActiveScheduler::Install( scheduler );
    CKeypadBuddyServer* server = CKeypadBuddyServer::NewL();
    RProcess me;
    me.Rendezvous(KErrNone);
    CActiveScheduler::Start();
    delete server;
    CleanupStack::PopAndDestroy( scheduler );
    }

TInt E32Main()
    {
    TInt result;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    if ( cleanup )
        {
        TRAP(result, RunServerL());
        }
    else
        {
        result = KErrNoMemory;
        }
    delete cleanup;
    return result;
    }
