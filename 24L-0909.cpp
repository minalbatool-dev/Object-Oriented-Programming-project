// ========================== HEADER FILES ==========================
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

// ========================== HELPER CLASS ==========================
class Helper {
public:
	static int getLength(const char* buffer);
	static void copyString(char*& dest, const char* srs);
	static char* getStringFromBuffer(const char* buffer);
	static bool StringCompare(const char* str1, const char* str2);
};

// ========================== HELPER CLASS DEFINITION ==========================
int Helper::getLength(const char* buffer)
{
	int len = 0;
	for (int i = 0; buffer[i] != '\0'; i++)
	{
		len++;
	}
	return len;
}
void Helper::copyString(char*& dest, const char* srs)
{
	delete[] dest;

	int len = getLength(srs);
	dest = new char[len + 2];

	int i = 0;
	for (; srs[i] != '\0'; i++) {
		dest[i] = srs[i];
	}
	dest[i] = '\0';

}
char* Helper::getStringFromBuffer(const char* buffer)
{
	int len = getLength(buffer);
	char* string = 0;
	if (len > 0)
	{
		string = new char[len + 1];
		copyString(string, buffer);
	}
	return string;
}
bool Helper::StringCompare(const char* str1, const char* str2)
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i])
		{
			return false;
		}
		i++;
	}
	if (str1[i] == '\0' && str2[i] == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ========================== Forward DECLARATIONS ==========================
class Post;
class Controller;
class Object;
class Comment;




// ========================== DATE CLASS ==========================
class Date {
private:
	int day, month, year;
public:
	Date();
	void setDate(int d, int m, int y);
	void printDate();
	int compareDate(Date& other);
	bool IsSameDay(Date& other);
	int getYear();
	~Date() {};
	Date GetDateTwoDaysAgo();
};

// ========================== DATE CLASS DEFINITION ==========================
Date::Date() {
	day = 0;
	month = 0;
	year = 0;
}

void Date::setDate(int d, int m, int y) {
	day = d;
	month = m;
	year = y;
}

void Date::printDate() {
	cout << day << "/" << month << "/" << year;
}

int Date::compareDate(Date& other) {
	//return 0 if equal
	//return -1 if earlier
	//return 1 if later

	//comparing years first, then menoth and then days
	if (year > other.year) {
		return 1;
	}
	else if (year < other.year) {
		return -1;
	}
	else {
		if (month > other.month) {
			return 1;
		}
		else if (month < other.month) {
			return -1;
		}
		else {
			if (day > other.day) {
				return 1;
			}
			else if (day < other.day) {
				return -1;
			}
			else {
				return 0;
			}
		}
	}
}

bool Date::IsSameDay(Date& other) {
	if (day == other.day && month == other.month && year == other.year) {
		return true;
	}
	else {
		return false;
	}
}

int Date::getYear() {
	return year;
}

Date Date::GetDateTwoDaysAgo() {
	Date result = *this;

	if (result.day > 2) {
		result.day -= 2;
		return result;
	}

	// Handle month transition
	const int daysInMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	// Move to previous month
	if (result.month > 1) {
		result.month--;
	}
	else {
		result.month = 12;
		result.year--;
	}

	// Get days in new month (simplified - doesn't handle leap years)
	int daysInPrevMonth = daysInMonth[result.month - 1];

	// Calculate new day
	result.day = daysInPrevMonth - (2 - result.day);

	return result;
}

// ========================== COMMENT CLASS ==========================
class Comment {
private:
	char* ID;
	char* text;
	Object* author;
public:
	Comment();
	~Comment();
	void SetValues(const char* id, const char* commentText, Object* authorObj);
	void PrintComment();
	char* GetID();
	char* GetText();
	Object* GetAuthor();
};

// ========================== POST CLASS ==========================
class Post {
private:
	char* ID;
	char* description;
	Object* sharedBy;
	Date sharedDate;
	Object** likedBy;
	int likesCount;
	Controller* ctr;
	Comment** comments;
	int commentCount;
public:
	Post(Controller* ctr);
	~Post();
	void SetSharedBy(Object*);
	Object* GetSharedBy();
	void viewLikedBy();
	void ReadPostsFromFile(ifstream& fin);
	char* GetID();
	void ViewPost(Date&);
	char* GetDescription();
	void PrintTimeSince(Date& currentDate);
	void LikePost(Object* liker);
	bool postFound(const char* id, Post* post);
	void AddComment(const char* id, const char* text, Object* author, Controller& ctr);
	void viewComments();
	void SetID(char* id);
	void SetDescription(char* desc);
	void SetSharedDate(Date d);
	Date GetSharedDate();
};


// ========================== OBJECT CLASS ==========================
class Object {
protected:
	char* ID;
	Post** timeLine;
	int postCount;
public:
	Object();
	virtual ~Object() = 0;
	void setID(char* id);
	char* getID();
	virtual void printName() = 0;
	void AddPostToTimeline(Post* p);
	void ViewTimeline(Date&);
	virtual void PrintListView() = 0;
	int getPostCount();
	Post* getPostFromTimeline(int index);
	void ViewRecentTimeline(Date currentDate, Date twoDaysAgo, Post**& recentPosts, int& recentCount);
};

// ========================== OBJECT CLASS DEFINITION ==========================
Object::Object() {
	ID = nullptr;
	timeLine = nullptr;
	postCount = 0;
}
Object::~Object() {
	//does nothing
	//we don't want double delete issue
	//pure virtual
}

void Object::setID(char* id) {
	Helper::copyString(ID, id);
}

char* Object::getID() {
	return ID;
}

//Made printName and printlistview pure virtual functions
//this is because Object must be an abstract base class whose obj cannot be created
//there can be an object of type User or Page but not Object

void Object::AddPostToTimeline(Post* p) {
	if (timeLine == nullptr) {
		timeLine = new Post * [10];
		for (int i = 0; i < 10; i++) {
			timeLine[i] = nullptr;
		}
		postCount = 0;
	}

	if (postCount < 10) {
		timeLine[postCount] = p;
		postCount++;
	}
	else {
		cout << "Cannot add more posts.\n";
	}
}

void Object::ViewRecentTimeline(Date currentDate, Date twoDaysAgo, Post**& recentPosts, int& recentCount) {

	if (timeLine == nullptr || postCount == 0) {
		return;
	}

	for (int i = 0; i < postCount; i++) {

		if (timeLine[i]) {
			Date postDate = timeLine[i]->GetSharedDate();

			// Check if post was shared in the last 2 days
			if (postDate.compareDate(twoDaysAgo) >= 0 &&
				postDate.compareDate(currentDate) <= 0) {

				// Add to recent posts array
				recentPosts[recentCount++] = timeLine[i];
			}
		}
	}
}




// ========================== POST CLASS DEFINITION ==========================
Post::Post(Controller* ctr) : ctr(ctr) {
	ID = nullptr;
	description = nullptr;
	likesCount = 0;
	likedBy = new Object * [10];
	for (int i = 0; i < 10; i++) {
		likedBy[i] = nullptr;
	}
	sharedBy = nullptr;
	comments = nullptr;
	commentCount = 0;
}

Post::~Post() {
	delete[] ID;
	delete[] description;
	if (likedBy) {
		delete[] likedBy;
	}
	if (comments) {
		for (int i = 0; i < commentCount; i++) {
			delete comments[i];
		}
		delete[] comments;
	}
}

char* Post::GetDescription() {
	return description;
}

Date Post::GetSharedDate() {
	return sharedDate;
}

char* Post::GetID() {
	return ID;
}

void Post::PrintTimeSince(Date& currentDate) {
	if (sharedDate.IsSameDay(currentDate)) {
		cout << "1h";             //hardcoded
	}
	else {
		sharedDate.printDate();
	}
}

void Post::LikePost(Object* liker) {
	if (likesCount >= 10) {
		return;
	}
	likedBy[likesCount] = liker;
	likesCount++;
}

bool Post::postFound(const char* id, Post* post) {
	if (!this->ID || !id) {
		return false;
	}
	return Helper::StringCompare(this->ID, id);
}


void Post::SetSharedBy(Object* obj) {
	sharedBy = obj;
}

Object* Post::GetSharedBy() {
	return sharedBy;
}

void Post::viewLikedBy() {
	cout << "Post Liked By:\n";
	for (int i = 0; i < likesCount; i++) {
		likedBy[i]->PrintListView();
	}
}


void Post::ViewPost(Date& currentDate) {
	if (!sharedBy || !description) return;
	cout << "--- ";
	sharedBy->printName();
	cout << " (";
	PrintTimeSince(currentDate);
	cout << "):\n";
	cout << "   \"" << description << "\"\n";

	//show comments
	if (commentCount > 0) {
		viewComments();
	}
}

void Post::AddComment(const char* id, const char* text, Object* author, Controller& ctr) {
	if (commentCount >= 10) {
		cout << "Cannot add more than 10 comments." << endl;
		return;
	}

	if (!comments) {
		comments = new Comment * [10];
		for (int i = 0; i < 10; i++) {
			comments[i] = nullptr;
		}
	}

	// Create new comment
	comments[commentCount] = new Comment();
	comments[commentCount]->SetValues(id, text, author);
	commentCount++;
}

void Post::viewComments() {
	for (int i = 0; i < commentCount; i++) {
		if (comments[i]) {
			cout << "    ";
			comments[i]->PrintComment();
		}
	}
}


void Post::SetID(char* id) {
	Helper::copyString(ID, id);
}

void Post::SetDescription(char* desc) {
	Helper::copyString(description, desc);
}

void Post::SetSharedDate(Date date) {
	sharedDate = date;
}

//function of object class which needs full definition of post class
void Object::ViewTimeline(Date& currentDate) {

	cout << " - Timeline\n";
	cout << "------------------------------------------------------------\n";

	if (!timeLine || postCount == 0) {
		cout << "No posts to display.\n";
		return;
	}

	// Print posts in chronological order (newest first)
	for (int i = postCount - 1; i >= 0; i--) {
		if (timeLine[i]) {
			timeLine[i]->ViewPost(currentDate);
		}
	}


}


int Object::getPostCount() {
	return postCount;
}

Post* Object::getPostFromTimeline(int index) {
	if (index >= 0 && index < postCount) {
		return timeLine[index];
	}
	return nullptr;
}
// ========================== PAGE CLASS ==========================
class Page : public Object {
private:
	char* Title;
public:
	Page();
	~Page();
	void LoadDataFromFile(ifstream& fin);
	const char* GetID() const;
	const char* GetTitle() const;
	void SetID(const char* id);
	bool PageFound(const char* id);
	void printName();
	void PrintListView();
};

// ========================== PAGE CLASS DEFINITION ==========================
void Page::LoadDataFromFile(ifstream& fin) {
	char buffer[50];
	fin >> buffer;
	ID = Helper::getStringFromBuffer(buffer);
	fin.getline(buffer, 50);
	// Trim leading whitespace
	const char* titleStart = buffer;
	while (*titleStart == ' ' || *titleStart == '\t') {
		titleStart++;
	}
	Title = Helper::getStringFromBuffer(buffer);
}

Page::Page() {
	ID = Title = nullptr;
}
Page::~Page() {
	if (ID) delete[] ID;
	if (Title) delete[] Title;
}
//getter and setter
const char* Page::GetID() const {
	return ID;
}

const char* Page::GetTitle() const {
	return Title;
}

void Page::SetID(const char* id) {
	if (ID) {
		delete[] ID;
	}
	ID = new char[strlen(id) + 1];
	Helper::copyString(ID, id);
}

//will need this in searchPagebyId
bool Page::PageFound(const char* id) {
	if (!this->ID || !id) {
		return false;  // return false if either string is null
	}
	return Helper::StringCompare(this->ID, id);
}

void Page::printName() {
	cout << Title;
}

void Page::PrintListView() {
	cout << ID << " " << Title << endl;
}

// ========================== USER CLASS ==========================
class User : public Object {
private:
	char* fName;
	char* lName;
	User** Friends;
	Page** LikedPages;
	int totalFriends, totalLikedPages;
public:
	User();
	~User();
	void LoadDataFromFile(ifstream& fin);
	const char* GetID() const;
	void SetID(const char* id);
	void setFirstName(const char* fn);
	void setlastName(const char* ln);
	char* getfirstName();
	char* getlastName();
	void AddFriend(User* u);
	void LikePage(Page* p);
	void PrintListView();
	void ViewFriendList();
	void ViewLikedPages();
	bool userFound(const char* id);
	void printName();
	void ViewHome(Date currentDate);
};

// ========================== USER CLASS DEFINTION ==========================
User::User() {
	ID = fName = lName = nullptr;
	Friends = nullptr;
	LikedPages = nullptr;
	totalFriends = totalLikedPages = 0;
}
User::~User() {
	if (ID) delete[] ID;
	if (fName) delete[] fName;
	if (lName) delete[] lName;
	if (Friends) delete[] Friends;
	if (LikedPages) delete[] LikedPages;
	//these are the only pointers managed by this class
}

//getters and setters
const char* User::GetID() const {
	return ID;
}

void User::SetID(const char* id) {
	if (ID) {
		delete[] ID;                     //safe approach: either already nullptr by the constructor or clears old memory if object created already
	}
	ID = new char[Helper::getLength(id) + 1];
	Helper::copyString(ID, id);
}

void User::setFirstName(const char* fn) {
	if (fName) {
		delete[] fName;
	}
	fName = new char[Helper::getLength(fn) + 1];
	Helper::copyString(fName, fn);
}

void User::setlastName(const char* ln) {
	if (lName) {
		delete[] lName;
	}
	lName = new char[Helper::getLength(ln) + 1];
	Helper::copyString(lName, ln);
}

char* User::getfirstName()
{
	return fName;
}
char* User::getlastName()
{
	return lName;
}

void User::LoadDataFromFile(ifstream& fin) {
	char buffer[50];
	fin >> buffer;
	ID = Helper::getStringFromBuffer(buffer);
	fin >> buffer;
	fName = Helper::getStringFromBuffer(buffer);
	fin >> buffer;
	lName = Helper::getStringFromBuffer(buffer);
}


void User::AddFriend(User* u) {
	//if the user currently has no friends, allocate memory for friends
	if (Friends == nullptr) {
		Friends = new User * [10];
	}
	if (totalFriends < 10)
	{
		//make the current friend pointer point to u
		Friends[totalFriends] = u;
		totalFriends++;                          //increment total friends
	}
	else
	{
		cout << "Error: Cannot add more than 10 friends." << endl;
	}
}


void User::LikePage(Page* p) {
	if (LikedPages == nullptr) {
		LikedPages = new Page * [10];
	}
	if (totalLikedPages < 10)
	{
		LikedPages[totalLikedPages] = p;
		totalLikedPages++;
	}
	else
	{
		cout << "Error: Cannot like more than 10 pages." << endl;
	}
}

void User::PrintListView() {
	cout << ID << " " << fName << " " << lName << endl;
}

void User::ViewFriendList() {
	cout << this->fName << " " << this->lName << " - Friend List" << endl;
	cout << endl;

	for (int i = 0; i < totalFriends; i++)
	{
		this->Friends[i]->PrintListView();
	}
}


void User::ViewLikedPages() {
	cout << this->fName << " " << this->lName << " - Liked Pages" << endl;
	cout << endl;

	for (int i = 0; i < totalLikedPages; i++)
	{
		this->LikedPages[i]->PrintListView();
	}

}

//will need this in searchUserbyId
bool User::userFound(const char* id) {
	if (!this->ID || !id) {
		return false;  // return false if either string is null
	}
	return Helper::StringCompare(this->ID, id);
}

void User::printName() {
	cout << fName << " " << lName;
}

void User::ViewHome(Date currentDate) {
	cout << this->fName << " " << this->lName << " - Home Page\n\n";
	cout << "// Showing posts from last 2 days\n\n";

	// Get date from 2 days ago
	Date twoDaysAgo = currentDate.GetDateTwoDaysAgo();

	// Array to store recent posts
	Post** recentPosts = new Post * [100]; // Temporary large size
	int recentCount = 0;

	// 1. Current user's posts
	this->ViewRecentTimeline(currentDate, twoDaysAgo, recentPosts, recentCount);

	// 2. Friends' posts
	for (int i = 0; i < totalFriends; i++) {
		if (Friends[i]) {
			Friends[i]->ViewRecentTimeline(currentDate, twoDaysAgo, recentPosts, recentCount);
		}
	}

	// 3. Liked pages' posts
	for (int i = 0; i < totalLikedPages; i++) {
		if (LikedPages[i]) {
			LikedPages[i]->ViewRecentTimeline(currentDate, twoDaysAgo, recentPosts, recentCount);
		}
	}

	// Display all collected posts
	for (int i = 0; i < recentCount; i++) {
		if (recentPosts[i]) {
			recentPosts[i]->ViewPost(currentDate);
			cout << endl;
		}
	}

	delete[] recentPosts;
}



// ========================== COMMENT CLASS DEFINITION ==========================
Comment::Comment() {
	ID = nullptr;
	text = nullptr;
	author = nullptr;
}

Comment::~Comment() {
	if (ID) delete[] ID;
	if (text) delete[] text;
	// Don't delete author as it's not owned by Comment
}

void Comment::SetValues(const char* id, const char* commentText, Object* authorObj) {
	Helper::copyString(ID, id);
	Helper::copyString(text, commentText);
	author = authorObj;
}

void Comment::PrintComment() {
	cout << "    ";
	if (author) {
		author->printName();
	}
	cout << ": \"" << text << "\"\n";
	//the character \" prints a double quote
}

char* Comment::GetID() {
	return ID;
}

char* Comment::GetText() {
	return text;
}

Object* Comment::GetAuthor() {
	return author;
}

// ========================== MEMORY CLASS ===================================
class Memory : public Post {
private:
	Post* originalPost;
public:
	Memory(Controller* ctr, Post* original, char* text, Object* sharedBy, Date shareDate);
	~Memory();
	void ViewPost(Date currentDate);
	Post* GetOriginalPost();
};


// ========================== MEMORY CLASS DEFINITION =========================
Memory::Memory(Controller* ctr, Post* original, char* text, Object* sharedBy, Date shareDate) : Post(ctr) {
	originalPost = original;

	// Set up memory post using Post's public interface
	this->SetID(Helper::getStringFromBuffer("memory"));
	this->SetDescription(Helper::getStringFromBuffer(text));
	this->SetSharedBy(sharedBy);
	this->SetSharedDate(shareDate);
}

Memory::~Memory() {
	// No dynamic members to delete beyond what Post handles
}

void Memory::ViewPost(Date currentDate) {
	// Show memory header
	cout << "~~~ ";
	this->GetSharedBy()->printName(); // Using GetSharedBy()
	cout << " shared a memory ~~~ (";
	this->PrintTimeSince(currentDate); // Using inherited method
	cout << ")\n";

	// Show memory text
	if (this->GetDescription()) { // Using GetDescription()
		cout << "   \"" << this->GetDescription() << "\"\n";
	}

	// Calculate and show years ago
	int yearsAgo = currentDate.getYear() - originalPost->GetSharedDate().getYear();
	cout << "   ~~~ " << yearsAgo << " years ago ~~~\n";

	// Show original post
	originalPost->ViewPost(currentDate);
}

Post* Memory::GetOriginalPost() {
	return originalPost;
}

// ========================== CONTROLLER CLASS ==========================
class Controller {
	User** AllUsers;
	Page** AllPages;
	Post** AllPosts;
	int totalUsers, totalPages, totalPosts;
	User* currentUser;
	Date currentDate;
public:
	Controller();
	~Controller();
	User* SearchUserByID(const char* id);
	Page* SearchPageByID(const char* id);
	Post* SearchPostByID(const char* id);
	Object* GetObjectByID(const char* id);
	void LoadUsersFromFile(const char* filename);
	void LoadPagesFromFile(const char* filename);
	void LoadPostsFromFile(const char* filename);
	void LoadCommentsFromFile(const char* filename);
	void LinkUsersAndPages(const char* filename);
	void LoadData();
	void ShareMemory(const char* postID, const char* memoryText);
	void SeeYourMemories();
	void ViewHome();
	void Run();
};

// ========================== CONTROLLER DEFINITIONS ==========================
Controller::Controller() {
	AllUsers = nullptr;
	AllPages = nullptr;
	AllPosts = nullptr;
	totalUsers = totalPages = totalPosts = 0;
	currentUser = nullptr;
}
Controller::~Controller() {
	if (AllUsers) {
		for (int i = 0; i < totalUsers; i++) {
			delete AllUsers[i];
		}
		delete[] AllUsers;
	}
	if (AllPages) {
		for (int i = 0; i < totalPages; i++) {
			delete AllPages[i];
		}
		delete[] AllPages;
	}
	if (AllPosts) {
		for (int i = 0; i < totalPosts; i++) {
			delete AllPosts[i];
		}
		delete[] AllPosts;
	}
}
User* Controller::SearchUserByID(const char* id) {
	for (int i = 0; i < totalUsers; i++) {
		if (AllUsers[i]->userFound(id)) {
			return AllUsers[i];
		}
	}
	return nullptr;
}
Page* Controller::SearchPageByID(const char* id) {
	for (int i = 0; i < totalPages; i++) {
		if (AllPages[i]->PageFound(id)) {
			return AllPages[i];
		}
	}
	return nullptr;
}
Post* Controller::SearchPostByID(const char* id) {
	for (int i = 0; i < totalPosts; i++) {
		if (AllPosts[i] && AllPosts[i]->postFound(id, AllPosts[i])) {
			return AllPosts[i];
		}
	}
	return nullptr;
}
Object* Controller::GetObjectByID(const char* id)
{
	if (!id) return nullptr;

	if (id[0] == 'p') {
		Page* page = SearchPageByID(id);
		return page;  // Implicit typecasting
	}
	else if (id[0] == 'u') {
		User* user = SearchUserByID(id);
		return user;  // Implicit typecasting
	}

	return nullptr;
}
void Controller::LoadUsersFromFile(const char* filename) {
	ifstream fin(filename);
	if (fin) {
		fin >> totalUsers;
		AllUsers = new User * [totalUsers];
		for (int i = 0; i < totalUsers; i++) {
			AllUsers[i] = new User;
			AllUsers[i]->LoadDataFromFile(fin);
		}
		fin.close();
	}
	else {
		cout << "Could not open file for loading users." << endl;
	}
}
void Controller::LoadPagesFromFile(const char* filename) {
	ifstream fin(filename);
	if (fin)
	{
		fin >> totalPages;
		AllPages = new Page * [totalPages];
		for (int i = 0; i < totalPages; i++)
		{
			AllPages[i] = new Page;
			AllPages[i]->LoadDataFromFile(fin);  // Fixed method call
		}
		fin.close();
	}
	else
	{
		cout << "Could not open file for laoding pages." << endl;
	}
}

void Post::ReadPostsFromFile(ifstream& fin) {
	//read ID
	char buffer[1000];
	fin >> buffer;
	ID = Helper::getStringFromBuffer(buffer);

	//read date
	int day, month, year;
	fin >> day >> month >> year;
	sharedDate.setDate(day, month, year);

	//read post description
	fin.ignore();
	fin.getline(buffer, sizeof(buffer));
	Helper::copyString(description, buffer);

	fin >> buffer;
	sharedBy = ctr->GetObjectByID(buffer); //non-static function must be called on object

	likesCount = 0;
	while (fin >> buffer) {
		if (Helper::StringCompare(buffer, "-1")) {
			break;
		}
		Object* liker = ctr->GetObjectByID(buffer);
		if (liker && likesCount < 10) {
			likedBy[likesCount++] = liker;
		}
	}
};

void Controller::LoadPostsFromFile(const char* filename)
{
	ifstream fin(filename);
	if (!fin) {
		cout << "Could not open file for loading posts." << endl;
		return;
	}

	fin >> totalPosts;
	AllPosts = new Post * [totalPosts];

	fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip rest of count line
	fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip full separator

	for (int i = 0; i < totalPosts; i++) {
		Post* post = new Post(this);

		post->ReadPostsFromFile(fin);

		// Add post to sharer's timeline if sharer is valid
		Object* sharer = post->GetSharedBy();
		if (sharer != nullptr) {
			sharer->AddPostToTimeline(post);
		}

		AllPosts[i] = post;

		if (i < totalPosts - 1) {
			fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip post data tail
			fin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip separator

		}
	}

	fin.close();
}

void Controller::LoadCommentsFromFile(const char* filename) {
	ifstream fin(filename);
	if (!fin) {
		cout << "Could not open file for loading comments." << endl;
		return;
	}

	int totalComments;
	fin >> totalComments;

	for (int i = 0; i < totalComments; i++) {
		char commentID[10], postID[10], authorID[10];
		char commentText[500];

		fin >> commentID >> postID >> authorID;
		fin.ignore();
		fin.getline(commentText, sizeof(commentText));

		Post* post = SearchPostByID(postID);
		Object* author = GetObjectByID(authorID);

		if (post && author) {
			//add comment to the post
			post->AddComment(commentID, commentText, author, *this);
		}
	}

	fin.close();
}


void Controller::LinkUsersAndPages(const char* filename) {
	ifstream fin(filename);
	if (!fin) {
		cout << "Could not open file to link users and pages." << endl;
		return;
	}

	char userID[5];
	while (true) {
		fin >> userID;
		if (Helper::StringCompare(userID, "-1")) {
			break;
		}

		User* currentUser = SearchUserByID(userID);
		if (!currentUser) {
			//move on to next iteration
			continue;
		}

		//finding friends
		char friendID[5];
		while (true) {
			fin >> friendID;
			if (Helper::StringCompare(friendID, "-1")) {
				break;
			}
			User* friendUser = SearchUserByID(friendID);
			if (friendUser) {
				currentUser->AddFriend(friendUser);
			}
		}

		//finding liked pages
		char pageID[5];
		while (true)
		{
			fin >> pageID;
			if (Helper::StringCompare(pageID, "-1")) {
				break;
			}
			Page* likedPage = SearchPageByID(pageID);
			if (likedPage) {
				currentUser->LikePage(likedPage);
			}
		}
	}
	fin.close();
}
void Controller::LoadData() {
	LoadUsersFromFile("SocialNetworkUsers.txt");
	LoadPagesFromFile("SocialNetworkPages.txt");
	LinkUsersAndPages("UsersFriendsAndLikedPages.txt");
	LoadPostsFromFile("SocialNetworkPosts.txt");
	LoadCommentsFromFile("SocialNetworkComments.txt");
}

void Controller::ShareMemory(const char* postID, const char* memoryText) {
	if (!currentUser || !postID || !memoryText) {
		return;
	}

	Post* originalPost = SearchPostByID(postID);
	if (!originalPost) {
		return;
	}

	Memory* memory = new Memory(this, originalPost, Helper::getStringFromBuffer(memoryText), currentUser, currentDate);

	currentUser->AddPostToTimeline(memory);
}

void Controller::SeeYourMemories() {
	if (!currentUser) {
		return;
	}

	cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n\n";
	cout << "On this Day\n";

	// Check all posts in the user's timeline for ones shared on the same day in previous years
	for (int i = 0; i < currentUser->getPostCount(); i++) {
		Post* post = currentUser->getPostFromTimeline(i);
		if (!post) continue;

		Date postDate = post->GetSharedDate();
		if (postDate.IsSameDay(currentDate)) {
			// This post was shared on the same day in a different year
			int yearsAgo = currentDate.getYear() - postDate.getYear();
			if (yearsAgo > 0) {
				cout << yearsAgo << " Years Ago\n";
				post->ViewPost(currentDate);
				cout << endl;
			}
		}
	}
	cout << "---------------------------------------------------------------------------------------------\n";
}

void Controller::ViewHome() {
	if (currentUser) {
		currentUser->ViewHome(currentDate);
	}
}




void Controller::Run() {
	//Set initial system date
	currentDate.setDate(17, 4, 2024);
	cout << "Command:                Set current System Date 17 4 2024\n";
	cout << "System Date:            ";
	currentDate.printDate();
	cout << endl << endl;

	// Set current user to u7 (Abdullah Nadeem)
	char id[] = "u7";
	currentUser = SearchUserByID(id);
	cout << "Command:                Set current user \"" << id << "\"\n";
	cout << currentUser->getfirstName() << " " << currentUser->getlastName() << " successfully set as Current User\n\n";

	// View Friend List
	cout << "Command:                View Friend List\n";
	cout << "---------------------------------------------------------------------------------------------\n";
	currentUser->ViewFriendList();
	cout << "---------------------------------------------------------------------------------------------\n\n";

	// View Liked Pages
	cout << "Command:                  View Liked Pages\n";
	cout << "---------------------------------------------------------------------------------------------\n";
	currentUser->ViewLikedPages();
	cout << "---------------------------------------------------------------------------------------------\n\n";

	// View Timeline
	cout << "Command:                  View TimeLine\n";
	cout << "---------------------------------------------------------------------------------------------\n";
	currentUser->printName();
	currentUser->ViewTimeline(currentDate);
	cout << "---------------------------------------------------------------------------------------------\n\n";

	// View Liked List for post5
	cout << "Command: ViewLikedList(post5)\n";
	cout << "------------------------------------------------------------\n";
	Post* post = SearchPostByID("post5");
	if (post) {
		post->viewLikedBy();
	}
	cout << "------------------------------------------------------------\n\n";

	// Like post5
	cout << "Command: LikePost(post5)\n";
	if (post) {
		post->LikePost(currentUser);
	}
	cout << "------------------------------------------------------------\n\n";

	// View Liked List for post5 again
	cout << "Command: ViewLikedList(post5)\n";
	cout << "------------------------------------------------------------\n";
	if (post) {
		post->viewLikedBy();
	}
	cout << "------------------------------------------------------------\n\n";

	// View Page p1 (Food Fusion)
	cout << "Command: ViewPage(p1)\n";
	cout << "------------------------------------------------------------\n";
	Page* page = SearchPageByID("p1");
	if (page) {
		page->ViewTimeline(currentDate);
	}
	cout << "------------------------------------------------------------\n\n";

	// View Home Page
	cout << "Command: View Home\n";
	cout << "------------------------------------------------------------\n";
	ViewHome();
	cout << "------------------------------------------------------------\n\n";

	// Comment on post4
	cout << "Command: PostComment(post4, Good Luck for your Result)\n";
	Post* post4 = SearchPostByID("post4");
	if (post4) {
		post4->AddComment("comment1", "Good Luck for your Result", currentUser, *this);
	}
	cout << "------------------------------------------------------------\n\n";

	// View post4 to see the comment
	cout << "Command: ViewPost(post4)\n";
	cout << "------------------------------------------------------------\n";
	if (post4) {
		post4->ViewPost(currentDate);
	}
	cout << "------------------------------------------------------------\n\n";

	// Comment on post8
	cout << "Command: PostComment(post8, Thanks for the wishes)\n";
	Post* post8 = SearchPostByID("post8");
	if (post8) {
		post8->AddComment("comment2", "Thanks for the wishes", currentUser, *this);
	}
	cout << "------------------------------------------------------------\n\n";

	// View post8 to see the comment
	cout << "Command: ViewPost(post8)\n";
	cout << "------------------------------------------------------------\n";
	if (post8) {
		post8->ViewPost(currentDate);
	}
	cout << "------------------------------------------------------------\n\n";

	// See Your Memories
	cout << "Command: SeeYourMemories()\n";
	cout << "------------------------------------------------------------\n";
	SeeYourMemories();
	cout << "------------------------------------------------------------\n\n";

	// Share a memory
	cout << "Command: ShareMemory(post10, \"Never thought I will be specialist in this field...\")\n";
	Post* post10 = SearchPostByID("post10");
	if (post10) {
		ShareMemory("post10", "Never thought I will be specialist in this field...");
	}
	cout << "------------------------------------------------------------\n\n";

	// View Timeline again to see the shared memory
	cout << "Command: View Timeline\n";
	cout << "------------------------------------------------------------\n";
	currentUser->ViewTimeline(currentDate);
	cout << "------------------------------------------------------------\n\n";

	// Switch to user u11 (Ahsan Ali)
	cout << "Command: Set current user \"u11\"\n";
	currentUser = SearchUserByID("u11");
	cout << currentUser->getfirstName() << " " << currentUser->getlastName() << " successfully set as Current User\n\n";

	// View Timeline for new user
	cout << "Command: ViewTimeline\n";
	cout << "------------------------------------------------------------\n";
	currentUser->ViewTimeline(currentDate);
	cout << "------------------------------------------------------------\n\n";
}

int main()
{
	Controller myApp;
	myApp.LoadData();
	myApp.Run();
	return 0;
}