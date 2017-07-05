# log

A lightweight & extensible logging framework for C/C++.

Output varys according to the actual log module loaded.

log_sqlite writes log to a sqlite3 database.
One can use tools such as
[DB Browser for SQLite](https://github.com/sqlitebrowser/sqlitebrowser)
to inspect it.

log_txt writes log to a txt file, which can be converted to spreadsheet
by LibreOffice Calc, Google Sheets or Microsoft Excel.

Any contribute is welcome!

## Example

	#define LOG_LEVEL	0

	#include <log/log.h>

	#include <thread>

	class A {
	public:
		A() {
			logm_debug("A construct");
		}
	}

	int main()
	{
		log_initialize(0);

		A a;

		logg_info("A %p constructed", &a);

		log_close();

		return 0;
	}
