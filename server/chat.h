#pragma once
#include <vector>
#include <string>

class User{
	std::string name;
	int lastMessage;
	public:
	User();
	User(const std::string& s);
	std::string getName() const;
	void setName(const std::string& s);
	const User& operator==(const User& );
};

class Message{
	int id;
	User user;
	std::string text;
public:
	Message(User user,const std::string& text);
	
	//Getters
	std::string getText() const;
	User getUser() const;
	
	// Setters
	/* Functionally do we even need this?*/
};

class Room{
private:
	std::string name;
	std::vector<User> users;
	std::vector<Message> messages;

	std::vector<User>::const_iterator getUserIndex(const std::string&) const;
public:
	Room(const std::string& s);
	bool addUser(User u);
	bool deleteUser(const std::string& n);
	bool newMessage(Message m);
	bool userExists(const std::string& s) const;
	std::string dump() const;
	
};
