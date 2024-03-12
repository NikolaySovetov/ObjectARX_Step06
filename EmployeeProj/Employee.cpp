// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
//----- Employee.cpp : Implementation of Employee
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Employee.h"
#include <exception>

//-----------------------------------------------------------------------------
Adesk::UInt32 Employee::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	Employee, AcDbEllipse,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, EMPLOYEE,
	EMPLOYEEPROJAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------

//Employee::Employee() : AcDbEllipse() {       // HACK: Exception when try to write text!!! 
Employee::Employee() : AcDbEllipse(AcGePoint3d(), AcGeVector3d(0, 0, 1), AcGeVector3d(1, 0, 0), 0.3) {
	context.m_nID = 0;
	context.m_nCube = 0;
	context.m_strFirstName = nullptr;
	context.m_strLastName = nullptr;
}

Employee::~Employee() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus Employee::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEllipse::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(Employee::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	// Write the data members
	context.DwgWrite(pFiler);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Employee::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEllipse::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > Employee::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Employee::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	switch (version) {
	case (1):
		context.DwgRead(pFiler);
		break;
	}

	return (pFiler->filerStatus());
}

//- Dxf Filing protocol
Acad::ErrorStatus Employee::dxfOutFields(AcDbDxfFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEllipse::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("Employee"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(kDxfInt32, Employee::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	// Write out the data members
	context.DxfWrite(pFiler);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Employee::dxfInFields(AcDbDxfFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEllipse::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_RXST("Employee")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > Employee::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Employee::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	context.DxfRead(rb, es, pFiler);

	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean Employee::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();

	// Draw the AcDbEllipse
	AcDbEllipse::subWorldDraw(mode);

	// Draw the Employee ID and Name
	AcGePoint3d position(center() + majorAxis());
	double height = minorAxis().length() / 2.0;
	position.x += position.x * 0.1;
	position.y += minorAxis().y - height;

	context.WorldDraw(position, normal(), majorAxis(), height, 1.6, mode);

	return Adesk::kTrue;
}

Adesk::UInt32 Employee::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEllipse::subSetAttributes(traits));
}

//---------------------------------------------------------
Acad::ErrorStatus Employee::SetId(const Adesk::Int32 nID) {
	try {
		assertWriteEnabled();
		context.m_nID = nID;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nException: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetId(Adesk::Int32 & nID) {
	try {
		assertReadEnabled();
		nID = context.m_nID;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nException: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetCube(const Adesk::Int32 nCube) {
	try {
		assertWriteEnabled();
		context.m_nCube = nCube;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nException: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetCube(Adesk::Int32 & nCube) {
	try {
		assertReadEnabled();
		nCube = context.m_nCube;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nException: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetFirstName(const TCHAR * strFirstName) {
	try {
		assertWriteEnabled();
		if (context.m_strFirstName) {
			free(context.m_strFirstName);
		}
		context.m_strFirstName = _tcsdup(strFirstName);
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nExeption: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetFirstName(TCHAR * &strFirstName) {
	try {
		assertReadEnabled();
		strFirstName = context.m_strFirstName;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nExeption: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetLastName(const TCHAR * strLastName) {
	try {
		assertWriteEnabled();
		if (context.m_strLastName) {
			free(context.m_strLastName);
		}
		context.m_strLastName = _tcsdup(strLastName);
	}
	catch (const std::exception& e) {
		acutPrintf(_T("\nExeption: %s"), e.what());
	}
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetLastName(TCHAR * &strLastName) {
	try {
		assertReadEnabled();
		strLastName = context.m_strLastName;
	}
	catch (const std::exception& e)
	{
		acutPrintf(_T("\nExeption: %s"), e.what());
	}
	return Acad::eOk;
}

//---------------------------------------------------------
void Employee::Context::DwgWrite(AcDbDwgFiler * pFiler) const {

	pFiler->writeItem(m_strLastName);
	pFiler->writeItem(m_strFirstName);
	pFiler->writeItem(m_nCube);
	pFiler->writeItem(m_nID);
}

void Employee::Context::DwgRead(AcDbDwgFiler * pFiler) {

	pFiler->readItem(&m_strLastName);
	pFiler->readItem(&m_strFirstName);
	pFiler->readItem(&m_nCube);
	pFiler->readItem(&m_nID);
}

void Employee::Context::DxfWrite(AcDbDxfFiler * pFiler) const {

	pFiler->writeItem(AcDb::kDxfXTextString, m_strLastName);
	pFiler->writeItem(AcDb::kDxfXTextString + 1, m_strFirstName);
	pFiler->writeItem(AcDb::kDxfInt32, m_nCube);
	pFiler->writeItem(AcDb::kDxfInt32 + 1, m_nID);
}

void Employee::Context::DxfRead(struct resbuf& rb, Acad::ErrorStatus & es, AcDbDxfFiler * pFiler) {
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
		case AcDb::kDxfXTextString:
			if (m_strLastName) {
				free(m_strLastName);
			}
			m_strLastName = _tcsdup(rb.resval.rstring);
			break;
		case AcDb::kDxfXTextString + 1:
			if (m_strLastName) {
				free(m_strLastName);
			}
			m_strLastName = _tcsdup(rb.resval.rstring);
			break;
		case AcDb::kDxfInt32:
			m_nCube = rb.resval.rlong;
			break;
		case AcDb::kDxfInt32 + 1:
			m_nID = rb.resval.rlong;
			break;
		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
}

void Employee::Context::WorldDraw(AcGePoint3d & textPosition,
	const AcGeVector3d & normal,
	const AcGeVector3d & direction,
	const double& height,
	const double& downOffsetByHeight,
	const AcGiWorldDraw * mode) {

	TCHAR buffer[128];
	const double width{ 1.0 };
	const double oblique{ 0.0 };

	_stprintf(buffer, _T("id: %d"), m_nID);
	mode->geometry().text(textPosition, normal, direction, height, width, oblique, buffer);

	textPosition.y -= height * downOffsetByHeight;
	_stprintf(buffer, _T("cube: %d"), m_nCube);
	mode->geometry().text(textPosition, normal, direction, height, width, oblique, buffer);

	textPosition.y -= height * downOffsetByHeight;
	_stprintf(buffer, _T("first name: %s"), m_strFirstName);
	mode->geometry().text(textPosition, normal, direction, height, width, oblique, buffer);

	textPosition.y -= height * downOffsetByHeight;
	_stprintf(buffer, _T("last name: %s"), m_strLastName);
	mode->geometry().text(textPosition, normal, direction, height, width, oblique, buffer);
}

