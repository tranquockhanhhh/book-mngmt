#include "ListBook.h"
#include "Book.hpp"

int main(void)
{

	sql::Driver* driver = get_driver_instance();
	sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "P@ssw0rd");
	sql::Statement* stmt{};
	sql::ResultSet* res{};
	ListBook listBook = ListBook(con, stmt, res);
	
	bool cont = true;
	while (cont)
	{
		system("cls");
		cout << "1. Find" << endl;
		cout << "2. Add" << endl;
		cout << "3. Remove" << endl;
		cout << "0. Exit" << endl;
		int option;
		cin >> option;
		switch (option)
		{
		case 1:
			listBook.findMenu();
			break;
		case 2:
			listBook.addFunc(con, stmt);
			break;
		case 3:
			listBook.removeFunc(con, stmt);
			break;
		case 0:
			cont = false;
		}
	}

	delete res;
	delete stmt;
	delete con;
	return EXIT_SUCCESS;
}