/*
 ============================================================================
 Name		: KeypadBuddyApplication.h
 Author	  :
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __KEYPADBUDDYAPPLICATION_H__
#define __KEYPADBUDDYAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "KeypadBuddy.hrh"

// CLASS DECLARATION

/**
 * CKeypadBuddyApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CKeypadBuddyApplication is the application part of the
 * AVKON application framework for the KeypadBuddy example application.
 */
class CKeypadBuddyApplication : public CAknApplication
    {
public:
    // Functions from base classes

    CKeypadBuddyApplication();

    /**
     * From CApaApplication, AppDllUid.
     * @return Application's UID (KUidKeypadBuddyApp).
     */
    TUid AppDllUid() const;

    inline TBool LanguageRestore() const
        {
        return iLanguageRestore;
        }

protected:
    // Functions from base classes

    /**
     * From CApaApplication, CreateDocumentL.
     * Creates CKeypadBuddyDocument document object. The returned
     * pointer in not owned by the CKeypadBuddyApplication object.
     * @return A pointer to the created document object.
     */
    CApaDocument* CreateDocumentL();

    TFileName ResourceFileName() const;

private:
    TBool iLanguageRestore;
    };

#endif // __KEYPADBUDDYAPPLICATION_H__
// End of File
