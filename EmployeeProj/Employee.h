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
//----- Employee.h : Declaration of the Employee
//-----------------------------------------------------------------------------
#pragma once

#ifdef EMPLOYEEPROJ_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbelipse.h"
#define EMPLOYEE_DBXSERVICE _T("EMPLOYEE_DBXSERVICE")

//-----------------------------------------------------------------------------
class DLLIMPEXP Employee : public AcDbEllipse {
private:
	struct {
	public:
		Adesk::Int32 m_nID;
		Adesk::Int32 m_nCube;
		TCHAR* m_strFirstName;
		TCHAR* m_strLastName;

	public:
		void DwgWrite(AcDbDwgFiler* pFiler) const {
			pFiler->writeItem(m_strLastName);
			pFiler->writeItem(m_strFirstName);
			pFiler->writeItem(m_nCube);
			pFiler->writeItem(m_nID);
		}
		void DwgRead(AcDbDwgFiler* pFiler) {
			pFiler->readItem(&m_strLastName);
			pFiler->readItem(&m_strFirstName);
			pFiler->readItem(&m_nCube);
			pFiler->readItem(&m_nID);
		}

		void DxfWrite(AcDbDxfFiler* pFiler) const {
			pFiler->writeItem(AcDb::kDxfXTextString, m_strLastName);
			pFiler->writeItem(AcDb::kDxfXTextString + 1, m_strFirstName);
			pFiler->writeItem(AcDb::kDxfInt32, m_nCube);
			pFiler->writeItem(AcDb::kDxfInt32 + 1, m_nID);
		}
		void DxfRead(struct resbuf& rb, Acad::ErrorStatus& es, AcDbDxfFiler* pFiler) {
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

	} context;

public:
	ACRX_DECLARE_MEMBERS(Employee);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	Employee();
	virtual ~Employee();

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler);

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits* traits);

public:
	Acad::ErrorStatus SetId(const Adesk::Int32 nID);
	Acad::ErrorStatus GetId(Adesk::Int32& nID);
	Acad::ErrorStatus SetCube(const Adesk::Int32 nCube);
	Acad::ErrorStatus GetCube(Adesk::Int32& nCube);
	Acad::ErrorStatus SetFirstName(const TCHAR* strFirstName);
	Acad::ErrorStatus GetFirstName(TCHAR*& strFirstName);
	Acad::ErrorStatus SetLastName(const TCHAR* strLastName);
	Acad::ErrorStatus GetLastName(TCHAR*& strLastName);
};

#ifdef EMPLOYEEPROJ_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Employee)
#endif
