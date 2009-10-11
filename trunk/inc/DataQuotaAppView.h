/*
DataQuotaAppView.h

Data Quota for S60 phones.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __DATAQUOTAAPPVIEW_H__
#define __DATAQUOTAAPPVIEW_H__

// CONSTANTS
_LIT(KVersion, "0.1.11");

// INCLUDES
#include <centralrepository.h>
#include <coecntrl.h>

// FORWARD DECLARATIONS
class CAknNavigationControlContainer;
class CAknNavigationDecorator;

// CLASS DECLARATION
class CDataQuotaAppView : public CCoeControl
	{

	public: // New methods
		static CDataQuotaAppView* NewL(const TRect& aRect);
		static CDataQuotaAppView* NewLC(const TRect& aRect);
		virtual ~CDataQuotaAppView();

	public:  // Functions from base classes
		void Draw(const TRect& aRect) const;

	public: // New methods
		void UpdateValuesL();
		TInt DataQuota();
		void SetDataQuota(TInt aDataQuota);
		TInt BillingDay();
		void SetBillingDay(TInt aBillingDay);

	private: // from CCoeControl
		virtual void SizeChanged();
		TTypeUid::Ptr MopSupplyObject(TTypeUid aId);
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, 
									TEventCode aType);

	private: // Constructors
		void ConstructL(const TRect& aRect);
		CDataQuotaAppView();

	// Drawing helper methods
		void DrawText(const TDesC& aText, 
					  const TPoint& aPoint, 
					  const TRgb& aPenColor) const;

		void DrawText(const TDesC& aText, 
					  const TRect& aRect, 
					  const TRgb& aPenColor, 
					  CGraphicsContext::TTextAlign aTextAlign, 
					  TInt aOffset) const;

		void DrawRect(const TRect& aRect, 
					  const TRgb& aPenColor, 
					  const TRgb& aBrushColor) const;

		void LoadResourceFileTextL();
		void LoadSettingsL();
		void SaveSettingsL();

		void DoChangePaneTextL() const;

	private:
		// text from resource files
		HBufC* iSentText;
		HBufC* iRcvdText;
		HBufC* iUsedText;
		HBufC* iDayText;
		HBufC* iSeperatorText;
		HBufC* iMegabyteText;

		CRepository*  iRepository;
		const CFont* iFont;

		MAknsControlContext* iBackground; // for skins support 

		TInt iDataQuota;

		TInt iSentData;
		TInt iRcvdData;

		TDateTime iDateTime;
		TInt iDaysThisPeriod;
		TInt iBillingDay;
		TInt iDaysSinceBillingDay;

		TRect iDataRect;
		TRect iSentRect;
		TRect iRcvdRect;

		TRect iDateRect;
		TRect iNowRect;

		CAknNavigationControlContainer *iNaviContainer;
		CAknNavigationDecorator* iNaviLabelDecorator;
	};

#endif // __DATAQUOTAAPPVIEW_H__

// End of file
