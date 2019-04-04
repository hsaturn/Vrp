/* 
 * File:   CrossOver.cpp
 * Author: francois
 * 
 * Created on 24 novembre 2015, 11:42
 */

#include "CrossOver.hpp"
#include "Individual.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

CrossOver::CrossOver() { }

CrossOver::~CrossOver() { }

int CrossOver::crossOver(Population::type &pa1, Population::type &pa2)
{
	shared_ptr<Individual> p1 = dynamic_pointer_cast<Individual>(pa1);
	shared_ptr<Individual> p2 = dynamic_pointer_cast<Individual>(pa2);
	
	if (p1==0 && p2==0)
		cerr << "ERROR : Bad CrossOver parameters, Individual* needed" << endl;
	else if (p1->size()>0 && (p1->size() != p2->size()))
		cerr << "ERROR : CrossOver needs Individuals of same sizes and not empty" << endl;
	else
	{
		unsigned int i=rand()%p1->size();

		shared_ptr<Individual> new1(new Individual());
		shared_ptr<Individual> new2(new Individual());
		
		for(auto it=p1->begin(); it!=p1->begin()+i; it++)
			new1->addGene(*it);
		
		for(const auto& it : *p2)
			if (!new1->hasPlanet(it->getPlanet()))
				new1->addGene(it);
		
		for(auto it=p2->begin(); it!=p2->begin()+i; it++)
			new2->addGene(*it);
		
		for(const auto& it : *p1)
			if (!new2->hasPlanet(it->getPlanet()))
				new2->addGene(it);
		
		pa1=new1;
		pa2=new2;
		return 2;
	}
	return 0;
}

