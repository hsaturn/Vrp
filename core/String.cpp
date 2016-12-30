/* 
 * File:   String.cpp
 * Author: harnaud
 * 
 * Created on 24 mars 2014, 17:15
 */

#include "String.hpp"
#include "string.h"

String& String::operator +(long l) const
{
	static String s(*this);
	return s;
}

bool String::startsWith(const string& token) const
{
	if(length() < token.length())
		return false;
	return (compare(0, token.length(), token) == 0);
}

bool String::startsWith(const char* p) const
{
	const char* p2 = c_str();

	for(;;)
	{
		if (*p == 0) return true;
		if ((*p2 == 0) || (*p++ != *p2++)) return false;
	}
	return false;
}

bool String::endsWith(const string& token) const
{
	if(length() < token.length())
		return false;
	return (compare(length()-token.length(), token.length(), token) == 0);
}

bool String::endsWith(const char* p) const
{
	size_t l=strlen(p);
	if (l>length())
		return false;

	return (compare(length()-l, l, p) == 0);
}

void String::replace(char srch, char repl)
{
	if (srch==repl) return;
	
	string::size_type t=0;
	while((t=find(srch, t)) != string::npos)
	{
		(*this)[t]= repl;
	}
}

void String::replace(const string& srch, const string& repl)
{
	if (srch==repl) return;
	if (srch.length()==0) return;
	
	string::size_type t=0;
	while((t=find(srch, t)) != string::npos)
	{
		string::replace(t, srch.length(), repl);
		t+=repl.length();
	}
}

bool String::eraseStartsWith(const string& s)
{
	if (startsWith(s))
	{
		erase(0,s.length());
		trim();
		return true;
	}
	else
		return false;
}

void String::trim(bool right)
{
	while(length() && (at(0)==' ' || at(0)=='\t')) erase(0,1);
	if (right) while(length() && (at(length()-1)==' '||at(length()-1)=='\t')) erase(length()-1,1);
}