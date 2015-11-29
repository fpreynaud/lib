#ifndef ABSTRACTPOPULATION_H
#define ABSTRACTPOPULATION_H
#define DEFAULT_POP_SIZE 100
#include <iostream>

template<class IndT>
class AbstractPopulation
{
	protected:
		IndT* pop;
		int size;
		IndT champion;
	
	public:
		//Constructors---------------------------------------------------------------
		//Valued Constructor
		AbstractPopulation(int s = DEFAULT_POP_SIZE) : size(s)
		{
			pop = new IndT[size];
		}

		//Copy constructor
		AbstractPopulation(const AbstractPopulation<IndT>& src) : size(src.size), champion(src.champion) { }

		//Destructor-----------------------------------------------------------------
		virtual ~AbstractPopulation()
		{
			delete[] pop;
		}

		//Getters--------------------------------------------------------------------
		int getSize() { return size; }
		IndT getChampion() { return champion; }

		//Setters--------------------------------------------------------------------
		void setPop(IndT*& newPop)
		{
			delete[] pop;
			pop = newPop;
			newPop = new IndT[size];
		}

		//Operators------------------------------------------------------------------
		IndT& operator[](unsigned int i) const
		{
			return pop[i];
		}

		//Abstract methods-----------------------------------------------------------
		virtual IndT Select() = 0;
		virtual void resetChampion() = 0; 
	protected: 
		virtual bool isNewChampion(const IndT& candidate) = 0;
};
#endif
