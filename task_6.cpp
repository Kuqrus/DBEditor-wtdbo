#include "task_6.h"

template<class Action>
void Book::persist(Action& a) {
	Wt::Dbo::field(a, title, "title");
	Wt::Dbo::belongsTo(a, publisher, "publisher");
	Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "book");
}

template<class Action>
void Publisher::persist(Action& a) {
	Wt::Dbo::field(a, name, "name");
	Wt::Dbo::hasMany(a, book, Wt::Dbo::ManyToOne, "publisher");
}

template<class Action>
void Shop::persist(Action& a) {
	Wt::Dbo::field(a, name, "name");
	Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "shop");
}

template<class Action>
void Stock::persist(Action& a) {
	Wt::Dbo::field(a, count, "count");
	Wt::Dbo::belongsTo(a, book, "book");
	Wt::Dbo::belongsTo(a, shop, "shop");
	Wt::Dbo::hasMany(a, sale, Wt::Dbo::ManyToOne, "stock");
}

template<class Action>
void Sale::persist(Action& a) {
	Wt::Dbo::field(a, count, "count");
	Wt::Dbo::field(a, price, "price");
	Wt::Dbo::field(a, date_sale, "date_sale");
	Wt::Dbo::belongsTo(a, stock, "stock");
}

void start()
{
	try
	{

		std::string connectionStr =
			"host=localhost "
			"port=5432 "
			"dbname=homework "
			"user=postgres "
			"password=123";

		auto postges = std::make_unique<Wt::Dbo::backend::Postgres>(connectionStr);

		Wt::Dbo::Session session;
		session.setConnection(std::move(postges));
		session.mapClass<Publisher>("Publisher");
		session.mapClass<Book>("Book");
		session.mapClass<Stock>("Stock");
		session.mapClass<Shop>("Shop");
		session.mapClass<Sale>("Sale");

		//session.createTables();

		// Добавляем издателя		
		{
			//Wt::Dbo::Transaction transaction{ session };

			//std::unique_ptr<Publisher> publisher{ new Publisher() };
			//publisher->name = "Mif";

			//Wt::Dbo::ptr<Publisher> publisherPtr = session.add(std::move(publisher));

			//transaction.commit();
		}

		// Добавляем книгу
		{
			//Wt::Dbo::Transaction transaction{ session };
			//Wt::Dbo::ptr<Publisher> mif = session.find<Publisher>().where("name = ?").bind("Mif");

			//std::unique_ptr<Book> book{ new Book() };
			//book->title = "Code";
			//book->publisher = mif;
			//Wt::Dbo::ptr<Book> bookPtr = session.add(std::move(book));

			//transaction.commit();
		}

		// Добавлям магазины
		{
			//Wt::Dbo::Transaction transaction{ session };

			//std::unique_ptr<Shop> shop{ new Shop() };
			//shop->name = "Ozon";

			//Wt::Dbo::ptr<Shop> shopPtr = session.add(std::move(shop));

			//std::unique_ptr<Shop> shop2{ new Shop() };
			//shop2->name = "Chitai-gorod";

			//Wt::Dbo::ptr<Shop> shop2Ptr = session.add(std::move(shop2));

			//transaction.commit();
		}

		// Добавляем наличие
		{
			//Wt::Dbo::Transaction transaction{ session };
			//std::unique_ptr<Stock> stock1{ new Stock()};

			//Wt::Dbo::ptr<Book> code = session.find<Book>().where("title = ?").bind("Code");
			//Wt::Dbo::ptr<Shop> ozon = session.find<Shop>().where("name = ?").bind("Ozon");
			//Wt::Dbo::ptr<Shop> chitai = session.find<Shop>().where("name = ?").bind("Chitai-gorod");

			//stock1->book = code;
			//stock1->shop = ozon;
			//stock1->count = 7;

			//Wt::Dbo::ptr<Stock> stock1Ptr = session.add(std::move(stock1));

			//std::unique_ptr<Stock> stock2{ new Stock() };

			//stock2->book = code;
			//stock2->shop = chitai;
			//stock2->count = 0;

			//Wt::Dbo::ptr<Stock> stock2Ptr = session.add(std::move(stock2));

			//transaction.commit();

		}

		// Добавляем продажу
		{
			//Wt::Dbo::Transaction transaction{ session };
			//Wt::Dbo::ptr<Stock> stock = session.find<Stock>().where("id = ?").bind(3);

			//std::unique_ptr<Sale> sale{ new Sale() };
			//sale->stock = stock;
			//sale->count = 10;
			//sale->date_sale = "04.10.2024";
			//sale->price = 1500;

			//Wt::Dbo::ptr<Sale> salePtr = session.add(std::move(sale));

			//transaction.commit();
		}

		Wt::Dbo::Transaction transaction{ session };

		int select;
		std::string search;
		std::cout << "Search by:\n";
		std::cout << "1. ID\n";
		std::cout << "2. Name\n";
		std::cin >> select;

		Wt::Dbo::ptr<Publisher> findPub;

		if (select == 1) {
			std::cout << "Enter ID: ";
			std::cin >> search;
			findPub = session.find<Publisher>().where("id = ?").bind(std::stoi(search));
		}
		else if (select == 2) {
			std::cout << "Enter name: ";
			std::cin >> search;
			findPub = session.find<Publisher>().where("name = ?").bind(search);
		}

		Wt::Dbo::collection<Wt::Dbo::ptr<Book>> findBooks = session.find<Book>().where("publisher_id = ?").bind(findPub.id());

		for (auto& books : findBooks) {
			Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> findStock = session.find<Stock>().where("book_id = ?").bind(books.id());
			for (auto& fb : findStock) {
				if (fb->count > 0) {
					Wt::Dbo::ptr<Shop> findShop = session.find<Shop>().where("id = ?").bind(fb->shop);
					std::cout << "Shop " << findShop->name << ", has " << fb->count << " books: " << books->title << std::endl;
				}
				else continue;
			}
		}

		transaction.commit();
	}
	catch (const Wt::Dbo::Exception& e)
	{
		std::cout << "DB error: " << e.what() << std::endl;
	}
}