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

// INCLUDE FILES
#include <AknAppUi.h>
#include <AknNaviDe.h>
#include <AknNaviLabel.h>
#include <AknUtils.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <centralrepository.h>
#include <DclCRKeys.h>
#include <EikSPane.h>
#include <s32file.h>
#include <StringLoader.h>

#ifdef __S60_50__
#include <DataQuota/DataQuotaTouchFeedbackInterface.h>
#include <touchfeedback.h>
#endif

#include "DataQuota.hrh"
#include "DataQuota.rsg.h"
#include "DataQuotaAppView.h"

const TInt KKilobyte(1024);
const TInt KDataQuota(20 * KKilobyte);
const TInt KBarHeight(20);
const TInt KMaxChars(256);
const TInt KMinutesInHour(60);
const TInt KHoursInDay(24);
const TInt KMargin(10);
const TInt KBufSize(50);

const TRgb KRgbSent(KRgbBlue);
const TRgb KRgbRcvd(KRgbDarkGreen);
const TRgb KRgbNow (KRgbYellow);
const TRgb KRgbOver(KRgbRed);
const TRgb KRgbTransparent(0x00, 0x00, 0x00, 0x00);
#ifdef __OVI_SIGNED__
const TUid KTouchFeedbackImplUid = {0x200427F9};
#else
const TUid KTouchFeedbackImplUid = {0xA89FD5B4};
#endif 

_LIT(KOldSettingsFile, "c:settings.dat");
_LIT(KNewSettingsFile, "c:settings2.dat");

CDataQuotaAppView* CDataQuotaAppView::NewL(const TRect& aRect)
	{
	CDataQuotaAppView* self(CDataQuotaAppView::NewLC(aRect));
	CleanupStack::Pop(self);
	return self;
	}


CDataQuotaAppView* CDataQuotaAppView::NewLC(const TRect& aRect)
	{
	CDataQuotaAppView* self(new (ELeave) CDataQuotaAppView);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}


void CDataQuotaAppView::ConstructL(const TRect& aRect)
	{
#ifdef __S60_50__
	TRAP_IGNORE(iFeedback = 
		static_cast<CDataQuotaTouchFeedbackInterface*>(
			REComSession::CreateImplementationL(
				KTouchFeedbackImplUid, iDtorIdKey)));
#endif

	LoadSettingsL();
	LoadResourceFileTextL();
	
	// Create a window for this application view
	CreateWindowL();

	// set the font
	iFont = iEikonEnv->AnnotationFont();

	// Set the windows size
	SetRect(aRect);

	iBackground = CAknsBasicBackgroundControlContext::NewL(
					KAknsIIDQsnBgAreaMain, Rect(), EFalse);// new a background

	iRepository = CRepository::NewL(KCRUidDCLLogs);

	iNaviContainer = static_cast<CAknNavigationControlContainer*>(iEikonEnv
							->AppUiFactory()->StatusPane()
							->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
	iNaviLabelDecorator = iNaviContainer->CreateNavigationLabelL();
	iNaviContainer->PushL(*iNaviLabelDecorator);
	
	DoChangePaneTextL();
	
	// Activate the window, which makes it ready to be drawn
	ActivateL();
	UpdateValuesL();
	}


CDataQuotaAppView::CDataQuotaAppView()
	{
	// No implementation required
	}


CDataQuotaAppView::~CDataQuotaAppView()
	{
	delete iBackground;
	delete iRepository;
	delete iSentText;
	delete iRcvdText;
	delete iUsedText;
	delete iHourText;
	delete iDayText;
	delete iSeperatorText;
	delete iMegabyteText;

	iNaviContainer->Pop(iNaviLabelDecorator);
	delete iNaviLabelDecorator;
#ifdef __S60_50__
	if (iFeedback)
		{
		delete iFeedback;
		REComSession::DestroyedImplementation(iDtorIdKey);
		}
#endif
	}

void CDataQuotaAppView::LoadResourceFileTextL()
	{
	iSentText 		= StringLoader::LoadL(R_DATAQUOTA_SENT);
	iRcvdText 		= StringLoader::LoadL(R_DATAQUOTA_RCVD);
	iUsedText 		= StringLoader::LoadL(R_DATAQUOTA_USED);
	iHourText 		= StringLoader::LoadL(R_DATAQUOTA_HOUR);
	iDayText 		= StringLoader::LoadL(R_DATAQUOTA_DAY);
	iSeperatorText	= StringLoader::LoadL(R_DATAQUOTA_SEPERATOR);
	iMegabyteText	= StringLoader::LoadL(R_DATAQUOTA_MEGABYTE);
	}


void CDataQuotaAppView::DrawText(const TDesC& aText, const TInt& aY, 
								 const TRgb& aPenColor) const
	{
	CWindowGc& gc(SystemGc());
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenColor(aPenColor);
	
	TBidiText* bidi(TBidiText::NewL(aText, 1));
	bidi->WrapText(iRectWidth, *iFont, NULL);
	bidi->DrawText(gc, TPoint(KMargin, aY));
	delete bidi;
	}


void CDataQuotaAppView::DrawRect(const TRect& aRect, const TRgb& aPenColor, 
								 const TRgb& aBrushColor) const
	{
	CWindowGc& gc(SystemGc());
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenColor(aPenColor);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	gc.SetBrushColor(aBrushColor);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
	}


void CDataQuotaAppView::Draw(const TRect& /*aRect*/) const
	{
	// Note that the whole screen is drawn everytime, 
	// so aRect-parameter is ignored

	// Get the standard graphics context
	CWindowGc& gc(SystemGc());

	 // draw background
	MAknsSkinInstance* skin(AknsUtils::SkinInstance());
	MAknsControlContext* cc(AknsDrawUtils::ControlContext(this));
	AknsDrawUtils::Background(skin, cc, this, gc, Rect());

	gc.UseFont(iFont);

	// Data

	DrawRect(iDataRect, KRgbBlack, KRgbWhite);
	if (iSentData + iRcvdData < iDataQuota)
		{
		DrawRect(iSentRect, KRgbSent, KRgbSent);
		DrawRect(iRcvdRect, KRgbRcvd, KRgbRcvd);
		}
	else
		{
		DrawRect(iSentRect, KRgbOver, KRgbOver);
		DrawRect(iRcvdRect, KRgbOver, KRgbOver);
		}
	
	DrawRect(iDataRect,	KRgbBlack, KRgbTransparent);

	gc.SetPenColor(KRgbNow);
	gc.SetPenStyle(CGraphicsContext::EDottedPen);
	gc.DrawLine(TPoint(iNowRect.iBr.iX, iDataRect.iTl.iY + 1), 
				TPoint(iNowRect.iBr.iX, iDataRect.iBr.iY - 1));

	 // "XXX,XXX.XX", 999,999.99 MB = 976.56249 GB
	TRealFormat format(10, 2);

	TBuf<KMaxChars> sentBuf(*iSentText);
	sentBuf.AppendNum(iSentData / (TReal)KKilobyte, format);
	sentBuf.Append(*iMegabyteText);

	TBuf<KMaxChars> rcvdBuf(*iRcvdText);
	rcvdBuf.AppendNum(iRcvdData / (TReal)KKilobyte, format);
	rcvdBuf.Append(*iMegabyteText);

	TBuf<KMaxChars> usedBuf(*iUsedText);
	usedBuf.AppendNum((iSentData + iRcvdData) / (TReal)KKilobyte, format);
	usedBuf.Append(*iSeperatorText);
	usedBuf.AppendNum(iDataQuota / (TReal)KKilobyte, format);
	usedBuf.Append(*iMegabyteText);
	
	TRgb textColour;
	AknsUtils::GetCachedColor(skin, textColour, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);

	if (iSentData + iRcvdData < iDataQuota)
		{
		DrawText(sentBuf, iDataRect.iTl.iY + (2 * KBarHeight), KRgbSent);
		DrawText(rcvdBuf, iDataRect.iTl.iY + (3 * KBarHeight), KRgbRcvd);
		DrawText(usedBuf, iDataRect.iTl.iY + (4 * KBarHeight), textColour);
		}
	else
		{
		DrawText(sentBuf, iDataRect.iTl.iY + (2 * KBarHeight), KRgbOver);
		DrawText(rcvdBuf, iDataRect.iTl.iY + (3 * KBarHeight), KRgbOver);
		DrawText(usedBuf, iDataRect.iTl.iY + (4 * KBarHeight), KRgbOver);
		}

	// Date

	DrawRect(iDateRect, KRgbBlack, KRgbWhite);
	DrawRect(iNowRect,  KRgbNow,   KRgbNow);
	DrawRect(iDateRect, KRgbBlack, KRgbTransparent);

	gc.SetPenColor(KRgbRcvd);
	gc.SetPenStyle(CGraphicsContext::EDottedPen);
	gc.DrawLine(TPoint(iRcvdRect.iBr.iX, iDateRect.iTl.iY + 1), 
				TPoint(iRcvdRect.iBr.iX, iDateRect.iBr.iY - 1));

	TBuf<KMaxChars> nowBuf;
	if (EDaily == iQuotaType)
		{
		nowBuf.Copy(*iHourText);
		nowBuf.AppendNum(iDateTime.Hour() + 1);
		nowBuf.Append(*iSeperatorText);
		nowBuf.AppendNum(KHoursInDay);
		}
	else // EMonthly
		{
		nowBuf.Copy(*iDayText);
		nowBuf.AppendNum(iDaysSinceBillingDay + 1);
		nowBuf.Append(*iSeperatorText);
		nowBuf.AppendNum(iDaysThisPeriod);
		}


	DrawText(nowBuf, iDateRect.iTl.iY + (2 * KBarHeight), textColour);

	gc.DiscardFont();
	}


void CDataQuotaAppView::SizeChanged()
	{
	UpdateValuesL();
	}


void CDataQuotaAppView::UpdateValuesL()
	{
	const TInt KDataBarY(Size().iHeight*1/4 - KBarHeight);
	const TInt KDateBarY(Size().iHeight*3/4 - KBarHeight);
	iRectWidth = Size().iWidth - (2 * KMargin);

	// Data

#ifdef __WINS__
	const TInt KSentData(0100);
	const TInt KRcvdData(0100);
	iSentData = KSentData * KKilobyte;
	iRcvdData = KRcvdData * KKilobyte;
#else
	if (iRepository)
		{
		TBuf<KBufSize> bytes;
		User::LeaveIfError(iRepository->Get(KLogsGPRSSentCounter, bytes));
		TLex lex(bytes);
		TInt64 integer;
		User::LeaveIfError(lex.Val(integer));
		iSentData = integer / KKilobyte; // Convert bytes to kilobytes
		
		User::LeaveIfError(iRepository->Get(KLogsGPRSReceivedCounter, bytes));
		lex = bytes;
		User::LeaveIfError(lex.Val(integer));
		iRcvdData = integer / KKilobyte; // Convert bytes to kilobytes
		}
#endif

	// MB are too small, bytes are too big, KB are just right
	iDataRect = TRect(TPoint(KMargin, KDataBarY),			TSize(iRectWidth, KBarHeight));
	iSentRect = TRect(TPoint(KMargin, KDataBarY),			TSize(iRectWidth * iSentData/iDataQuota, KBarHeight));
	iRcvdRect = TRect(TPoint(iSentRect.iBr.iX, KDataBarY),	TSize(iRectWidth * iRcvdData/iDataQuota, KBarHeight));

	// Date

	TTime time; // time in microseconds since 0AD nominal Gregorian
	time.HomeTime(); // set time to home time
	iDateTime = time.DateTime(); // convert to fields
	
	switch (iQuotaType)
		{
		case EDaily:
			iNowRect = TRect(TPoint(KMargin, KDateBarY), 
							 TSize(iRectWidth * (iDateTime.Hour() + (iDateTime.Minute()/KMinutesInHour))/KHoursInDay, 
								   KBarHeight));
			break;

		case EMonthly:	// intentional fall-through
		default:		// intentional fall-through
			{
			iDaysSinceBillingDay = 0;
			TInt billingDay(iBillingDay);
			
			iDaysThisPeriod = time.DaysInMonth();
			
			if (billingDay > iDaysThisPeriod)
				{
				billingDay = iDaysThisPeriod;
				}
			
			if (iDateTime.Day() > billingDay)
				{
				iDaysSinceBillingDay = iDateTime.Day() - billingDay;
				}
			else if (iDateTime.Day() < billingDay)
				{
				iDaysSinceBillingDay = iDateTime.Day() - billingDay + iDaysThisPeriod;
				}
			
			iNowRect = TRect(TPoint(KMargin, KDateBarY), TSize(iRectWidth * (iDaysSinceBillingDay+(iDateTime.Hour()/KHoursInDay))/iDaysThisPeriod, KBarHeight));
			}
			break;
		}
	iDateRect = TRect(TPoint(KMargin, KDateBarY), TSize(iRectWidth, KBarHeight));
	}


void CDataQuotaAppView::ResetQuota()
	{
	TBuf<KBufSize> zeroBytes;
	zeroBytes.AppendNum(0);
	iRepository->Set(KLogsGPRSSentCounter, zeroBytes);
	iRepository->Set(KLogsGPRSReceivedCounter, zeroBytes);
	}


TTypeUid::Ptr CDataQuotaAppView::MopSupplyObject(TTypeUid aId)
	{
	if (aId.iUid == MAknsControlContext::ETypeId && iBackground)
		{
		return MAknsControlContext::SupplyMopObject(aId, iBackground);
		}
	return CCoeControl::MopSupplyObject(aId);
	}


TKeyResponse CDataQuotaAppView::OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/, 
												TEventCode /*aType*/)
	{
	UpdateValuesL();
	DrawDeferred();
	return EKeyWasNotConsumed;
	}


TInt CDataQuotaAppView::DataQuota()
	{
	return iDataQuota/KKilobyte;
	}


void CDataQuotaAppView::SetDataQuotaL(TInt aDataQuota)
	{
	iDataQuota = aDataQuota * KKilobyte;
	SaveSettingsL();
	}


TInt CDataQuotaAppView::BillingDay()
	{
	return iBillingDay;
	}


void CDataQuotaAppView::SetBillingDayL(TInt aBillingDay)
	{
	iBillingDay = aBillingDay;
	SaveSettingsL();
	}


void CDataQuotaAppView::SetQuotaTypeL(TQuotaType aQuotaType)
	{
	iQuotaType = aQuotaType;
	SaveSettingsL();
	}


void CDataQuotaAppView::LoadSettingsL()
	{
	iDataQuota = KDataQuota;
	iBillingDay = 0;
	iQuotaType = EMonthly;
	
	// First check if there's a new settings file that uses megabytes
	RFile newFile;
	CleanupClosePushL(newFile);
	TInt openError(newFile.Open(CCoeEnv::Static()->FsSession(), 
								KNewSettingsFile, EFileRead));
	
	if (openError == KErrNone)
		{
		RFileReadStream readStream(newFile);
		CleanupClosePushL(readStream);
		
		iDataQuota = readStream.ReadInt32L(); // kilobytes
		if (iDataQuota < 1)
			{
			iDataQuota = 1;
			}
		iBillingDay = readStream.ReadInt32L();
		
		// Trap new things that aren't there in older versions
		TRAP_IGNORE(iQuotaType = (TQuotaType)readStream.ReadInt32L());
		
		CleanupStack::PopAndDestroy(&readStream);
		}
	else
		{
		// Check if there's an old settings file that uses bytes not kilobytes
		RFile oldFile;
		CleanupClosePushL(oldFile);
		TInt openError(oldFile.Open(CCoeEnv::Static()->FsSession(), 
									KOldSettingsFile, EFileRead));
		
		if (openError == KErrNone)
			{
			RFileReadStream readStream(oldFile);
			CleanupClosePushL(readStream);
			
			// The quota is stored in bytes in the old settings file, 
			// so read in and convert to kilobytes
			iDataQuota = readStream.ReadInt32L();
			iDataQuota /= KKilobyte;
			if (iDataQuota < 1)
				{
				iDataQuota = KDataQuota;
				}
			TRAP_IGNORE(iBillingDay = readStream.ReadInt32L());
			
			CleanupStack::PopAndDestroy(&readStream);
			}
		
		// Save a new settings file and delete the old one
		SaveSettingsL();
		CCoeEnv::Static()->FsSession().Delete(KOldSettingsFile);
		
		CleanupStack::PopAndDestroy(&oldFile);
		}
	
	CleanupStack::PopAndDestroy(&newFile);
	}


void CDataQuotaAppView::SaveSettingsL()
	{
	CCoeEnv::Static()->FsSession().MkDirAll(KNewSettingsFile);
	
	RFile file;
	CleanupClosePushL(file);
	TInt replaceError(file.Replace(CCoeEnv::Static()->FsSession(), 
													KNewSettingsFile, EFileWrite));
	
	if (replaceError == KErrNone)
		{
		RFileWriteStream writeStream(file);
		CleanupClosePushL(writeStream);
		
		writeStream.WriteInt32L(iDataQuota);
		writeStream.WriteInt32L(iBillingDay);
		writeStream.WriteInt32L(iQuotaType);
		
		CleanupStack::PopAndDestroy(&writeStream);
		}
	
	CleanupStack::PopAndDestroy(&file);
	}


void CDataQuotaAppView::DoChangePaneTextL() const
	{
	TBuf<KMaxChars> dateText;
	
	TTime time;
	time.HomeTime(); // set time to home time
	_LIT(KDateFormat, "%/0%1%/1%2%/2%3%/3");
	time.FormatL(dateText, KDateFormat); 
	
	TBuf<KMaxChars> stateText(KVersion);
	_LIT(KSpace, " - ");
	stateText.Append(KSpace);
	stateText.Append(dateText);
	
	static_cast<CAknNaviLabel*>(iNaviLabelDecorator->DecoratedControl())->
														SetTextL(stateText);
	iNaviContainer->Pop();
	iNaviContainer->PushL(*iNaviLabelDecorator);
	}

void CDataQuotaAppView::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	// Check if they have touched any of the buttons.
	// If so, issue a command.
	
	TInt command(EDataQuotaRefresh);

	if (aPointerEvent.iType == TPointerEvent::EButton1Down ||
		aPointerEvent.iType == TPointerEvent::EButton1Up)
		{
		if (aPointerEvent.iType == TPointerEvent::EButton1Down)
			{
			iLastTouchPosition = aPointerEvent.iPosition;
			}
		
		if (iDataRect.Contains(aPointerEvent.iPosition) &&
			iDataRect.Contains(iLastTouchPosition))
			{
			command = EDataQuotaEditQuota;
			}
		else if (EMonthly == iQuotaType && 
				 iDateRect.Contains(aPointerEvent.iPosition) &&
				 iDateRect.Contains(iLastTouchPosition))
			{
			command = EDataQuotaEditBillingDay;
			}
		}

	if (command != EDataQuotaRefresh && iFeedback)
		{
#ifdef __S60_50__
		iFeedback->InstantFeedback(ETouchFeedbackBasic);
#endif
		}

	if (aPointerEvent.iType == TPointerEvent::EButton1Up)
		{
		iAvkonAppUi->HandleCommandL(command);
		}
	
	CCoeControl::HandlePointerEventL(aPointerEvent);
	UpdateValuesL();
	DrawDeferred();
	}

/* void CDataQuotaAppView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
	{
	if (R_DATAQUOTA_EDIT_MENU_PANE == aResourceId)
		{
		TBool dailyQuota(EDaily == iQuotaType);
		// Hide if daily:
		aMenuPane->SetItemDimmed(EDataQuotaEditDailyQuota, dailyQuota);
		// Hide if monthly:
		aMenuPane->SetItemDimmed(EDataQuotaEditMonthlyQuota, !dailyQuota);
		aMenuPane->SetItemDimmed(EDataQuotaEditBillingDay, !dailyQuota);
		}
	
	// Third edition only due to an S60 5th edition bug (issue 364)
//	aMenuPane->EnableMarqueeL(!iMobblerStatusControl->IsFifthEdition());
	} */

// End of file
