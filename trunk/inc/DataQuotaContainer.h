/*
Data Quota for Symbian phones.
http://code.google.com/p/dataquota/
Copyright (C) 2008, 2009, 2010, 2011  Hugo van Kemenade

This file is part of Data Quota.

Data Quota is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

Data Quota is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Data Quota.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DATAQUOTACONTAINER_H__
#define __DATAQUOTACONTAINER_H__

// INCLUDES
#include <CoeCntrl.h>

// FORWARD DECLARATIONS
class CAknNavigationControlContainer;
class CAknNavigationDecorator;
class CAknsBasicBackgroundControlContext;
class CDataQuotaTouchFeedbackInterface;
class CDataQuotaView;
class CRepository;

// CONSTANTS
#ifdef __OVI_SIGNED__
_LIT(KVersion, "2.40");
#else
_LIT(KVersion, "1.40");
#endif

// CLASS DECLARATION

/**
* CDataQuotaContainer container control class.
*/
class CDataQuotaContainer : public CCoeControl
	{
	public: // Enumerations
		enum TQuotaType
			{
			EDaily,
			EMonthly
			};

	public: // Constructors and destructor
		CDataQuotaContainer(CDataQuotaView* aView);
		void ConstructL(const TRect& aRect);
		virtual ~CDataQuotaContainer();

	public: // New methods
		void UpdateValuesL();
		void ResetQuota();
		TInt DataQuota();
		void SetDataQuotaL(TInt aDataQuota);
		TInt BillingDay();
		void SetBillingDayL(TInt aBillingDay);
		TBool IsDailyQuotaType();
		void SetQuotaTypeL(TQuotaType aQuotaType);

	private: // New methods
		// Drawing helper methods
		void DrawText(const TDesC& aText, 
					  const TInt& aY, 
					  const TRgb& aPenColor) const;

		void DrawRect(const TRect& aRect, 
					  const TRgb& aPenColor, 
					  const TRgb& aBrushColor) const;

		void LoadResourceFileTextL();
		void LoadSettingsL();
		void SaveSettingsL();

		void DoChangePaneTextL() const;

	private: // From CCoeControl
		TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, 
									TEventCode aType);
		void SizeChanged();
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
		void HandleResourceChange(TInt aType);
		void Draw(const TRect& aRect) const;
		TTypeUid::Ptr MopSupplyObject(TTypeUid aId);

	private: // Data
		CDataQuotaView* iView;
		// text from resource files
		HBufC* iSentText;
		HBufC* iRcvdText;
		HBufC* iUsedText;
		HBufC* iHourText;
		HBufC* iDayText;
		HBufC* iSeperatorText;
		HBufC* iMegabyteText;
		HBufC* iGigabyteText;

		CRepository*  iRepository;
		const CFont* iFont;

		CAknsBasicBackgroundControlContext* iBackground; // for skins support 

		TInt iDataQuota;
		TInt iSentData;
		TInt iRcvdData;

		TDateTime iDateTime;
		TInt iDaysThisPeriod;
		TInt iBillingDay;
		TInt iDaysSinceBillingDay;
		TQuotaType iQuotaType;

		TRect iDataRect;
		TRect iSentRect;
		TRect iRcvdRect;

		TRect iDateRect;
		TRect iNowRect;
		TInt iRectWidth;

		CAknNavigationControlContainer *iNaviContainer;
		CAknNavigationDecorator* iNaviLabelDecorator;

		TPoint iLastTouchPosition;
		CDataQuotaTouchFeedbackInterface* iFeedback;
		TUid iDtorIdKey;
	};

#endif  // __DATAQUOTACONTAINER_H__

// End of file
