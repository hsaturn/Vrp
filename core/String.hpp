/* 
 * File:   String.hpp
 * Author: harnaud
 *
 * Created on 24 mars 2014, 17:15
 */

#ifndef STRING_HPP
#define	STRING_HPP

#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <StringUtil.hpp>

using namespace std;


class String : public string
{
	public:
		String(): string() {};
		String(const string& s): string(s) {}
		
		template<class T>
		String(const T &t): string(StringUtil::to_string(t)) {}
		
		String(const char* &p)
		{
			if (p)
			{
				assign(StringUtil::to_string(p));
			}
			else
			{
				assign("");
			}
		}

		template< class T> String& operator +=(const T &l);
		String& operator +(long l) const;
	
		template< class T> String& operator =(const T &l);
	
		bool operator == (long l) { return atol(this->c_str()); }
		
		bool startsWith	(const string& s)	const;
		bool startsWith	(const char* p)		const;
		
		bool endsWith	(const string& s)	const;
		bool endsWith	(const char* p)		const;
		
		void replace(char srch, char repl);
		void replace(const string& srch, const string& repl);
		
		bool eraseStartsWith(const string& s);
		
		void trim(bool right=false);
};



template< class T> String& String::operator +=(const T &t)
{
	string::operator +=(StringUtil::to_string(t));

	return *this;
}

template< class T> String& String::operator =(const T &t)
{
	string::operator =(StringUtil::to_string(t));

	return *this;
}

#endif	/* STRING_HPP */

