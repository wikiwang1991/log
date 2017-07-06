# log [![Build Status](https://travis-ci.org/wwqwwqwd/log.svg?branch=master)](https://travis-ci.org/wwqwwqwd/log) [![Coverage Status](https://coveralls.io/repos/github/wwqwwqwd/log/badge.svg?branch=master)](https://coveralls.io/github/wwqwwqwd/log?branch=master)

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

	class A {
	public:
		A() {
			logm_debug("A construct");
		}
	};

	int main()
	{
		log_initialize(0);

		A a;

		logg_info("A %p constructed", &a);

		log_close();

		return 0;
	}

| time | thread | level | object | function | line | message |
| - | - | - | - | - | - | - |
| 2017-07-05 08:43:42.897137900 | 29232 | 0 | 000000F8B2CFFA40 | A::A | 8 | A construct |
| 2017-07-05 08:43:42.897172900 | 29232 | 1 | 0000000000000000 | main | 18 | A 000000F8B2CFFA40 constructed |
