#include "chat.h"
#include <iostream>

User::User(const std::string& s){
	setName(s);
}
User::User(){

}
std::string User::getName() const{
	return this->name;
}

void User::setName(const std::string& s){
	this->name = s;
}


Message::Message(User* user, const std::string& text){
	this->user = user;
	this->text = text;
}

std::string Message::getText() const{
	return this->text;
}
User* Message::getUserPointer() const{
	return this->user;
}

Room::Room(const std::string& s){
	this->name = s;
}
std::vector<User>::const_iterator Room::getUserIndex(const std::string& s) const{
	for(std::vector<User>::const_iterator i=users.begin();i<users.end();i++){
		if((*i).getName()==s){
			return i;
		}
	}
	std::cout<<"FAILED TO FIND USER\n";
	return users.end();
}
bool Room::addUser(User u){
	this->users.push_back(u);
	return true;
}
bool Room::deleteUser(const std::string& n){	
	this->users.erase(getUserIndex(n));
	return true;
}
bool Room::newMessage(Message m){
	messages.push_back(m);
	return true;
}

