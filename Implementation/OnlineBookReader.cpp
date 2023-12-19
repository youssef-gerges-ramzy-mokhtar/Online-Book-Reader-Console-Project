#include <bits/stdc++.h>
using namespace std;

/*
	Notes:
		1. Think of using Templates with the UserInfo Mapper
		class UserInfoMapper {
		private:
			unordered_map<string, AdminUser> admins;
			unordered_map<string, NormalUser> normalUsers;

		public:
			// AdminUser getAdminUser(string username) {
				// if (admins.count(username))
					// return admins[username];

				// AdminUser adminUser;
				// admins[username]
			// }

			// NormalUser getNormalUser() {

			// }
		};
		
	Bugs:
		1. When normal user logouts and log back-in all his sessions data is lost
			- That is because in the Login Class we are storing just the UserInfo and every time the user Logs in we create a new NormalUser Object

*/

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

int menuDisplay(const vector<string> &options) {
	cout << "Menu:\n";
	for (int i = 0; i < options.size(); i++)
		cout << "\t" << i+1 << ". " << options[i] << "\n";
	
	cout << "\n";

	int choice;
	cout << "Enter number in range 1 - " << options.size() << ": ";
	cin >> choice;

	assert(1 <= choice && choice <= options.size());
	return choice;
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

class Book {
private:
	vector<string> pages;
	int curPage;
	int isbn;
	string title;
	string author;

	void nextPage() {
		if (curPage < (int) pages.size()-1)
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
		vector<string> options = {"Next Page", "Previous Page", "Stop Reading"};

		while (true) {
			readPage();
			choice = menuDisplay(options);

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
		cout << "Our current book collection:\n";
		for (int i = 0; i < books.size(); i++) 
			cout << "\t" << i+1 << " " << books[i].getName() << "\n";
	}

	BookHandler() {}

public:
	void addBook(Book book) {
		books.push_back(book);
	}

	Book selectBook() {
		displayBooks();
		
		cout << "\nWhich book to read?:\n";
		cout << "Enter number in range 1 - " << books.size() << ": ";
		int choice;
		cin >> choice;

		--choice;
		assert(0 <= choice && choice < books.size());
		return books[choice];
	}

	static BookHandler& getInstance() {
		static BookHandler bookHandler = BookHandler();
		return bookHandler;
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
	void addBook(Book book) {
		sessions.push_back({book, currentDateTime()});
	}

	void menu() {
		displayHistory();
		
		int choice;
		cout << "\nWhich sessions to open?: \n";
		cout << "Enter number in range 1 - " << sessions.size() << ": ";
		cin >> choice;

		--choice;
		assert(0 <= choice && choice < sessions.size());

		sessions[choice].first.menu();
		sessions[choice].second = currentDateTime();
	}
};

class AdminUser {
private:
	UserInfo user;
	BookHandler &bookHandler;

	void addBook() {
		int isbn;
		string title, author;

		cout << "Enter ISBN: ";
		cin >> isbn;

		cout << "Enter Title: ";
		cin >> title;

		cout << "Enter Author Name: ";
		cin >> author;

		Book book = Book(isbn, title, author);

		int numPages;
		cout << "Enter How many pages: ";
		cin >> numPages;

		assert(numPages >= 0);
		for (int i = 0; i < numPages; i++) {
			string page;
			cout << "Enter page # " << i+1 << ": ";
			cin >> page;

			book.addPage(page);
		}

		bookHandler.addBook(book);
		cout << "\n";
	}

public:
	AdminUser(UserInfo user) : 
		user(user),
		bookHandler(BookHandler::getInstance()) {}

	void menu() {
		while (true) {
			vector<string> options = {"View Profile", "Add Book", "Logout"};
			int choice = menuDisplay(options);

			if (choice == 1)
				user.viewProfile();
			else if (choice == 2)
				addBook();
			else
				break;
		}
	}
};

class NormalUser {
private:
	UserInfo user;
	BookHandler &bookHandler;
	ReadingHistory readingHistory;

	void browseBooks() {
		Book book = bookHandler.selectBook();
		book.menu();
		this->readingHistory.addBook(book);
	}

	void viewHistory() {
		readingHistory.menu();
	}

public:
	NormalUser(UserInfo user) : user(user), bookHandler(BookHandler::getInstance()) {
		this->readingHistory = ReadingHistory();
	}

	void menu() {
		while (true) {
			vector<string> options = {"View Profile", "List & Select from My Reading History", "List & Select from Available Books", "Logout"}; 
			int choice = menuDisplay(options);

			if (choice == 1)
				user.viewProfile();
			else if (choice == 2)
				viewHistory();
			else if (choice == 3)
				browseBooks();
			else
				break;
		}
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
				return registeredUser;

		return {"#", "#", "#", "#", 0};
	}

	UserInfo signup() {
		UserInfo user;
		user.isAdmin = false;

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

	void hardCodeAdminUsers() {
		UserInfo admin1 = {"youssef", "youssef123", "youssef_gerges_ramzy", "youssef@yahoo.com", true};
		UserInfo admin2 = {"mostafa", "mostafa321", "mostafa_saad_ibrahim", "mostafa@gmail.com", true};
		users.push_back(admin1);
		users.push_back(admin2);
	}

public:
	Login() {
		this->hardCodeAdminUsers();
	}

	UserInfo loginMenu() {
		vector<string> options = {"Login", "Sign Up"};
		int choice = menuDisplay(options);

		if (choice == 1)
			return login();
		else
			return signup();
	}
};

class OnlineBookReader {
private:
	Login login;

public:
	OnlineBookReader() {
		this->login = Login();
	}

	void run() {
		while (true) {
			UserInfo userInfo = login.loginMenu();
			cout << "Hello " << userInfo.name << " | ";

			if (userInfo.isAdmin) {
				cout << "Admin View\n\n";
				AdminUser user = AdminUser(userInfo);
				user.menu();
			} else {
				cout << "User View\n\n";
				NormalUser user = NormalUser(userInfo);
				user.menu();
			}
		}
	}
};

int main() {
	OnlineBookReader onlineBookReader;
	onlineBookReader.run();

	return 0;
}