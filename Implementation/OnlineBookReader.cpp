#include <bits/stdc++.h>
using namespace std;

// Helper Functions //
string currentDateTime() {
    auto currentTime = std::chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(currentTime);
    string format = "%Y-%m-%d %H:%M:%S";
    
    tm* timeinfo = localtime(&time);
    char buffer[70];
    strftime(buffer, sizeof(buffer), format.c_str(), timeinfo);
    return buffer;
}
// Helper Functions //

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
		cout << "Current Page: " << this->currentPage() << "\n";
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
			readPage();

			cout << "Menu:\n";
			cout << "\t1. Next Page\n";
			cout << "\t2. Previous Page\n";
			cout << "\t3. Stop Reading\n";

			cout << "Enter number in range 1 - 3: ";
			cin >> choice;

			if (choice == 1)
				nextPage();
			else if (choice == 2)
				prevPage();
			else
				break;
		}
	}

	// Getters //
	string getName() {
		return this->title;
	}

	string currentPage() {
		return to_string(curPage+1) + "/" + to_string(pages.size());
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

class ReadingHistory {
private:
	vector<pair<Book, string>> sessions;

	void displayHistory() {
		for (int i = 0; i < sessions.size(); i++) {
			auto &book = sessions[i].first;
			auto &sessionTime = sessions[i].second;
			cout << i+1 << ": " << book.getName() << ": " << book.currentPage() << " - " << sessionTime << endl;
		}
	}

public:
	addBook(Book book) {
		sessions.push_back({book, currentDateTime()});
	}

	void menu() {
		displayHistory();
		
		int choice;
		cout << "\nWhich sessions to opne?:";
		cout << "Enter number in range 1 - " << sessions.size() << ": ";
		cin >> choice;

		--choice;
		assert(0 <= choice && choice < sessions.size());

		sessions[choice].first.menu();
	}
};

int main() {
	return 0;
}