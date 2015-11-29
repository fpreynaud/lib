#ifndef BASETHREADPOPULATION_H
#define BASETHREADPOPULATION_H
#include <AbstractPopulation.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>

template<class IndT>
class ThreadPopulation : public AbstractPopulation<IndT>
{
	protected:
		pthread_mutex_t* selectMutex;
		pthread_mutex_t updateBestMutex;

	public:
		//Constructors---------------------------------------------------------------
		//Valued constructor
		ThreadPopulation(int s = DEFAULT_POP_SIZE): AbstractPopulation<IndT>(s)
		{
			selectMutex = new pthread_mutex_t[s];

			for(int i = 0; i < s; i++)
				pthread_mutex_init(&selectMutex[i], NULL);

			pthread_mutex_init(&updateBestMutex, NULL);
		};

		//Copy constructor
		ThreadPopulation(const ThreadPopulation<IndT>& src) : AbstractPopulation<IndT>(src)
		{
			selectMutex = new pthread_mutex_t[this->size];

			for(int i = 0; i < this->size; i++)
				pthread_mutex_init(&selectMutex[i], NULL);

			pthread_mutex_init(&updateBestMutex, NULL);
		}

		//Destructor-----------------------------------------------------------------
		virtual ~ThreadPopulation()
		{
			for(int i = 0; i < this->size; i++)
				pthread_mutex_destroy(&selectMutex[i]);

		};

		//Abstract methods--------------------------------------------------------------
		virtual IndT Select() = 0;
		virtual void resetChampion() = 0;
	protected:
		virtual bool isNewChampion(const IndT& candidate) = 0;
};
#endif
