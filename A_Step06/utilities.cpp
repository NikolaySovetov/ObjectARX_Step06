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

bool SetEmployeeContext(std::unique_ptr<Employee> pEmpl) {
	return SetEmployeeContext(pEmpl.get());
}

//--------------------------------------------------------------
BlockTableWrapper::BlockTableWrapper(AcDb::OpenMode mode) {

	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(m_pBlockTable, mode)
		!= Acad::eOk) {
		// TODO: message
		m_pBlockTable = nullptr;
	}

}

BlockTableWrapper::~BlockTableWrapper() {
	if (m_pBlockTable) {
		m_pBlockTable->close();
	}
}

const AcDbBlockTable* BlockTableWrapper::Get() const {
	return m_pBlockTable;
}

//--------------------------------------------------------------
BlockTableRecordWrapper::BlockTableRecordWrapper
(const AcDbBlockTable* pBlockTable, const ACHAR* entryName, AcDb::OpenMode mode) {

	if (!pBlockTable) {
		return;
	}

	if (pBlockTable->getAt(entryName, m_pBlockTableRecord, mode) != Acad::eOk) {
		// TODO: message
		m_pBlockTableRecord = nullptr;
	}
}

BlockTableRecordWrapper::~BlockTableRecordWrapper() {
	if (m_pBlockTableRecord) {
		m_pBlockTableRecord->close();
	}
}

const AcDbBlockTableRecord* BlockTableRecordWrapper::Get() const  {
	return m_pBlockTableRecord;
}




