#pragma once

#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>

class Publisher; class Book; class Stock; class Shop; class Sale;

class Book {
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection< Wt::Dbo::ptr<Stock>> stock;

	template<class Action>
	void persist(Action& a);
};

class Publisher {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> book;

	template<class Action>
	void persist(Action& a);
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection < Wt::Dbo::ptr<Stock>> stock;

	template<class Action>
	void persist(Action& a);
};

class Stock {
public:
	int count = 0;
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>>  sale;

	template<class Action>
	void persist(Action& a);
};

class Sale {
public:
	int count = 0;
	int price = 0;
	std::string date_sale = "";
	Wt::Dbo::ptr<Stock> stock;

	template<class Action>
	void persist(Action& a);
};

void start();