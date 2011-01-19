/*
Data Quota for S60 phones.
http://code.google.com/p/dataquota/
Copyright (C) 2010  Hugo van Kemenade

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

This file incorporates work covered by the following copyright and  
permission notice:  

	Mobbler, a Last.fm mobile scrobbler for Symbian smartphones.
	Copyright (C) 2008, 2009  Michael Coffey
	Copyright (C) 2009  Hugo van Kemenade

	http://code.google.com/p/mobbler

	This file is part of Mobbler.

	Mobbler is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	Mobbler is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Mobbler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <ecom/implementationproxy.h>
#include "dataquotatouchfeedback.h"

const TInt KImplementationUid = {0xA89FD5B4};


const TImplementationProxy ImplementationTable[] =
	{
	{KImplementationUid, TProxyNewLPtr(CDataQuotaTouchFeedback::NewL)}
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

CDataQuotaTouchFeedback* CDataQuotaTouchFeedback::NewL()
	{
	CDataQuotaTouchFeedback* self(new(ELeave) CDataQuotaTouchFeedback());
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CDataQuotaTouchFeedback::CDataQuotaTouchFeedback()
	{
	}

void CDataQuotaTouchFeedback::ConstructL()
	{
#ifdef  __S60_50__
	iTouchFeedback = MTouchFeedback::Instance();
	iTouchFeedback->SetFeedbackEnabledForThisApp(ETrue);
#else
	User::Leave(KErrNotSupported);
#endif
	}

CDataQuotaTouchFeedback::~CDataQuotaTouchFeedback()
	{
	}

void CDataQuotaTouchFeedback::InstantFeedback(TInt aType)
	{
#ifdef  __S60_50__
	iTouchFeedback->InstantFeedback(static_cast<TTouchLogicalFeedback>(aType));
#endif
	}

// End of file
