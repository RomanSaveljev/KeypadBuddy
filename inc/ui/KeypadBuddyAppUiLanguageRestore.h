#ifndef KEYPADBUDDYAPPUILANGUAGERESTORE_H
#define KEYPADBUDDYAPPUILANGUAGERESTORE_H

#include <aknappui.h>
#include <eikedwin.h>

class CEikEdwin;

class CMyEdWin : public CEikEdwin
    {
public:
    void ConstructL()
        {
        CreateWindowL();
        CEikEdwin::ConstructL();
        }
    };

class CKeypadBuddyAppUiLanguageRestore : public CAknAppUi
    {
public:
    ~CKeypadBuddyAppUiLanguageRestore();
    void ConstructL();
private:
    static TInt CallBack(TAny* aSelf);
private:
    CAsyncCallBack* iAsyncCallBack;
    CPeriodic* iPeriodic;
    CMyEdWin* iEdWin;
    };

#endif //KEYPADBUDDYAPPUILANGUAGERESTORE_H
