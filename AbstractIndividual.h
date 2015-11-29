#ifndef ABSTRACTINDIVIDUAL_H
#define ABSTRACTINDIVIDUAL_H
#include <iostream>

template<class GeneT>
class AbstractIndividual
{
	protected:
		GeneT* genes;
		float fitness;
		int length;

	protected:
		//Constructors---------------------------------------------------------------
		//Default constructor
		AbstractIndividual() : genes(0), fitness(0), length(0) {}

		//Valued constructor
		AbstractIndividual(int l) : fitness(0), length(l)
		{
			genes = new GeneT[length];
		}

		//Copy constructor
		AbstractIndividual(const AbstractIndividual<GeneT>& src): fitness(src.fitness), length(src.length)
		{
			genes = new GeneT[length];

			for(int i = 0; i < length; i++)
					genes[i] = src.genes[i];
		}

		//Destructor-----------------------------------------------------------------
		virtual ~AbstractIndividual()
		{
			delete genes;
		}

	public:
		//Operators------------------------------------------------------------------
		virtual AbstractIndividual<GeneT>& operator=(const AbstractIndividual<GeneT>& src) 
		{ 
#ifdef DEBUG
			//std::cout << "AbstractIndividual::" << __func__ << "\n";
#endif
			fitness = src.fitness;
			length = src.length;

			delete[] genes;
			genes = new GeneT[length];

			for(int i = 0; i < length; i++)
				genes[i] = src.genes[i];

			return *this;
		}

		virtual GeneT& operator[] (unsigned int i) const { return genes[i]; }

		//Getters--------------------------------------------------------------------
		float getFitness() const { return fitness; }
		GeneT* getGenes() const { return genes ; }
		int getLength() const { return length; }

		//Abstract methods-----------------------------------------------------------
		virtual AbstractIndividual<GeneT>& Mate(const AbstractIndividual<GeneT>& partner) = 0;
		virtual AbstractIndividual<GeneT>& Mutate() = 0;
		virtual void Evaluate() = 0;
};

#endif
