
#include <iostream>
#include "db.h"

using namespace std;

int main() {
	DB db;

	if (!db.Init()) {
		cerr << "Failed to connect to database!" << endl;

		return 1;
	}

	SQLCHAR query[] = "SELECT * FROM my_table";


	// 결과셋 가져오기
	SQLCHAR column1[50], column2[50], column3[50];
	SQLLEN len1 = 0, len2 = 0, len3 = 0;

	while (SQLFetch(db.hstmt) == SQL_SUCCESS) {
		SQLGetData(db.hstmt, 1, SQL_C_CHAR, column1, 50, &len1);
		SQLGetData(db.hstmt, 2, SQL_C_CHAR, column2, 50, &len2);
		SQLGetData(db.hstmt, 3, SQL_C_CHAR, column3, 50, &len3);
	}

	db.~DB();
}