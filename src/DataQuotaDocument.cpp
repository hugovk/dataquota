/*
DataQuotaDocument.cpp

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
#include "DataQuotaAppUi.h"
#include "DataQuotaDocument.h"

CDataQuotaDocument* CDataQuotaDocument::NewL(CEikApplication& aApp)
	{
	CDataQuotaDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

CDataQuotaDocument* CDataQuotaDocument::NewLC(CEikApplication& aApp)
	{
	CDataQuotaDocument* self = new (ELeave) CDataQuotaDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CDataQuotaDocument::ConstructL()
	{
	// No implementation required
	}

CDataQuotaDocument::CDataQuotaDocument(CEikApplication& aApp)
	: CAknDocument(aApp)
	{
	// No implementation required
	}

CDataQuotaDocument::~CDataQuotaDocument()
	{
	// No implementation required
	}

CEikAppUi* CDataQuotaDocument::CreateAppUiL()
	{
	return (static_cast <CEikAppUi*> (new (ELeave) CDataQuotaAppUi));
	}

// End of file
