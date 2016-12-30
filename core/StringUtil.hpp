#ifndef STRING_UTIL_HPP_
#define STRING_UTIL_HPP_

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class StringUtil
{
public:
	static void epure(string& s);
	static int LevenshteinDistance(const string &s1, const string &s2);

	template <class T>
	static inline std::string to_string (const T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	static void toLower(string& s);
	static void toUpper(string& s);

	/** remplace les _ par des espaces et met la/es premières lettres en majuscule */
	static void upperFirst(string& s, bool bAll=true);
    
    static long getLong(string& s);
	static float getFloat(string &s);
    static string getWord(string& s, char cSep=' ', char cSep2=0);
    /*
     *  Get the first word separated by
     */
    static string getWord(string& s, const string& sSeparators);
    
    /**
	 * Check if a string contains only 0-9 chars (integer)
	 *
	 * @input string s
	 * @return bool
	 **/
	static bool is_integer(const string& s) { return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos; }

	/**
	 * Simple but fast almost regex matcher.
	 *
	 * for simple regexps, return value is same as preg_match(sR, s, true);
	 *
	 * Syntax allowed are
	 *
	 * [abc]	a, b or c
	 * [^abc]	All but a,b and c
	 * [a-z]	One char from a to z
	 * [^a-z]	All but from a to z
	 *
	 * Any combinaison of this with optional star at the end
	 * [1-5a-d]*	from 1 to 5 or from a to d, repeated 0 or many times.
	 * .        Any char exactly one
	 * .*       0 or many char
	 * \        Escape char
	 *
	 * NOT IMPLEMENTED :
	 *
	 * (regexp1)			note : regexp1 cannot contains any ) (unless escaped)
	 * (regexp1)|...	Either 1 or ...
	 * (regexp1)(regexp2)
	 *
	 * @input sR the pattern
	 * @input s  the 
	 * @return   int 0 if no match
	 *
	 * @see StringUtil::preg_match
	 **/
	static int match(string sR, string s);

	/**
	 * regex match
	 *
	 * @input string sStrRegex   the regex (^ and $ are appended if bAll)
	 * @input string s2          the string to check against s2
	 *
	 * @return int  0 if no match
	 **/
	static int preg_match(string sStrRegex, string s2, bool bAll = true);

    /**
	 * Check if a string contains only 0-9 chars (integer) and a optional dot somewhere
	 *
	 * @input string s
	 * @return bool
	 **/
	static bool is_float(const string& s) {
		return (!s.empty()) &&
				(s.find_first_not_of("0123456789.") != std::string::npos) &&
				(std::count(s.begin(), s.end(), '_')<=1); }
    
    /**
     * Left trim a string
     * @input string s (modified)
     * @return void
     **/
    static void trim(string& s, bool bWithCr=false);
    
    /**
     * Check that the first word of s is sWord
     * if yes, the word is removed from s and true is returned.
     * else, s stay unchanged and false is returned.
     * s is trimmed before.
	 * The word is not considered ok if it is not exactly word.
	 * Thus, expectWord("order","or") is false.
	 *
     * @param string s     Input string (modified)
     * @param string sWord Expected word
     * @return bool
     **/
    static bool expectWord(string& s, const string& sWord);

	/**
	 * Check if the first char is c and remove it if yes.
	 *
     * @param string s     Input string (modified)
     * @param string sWord Expected word
     * @return bool
	 * */
	static bool expectChar(string& s, char c);
    
    /**
     * Check if s[0] == c and remove it if yes
     * @return bool
     **/
    static bool checkCharRemove(string& s, char c);
    
    /**
     * Return a serialized string.
     * nnn str where nnn is the size and str is the string
     * @param string s string to serialize
     * @return string
     **/
    static inline string serialize(const string& s) { return to_string(s.length())+" "+s; }
    
    /**
     * read and remove a serialized string from s
     * @param string& s string that contains many serialized string, 1st is removed
     * @return string
     **/
    static string unserialize(string& s);

    /**
     * read and remove an identifier [a-zA-Z][_0-9]*
     * @param string& s string (modified)
     * @return string
     **/
	static string getIdentifier(string& s);
	
	/**
	 * Get the nth field of a string separated by cSep
	 * @input const string& s
	 * @input uint8t_t iCol Column to retrieve (0 is first column)
	 * @input char cSep, separator
	 */
	static string getToken(const string& s, uint8_t iCol, char cSep=';');

	/**
	 * cut the string with separators and fill the vector
	 * @param container  Any container that accepts push_back();
	 * @param char cSep	separator (default is space)
	 * @return long number of items added.
	 **/
	template < class T >
	static long fillVector(T &container, string& s, char cSep=' ');


	static void removeQuotes(string &s);
	static void removeDoubleQuotes(string &s);
    
	/**
	 * fetch the next string
	 * "xxx" or 'string' are valid strings.
	 * @param string s input string (modified)
	 * @return string string with quotes or empty string if error
	 */
	static string getString(string& s, bool bRemoveQuote=false);
	
	// Get a string if any or the next word else
	static string getStringWord(string& s, bool bRemoveQuotes=false);

	// Basic string conversion (incomplete)
	static string windows1252ToUtf8(const string& s);
	static string isoToUtf8(const string& s);
	static string utf8ToIso(const string& s);

	/**
	 * Empiric utf8 detection.
	 * Return true if string is utf8
	 */
	static bool isWindows1252(const string&);
	static bool isWindows1252(const char*);
	static bool isUtf8(const char*);
	static bool isIso(const char*);

	static bool isUtf8(const string& s) { return isUtf8(s.c_str()); }
	static bool isIso(const string& s) { return isIso(s.c_str()); }

private:
	static bool mbInit;

};


#endif
