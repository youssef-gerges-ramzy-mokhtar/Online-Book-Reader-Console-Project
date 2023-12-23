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
	static BookHandler *bookHandler;

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

	static BookHandler* getInstance() {
		if (bookHandler == nullptr)
			bookHandler = new BookHandler();

		return bookHandler;
	}
};
BookHandler* BookHandler::bookHandler = nullptr;

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
		if (sessions.size() == 0) {
			cout << "You don't have any active sessions\n";
			return;
		}

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
	BookHandler *bookHandler;

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

		bookHandler->addBook(book);
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
	BookHandler *bookHandler;
	ReadingHistory readingHistory;

	void browseBooks() {
		Book book = bookHandler->selectBook();
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

class UserInfoMapper {
private:
	unordered_map<string, NormalUser> users;

public:
	NormalUser& getNormalUser(UserInfo userInfo) {
		if (!users.count(userInfo.username)) {
			NormalUser user = NormalUser(userInfo);
			users.emplace(userInfo.username, user);
		}

		return users.at(userInfo.username);
	}
};

class OnlineBookReader {
private:
	Login login;
	UserInfoMapper userInfoMapper;

public:
	OnlineBookReader() {
		this->login = Login();
		this->userInfoMapper = UserInfoMapper();
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
				NormalUser& user = userInfoMapper.getNormalUser(userInfo);
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

/*
	Personal Notes

	1. Design notes:
		- Speed
			. The code is to some extent fast, maybe one suggestion is for the login to use a unordered_map instead of just a vector
		
		- Code Structure ease of understanding
			. The code is very logical and very easy to understand and to reason about

		- Keep it Stuip Simple (KISS)
			. the code is to some extent simple

		- You ain't gonna need it (YAGNI)
			. We are not implementing any code that doesn't directly server the functional requrirements of the project

		- DRY (Don't repeat yourself)
			. The code is to some extent very DRY
			. Maybe the only thing that I can critique is that we are using the menu() function in multiple classes (probably not bad)

		- Decoupled system
			. Here this might be the worst aspect of the design as the program logic is mixed with the user interface (aka. printing to the console)
			. There might have been 2 solutions to this problem but I felt that it will make the code much more complicated and length:
				- We could have created a UI Class for every class we have in the project e.g. NormalUserUi, BookUi, BookHandlerUi, etc.. and every UI Class will be responsible for getting the input from the console (User Interface) and each UI Class will have an instance of the normal class and just make a class to the normal class with the inputs got from the user
				
				e.g. NormalUserUi will have an instance of NormalUser and will call getHistory() which will return a collection of History Sessions to the NormalUserUi class and it is responsible for displaying it

				But this means that the programmer who is responsible for the front-end is tightly coupled with the classes responsbiel for the main logic

				- A 2nd approach would have been that we will provide a single Wrapper class that has all the methods that the front-end/UI might need to use to operate and this Wrapper Class will contain direct the front-end method calls to the Objects responsbile for the code-logic and return the results back to the front-end
				
				e.g. a OnlineBookReaderManager that has an instance of Login, NormalUser, etc..
				The front-end will make a request as following to the OnlineBookReaderManger getAllSessions(username, email)
				then the OnlineBookReaderManger will use the Login instance to get the User and will use the NormalUser instance returned by callin gthe getSessions() method of the NormalUser Instance and will return the result back to the front-end

				But this means that the OnlineBookReaderManger have to be very tightly coupled with all the Classes that provide the main logic of the program

			. Last note regarding the decoupling between the Program Logic and User Interface It is Ok for the User Interface Classes to depend on the Logic Classes in any way but it is not OK for the Program Logic to depend on the User Interface. Given that the User Interface should be coupled with the Logic and in that case this means that the User Interface should be able to understand how the Logic is working but the Logic shouldn't be coupled with the User Interface

		- Single Responsibility
			. YES every class is responsible for its own tasks

	2. MISC:
		- It might have been a good idea in the BookHandler to make the data members static and update the static members without making the class singeltong and this would have the same effect of a singelton class. It would been a good idea because for all the classes that was used to creating instances of the BookHandler they will not need to change
*/