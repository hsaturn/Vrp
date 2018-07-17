/* 
 * File:   Individual.hpp
 * Author: hsaturn
 *
 * Created on 24 novembre 2015, 02:28
 */

#ifndef INDIVIDUAL_HPP
#define	INDIVIDUAL_HPP

#include <AbstractIndividual.hpp>
#include <vector>
#include <genetic.hpp>
#include <memory>

using namespace std;
using namespace genetic;

class Galaxy;
class Planet;

class Gene : public AbstractGene
{
public:
	Gene(Planet* p) : planet(p) {}
	bool operator==(const Planet* p) const  { return planet==p; }
	
	void render(const shared_ptr<Gene> prev, int quality) const;
	
	
	Planet* getPlanet() const { return planet; }
private:
	Planet* planet;
};

class Individual : public AbstractIndividual {
public:
	Individual();
	~Individual() override {};
	
	void render(int quality) override;
	
	bool hasPlanet(Planet*) const;
	
	typedef vector<shared_ptr<Gene>> container;
	typedef container::iterator iterator;
	typedef container::const_iterator const_iterator;
	
	iterator begin() { return genes.begin(); }
	iterator end() { return genes.end(); }
	
	const_iterator begin() const { return genes.begin(); }
	const_iterator end() const { return genes.end(); }
	
	unsigned long size() const { return genes.size(); }
	
	// TODO ? in abstract class ?
	void addGene(shared_ptr<Gene> g) { genes.push_back(g); }
	
	void dump(ostream&) const override;
	void mutate()             override;
	virtual void createGenes(Galaxy*);

protected:
	container genes;
	
private:
	Individual(const Individual& orig);

};

#endif	/* INDIVIDUAL_HPP */

