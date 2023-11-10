#include <bits/stdc++.h>
using namespace std;

struct UserInfo {
	string username;
	string password;
	string name;
	string email;
	bool isAdmin;

	void viewProfile() {
		cout << "Name: " << name << "\n";
		cout << "Email: " << email << "\n";
		cout << "User name: " << username << "\n";
		cout << endl;
	}

	bool sameUserCredentials(UserInfo &user) {
		return (user.username == this->username && user.password == this->password);
	}
};

class Login {
private:
	vector<UserInfo> users;

	UserInfo login() {
		UserInfo user;

		cout << "Enter user name (No spaces): ";
		cin >> user.username;

		cout << "Enter password (No spaces): ";
		cin >> user.password;

		for (auto &registeredUser: users)
			if (registeredUser.sameUserCredentials(user))
				return user;

		return {"#", "#", "#", "#", 0};
	}

	UserInfo signup() {
		UserInfo user;

		cout << "Enter user name (No spaces): ";
		cin >> user.username;

		cout << "Enter password (No spaces): ";
		cin >> user.password;

		cout << "Enter name (No spaces): ";
		cin >> user.name;

		cout << "Enter email (No spaces): ";
		cin >> user.email;

		users.push_back(user);
		return user;
	}

public:
	Login() {}

	UserInfo loginMenu() {
		cout << "Menu:\n";
		cout << "\t1. Login\n";
		cout << "\t2. Sign Up\n";
		cout << endl;

		int choice;
		cout << "Enter number in range 1 - 2: ";
		cin >> choice;

		if (choice == 1)
			return login();
		else
			return signup();
	}
};

class Book {
private:
	vector<string> pages;
	int curPage;
	int isbn;
	string title;
	string author;

	void nextPage() {
		if (curPage < pages.size())
			++curPage;
	}

	void prevPage() {
		if (curPage > 0)
			--curPage;
	}

	void readPage() {
		assert(0 <= curPage && curPage < pages.size());
		cout << "Current Page: " << curPage+1 << "/" << pages.size() << "\n";
		cout << pages[curPage] << "\n\n";
	}

public:
	Book(int isbn, string title, string author) {
		this->isbn = isbn;
		this->title = title;
		this->author = author;
		this->curPage = 0;
	}

	void addPage(string page) {
		pages.push_back(page);
	}

	void menu() {
		int choice = 2;
		while (true) {
			if (choice == 1)
				nextPage(), readPage();
			else if (choice == 2)
				prevPage(), readPage();
			else
				break;

			cout << "Menu:\n";
			cout << "\t1. Next Page\n";
			cout << "\t2. Previous Page\n";
			cout << "\t3. Stop Reading\n";

			cout << "Enter number in range 1 - 3: ";
			cin >> choice;
		}
	}

	// Getters //
	string getName() {
		return this->title;
	}
};

class BookHandler {
private:
	vector<Book> books;

	void displayBooks() {
		cout << "Our current book collection:";
		for (int i = 0; i < books.size(); i++) 
			cout << "\t" << i+1 << " " << books[i].getName() << "\n";
	}

public:
	void addBook(Book book) {
		books.push_back(book);
	}

	Book selectBook() {
		displayBooks();
		
		cout << "\nWhich book to read?:";
		cout << "Enter number in range 1 - 3: ";
		int choice;
		cin >> choice;

		--choice;
		assert(0 <= choice && choice < books.size());
		return books[choice];
	}
};

int main() {
	
	return 0;
}