#include "PatcherDialog.h"
#include <e32base.h>
#include <eiknotapi.h>
#include <implementationproxy.h>
#include "KeypadBuddyUids.hrh"

LOCAL_C void CreateNotifiersL(CArrayPtrFlat<MEikSrvNotifierBase2>* aNotifiers)
    {
    MEikSrvNotifierBase2* notifier;
    notifier = new(ELeave) CPatcherDialog();
    CleanupStack::PushL(notifier);
    aNotifiers->AppendL(notifier);
    CleanupStack::Pop(notifier);
    }

CArrayPtr<MEikSrvNotifierBase2>* NotifierArray()
    {
    CArrayPtrFlat<MEikSrvNotifierBase2>* notifiers = new CArrayPtrFlat<MEikSrvNotifierBase2> (5);
    if (notifiers)
        {
        TRAPD( err, CreateNotifiersL( notifiers ) );
        if (err)
            {
            TInt count = notifiers->Count();
            while (count--)
                {
                (*notifiers)[count]->Release();
                }
            delete notifiers;
            notifiers = NULL;
            }
        }
    return notifiers;
    }

const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KKeypadBuddyNotifierUidValue, NotifierArray )
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
