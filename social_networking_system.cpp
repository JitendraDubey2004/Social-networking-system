#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// Post class representing individual posts
class Post {
private:
    string content;

public:
    // Constructors
    Post() : content("") {}
    Post(const string& content) : content(content) {}

    // Getters
    string getContent() const { return content; }

    // Display Post information
    void display() const {
        cout << "Post: " << content << endl;
    }

    // Overloading << operator for file output
    friend ofstream& operator<<(ofstream& ofs, const Post& post) {
        ofs << post.content << endl;
        return ofs;
    }
// Overloading >> operator for file input
    friend ifstream& operator>>(ifstream& ifs, Post& post) {
        getline(ifs, post.content);
        return ifs;
    }
};

// User class representing individual user profiles
class User {
private:
    string username;
    string password;
    vector<string> friends;
    vector<Post> posts;

public:
    // Constructors
    User() : username(""), password("") {}
    User(const string& username, const string& password) 
        : username(username), password(password) {}

    // Getters and Setters
    string getUsername() const { return username; }
    string getPassword() const { return password; }

    void setUsername(const string& username) { this->username = username; }
    void setPassword(const string& password) { this->password = password; }

// Add a friend
    void addFriend(const string& friendUsername) {
        friends.push_back(friendUsername);
    }

    // Add a post
    void addPost(const string& content) {
        posts.push_back(Post(content));
    }

    // Display friends
    void displayFriends() const {
        cout << "Friends of " << username << ":" << endl;
        for (const auto& friendName : friends) {
            cout << "- " << friendName << endl;
        }
    }

 // Display posts
    void displayPosts() const {
        cout << "Posts by " << username << ":" << endl;
        for (const auto& post : posts) {
            post.display();
        }
    }

    // Overloading << operator for file output
    friend ofstream& operator<<(ofstream& ofs, const User& user) {
        ofs << user.username << " " << user.password << endl;
        ofs << user.friends.size() << endl;
        for (const auto& friendName : user.friends) {
            ofs << friendName << endl;
        }
        ofs << user.posts.size() << endl;
        for (const auto& post : user.posts) {
            ofs << post;
        }
        return ofs;
    }

    // Overloading >> operator for file input
    friend ifstream& operator>>(ifstream& ifs, User& user) {
        size_t friendCount, postCount;
        ifs >> user.username >> user.password;
        ifs >> friendCount;
        ifs.ignore();
        for (size_t i = 0; i < friendCount; ++i) {
            string friendName;
            getline(ifs, friendName);
            user.friends.push_back(friendName);
        }
        ifs >> postCount;
        ifs.ignore();
        for (size_t i = 0; i < postCount; ++i) {
            Post post;
  ifs >> post;
            user.posts.push_back(post);
        }
        return ifs;
    }
};

// SocialNetwork class managing users and their interactions
class SocialNetwork {
private:
    map<string, User> users;

public:
    // Add a new user
    void addUser(const string& username, const string& password) {
        if (users.find(username) == users.end()) {
            users[username] = User(username, password);
            cout << "User " << username << " created successfully." << endl;
        } else {
            cout << "Username already exists. Try a different one." << endl;
        }
    }

    // Add a friend to a user
    void addFriend(const string& username, const string& friendUsername) {
        if (users.find(username) != users.end() && users.find(friendUsername) != users.end()) {
            users[username].addFriend(friendUsername);
            users[friendUsername].addFriend(username);
            cout << friendUsername << " added as a friend to " << username << endl;
} else {
            cout << "One or both usernames do not exist." << endl;
        }
    }

    // Post a message
    void postMessage(const string& username, const string& content) {
        if (users.find(username) != users.end()) {
            users[username].addPost(content);
            cout << "Post added successfully." << endl;
        } else {
            cout << "Username does not exist." << endl;
        }
    }

    // Display all users
    void displayUsers() const {
        for (const auto& pair : users) {
            cout << "User: " << pair.first << endl;
        }
    }

    // Save users to a file
    void saveToFile(const string& filename) const {
        ofstream ofs(filename);
        ofs << users.size() << endl;
        for (const auto& pair : users) {
            ofs << pair.second;
        }
        ofs.close();
    }

    // Load users from a file
    void loadFromFile(const string& filename) {
        ifstream ifs(filename);
        size_t userCount;
        ifs >> userCount;
ifs.ignore();
        for (size_t i = 0; i < userCount; ++i) {
            User user;
            ifs >> user;
            users[user.getUsername()] = user;
        }
        ifs.close();
    }

    // Log in a user
    bool login(const string& username, const string& password) {
        if (users.find(username) != users.end() && users[username].getPassword() == password) {
            cout << "Login successful." << endl;
            return true;
        } else {
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    // Display user's friends and posts
    void displayUserProfile(const string& username) const {
        if (users.find(username) != users.end()) {
            users.at(username).displayFriends();
            users.at(username).displayPosts();
        } else {
            cout << "Username does not exist." << endl;
        }
    }
};

// Function prototypes
void displayMenu();
void handleUserInput(SocialNetwork& network);

int main() {
    SocialNetwork network;
    network.loadFromFile("socialnetwork.txt");

    handleUserInput(network);

    network.saveToFile("socialnetwork.txt");

    return 0;
}

// Function to display the menu
void displayMenu() {
    cout << "Simple Social Network" << endl;
    cout << "1. Create Profile" << endl;
    cout << "2. Add Friend" << endl;
    cout << "3. Post Message" << endl;
    cout << "4. View User Profile" << endl;
    cout << "5. Display All Users" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

// Function to handle user input and interact with the social network
void handleUserInput(SocialNetwork& network) {
    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password;
  cout << "Enter Username: ";
                cin >> username;
                cout << "Enter Password: ";
                cin >> password;
                network.addUser(username, password);
                break;
            }
            case 2: {
                string username, friendUsername;
                cout << "Enter Your Username: ";
                cin >> username;
                cout << "Enter Friend's Username: ";
                cin >> friendUsername;
                network.addFriend(username, friendUsername);
                break;
            }
            case 3: {
                string username, content;
                cout << "Enter Your Username: ";
                cin >> username;
                cout << "Enter Post Content: ";
                cin.ignore();
                getline(cin, content);
                network.postMessage(username, content);
                break;
            }
            case 4: {
                string username;
                cout << "Enter Username to View Profile: ";
                cin >> username;
                network.displayUserProfile(username);
                break;
            }
            case 5: {
                network.displayUsers();
                break;
  }
            case 6: {
                cout << "Exiting..." << endl;
                break;
            }
            default: {
                cout << "Invalid choice, please try again." << endl;
                break;
            }
        }
    } while (choice != 6);
}
