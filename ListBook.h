#pragma once

#include <list>
#include "Book.hpp"

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using std::cin;
using std::list;
using std::to_string;

class ListBook : list<Book>
{
public:
	ListBook(sql::Connection* con, sql::Statement* stmt, sql::ResultSet* res)
	{
		try
		{
			/* Connect to the MySQL test database */
			con->setSchema("librarydb");
			stmt = con->createStatement();
			res = stmt->executeQuery("SELECT * FROM librarydb.book");
			while (res->next())
			{
				Book book(res->getString("title"), res->getString("author"),
					res->getString("id"), res->getString("publication"), res->getInt("available"));
				this->push_back(book);

			}
		}
		catch (sql::SQLException& e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

#pragma region findMenu
	void findByCategory()
	{
		bool cont = true;
		while (cont)
		{
			system("cls");
			cout << "1. IT" << endl;
			cout << "2. Literature" << endl;
			cout << "3. ... and more but not now" << endl;
			cout << "0. Back" << endl;
			int option;
			cin >> option;
			switch (option)
			{
			case 1:
				for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
				{
					if ((*it).getID().substr(0, 2) == "IT")
						(*it).display();
				}
				system("pause");
				break;
			case 2:
				for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
				{
					if ((*it).getID().substr(0, 2) == "VH")
						(*it).display();
				}
				system("pause");
				break;
			case 3:
				cout << "Really? ._." << endl;
				system("pause");
				break;
			case 0:
				cont = false;
			}
		}
	}

	void findByID()
	{
		system("cls");
		cout << "Book ID: ";
		string checkID;
		cin.ignore();
		getline(cin, checkID);
		for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
		{
			if (checkID == (*it).getID())
			{
				(*it).display();
				break;
			}
		}
		system("pause");
	}

	void findByTitle()
	{
		cout << "Enter keyword: ";
		string keyword;
		cin.ignore();
		getline(cin, keyword);
		for_each(keyword.begin(), keyword.end(), [](char& c) {
			c = tolower(c);
			});
		for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
		{
			string title = (*it).getTitle();
			for_each(title.begin(), title.end(), [](char& c) {
				c = tolower(c);
				});
			if (title.find(keyword) != std::string::npos)
				(*it).display();
		}
		system("pause");
	}

	void findMenu()
	{
		bool cont = true;
		while (cont)
		{
			system("cls");
			cout << "1. By category" << endl;
			cout << "2. Has a book ID?" << endl;
			cout << "3. By title" << endl;
			cout << "0. Back" << endl;
			int option;
			cin >> option;
			switch (option)
			{
			case 1:
				findByCategory();
				break;
			case 2:
				findByID();
				break;
			case 3:
				findByTitle();
				break;
			case 0:
				cont = false;
			}
		}
	}
#pragma endregion findMenu

#pragma region addFunc
	void addFunc(sql::Connection* con, sql::Statement* stmt)
	{
		cout << "New book ID: ";
		string newID;
		cin.ignore();
		getline(cin, newID);
		for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
		{
			if (newID == it->getID())
			{
				cout << "Book is already existed." << endl;
				system("pause");
				return;
			}
		}
		cout << "Title: ";
		string newTitle;
		getline(cin, newTitle);

		cout << "Author: ";
		string newAuthor;
		getline(cin, newAuthor);

		cout << "Publisher: ";
		string newPublish;
		getline(cin, newPublish);

		cout << "Quantity: ";
		int newQuantity;
		cin >> newQuantity;

		Book book = Book(newTitle, newAuthor, newID, newPublish, newQuantity);
		this->push_back(book);

		try
		{
			/* Connect to the MySQL test database */
			con->setSchema("librarydb");
			stmt = con->createStatement();
			stmt->executeQuery("INSERT INTO `librarydb`.`book`(`title`, `author`, `id`, `publication`, `available`)"
				"VALUES ('" + newTitle + "', '" + newAuthor + "', '" + newID + "', '" + newPublish + "', '" + to_string(newQuantity) + "');");
		}
		catch (sql::SQLException& e) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}
#pragma endregion addFunc

#pragma region removeFunc
	void removeFunc(sql::Connection* con, sql::Statement* stmt)
	{
		cout << "Enter book ID: ";
		string delID;
		cin.ignore();
		getline(cin, delID);
		bool found = false;
		for (list<Book>::iterator it = this->begin(); it != this->end(); it++)
		{
			if (delID == it->getID())
			{
				try
				{
					/* Connect to the MySQL test database */
					con->setSchema("librarydb");
					stmt = con->createStatement();
					stmt->executeQuery("DELETE FROM `librarydb`.`book` WHERE (`id` = '" + delID + "');");
				}
				catch (sql::SQLException& e) {
					cout << "# ERR: SQLException in " << __FILE__;
					cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
					cout << "# ERR: " << e.what();
					cout << " (MySQL error code: " << e.getErrorCode();
					cout << ", SQLState: " << e.getSQLState() << " )" << endl;
				}
				it = this->erase(it);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "Book not found!" << endl;
			system("pause");
		}
	}
#pragma endregion removeFunc

	~ListBook()
	{
		this->clear();
	}
};
