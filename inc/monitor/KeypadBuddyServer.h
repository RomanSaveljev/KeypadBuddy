#ifndef KEYPADBUDDYSERVER_H
#define KEYPADBUDDYSERVER_H

#include <e32base.h>
#include <centralrepository.h>

class CForegroundApplicationWatcher;
class CInputMethodWatcher;

class CKeypadBuddyServer : public CServer2
    {
public:
    static CKeypadBuddyServer* NewL();
    ~CKeypadBuddyServer();
    CSession2* NewSessionL( const TVersion& aVersion, const RMessage2& aMessage ) const;
    void ForegroundApplicationChanged();
    void InputMethodSettingsChanged();
    CRepository& Repository();
    CRepository& FepRepository();
private:
    CKeypadBuddyServer();
    void ConstructL();
    void ForegroundApplicationChangedL();
    void InputMethodSettingsChangedL();
    void StartWatchingFepKeysL();
    void CancelWatchingFepKeys();
private:
    TInt iLastError;
    CRepository* iRepository;
    CRepository* iFepRepository;
    CForegroundApplicationWatcher* iForegroundAppWatcher;
    CInputMethodWatcher* iChineseInputModeWatcher;
    CInputMethodWatcher* iInputTxtLangWatcher;
    CInputMethodWatcher* iJapanesePredTxtFlagWatcher;
    CInputMethodWatcher* iPredTxtFlagWatcher;
    CInputMethodWatcher* iCangJieModeWatcher;
    };

#endif //KEYPADBUDDYSERVER_H
