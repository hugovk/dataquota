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
	Copyright (C) 2008  Michael Coffey
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

#ifndef __DATAQUOTATOUCHFEEDBACK_H__
#define __DATAQUOTATOUCHFEEDBACK_H__

#include <e32base.h>
#include <dataquota\DataQuotaTouchFeedbackInterface.h>

#ifdef  __S60_50__
#include <touchfeedback.h>
#endif

class CDataQuotaTouchFeedback : public CDataQuotaTouchFeedbackInterface
	{
public:
	static CDataQuotaTouchFeedback* NewL();
	~CDataQuotaTouchFeedback();
	
private:
	CDataQuotaTouchFeedback();
	void ConstructL();

private: // from CDataQuotaTouchFeedbackInterface
	void InstantFeedback(TInt aType);

private:
#ifdef  __S60_50__
	MTouchFeedback* iTouchFeedback;
#endif
	};

#endif // __DATAQUOTATOUCHFEEDBACK_H__

// End of file
