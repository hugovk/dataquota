/*
DataQuotaAppView.cpp

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

// INCLUDE FILES
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <AknNaviDe.h>
#include <AknNaviLabel.h>
#include <AknUtils.h>
#include <CoeMain.h>
#include <DclCRKeys.h>
#include <EikSPane.h>
#include <s32file.h>
#include <StringLoader.h>

#include <DataQuota.rsg>
#include "DataQuotaAppView.h"

const TInt KKilobyte(1024);
const TInt KDataQuota(20 * KKilobyte);
const TInt KBarHeight(20);

const TRgb KRgbSent(KRgbBlue);
const TRgb KRgbRcvd(KRgbDarkGreen);
const TRgb KRgbNow (KRgbYellow);
const TRgb KRgbOver(KRgbRed);
const TRgb KRgbTransparent(0x00, 0x00, 0x00, 0x00);

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

	if(iRepository)
		{
		delete iRepository;
		iRepository = NULL;
		}

	delete iSentText;
	delete iRcvdText;
	delete iUsedText;
	delete iDayText;
	delete iSeperatorText;
	delete iMegabyteText;

	iNaviContainer->Pop(iNaviLabelDecorator);
	delete iNaviLabelDecorator;
	}

void CDataQuotaAppView::LoadResourceFileTextL()
	{
	iSentText 		= StringLoader::LoadL(R_DATAQUOTA_SENT);
	iRcvdText 		= StringLoader::LoadL(R_DATAQUOTA_RCVD);
	iUsedText 		= StringLoader::LoadL(R_DATAQUOTA_USED);
	iDayText 		= StringLoader::LoadL(R_DATAQUOTA_DAY);
	iSeperatorText	= StringLoader::LoadL(R_DATAQUOTA_SEPERATOR);
	iMegabyteText	= StringLoader::LoadL(R_DATAQUOTA_MEGABYTE);
	}


void CDataQuotaAppView::DrawText(const TDesC& aText, const TPoint& aPoint, 
								 const TRgb& aPenColor) const
	{
	CWindowGc& gc(SystemGc());
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenColor(aPenColor);
	gc.SetBrushColor(TRgb(255, 255, 255, 0));
	gc.SetBrushStyle(CGraphicsContext::ENullBrush);
	gc.DrawText(aText, aPoint);
	}


void CDataQuotaAppView::DrawText(const TDesC& aText, const TRect& aRect, 
								 const TRgb& aPenColor, 
								CGraphicsContext::TTextAlign aTextAlign, 
								TInt aOffset) const
	{
	CWindowGc& gc(SystemGc());
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenColor(aPenColor);
	gc.SetBrushColor(TRgb(255, 255, 255, 0));
	gc.SetBrushStyle(CGraphicsContext::ENullBrush);
	gc.DrawText(aText, aRect, aRect.Height() - iFont->DescentInPixels() - 1, 
				aTextAlign, aOffset);
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

	DrawRect(iDataRect,	KRgbBlack, KRgbWhite);
	if (iSentData + iRcvdData < iDataQuota)
		{
		DrawRect(iSentRect,	KRgbSent, KRgbSent);
		DrawRect(iRcvdRect, KRgbRcvd, KRgbRcvd);
		}
	else
		{
		DrawRect(iSentRect,	KRgbOver, KRgbOver);
		DrawRect(iRcvdRect, KRgbOver, KRgbOver);
		}
	DrawRect(iDataRect,	KRgbBlack, KRgbTransparent);

	gc.SetPenColor(KRgbNow);
	gc.SetPenStyle(CGraphicsContext::EDottedPen);
	gc.DrawLine(TPoint(iNowRect.iBr.iX, iDataRect.iTl.iY + 1), 
				TPoint(iNowRect.iBr.iX, iDataRect.iBr.iY - 1));

	 // "XXX,XXX.XX", 999,999.99 MB = 976.56249 GB
	TRealFormat format(10, 2);

	TBuf<255> sentBuf(*iSentText);
	sentBuf.AppendNum(iSentData / (TReal)KKilobyte, format);
	sentBuf.Append(*iMegabyteText);

	TBuf<255> rcvdBuf(*iRcvdText);
	rcvdBuf.AppendNum(iRcvdData / (TReal)KKilobyte, format);
	rcvdBuf.Append(*iMegabyteText);

	TBuf<255> usedBuf(*iUsedText);
	usedBuf.AppendNum((iSentData + iRcvdData) / (TReal)KKilobyte, format);
	usedBuf.Append(*iSeperatorText);
	usedBuf.AppendNum(iDataQuota / (TReal)KKilobyte, format);
	usedBuf.Append(*iMegabyteText);
	
	TRgb textColour;
	AknsUtils::GetCachedColor(skin, textColour, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);

	if (iSentData + iRcvdData < iDataQuota)
		{
		DrawText(sentBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (2 * KBarHeight)), KRgbSent);
		DrawText(rcvdBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (3 * KBarHeight)), KRgbRcvd);
		DrawText(usedBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (4 * KBarHeight)), textColour);
		}
	else
		{
		DrawText(sentBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (2 * KBarHeight)), KRgbOver);
		DrawText(rcvdBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (3 * KBarHeight)), KRgbOver);
		DrawText(usedBuf, TPoint(iDataRect.iTl.iX, iDataRect.iTl.iY + (4 * KBarHeight)), KRgbOver);
		}

	// Date

	DrawRect(iDateRect, KRgbBlack,	KRgbWhite);
	DrawRect(iNowRect,  KRgbNow,	KRgbNow);
	DrawRect(iDateRect, KRgbBlack,	KRgbTransparent);

	gc.SetPenColor(KRgbRcvd);
	gc.SetPenStyle(CGraphicsContext::EDottedPen);
	gc.DrawLine(TPoint(iRcvdRect.iBr.iX, iDateRect.iTl.iY + 1), 
				TPoint(iRcvdRect.iBr.iX, iDateRect.iBr.iY - 1));

	TBuf<255> nowBuf(*iDayText);
	nowBuf.AppendNum(iDaysSinceBillingDay + 1);
	nowBuf.Append(*iSeperatorText);
	nowBuf.AppendNum(iDaysThisPeriod);

	DrawText(nowBuf, TPoint(iDateRect.iTl.iX, iDateRect.iTl.iY + (2 * KBarHeight)), textColour);

	gc.DiscardFont();
	}


void CDataQuotaAppView::SizeChanged()
	{
	UpdateValuesL();
	}


void CDataQuotaAppView::UpdateValuesL()
	{
	const TInt KMargin(10);
	const TInt KDataBarY(Size().iHeight*1/4 - KBarHeight);
	const TInt KDateBarY(Size().iHeight*3/4 - KBarHeight);
	
	TInt rectWidth(Size().iWidth - (2 * KMargin));

	// Data

#ifdef __WINS__
	iSentData = 0100 * KKilobyte;
	iRcvdData = 2400 * KKilobyte;
#else
if (iRepository)
	{
	TBuf<50> bytes;
	User::LeaveIfError(iRepository->Get(KLogsGPRSSentCounter, bytes));
	TLex lex(bytes);
	User::LeaveIfError(lex.Val(iSentData));
	iSentData /= KKilobyte; // Convert bytes to kilobytes
	
	User::LeaveIfError(iRepository->Get(KLogsGPRSReceivedCounter, bytes));
	lex = bytes;
	User::LeaveIfError(lex.Val(iRcvdData));
	iRcvdData /= KKilobyte; // Convert bytes to kilobytes
	}
#endif

	// MB are too small, bytes are too big, KB are just right
	iDataRect = TRect(TPoint(KMargin, KDataBarY),			TSize(rectWidth, KBarHeight));
	iSentRect = TRect(TPoint(KMargin, KDataBarY),			TSize(rectWidth * iSentData/iDataQuota, KBarHeight));
	iRcvdRect = TRect(TPoint(iSentRect.iBr.iX, KDataBarY),	TSize(rectWidth * iRcvdData/iDataQuota, KBarHeight));

	// Date

	TTime time; // time in microseconds since 0AD nominal Gregorian
	time.HomeTime(); // set time to home time
	iDateTime = time.DateTime(); // convert to fields
	
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
	
	iDateRect = TRect(TPoint(KMargin, KDateBarY),	TSize(rectWidth, KBarHeight));
	
	iNowRect  = TRect(TPoint(KMargin, KDateBarY),	TSize(rectWidth * (iDaysSinceBillingDay+(iDateTime.Hour()/24))/iDaysThisPeriod, KBarHeight));
	}


TTypeUid::Ptr CDataQuotaAppView::MopSupplyObject(TTypeUid aId)
	{
	if(aId.iUid == MAknsControlContext::ETypeId && iBackground)
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


void CDataQuotaAppView::SetDataQuota(TInt aDataQuota)
	{
	iDataQuota = aDataQuota * KKilobyte;
	SaveSettingsL();
	}


TInt CDataQuotaAppView::BillingDay()
	{
	return iBillingDay;
	}


void CDataQuotaAppView::SetBillingDay(TInt aBillingDay)
	{
	iBillingDay = aBillingDay;
	SaveSettingsL();
	}


void CDataQuotaAppView::LoadSettingsL()
	{
	iDataQuota = KDataQuota;
	iBillingDay = 0;
	
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
		
		CleanupStack::PopAndDestroy(&writeStream);
		}
	
	CleanupStack::PopAndDestroy(&file);
	}

void CDataQuotaAppView::DoChangePaneTextL() const
	{
	TBuf<255> dateText;
	
	TTime time;
	time.HomeTime(); // set time to home time
	_LIT(KDateFormat, "%/0%1%/1%2%/2%3%/3");
	time.FormatL(dateText, KDateFormat); 
	
	TBuf<255> stateText(KVersion);
	stateText.Append(_L(" - "));
	stateText.Append(dateText);
	
	static_cast<CAknNaviLabel*>(iNaviLabelDecorator->DecoratedControl())->
														SetTextL(stateText);
	iNaviContainer->Pop();
	iNaviContainer->PushL(*iNaviLabelDecorator);
	}

// End of file
