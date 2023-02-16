#pragma once
#include <sqlext.h> 
#include <atomic>
#include "define.h"

class DB
{
public:
	DB();
	~DB();

	bool Init();


	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);
private:
	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;

	SQLRETURN retcode;

	SQLCHAR* OutConnStr = (SQLCHAR*)malloc(255);
	SQLSMALLINT* OutConnStrLen = (SQLSMALLINT*)malloc(255);

};
