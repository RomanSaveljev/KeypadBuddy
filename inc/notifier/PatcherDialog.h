#ifndef PATCHERDIALOG_H
#define PATCHERDIALOG_H

#include <eiknotapi.h>

class CPatcherDialog : public CBase, public MEikSrvNotifierBase2
    {
public:
    void Release();
    TNotifierInfo RegisterL();
    TNotifierInfo Info() const;
    TPtrC8 StartL(const TDesC8& aBuffer);
    void StartL(const TDesC8& aBuffer, TInt aReplySlot, const RMessagePtr2& aMessage);
    void Cancel();
    TPtrC8 UpdateL(const TDesC8& aBuffer);
    };

#endif //PATCHERDIALOG_H
