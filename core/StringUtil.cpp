#include "StringUtil.hpp"

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <regex.h>

#include <string.h>

using namespace std;


bool StringUtil::mbInit = false;

void StringUtil::removeQuotes(string &s)
{
	while (s.find('\'')!=string::npos)
	{
		s[s.find('\'')] = ' ';
	}
}

void StringUtil::removeDoubleQuotes(string &s)
{
	while (s.find('"')!=string::npos)
	{
		s[s.find('"')] = ' ';
	}
}

int StringUtil::preg_match(string sStrRegex, string s2, bool bAll)
{
	regex_t tpReg;

	if (bAll)
	{
		sStrRegex = "^"+sStrRegex+"$";
	}
	int iResult;

	iResult = regcomp(&tpReg, sStrRegex.c_str(), REG_NOSUB|REG_EXTENDED);

	if (iResult==0)
	{
		int match;

		match = regexec(&tpReg, s2.c_str(), 0, NULL, 0);
		regfree(&tpReg);
		if (match==0)
		{
			iResult = 1;
		}
		else if (match==REG_NOMATCH)
		{
			iResult = 0;
		}
		else // Impossible de v�rifier (erreur interne)
		{
			iResult = 2;
		}
	}

	return iResult;
}

int StringUtil::match(string sR, string s)
{
	// cout << "match(" << sR << ',' << s << ')' << endl;
	if (sR.length()==0)
	{
		return s.length()==0;
	}
	else
	{
		switch (sR[0])
		{
			case '$':
				return s[0]==0;
				break;

			case '[':
			{
				string::size_type iPos = sR.find(']');
				if (iPos==string::npos)
				{
					cerr<<"missing closing braket ("<<sR<<")"<<endl;
					return false;
				}
				bool bRet = false;
				int iOneOrGt = (sR[iPos+1]=='+'?1:0);
				int iMulti = (sR[iPos+1]=='*'?1:iOneOrGt);

				bool bOk;
				string::size_type i;

				do
				{
					char c = s[0];
					// cout << "[" << sR << "," << s << ']' << endl;
					bOk = false;
					bool bNot = false;

					if (sR[1]=='^')
					{
						if ((s.length()==0)&&iMulti==0)
						{
							return false;
						}

						bNot = true;
						bOk = true;
						i = 2;
					}
					else
					{
						i = 1;
					}

					// cout << "while [" << sR << "," << s << ']' << endl;
					while (sR[i]!=']')
					{
						if (sR[i+1]=='-')
						{
							char cStart = sR[i];
							char cEnd = sR[i+2];
							if (bNot)
							{
								bOk = bOk&&(c<cStart||c>cEnd);
							}
							else
							{
								bOk = bOk||(c>=cStart&&c<=cEnd);
							}
							i += 2;
							if (i>iPos)
							{
								cerr<<"bad [x-y] syntax"<<endl;
								return false;
							}
						}
						else if (bNot)
						{
							bOk = bOk&&c!=sR[i];
						}
						else
						{
							bOk = bOk||c==sR[i];
						}
						i++;
					}
					if (bOk)
					{
						s.erase(0, 1);
						// cout << "bOk [" << sR << "," << s << ']' << endl;
						if (match(sR.substr(iPos+1+iMulti), s))
						{
							return true;
						}
					}
					bRet = bRet||bOk;
				} while (bOk&&iMulti&&s.length()!=0);
				if (sR[i+1]=='*')
				{
					return bRet&&match(sR.substr(i+2), s);
				}
				else
				{
					return bRet&&match(sR.substr(i+1), s);
				}
			}
				break;


			case '.':
				if (sR[1]=='*')
				{
					// cout << sR << "(*)";
					sR.erase(0, 2);
					if (s.length()==0) return true;
					while (s.length())
					{
						if (match(sR, s))
						{
							return true;
						}
						s.erase(0, 1);
					}
					return match(sR, s);
				}
				else
				{
					s.erase(0, 1);
					return match(sR.substr(1), s);
				}
				break;

			case '\\':
				sR.erase(0, 1);

			default:
				if ((s.length()==0)||((sR[0]!=s[0])&&sR[1]!='*'))
				{
					return false;
				}
				else if (sR[1]=='*')
				{
					do
					{
						s.erase(0, 1);
						if (match(sR.substr(2), s))
						{
							return true;
						}
					} while (s.length() && (sR[0]==s[0]));

					return match(sR.substr(2), s);
				}
				else
				{
					int iRet = match(sR.substr(1), s.substr(1));
					// cout << "ici=" << iRet << endl;

					return iRet;
				}
		}
	}

	return false;
}

void StringUtil::epure(string& sSrce)
{
	typedef map<unsigned char, string> CharConvert;
	typedef CharConvert::const_iterator const_iterator;
	static CharConvert m;

	if (mbInit==false)
	{
		mbInit = true;

		for (unsigned char c = 1; c<31; c++)
		{
			m[c] = ' ';
		}

		m['\''] = ' ';
		m['_'] = ' ';
		m['('] = ' ';
		m[')'] = ' ';
		m['A'] = 'a';
		m['B'] = 'b';
		m['C'] = 'c';
		m['D'] = 'd';
		m['E'] = 'e';
		m['F'] = 'f';
		m['G'] = 'g';
		m['H'] = 'h';
		m['I'] = 'i';
		m['J'] = 'j';
		m['K'] = 'k';
		m['L'] = 'l';
		m['M'] = 'm';
		m['N'] = 'n';
		m['O'] = 'o';
		m['P'] = 'p';
		m['Q'] = 'q';
		m['R'] = 'r';
		m['S'] = 's';
		m['T'] = 't';
		m['U'] = 'u';
		m['V'] = 'v';
		m['W'] = 'w';
		m['X'] = 'x';
		m['Y'] = 'y';
		m['Z'] = 'z';
		m['|'] = ' ';
		m['&'] = ' ';
		m[-10] = 'o';
		m[-18] = 'i'; // î
		m[-30] = 'a'; // â
		m[-55] = 'e'; // é
		m[-78] = '2'; // ²
		m[-79] = '%'; // +/-
		m[-22] = 'e'; // ê
		m[-23] = 'e'; // é
		m[-24] = 'e'; // è
		m[-25] = 'c'; // ç
		m[-30] = 'a'; // â
		m[-32] = 'a'; // à
		m[-52] = 'i'; // `I
		m[-61] = "";
		m[-67] = 'e'; // é
		m[-64] = 'a'; // `A
		m[-68] = 'u'; // �
		m[-69] = ' ';
		m[-71] = 'u';
		m[-74] = 'o'; // �
		m[-75] = ' ';
		m[-76] = 'o'; // �
		m[-81] = 'i'; // �
		m[-82] = 'i'; // �
		m[-85] = ' ';
		m[-86] = 'e'; // �
		m[-87] = 'e';
		m[-88] = 'e';
		m[-89] = 'c';
		m[-92] = 'a'; // �
		m[-94] = 'a'; // �
		m[-96] = ' ';
		m[-100] = "oe";
		m[-103] = ' ';
		m[-106] = ' ';
		m[-108] = ' ';
		m[-109] = ' ';
		m[-110] = ' ';
		m[-111] = ' ';
		m['\''] = ' ';
		m['('] = ' ';
		m['~'] = ' ';
		m['{'] = ' ';
		m['}'] = ' ';
	}

	unsigned int i = 0;
	//cout << sSrce << endl;
	while (i<sSrce.length())
	{
		// cout << ' ' << (int)sSrce[i]<< sSrce[i];
		if (sSrce[i]==-17&&sSrce[i+1]==-65)
		{
			sSrce.erase(i, 2);
		}
		else
		{
			const_iterator oit = m.find((char) sSrce[i]);
			if (oit!=m.end())
			{
				sSrce.erase(i, 1);
				sSrce.insert(i, oit->second);
				i += oit->second.length();
			}
			else
				i++;
		}
	}
	//cout << sSrce << endl;
}

int StringUtil::LevenshteinDistance(const string &s1, const string &s2)
{
	string::size_type N1 = s1.length();
	string::size_type N2 = s2.length();
	string::size_type i, j;
	vector<int> T(N2+1);

	for (i = 1; i<=N2; i++)
		T[i] = i;

	for (i = 0; i<N1; i++)
	{
		T[0] = i+1;
		int corner = i;
		for (j = 0; j<N2; j++)
		{
			int upper = T[j+1];
			if (s1[i]==s2[j])
				T[j+1] = corner;
			else
				T[j+1] = min(T[j], min(upper, corner))+1;
			corner = upper;
		}
	}
	return T[N2];
}

void StringUtil::toLower(string& str)
{
	for (unsigned int i = 0; i<str.length(); i++)
		if (str[i]>=0x41&&str[i]<=0x5A)
			str[i] = str[i]+0x20;
}

void StringUtil::upperFirst(string& str, bool bAll)
{
	bool bSpace = true;

	for (unsigned int i = 0; i<str.length(); i++)
	{
		if (str[i]=='_')
		{
			str[i] = ' ';
			bSpace = bAll;
		}
		else if (str[i]==' ')
		{
			bSpace = bAll;
		}
		else if (bSpace)
		{
			str[i] = toupper(str[i]);
			bSpace = false;
		}
	}
}

void StringUtil::toUpper(string& str)
{
	for (unsigned int i = 0; i<str.length(); i++)
	{
		str[i] = toupper(str[i]);
	}
}

// TODO cette fonction n'a rien a faire ici mais elle semble efficace against l'autre

std::string trim(std::string& s, const std::string& drop = " ")
{
	std::string r = s.erase(s.find_last_not_of(drop)+1);

	return r.erase(0, r.find_first_not_of(drop));
}

void StringUtil::trim(string& s, bool bWithCr)
{
	while (s.length()>0&&(s[0]==' '||(bWithCr&&(s[0]==13||s[0]==10))))
		s.erase(0, 1);
	while (s.length()>0&&(s[s.length()-1]==' '||(bWithCr&&(s[s.length()-1]==13||s[s.length()-1]==10))))
		s.erase(s.length()-1, 1);
}

long StringUtil::getLong(string& s)
{
	string::size_type i = 0;
	long l = 0;
	long sign = 1;

	if (s[0]=='-')
	{
		sign = -1;
		s.erase(0, 1);
	}
	else if (s[0]=='+')
		s.erase(0, 1);

	trim(s);

	while (s.length()&&isdigit(s[i++]));

	if (isdigit(s[0]))
	{
		l = atol(s.substr(0, i).c_str());
		s.erase(0, i-1);
	}

	trim(s);

	return l*sign;
}

float StringUtil::getFloat(string& s)
{
	string::size_type i = 0;
	float f = 0;
	bool bDot = true;
	float fSign = 1;

	trim(s);

	if (s[0]=='-')
	{
		s.erase(0, 1);
		fSign = -1;
	}
	else if (s[0]=='+')
		s.erase(0, 1);

	while (s.length() && (isdigit(s[i])||(bDot&&s[i]=='.')))
	{
		if (s[i]=='.') bDot = false;
		i++;
	}

	if (isdigit(s[0])||s[i]=='.')
	{
		f = atof(s.substr(0, i).c_str());
		s.erase(0, i);
	}

	trim(s);

	return f*fSign;
}

string StringUtil::getWord(string& s, const string &sSeparators)
{
	string sWord;
	trim(s);

	if (s.length())
	{
		string::size_type iPos = string::npos;

		for (string::size_type i = 0; i<sSeparators.length(); i++)
		{
			string::size_type i1 = s.find(sSeparators[i]);

			if (i1!=string::npos)
			{
				if (iPos==string::npos||iPos>i1)
				{
					iPos = i1;
				}
			}
		}

		if (iPos!=string::npos)
		{
			sWord = s.substr(0, iPos);
			s.erase(0, iPos+1);
			trim(s);
		}
		else
		{
			sWord = s;
			s = "";
		}
	}
	return sWord;
}

string StringUtil::getWord(string& s, char cSep, char cSep2)
{
	string sWord;
	trim(s);

	if (s.length())
	{
		string::size_type i1 = s.find(cSep);
		string::size_type i2 = string::npos;

		if (i1==string::npos&&i2==string::npos&&s.length())
		{
			i1 = s.length()+1;
		}

		if (cSep2!=0) i2 = s.find(cSep2);

		if (i1!=string::npos&&i2!=string::npos)
		{
			if (i1>i2) i1 = i2;
		}
		else if (i1==string::npos)
			i1 = i2;

		if (i1!=string::npos)
		{
			sWord = s.substr(0, i1);
			s.erase(0, i1+1);
			trim(s);
		}
	}

	return sWord;
}

bool StringUtil::checkCharRemove(string& s, char c)
{
	if (s.length()>0&&s[0]==c)
	{
		s.erase(0, 1);

		return true;
	}

	return false;
}

bool StringUtil::expectWord(string& s, const string& sWord)
{
	trim(s);

	if (s.substr(0, sWord.length())==sWord)
	{
		char c = s[sWord.length()]|32;

		if (c<='a'||c>='z')
		{
			s.erase(0, sWord.length());
			trim(s);

			return true;
		}
	}

	return false;
}

bool StringUtil::expectChar(string& s, char c)
{
	if (s[0]==c)
	{
		s.erase(0, 1);
		return true;
	}

	return false;
}

string StringUtil::unserialize(string& s)
{
	trim(s);
	string::size_type iSpc = s.find(' ');

	if (iSpc==string::npos)
	{
		throw string("unable to unserialize [")+s.substr(0, 20)+string("...]");
	}

	string::size_type lSize = atol(s.substr(0, iSpc).c_str());
	//     cout << "lsize " << lSize << endl;
	string sRet = s.substr(iSpc+1, lSize);
	s.erase(0, iSpc+lSize+1);

	return sRet;
}

template < class T >
long StringUtil::fillVector(T &container, string& s, char cSep)
{
	long lRet = 0;
	string s2(s);

	while (s2.length())
	{
		string sWord = StringUtil::getWord(s, cSep);

		if (sWord.length())
		{
			lRet++;
			container.push_back(sWord);
		}
	}

	return lRet;
}

string StringUtil::getString(string& s, bool bRemoveQuotes)
{
	string sResult = "";
	trim(s);

	if (s.length())
	{
		char cQuote = s[0];
		string::size_type iPos = s.find(cQuote, 1);

		if (iPos!=string::npos)
		{
			if (bRemoveQuotes)
			{
				sResult = s.substr(1, iPos-1);
			}
			else
			{
				sResult = s.substr(0, iPos+1);
			}
			s.erase(0, iPos+1);
		}
	}

	return sResult;
}

string StringUtil::getStringWord(string& s, bool bRemoveQuotes)
{
	if (s[0]=='\''||s[0]=='"')
	{
		return getString(s, bRemoveQuotes);
	}
	else
	{
		return getWord(s);
	}
}

string StringUtil::getIdentifier(string& s)
{
	bool bMore = false;
	string sResult = "";
	trim(s);

	for (string::const_iterator oit = s.begin(); oit!=s.end(); oit++)
	{
		char c = *oit;

		if (isalpha(c)||(bMore&&(c=='_'||isdigit(c))))
		{
			sResult += c;
			bMore = true;
		}
		else
			break;
	}

	if (sResult.length())
	{
		s.erase(0, sResult.length());
		trim(s);
	}
	else
	{
		throw "Identifier expected.";
	}

	return sResult;
}

string StringUtil::getToken(const string& s, uint8_t iCol, char cSep)
{
	string::size_type i1 = 0;
	string::size_type i2 = s.find(cSep);
	while (iCol>0&&(i1!=string::npos))
	{
		i1 = i2+1;
		i2 = s.find(cSep, i1);
		if (i2==string::npos)
		{
			i2 = s.length()+1;
			if (iCol>1) return "";
			break;
		}
		iCol--;
	}
	if (i1!=string::npos&&i2!=string::npos) return s.substr(i1, i2-i1);
	return "";
}

bool StringUtil::isWindows1252(const char* p)
{
	unsigned char c;

	while ((c = (unsigned char) *p++))
	{
		unsigned char c1 = (unsigned char) p[0];
		//unsigned char c2 = c1 ? (unsigned char) p[1] : 0;

		if
			(
			// oe
			(c==0x9C)
			||
			// apostrophe ’
			(c==0x92)
			||
			// tm
			(c==0x99)
			||
			// €		  CONFLIT AVEC
			((c1==0x80) && (c!=0xc3))
			)
		{
			return true;
		}
	}

	return false;
}

bool StringUtil::isWindows1252(const string &s)
{
	return isWindows1252(s.c_str());
}

string StringUtil::windows1252ToUtf8(const string &s)
{
	string sOut(s);

	for (string::size_type iPos = 0; iPos<sOut.length(); iPos++)
	{
		switch ((unsigned char) sOut[iPos])
		{
			case 0x80: // €
				sOut[iPos] = (unsigned char) 0xA4;
				break;

			case 0x9C: // oe
				sOut[iPos] = (unsigned char) 0xBD;
				break;
		}
	}

	return StringUtil::isoToUtf8(sOut); // Bin ouais, le reste, c'est quasi pareil !
}

string StringUtil::isoToUtf8(const string& s)
{
	string sOut;
	unsigned long iLen = s.length();

	for (unsigned long i = 0; i<iLen; i++)
	{
		unsigned char in = s[i];

		if (in<128)
		{
			sOut += in;
		}
		else if (in==0xe9)
		{
			sOut += (unsigned char) 0xc3;
			sOut += (unsigned char) 0xa9;
		}
		else if (in==0xBD) // oe
		{
			sOut += (unsigned char) 0xC5;
			sOut += (unsigned char) 0x93;
		}
		else if (in==0xA4)
		{
			sOut += "€"; // TODO non testé
		}
		else if (in==0xe7) // ç
		{
			sOut += (unsigned char) 0xc3;
			sOut += (unsigned char) 0xa7;
		}
			// Cette partie concerne le range 0x80...0x9F qui est une zone non utilisée de iso, mais utilisée par win 1252
			// -> petite bidouille pour faciliter windows1252toUtf8
		else if (in==0x92) // ' win1252
		{
			sOut += (unsigned char) 0xe2;
			sOut += (unsigned char) 0x80;
			sOut += (unsigned char) 0x99;
		}
		else if (in==0x99) // tm win1252
		{
			sOut += "&trade;";
		}
		else if ((in==0xA0)||(in==0xC2)) // no-break space
		{
			sOut += (unsigned char) 0x20;
		}
		else if ((in==0xfb))
		{
			sOut += (unsigned char) 0xc3;
			sOut += (unsigned char) 0xbb;
		}
		else if (in==0xae)
		{
			sOut += (unsigned char) 0xc2;
			sOut += (unsigned char) 0xae;
		}
		else
		{
			sOut += (char) (0xc2+(in>0xbf));
			sOut += (char) ((in&0x3f)+0x80);
		}
	}

	return sOut;
}

string StringUtil::utf8ToIso(const string& s)
{
	string sOut;
	unsigned long iLen = s.length();

	for (unsigned long i = 0; i<iLen; i++)
	{
		unsigned char in = s[i];

		if ((unsigned int) (in)<128)
		{
			sOut += in;
		}
		else if ((unsigned int) (in)==0xC2)
		{
			i++;
			sOut += (char) (in);
		}
		else if ((unsigned int) (in)==0xC3)
		{
			i++;
			sOut += (char) (s[i]+0x40);
		}
		else if ((unsigned int) (in)==0xDF)
		{
			i += 1;
		}
		else if ((unsigned int) (in)==0xEF)
		{
			i += 2;
		}
		else if ((unsigned int) (in)==0xF7)
		{
			i += 3;
		}
		else if ((unsigned int) (in)==0xFB)
		{
			i += 4;
		}
		else if ((unsigned int) (in)==0xFD)
		{
			i += 5;
		}
	}

	return sOut;
}

bool StringUtil::isUtf8(const char* s)
{
	const unsigned char * bytes = (const unsigned char *) s;

	while (*bytes)
	{
		unsigned char c = (unsigned char) (*bytes++);

		if (c>128)
		{
			if (c==0xC2||c==0xC3||c==0xDF||c==0xEF||c==0xF7||c==0xFB||c==0xFD)
			{
				return true;
			}
		}
	}

	return false;
}

bool StringUtil::isIso(const char* p)
{
	//cout << "<!-- isIso(" << p << ") -->" << endl;
	unsigned char c;

	while ((c = (unsigned char) *p++))
	{
		unsigned char c1 = (unsigned char) p[0];
		unsigned char c2 = c1?(unsigned char) p[1]:0;

		//cout << "[" << c << "]" << hex << int(c) << dec;

		if
			(
			(c>=128)
			&&
			(
			c==0xa3||c==0xb2||c==0xb5||c==0xc8||c==0xc9||c==0xca||
			(c==0xe0&&(c1<0xA0||c1>0xBF))||// extension utf8
			(c==0xe2&&(c1<0x80||c1>0xBF) && (c2<0x80||c2>0xBF))||// extension utf8
			(c1==0xA0&&c!=0xc3)||(c==0xc2&&c1==0xa0)||
			(c1==0xae&&c!=0xc3)||
			c==0xe4||c==0xe8||c==0xee||c==0x6f||c==0xf4||c==0xe6||c==0xe7||c==0x92||c==0xfb||c==0xae||c==0xe9
			)
			)
		{
			//cout << "     iso" << endl;
			return true;
		}

		//cout << "     noiso" << endl;
	}

	return false;
}
