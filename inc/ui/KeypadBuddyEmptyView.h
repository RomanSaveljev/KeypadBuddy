#ifndef KEYPADBUDDYEMPTYVIEW_H
#define KEYPADBUDDYEMPTYVIEW_H

#include <coecntrl.h>

class CKeypadBuddyEmptyView : public CCoeControl
    {
public:
    static CKeypadBuddyEmptyView* NewL();
private:
    CKeypadBuddyEmptyView();
    void ConstructL();
    };

#endif //KEYPADBUDDYEMPTYVIEW_H
