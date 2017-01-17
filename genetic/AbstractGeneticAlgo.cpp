/* 
 * File:   GeneticAlgo.cpp
 * Author: francois
 * 
 * Created on 23 novembre 2015, 16:33
 */

#include "AbstractGeneticAlgo.hpp"
#include "Population.hpp"
#include "AbstractIndividual.hpp"
#include "AdaptatorFunction.hpp"
#include "Random.hpp"
#include "Background.h"
#include <map>
#include <unistd.h>

#include <iostream>
using namespace std;

namespace genetic
{

  AbstractGeneticAlgo::AbstractGeneticAlgo(
										   AbstractIndividualFactory* oIndividualFactory,
										   AbstractCrossOver* oCrossOver,
										   AdaptatorFunction* fAdaptator,
										   unsigned int iPopulationSize)
  :
  actualPopulation(0),
  mfAdaptator(fAdaptator),
  moIndividualFactory(oIndividualFactory),
  moCrossOver(oCrossOver),
  miPopulationSize(iPopulationSize),
  bestScore(9e9),
  bestPop(0),
  best(0)
  {
	  generateInitialPopulation();
  }

  void AbstractGeneticAlgo::generateInitialPopulation()
  {
	  mpop = 1;
	  cout << __FILE__ << ':' << __LINE__ << " Generating initial population" << endl;
	  if (actualPopulation) delete actualPopulation;
	  actualPopulation = new Population();

	  moIndividualFactory->build();	// FIXME Et Sans ? Ca marche aussi (première crèpe (c) Arnaud)

	  for (int i = miPopulationSize; i > 0; --i)
	  {
		  actualPopulation->push_back(moIndividualFactory->build());
	  }

  }

  void AbstractGeneticAlgo::mutate(Population* newPopulation, int percent)
  {
	  int count = newPopulation->size();
	  while (count)
	  {
		  count--;
		  if (Random::rand(0, 100) < percent)
		  {
			  newPopulation->chooseRandom()->mutate();
		  }
	  }
  }

  bool AbstractGeneticAlgo::generateNextPopulation()
  {
	  bool bRet = false;
	  if (actualPopulation)
	  {
		  Population* elite = new Population;
		  Population* newPopulation = new Population;

		  elitism(elite);
		  crossOver(newPopulation, miPopulationSize - elite->size());
		  mutate(newPopulation, 70);

		  for (auto ind : *elite)
			  newPopulation->push_back(ind);

		  delete actualPopulation;
		  actualPopulation = newPopulation;

		  //cout << "current population (" << actualPopulation->size() << ") ";
		  float min = 9e9, max = 0;
		  for (Population::type p : *actualPopulation)
		  {
			  float dist = mfAdaptator->fitnessScore(p);
			  //p->dump();
			  if (dist > max) max = dist;
			  if (dist < min)
			  {
				  min = dist;
				  if (dist < bestScore)
				  {
					  bRet = true;
					  best = p;
					  bestScore = min;
					  bestPop = mpop;
				  }
			  }
		  }
		  cout << __FILE__ << ':' << __LINE__ << " Population # " << mpop++;
		  cout << " (" << (long) min << " -> " << (long) max << "), best found in " << bestPop << "  \r" << flush;
	  }
	  else
	  {
		  generateInitialPopulation();
	  }
	  return bRet;
  }

  void AbstractGeneticAlgo::pop(ostream& out)
  {
	  if (actualPopulation)
	  {
		  // FIXME cet algo permet de trier la population par score.
		  // c'est utilisé ailleurs ! Copy /paste warning
		  multimap<float, Population::type> sorted;
		  for (Population::type p : *actualPopulation)
		  {
			  sorted.insert(pair<float, Population::type>(mfAdaptator->fitnessScore(p), p));
		  }
		  out << "Population size: " << actualPopulation->size();
		  for (auto it : sorted)
		  {
			  cout << "  ";
			  it.second->dump(out);
			  out << "dist : " << it.first << ' ';
		  }

	  }
	  else
		  out << "No population yet" << endl;
  }

  void AbstractGeneticAlgo::crossOver(Population* newPopulation, unsigned int iIndividuals)
  {
	  int failed = 0;

	  while (iIndividuals)
	  {
		  Population::type p1 = actualPopulation->chooseRandom();
		  Population::type p2 = actualPopulation->chooseRandom();

		  int count = moCrossOver->crossOver(p1, p2);
		  if (count)
		  {
			  newPopulation->push_back(Population::type(p1)); // FIXME boooooo
			  iIndividuals--;

			  if (iIndividuals && count > 1)
			  {
				  newPopulation->push_back(p2);
				  iIndividuals--;
			  }

		  }
		  else if (failed++ == 10)
			  break;
	  }
	  while (iIndividuals)
	  {
		  cerr << __FILE__ << ':' << __LINE__ << " : building missing individual !!!" << endl;
		  newPopulation->push_back(moIndividualFactory->build());
	  }
  }

  bool AbstractGeneticAlgo::step()
  {
	  return generateNextPopulation();
  }

}