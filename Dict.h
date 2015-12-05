#ifndef DICT_H
#define DICT_H
#include "BST.h"
#include <set>

template<class K, class V>
class Couple
{
	public:
		K first;
		V second;

		//Constructors----------------------------------------------------------
		//Default constructor
		Couple() {}
		//Valued constructor
		Couple(K f, V s): first(f), second(s) {}
		//Copy constructor
		Couple(const Couple<K,V>& src): first(src.first), second(src.second) {}
		//Destructors-----------------------------------------------------------
		//Operators-------------------------------------------------------------
		Couple& operator=(const Couple<K,V>& src)
		{
			first = src.first;
			second = src.second;
			return *this;
		}

		bool operator<(const Couple<K,V>& c) { return first < c.first; }
		bool operator>(const Couple<K,V>& c) { return first > c.first; }
		bool operator!=(const Couple<K,V>& c) { return *this < c || *this > c; }
		bool operator==(const Couple<K,V>& c) { return !(*this != c); }
		bool operator<=(const Couple<K,V>& c) { return !(*this > c); }
		bool operator>=(const Couple<K,V>& c) { return !(*this < c); }
		friend std::ostream& operator<<(std::ostream& f, Couple<K,V>const&  c)
		{
			f << "(" << c.first << "," << c.second << ")";
			return f;
		}

		//Getters---------------------------------------------------------------
		//Setters---------------------------------------------------------------
		//Other methods---------------------------------------------------------
};


template<class K, class V>
class Dict
{
	protected:
		BST<Couple<K,V> > container;
		std::set<K> keys;

	public:
		//Constructors----------------------------------------------------------
		//Default constructor
		Dict() {}
		//Valued constructor
		//Copy constructor
		//Destructors-----------------------------------------------------------
		//Operators-------------------------------------------------------------
		V& operator[](const K& key)
		{
			return container.get(Couple<K,V>(key,V())).second;
		}
		//Getters---------------------------------------------------------------
		//Setters---------------------------------------------------------------
		void set(K key, V value)
		{
			container.add(Couple<K,V>(key,value));
		}
		//Other methods---------------------------------------------------------
		void del(K key)
		{
			std::cout << "del " << key << "\n";
			container.remove(Couple<K,V>(key,V()));
		}

		void print()
		{
			container.print();
		}
};
#endif
