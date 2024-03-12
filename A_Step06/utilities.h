#pragma once
#include "StdAfx.h"
#include "Employee.h"
#include <memory>

bool SetEmployeeContext(Employee* pEmpl);

bool SetEmployeeContext(std::unique_ptr<Employee> pEmpl);

class BlockTableWrapper {
private:
	AcDbBlockTable* m_pBlockTable{};

public:
	BlockTableWrapper(AcDb::OpenMode mode);
	~BlockTableWrapper();
	const AcDbBlockTable* Get() const;
};

class BlockTableRecordWrapper {
private:
	AcDbBlockTableRecord* m_pBlockTableRecord{};

public:
	BlockTableRecordWrapper
	(const AcDbBlockTable* pBlockTable, const ACHAR* entryName, AcDb::OpenMode mode);
	~BlockTableRecordWrapper();
	const AcDbBlockTableRecord* Get() const;

	template<typename T>
	void appendEntity(std::unique_ptr<T>& pEntity) {

		if (!m_pBlockTableRecord) {
			return;
		}

		AcDbObjectId id;
		if (m_pBlockTableRecord->appendAcDbEntity(id, pEntity.get()) != Acad::eOk) {
			//TODO: message
			return;
		}
		pEntity->close();
		pEntity.release();
	}
};



