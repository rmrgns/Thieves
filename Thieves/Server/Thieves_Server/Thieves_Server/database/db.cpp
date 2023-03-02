#include "header.h"
#include "db.h"

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


DB::DB()
{

}

DB::~DB()
{
	cout << "db ���� " << endl;
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

bool DB::Init()
{
	cout << "db ���� " << endl;
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"thieves_db_odbc", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				// Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
					cout << "ODBC Connected\n";
					
				}


			}
		}
	}


	return true;
}

// ���̵� ��й�ȣ �����ͺ��̽� �����ϴ� �Լ�
// �����ϸ� LOGINFAIL_TYPE::OK ��ȯ ���н� DB_ERROR ��ȯ
// name�� password�� wchar_tŸ������ ��ȯ�Ͽ� ����
//exec�� �����ͺ��̽��� �����Ͽ� �������ν����� ����
LOGINFAIL_TYPE DB::SaveData(char* name, char* password)
{
	wchar_t exec[256];
	wchar_t wname[MAX_NAME_SIZE];
	wchar_t wpassword[MAX_NAME_SIZE];
	LOGINFAIL_TYPE ret = LOGINFAIL_TYPE::OK;
	size_t len;
	mbstowcs_s(&len, wname, MAX_NAME_SIZE, name, MAX_NAME_SIZE);
	mbstowcs_s(&len, wpassword, MAX_NAME_SIZE, password, MAX_PASSWORD_SIZE);
	wsprintf(exec, L"EXEC insert_user_info @Param1=N'%ls',@Param2=%ls", wname, wpassword);
	wcout << exec << endl;
	//retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)exec, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		cout << "seccess" << endl;
	}
	else
	{
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		ret = LOGINFAIL_TYPE::DB_ERROR;
	}
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//�޸� ����
		SQLCloseCursor(hstmt);
		SQLFreeStmt(hstmt, SQL_UNBIND);
	}
	return ret;
}

// �����ͺ��̽����� �α��� �����͸� Ȯ���ϴ� �Լ�
LOGINFAIL_TYPE DB::CheckLoginData(char* name, char* password)
{
	wchar_t exec[256];
	wchar_t wname[MAX_NAME_SIZE];
	size_t len;
	LOGINFAIL_TYPE ret = LOGINFAIL_TYPE::OK;
	mbstowcs_s(&len, wname, MAX_NAME_SIZE, name, MAX_NAME_SIZE);
	wsprintf(exec, L"EXEC select_user_info @Param1=N'%ls'", wname);
	wcout << exec << endl;
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)exec, SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, m_id, MAX_NAME_SIZE + 1, &cb_id);
		retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, m_password, MAX_PASSWORD_SIZE + 1, &cb_password);

		retcode = SQLFetch(hstmt);
		if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO)
		{
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
			ret= LOGINFAIL_TYPE::DB_ERROR;
		}
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			if (cb_id == 0) ret = LOGINFAIL_TYPE::NO_ID;
			else if (false == CompWcMc(reinterpret_cast<wchar_t*>(m_password), password))
				ret = LOGINFAIL_TYPE::WRONG_PASSWORD;
		}
		if (retcode == SQL_NO_DATA)
		{
			ret = LOGINFAIL_TYPE::NO_ID;
			SQLCloseCursor(hstmt);
			SQLFreeStmt(hstmt, SQL_UNBIND);
		}


	}
	else {
		HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		ret = LOGINFAIL_TYPE::DB_ERROR;
	}
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		//SQLCancel(hstmt);
		SQLCloseCursor(hstmt);
		SQLFreeStmt(hstmt, SQL_UNBIND);
	}
	return ret;
}

// wchar_t�� ������ char�� ������ ���Ͽ� ������ true �ٸ��� false�� ��ȯ�ϴ� �Լ�
bool DB::CompWcMc(wchar_t* wc, char* mc)
{
	wstring comp_wc{ wc };
	comp_wc.erase(remove(comp_wc.begin(), comp_wc.end(), ' '), comp_wc.end());
	string a{ mc };
	wstring comp_mc{ a.begin(),a.end() };
	if (comp_wc.compare(comp_mc) == 0)return true;
	return false;
}

void DB::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	WCHAR wszMessage[1000];
	WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}