#ifndef PARSER_H
#define PARSER_H
#include <Dict.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Parser
{
	private:
		Dict<std::string, std::string> parameters;

	public:
		//Constructors---------------------------------------------------------
		/**
		 * Default constructor
		 */
		Parser() { }

		/**
		 * Constructor to fill the parser directly with the parameters taken
		 * from the file pointed by filename
		 * @param filename The name of the config file to parse
		 */
		Parser(std::string filename)
		{
			read(filename);
		}

		//Destructor------------------------------------------------------------
		//Operators-------------------------------------------------------------
		/**
		 * Look for the value of parameter by providing its name
		 * @param key The name of the parameter to look for
		 */
		std::string operator[](const std::string& key)
		{
			return parameters[key];
		}

		//Getters---------------------------------------------------------------
		//Setters---------------------------------------------------------------
		//Other methods---------------------------------------------------------
		/**
		 * Reads the content of the configuration file pointed by filename ands stores
		 * it in the parameters dictionnary
		 * @param filename The name of the configuration file to read
		 */
		void read(std::string filename)
		{
			std::ifstream file(filename.c_str());

			if(!file.good())
				throw std::string("Error reading ") + filename + std::string("\n");

			while(!file.eof())
			{
				std::string key, value;
				std::getline(file, key, '=');
				std::getline(file, value);
				if(key != "")
					parameters.set(key, value);
			}
		}

		/**
		 * Prints the content of the dictionnary
		 */
		void print()
		{
			parameters.print();
		}

		/**
		 * Converts a string to another type. Assuming operator>> is defined
		 * for T (which is the case for simple types such as integers,
		 * floats, etc). For types not overloading operator>>, a user-defined
		 * conversion will most likely be necessary.
		 * @param s The string to convert
		 */
		template<class T> T string_to_T(std::string s)
		{
			std::stringstream ss;
			T ret;

			ss << s;
			ss >> ret;

			return ret;
		}
};

#endif
