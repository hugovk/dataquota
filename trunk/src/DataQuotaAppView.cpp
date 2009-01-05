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
#include <AknUtils.h> 
#include <CoeMain.h>
#include <DclCRKeys.h>
#include <s32file.h>
#include <StringLoader.h>

#include <DataQuota.rsg>
#include "DataQuotaAppView.h"

const TInt KKilobyte(1024);
const TInt KMegabyte(KKilobyte * KKilobyte);
const TInt KDataQuota(20 * KMegabyte);
const TInt KBarHeight(20);

const TRgb KRgbSent(KRgbBlue);
const TRgb KRgbRcvd(KRgbDarkGreen);
const TRgb KRgbNow (KRgbYellow);
const TRgb KRgbOver(KRgbRed);
const TRgb KRgbTransparent(0x00, 0x00, 0x00, 0x00);

_LIT(KSettingsFile, "c:settings.dat");


CDataQuotaAppView* CDataQuotaAppView::NewL(const TRect& aRect)
	{
	CDataQuotaAppView* self = CDataQuotaAppView::NewLC(aRect);
	CleanupStack::Pop(self);
	return self;
	}


CDataQuotaAppView* CDataQuotaAppView::NewLC(const TRect& aRect)
	{
	CDataQuotaAppView* self = new (ELeave) CDataQuotaAppView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}


void CDataQuotaAppView::ConstructL(const TRect& aRect)
	{
	LoadQuotaL();
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
	CWindowGc& gc = SystemGc();
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
	CWindowGc& gc = SystemGc();
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
	CWindowGc& gc = SystemGc();
	gc.SetDrawMode(CGraphicsContext::EDrawModePEN);
	gc.SetPenColor(aPenColor);
	gc.SetPenStyle(CGraphicsContext::ESolidPen);
	gc.SetBrushColor(aBrushColor);
	gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	gc.DrawRect(aRect);
	}


void CDataQuotaAppView::Draw(const TRect& /*aRect*/) const
	{
	// note that the whole screen is drawn everytime, so aRect-parameter
	// is ignored

	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	 // draw background
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
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

	TRealFormat format(5, 2);

	TBuf<255> sentBuf(*iSentText);
	sentBuf.AppendNum(iSentData / (TReal)KMegabyte, format);
	sentBuf.Append(*iMegabyteText);

	TBuf<255> rcvdBuf(*iRcvdText);
	rcvdBuf.AppendNum(iRcvdData / (TReal)KMegabyte, format);
	rcvdBuf.Append(*iMegabyteText);

	TBuf<255> usedBuf(*iUsedText);
	usedBuf.AppendNum((iSentData + iRcvdData) / (TReal)KMegabyte, format);
	usedBuf.Append(*iSeperatorText);
	usedBuf.AppendNum(iDataQuota / (TReal)KMegabyte, format);
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
	nowBuf.AppendNum(iDateTime.Day()+1);
	nowBuf.Append(*iSeperatorText);
	nowBuf.AppendNum(iDaysThisMonth);

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

	TInt rectWidth = Size().iWidth - (2 * KMargin);

	// Data

#ifdef __WINS__
	iSentData = 1.03 * KMegabyte;
	iRcvdData = 8.51 * KMegabyte;
#else
if (iRepository)
	{
	TBuf<50> bytes;
	User::LeaveIfError(iRepository->Get(KLogsGPRSSentCounter, bytes));
	TLex lex(bytes);
	User::LeaveIfError(lex.Val(iSentData));

	User::LeaveIfError(iRepository->Get(KLogsGPRSReceivedCounter, bytes));
	lex = bytes;
	User::LeaveIfError(lex.Val(iRcvdData));
	}
#endif

	// MB are too small, bytes are too big, KB are just right
	TInt sentData  = iSentData/KKilobyte;
	TInt rcvdData  = iRcvdData/KKilobyte;
	TInt dataQuota = iDataQuota/KKilobyte;
	
	iDataRect = TRect(TPoint(KMargin, KDataBarY),			TSize(rectWidth, KBarHeight));
	iSentRect = TRect(TPoint(KMargin, KDataBarY),			TSize(rectWidth * sentData/dataQuota, KBarHeight));
	iRcvdRect = TRect(TPoint(iSentRect.iBr.iX, KDataBarY),	TSize(rectWidth * rcvdData/dataQuota, KBarHeight));

	// Date

	TTime time; // time in microseconds since 0AD nominal Gregorian
	time.HomeTime(); // set time to home time
	iDateTime = time.DateTime(); // convert to fields
	iDaysThisMonth = time.DaysInMonth();

	iDateRect = TRect(TPoint(KMargin, KDateBarY),	TSize(rectWidth, KBarHeight));
	iNowRect  = TRect(TPoint(KMargin, KDateBarY),	TSize(rectWidth * (iDateTime.Day()+(iDateTime.Hour()/24))/iDaysThisMonth, KBarHeight));
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
	return iDataQuota/KMegabyte;
	}


void CDataQuotaAppView::SetDataQuota(TInt aDataQuota)
	{
	iDataQuota = aDataQuota * KMegabyte;
	SaveQuotaL();
	}


void CDataQuotaAppView::LoadQuotaL()
	{
	RFile file;
	CleanupClosePushL(file);
	TInt openError = file.Open(CCoeEnv::Static()->FsSession(), 
													KSettingsFile, EFileRead);

	if (openError == KErrNone)
		{
		RFileReadStream readStream(file);
		CleanupClosePushL(readStream);

		iDataQuota = readStream.ReadInt32L();

		CleanupStack::PopAndDestroy(&readStream);
		}
	else
		{
		iDataQuota = KDataQuota;
		}

	CleanupStack::PopAndDestroy(&file);
	}


void CDataQuotaAppView::SaveQuotaL()
	{
	CCoeEnv::Static()->FsSession().MkDirAll(KSettingsFile);

	RFile file;
	CleanupClosePushL(file);
	TInt replaceError = file.Replace(CCoeEnv::Static()->FsSession(), 
													KSettingsFile, EFileWrite);

	if (replaceError == KErrNone)
		{
		RFileWriteStream writeStream(file);
		CleanupClosePushL(writeStream);

		writeStream.WriteInt32L(iDataQuota);

		CleanupStack::PopAndDestroy(&writeStream);
		}

	CleanupStack::PopAndDestroy(&file);
	}

// End of file
