#ifndef KEYPADBUDDYSESSION_H
#define KEYPADBUDDYSESSION_H

#include <e32base.h>

class CKeypadBuddyServer;

class CKeypadBuddySession : public CSession2
    {
public:
    CKeypadBuddySession(CKeypadBuddyServer& aServer);
    void ServiceL(const RMessage2& aMessage);
private:
    CKeypadBuddyServer& iServer;
    };

#endif //KEYPADBUDDYSESSION_H
