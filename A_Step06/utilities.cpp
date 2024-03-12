#include "StdAfx.h"
#include "utilities.h"

#pragma comment (lib, "EmployeeProj.lib")

bool SetEmployeeContext(Employee* pEmpl) {
	int id, cubeNumber;
	TCHAR strFirstName[128];
	TCHAR strLastName[128];
	AcGePoint3d pt;

	if (acedGetInt(_T("Enter employee ID: "), &id) != RTNORM
		|| acedGetInt(_T("Enter cube number: "), &cubeNumber) != RTNORM
		|| acedGetString(0, _T("Enter employee first name: "), strFirstName) != RTNORM
		|| acedGetString(0, _T("Enter employee last name: "), strLastName) != RTNORM
		|| acedGetPoint(NULL, _T("Employee position: "), asDblArray(pt)) != RTNORM
		) {
		return false;
	}

	pEmpl->SetId(id);
	pEmpl->SetCube(cubeNumber);
	pEmpl->SetFirstName(strFirstName);
	pEmpl->SetLastName(strLastName);
	pEmpl->setCenter(pt);

	return true;
}


