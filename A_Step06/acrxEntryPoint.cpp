// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CA_Step06App : public AcRxArxApp {

public:
	CA_Step06App() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here
		AcRxObject* pSvc;
		if (!(pSvc = acrxServiceDictionary->at(EMPLOYEE_DBXSERVICE)))
		{
			// Try to load the module, if it is not yet present 
			if (!acrxDynamicLinker->loadModule(_T("Employee.dbx"), 0))
			{
				acutPrintf(_T("Unable to load EmployeeDetails.dbx. Unloading this application...\n"));
				return (AcRx::kRetError);
			}
		}

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload depend



		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Step06_createEmployee() {

		std::unique_ptr<Employee> pEmpl{ std::make_unique<Employee>() };
		SetEmployeeContext(pEmpl.get());

	    BlockTableWrapper blockTableWrap(AcDb::kForRead);

		BlockTableRecordWrapper blockTableRecordWrap
		(blockTableWrap.Get(), ACDB_MODEL_SPACE, AcDb::kForWrite);

		blockTableRecordWrap.appendEntity<Employee>(pEmpl);

	}

};



//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CA_Step06App)

ACED_ARXCOMMAND_ENTRY_AUTO(CA_Step06App, Step06, _createEmployee, createEmployee, ACRX_CMD_TRANSPARENT, NULL)

