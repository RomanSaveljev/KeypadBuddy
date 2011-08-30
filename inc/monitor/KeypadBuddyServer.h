#ifndef KEYPADBUDDYSERVER_H
#define KEYPADBUDDYSERVER_H

#include <e32base.h>
#include <centralrepository.h>
#include <f32file.h>

class CForegroundApplicationWatcher;
class CInputMethodWatcher;
class CDictionaryFileStore;
class TInputMethodSettings;

class CKeypadBuddyServer : public CServer2
    {
public:
    static CKeypadBuddyServer* NewL(RFs& aFs);
    ~CKeypadBuddyServer();
    CSession2* NewSessionL( const TVersion& aVersion, const RMessage2& aMessage ) const;
    void ForegroundApplicationChanged();
    void InputMethodSettingsChanged();
    CRepository& FepRepository();
    void ResetCacheL();
    void DeactivateServerL();
    static void RunServerL();
private:
    CKeypadBuddyServer(RFs& aFs);
    void ConstructL();
    void ForegroundApplicationChangedL();
    void InputMethodSettingsChangedL();
    void StartWatchingFepKeysL();
    void CancelWatchingFepKeys();
    static CDictionaryFileStore* CreateFileStoreLC(RFs& aFs, TUint32 aStreamUid);
    static void WriteActivationEnabledSettingL(CDictionaryFileStore& aFileStore, TBool aValue);
    static void ResumeWatchForegroundApplicationCleanupOperation(TAny* aForegroundWatcher);
    static void ResumeWatchingFepKeysCleanupOperation(TAny* aSelf);
    void InitiateInputMethodSettingsFromFepRepository(TInputMethodSettings& aSettings) const;
private:
    RFs& iFs;
    TInt iLastError;
    CRepository* iFepRepository;
    CForegroundApplicationWatcher* iForegroundAppWatcher;
    CInputMethodWatcher* iChineseInputModeWatcher;
    CInputMethodWatcher* iInputTxtLangWatcher;
    CInputMethodWatcher* iJapanesePredTxtFlagWatcher;
    CInputMethodWatcher* iPredTxtFlagWatcher;
    CInputMethodWatcher* iCangJieModeWatcher;
    };

#endif //KEYPADBUDDYSERVER_H
