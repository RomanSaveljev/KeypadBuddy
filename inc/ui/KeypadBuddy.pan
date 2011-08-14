/*
 ============================================================================
 Name		: KeypadBuddy.pan
 Author	  : 
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __KEYPADBUDDY_PAN__
#define __KEYPADBUDDY_PAN__

/** KeypadBuddy application panic codes */
enum TKeypadBuddyPanics
    {
    EKeypadBuddyUi = 1
    // add further panics here
    };

inline void Panic(TKeypadBuddyPanics aReason)
    {
    _LIT(applicationName, "KeypadBuddy");
    User::Panic(applicationName, aReason);
    }

#endif // __KEYPADBUDDY_PAN__
