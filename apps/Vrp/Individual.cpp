/* 
 * File:   Individual.cpp
 * Author: hsaturn
 * 
 * Created on 24 novembre 2015, 02:28
 */

#include "Individual.hpp"
#include "Galaxy.hpp"
#include "Planet.hpp"
#include <algorithm>
#include "glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "Random.hpp"

using namespace std;

Individual::Individual()
{
}

void Individual::createGenes(Galaxy* galaxy) {
	if (galaxy) {
		for (auto planet : *galaxy)
			genes.push_back(shared_ptr<Gene>(new Gene(planet)));
		random_shuffle(genes.begin(), genes.end());
	}
}

bool Individual::hasPlanet(Planet* p) const {
	for (auto gene : genes)
		if (gene->getPlanet() == p)
			return true;
	return false;
}

void Individual::render(int quality) {
	// FIXME Orienter car la galaxy peut être scalée et déplacée
	Color::white.render();
	shared_ptr<Gene> prev;
	for (shared_ptr<Gene> gene : genes) {
		gene->render(prev, quality);
		prev = gene;
	}
}

void Individual::mutate() {
	if (size() >= 2) {
		int muts = size()*Random::rand(0,0.05);
		if (muts==0) muts=1;
		for (int i = 0; i < muts; i++) {
			unsigned int gene1 = rand() % size();
			unsigned int gene2 = rand() % size();
			while (gene2 == gene1)
				gene2 = rand() % size();

			iter_swap(genes.begin() + gene1, genes.begin() + gene2);
		}
	}
}

void Individual::dump(ostream& out) const {
	out << "[ ";
	for (shared_ptr<Gene> gene : genes) {
		out << gene->getPlanet()->getName() << ' ';
	}
	out << " ]" << endl;
}

void Gene::render(const shared_ptr<Gene> prev, int quality) const {
	if (prev == 0) return;
	Color::dark_gray.render();

	glBegin(GL_LINES);
	const Point& p1 = prev->planet->pos();
	const Point& p2 = planet->pos();
	p1.render();
	glm::vec3 v(p2.x() - p1.x(), p2.y() - p1.y(), p2.z() - p1.z());
	glm::vec3 v1 = glm::normalize(v);
	glm::vec3 vr = planet->ray() * v1;

	Point pConeTop(p2.x() - vr.x, p2.y() - vr.y, p2.z() - vr.z);
	pConeTop.render();
	glEnd();

	Color::white.render();
	const float cone_length = 5.0;
	glm::vec3 vb = cone_length*v1;
	Point pConeBase(p2.x() - vb.x, p2.y() - vb.y, p2.z() - vb.z);

	glm::vec3 v3(pConeTop.y(), -pConeTop.x(), 0);
	glm::vec3 ortho = glm::normalize(v3);

	// cone
	glBegin(GL_TRIANGLE_FAN);
	pConeTop.render();
	float angle = 2 * M_PI / quality;
	for (int i = 0; i <= quality; i++) {
		(Color::gray * (0.5 + sin(angle)*0.5)).render();
		Point p(pConeBase);
		p.translate(ortho);
		p.render();
		ortho = glm::rotate(ortho, angle, v1);
	}
	glEnd();

	if (false) {
		// base circle
		glBegin(GL_TRIANGLE_FAN);
		pConeBase.render();
		Color::dark_gray.render();
		angle = 2 * M_PI / quality;
		for (int i = 0; i <= quality; i++) {
			(Color::gray * (0.5 + sin(angle)*0.5)).render();
			Point p(pConeBase);
			p.translate(ortho);
			p.render();
			ortho = glm::rotate(ortho, angle, v1);
		}
		glEnd();
	}
	//pConeBase.render();

	//v -= planet->ray()*v/v.length();
	///planet->pos().render();
}
