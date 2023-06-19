#include "header.h"

//return the same string but lowercase, can also replace underscore if param is set
std::string toLowerCase(std::string str, bool replaceSpaceByUnderscore) {

	int size = str.size();

	std::string lowercase_str = str;
	for (int i = 0; i < size; i++) {

		lowercase_str[i] = std::tolower(str[i]);
		if (replaceSpaceByUnderscore) {
			if (lowercase_str[i] == ' ') {
				lowercase_str[i] = '_';
			}
		}
	}
	return lowercase_str;
}
std::string removeUnderscore(std::string str) {

	str.erase(std::remove(str.begin(), str.end(), '_'));

	return str;
}
/// <summary>
/// This function returns a vector of Unit*
/// the vector can have multiple values based on how strict and case sensitive the research is
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::vector<Unit*> searchUnit(std::string str,Unit* _array[]) {

	std::vector<Unit*> results;
	Unit* current = nullptr;
	for (int i = 0; i < NUMBER_OF_UNITS; i++) {

		current = _array[i];

		//the idea is to make the string from the user the same as the one in the array
		//first, lets make both lowercase
		std::string new_string = toLowerCase(str,true);
		std::string name_lowercase = toLowerCase(current->name,false); // no need to set true, no spaces in name attribute

		std::string new_string_w = removeUnderscore(new_string); //removing underscore
		std::string name_lowercase_w = removeUnderscore(name_lowercase);
		

		if (strstr(name_lowercase_w.c_str(), new_string_w.c_str())) {
			results.push_back(current);
		}
		
	}

	return results;
}