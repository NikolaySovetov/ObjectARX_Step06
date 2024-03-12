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
	assertWriteEnabled();
	context.m_nID = nID;
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetId(Adesk::Int32 & nID) {
	assertReadEnabled();
	nID = context.m_nID;
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetCube(const Adesk::Int32 nCube) {
	assertWriteEnabled();
	context.m_nCube = nCube;
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetCube(Adesk::Int32 & nCube) {
	assertReadEnabled();
	nCube = context.m_nCube;
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetFirstName(const TCHAR * strFirstName) {
	assertWriteEnabled();
	if (context.m_strFirstName) {
		free(context.m_strFirstName);
	}
	context.m_strFirstName = _tcsdup(strFirstName);
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetFirstName(TCHAR * &strFirstName) {
	assertReadEnabled();
	strFirstName = context.m_strFirstName;
	return Acad::eOk;
}

Acad::ErrorStatus Employee::SetLastName(const TCHAR * strLastName) {
	assertWriteEnabled();
	if (context.m_strLastName) {
		free(context.m_strLastName);
	}
	context.m_strLastName = _tcsdup(strLastName);
	return Acad::eOk;
}

Acad::ErrorStatus Employee::GetLastName(TCHAR * &strLastName) {
	assertReadEnabled();
	strLastName = context.m_strLastName;
	return Acad::eOk;
}
