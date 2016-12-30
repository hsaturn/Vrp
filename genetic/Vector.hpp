/* 
 * File:   Vector.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 17:16
 */

#ifndef VECTOR_HPP
#define	VECTOR_HPP

#include <vector>

using namespace std;

namespace genetic {

	template <typename T>
	class Vector {
	public:
		Vector();
		virtual ~Vector();

		typedef typename vector<T>::iterator iterator;
		typedef typename vector<T>::const_iterator const_iterator;

		iterator begin() {
			return vec.begin();
		}

		iterator end() {
			return vec.end();
		}

		const_iterator begin() const {
			return vec.begin();
		}

		const_iterator end() const {
			return vec.end();
		}

		void push_back(const T &t) {
			vec.push_back(t);
		}

		unsigned long size() const { return vec.size(); }

	private:
		Vector(const Vector& orig);

		vector<T> vec;
	};

}
#endif	/* VECTOR_HPP */

