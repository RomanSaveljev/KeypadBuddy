/*
 ============================================================================
 Name		: KeypadBuddyAppUi.h
 Author	  :
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __KEYPADBUDDYAPPUI_h__
#define __KEYPADBUDDYAPPUI_h__

// INCLUDES
#include <aknappui.h>
#include "KeypadBuddyEngine.h"
#include <eikmenup.h>

// FORWARD DECLARATIONS
class CKeypadBuddyAppView;
class CKeypadBuddyEmptyView;

// CLASS DECLARATION
/**
 * CKeypadBuddyAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CKeypadBuddyAppUi : public CAknAppUi, public MMonitorStateObserver
    {
public:
    // Constructors and destructor

    /**
     * ConstructL.
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * CKeypadBuddyAppUi.
     * C++ default constructor. This needs to be public due to
     * the way the framework constructs the AppUi
     */
    CKeypadBuddyAppUi(TBool aCreateEmptyView);

    /**
     * ~CKeypadBuddyAppUi.
     * Virtual Destructor.
     */
    virtual ~CKeypadBuddyAppUi();

    void MonitorDeactivated();
    void MonitorActivated();

    void HandleForegroundEventL(TBool aForeground);

private:
    // Functions from base classes

    /**
     * From CEikAppUi, HandleCommandL.
     * Takes care of command handling.
     * @param aCommand Command to be handled.
     */
    void HandleCommandL(TInt aCommand);

    /**
     *  HandleStatusPaneSizeChange.
     *  Called by the framework when the application status pane
     *  size is changed.
     */
    void HandleStatusPaneSizeChange();
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
private:
    // Data

    /**
     * The application view
     * Owned by CKeypadBuddyAppUi
     */
    CKeypadBuddyAppView* iAppView;
    CKeypadBuddyEmptyView* iEmptyView;
    CKeypadBuddyEngine* iEngine;
    TBool iRestoreLanguage;
    };

#endif // __KEYPADBUDDYAPPUI_h__
// End of File
