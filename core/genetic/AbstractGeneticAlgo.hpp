/* 
 * File:   AbstractGeneticAlgo.hpp
 * Author: francois
 *
 * Created on 23 novembre 2015, 16:33
 */

#ifndef ABSTRACTGENETICALGO_HPP
#define	ABSTRACTGENETICALGO_HPP

#include "AdaptatorFunction.hpp"
#include "AbstractIndividualFactory.hpp"
#include "AbstractCrossOver.hpp"


namespace genetic {

	// Generic genetic class

	class AdaptatorFunction;
	class Population;

	class AbstractGeneticAlgo {
	public:

		AbstractGeneticAlgo(
				AbstractIndividualFactory* oIndividualFactory,
				AbstractCrossOver* oCrossOver,
				AdaptatorFunction* fAdaptator,
				unsigned int iPopulationSize = 50
				);

		virtual ~AbstractGeneticAlgo() {
		};
		
		void generateInitialPopulation();

		/**
		 * Run the algo until stop condition is reached.
		 * @return 
		 */
		bool run();
		
		Population::type getBest() { return best; }
		float getBestFitness() const { return bestScore; }

		/**
		 * Run one step the algo 
		 * @return true if goal is reached
		 * TODO documente why the goal is reached 
		 */
		bool step();

		long getBestPopulation() const { return bestPop; }
		long getPopulationsGenerated() const { return mpop; }
		
		// dump the popuplation
		void pop(ostream&);
		bool generateNextPopulation();

		void crossOver(Population* newPopulation, unsigned int iIndividuals);

		/**
		 * Override in case genetic algo chooses to elite some individuals.
		 * @param newPopulation
		 */
		virtual void elitism(Population* newPopulation) {}
		virtual void mutate(Population* newPopulation, int percent);
		
		const Population* getActualPopulation() const { return actualPopulation; }

	protected:
		Population* actualPopulation; // Current population
		AdaptatorFunction* mfAdaptator;
		AbstractIndividualFactory* moIndividualFactory;
		
	private:
		AbstractGeneticAlgo(const AbstractGeneticAlgo& orig);

		AbstractCrossOver* moCrossOver;
		unsigned int miPopulationSize;
		long mpop;
		
		float bestScore;
		long bestPop;
		Population::type best;

	};

}
#endif	/* ABSTRACTGENETICALGO_HPP */

