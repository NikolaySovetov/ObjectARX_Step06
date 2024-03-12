#pragma once
#include "StdAfx.h"
#include "Employee.h"
#include <memory>

bool SetEmployeeContext(Employee* pEmpl);

bool SetEmployeeContext(std::unique_ptr<Employee> pEmpl);

class AcDbBlockTableWrapper {
private:
	AcDbBlockTable* pBlockTable{};

public:
	AcDbBlockTableWrapper(AcDb::OpenMode mode);
	~AcDbBlockTableWrapper();

	AcDbBlockTable* Get();
};





