/*
 ============================================================================
 Name		: KeypadBuddyAppView.h
 Author	  :
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __KEYPADBUDDYAPPVIEW_h__
#define __KEYPADBUDDYAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>
#include <eikimage.h>
#include <eikmenup.h>

class CAknsBasicBackgroundControlContext;

// CLASS DECLARATION
class CKeypadBuddyAppView : public CCoeControl
    {
public:
    // New methods

    /**
     * NewL.
     * Two-phased constructor.
     * Create a CKeypadBuddyAppView object, which will draw itself to aRect.
     * @param aRect The rectangle this view will be drawn to.
     * @return a pointer to the created instance of CKeypadBuddyAppView.
     */
    static CKeypadBuddyAppView* NewL(const TRect& aRect, TBool aMonitorActive);

    /**
     * NewLC.
     * Two-phased constructor.
     * Create a CKeypadBuddyAppView object, which will draw itself
     * to aRect.
     * @param aRect Rectangle this view will be drawn to.
     * @return A pointer to the created instance of CKeypadBuddyAppView.
     */
    static CKeypadBuddyAppView* NewLC(const TRect& aRect, TBool aMonitorActive);

    /**
     * ~CKeypadBuddyAppView
     * Virtual Destructor.
     */
    virtual ~CKeypadBuddyAppView();

    void SetMonitorActiveL(TBool aActive);

public:
    // Functions from base classes

    /**
     * From CCoeControl, Draw
     * Draw this CKeypadBuddyAppView to the screen.
     * @param aRect the rectangle of this view that needs updating
     */
    void Draw(const TRect& aRect) const;

    /**
     * From CoeControl, SizeChanged.
     * Called by framework when the view size is changed.
     */
    virtual void SizeChanged();

    /**
     * From CoeControl, HandlePointerEventL.
     * Called by framework when a pointer touch event occurs.
     * Note: although this method is compatible with earlier SDKs,
     * it will not be called in SDKs without Touch support.
     * @param aPointerEvent the information about this event
     */
    virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);

    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

    TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
private:
    // Constructors

    /**
     * ConstructL
     * 2nd phase constructor.
     * Perform the second phase construction of a
     * CKeypadBuddyAppView object.
     * @param aRect The rectangle this view will be drawn to.
     */
    void ConstructL(const TRect& aRect);

    /**
     * CKeypadBuddyAppView.
     * C++ default constructor.
     */
    CKeypadBuddyAppView(TBool aMonitorActive);

    void SizeChangedL(TBool aMonitorActive);

    TBool iMonitorActive;
    CFbsBitmap* iBitmap;
    CFbsBitmap* iMask;
    CEikImage* iImage;
    CEikMenuPane* iMenuPane;
    CAknsBasicBackgroundControlContext* iBgContext;
    };

#endif // __KEYPADBUDDYAPPVIEW_h__
// End of File
