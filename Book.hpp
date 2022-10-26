#pragma once

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Book
{
private:
	string title;
	string author;
	string id;
	string publication;
	int avai;
public:
	Book(string title, string author, string id, string publication, int avai)
	{
		this->title = title;
		this->author = author;
		this->id = id;
		this->publication = publication;
		this->avai = avai;
	}

	string getTitle()
	{
		return this->title;
	}

	string getAuthor()
	{
		return this->author;
	}

	string getID()
	{
		return this->id;
	}

	string getPublication()
	{
		return this->publication;
	}

	int getAvai()
	{
		return this->avai;
	}

	void display()
	{
		cout << "Title: " << this->title << endl;
		cout << "Author: " << this->author << endl;
		cout << "ID: " << this->id << endl;
		cout << "Publication: " << this->publication << endl;
	}

	~Book()
	{

	}
};