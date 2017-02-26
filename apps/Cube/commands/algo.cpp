/* 
 * File:   svr_algo.cpp
 * Author: francois
 * 
 */

#include "algo.hpp"
#include <StringUtil.hpp>

using namespace std;

namespace core {
	algo algo_instance;

	algo::algo() {
		registerSyntax("algo", "", "Commande de algo");
	}

	bool algo::run(Server* server, string &sCmd, string& incoming, stringstream& out, stringstream& err) {
		bool bRet = false;

		static map<char, map<char, char>> face_rotate;

		if (face_rotate.size() == 0) {
			map<char, char> faces;

			faces['t'] = 'f';
			faces['f'] = 'd';
			faces['d'] = 'b';
			faces['b'] = 't';
			faces['e'] = 'S';
			faces['s'] = 'e';
			face_rotate['x'] = faces; // bug b'e'b2eeb't2beeb2ebt2

			faces = map<char, char>();
			faces['f'] = 'r';
			faces['r'] = 'b';
			faces['b'] = 'l';
			faces['l'] = 'f';
			face_rotate['y'] = faces;

			faces = map<char, char>();
			faces['t'] = 'r';
			faces['r'] = 'd';
			faces['d'] = 'l';
			faces['l'] = 't';
			face_rotate['z'] = faces;

		}
		string cmd = StringUtil::getWord(incoming);
		if (cmd == "rotate") {
			string axes = StringUtil::getWord(incoming);
			string algo;
			string new_algo = incoming;
			while (algo.find(' ') != string::npos)
				algo.erase(algo.find(' '), 1);

			while (axes.length()) {
				auto it = face_rotate.find(axes[0]);
				axes.erase(0, 1);
				if (it != face_rotate.end()) {
					map<char, char> rotate = it->second;

					algo = new_algo;
					new_algo = "";
					while (algo.length()) {
						if (algo.length() == 0) break;

						bool reverse = false;
						char face = algo[0];
						algo.erase(0, 1);

						auto itr = rotate.find(face);

						if (itr != rotate.end()) {
							char newface = itr->second;
							if (newface < 'Z') {
								newface |= 32;
								reverse = true;
							}
							new_algo += newface;
						} else
							new_algo += face;
						while (algo[0] == '\'' || algo[0] == '2') {
							if (algo[0] == '\'')
								reverse = !reverse;
							else
								new_algo += algo[0];
							algo.erase(0, 1);
						}
						if (reverse) new_algo += '\'';
					}
				}
			}
			cout << "rotate = " << new_algo << endl;
			server->send("#OK algo rotate=" + new_algo);
		} else
			server->send("#KO algo, unknown subcommand " + cmd);

		return bRet;
	}
}