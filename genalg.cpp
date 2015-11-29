#ifndef GENALG_H
#define GENALG_H

#include <ctime>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include "ThreadPopulation.cpp"

#define DEFAULT_MGENS 500
#define DEFAULT_TFTNS 70
#define DEFAULT_TLIMT 5
#define st_args ((SelectArgs<IndT, ThreadPopulation<IndT> >*)args)

template<class IndT, class PopT>
class SelectArgs
{
	private:
		PopT* population;
		IndT ret;

	public:
		//Constructors----------------------------------------------------------
		//Default constructor
		SelectArgs() { };

		//Valued constructor
		SelectArgs(PopT* p): population(p)
		{
#ifdef DEBUG
			std::cout << "SelectArgs::" << __func__ << "(" << p << ")\n";
#endif
		};

		//Copy constructor
		SelectArgs(const SelectArgs<IndT, PopT>& src): ret(src.ret) {}

		//Getters---------------------------------------------------------------
		IndT getRet() const { return ret; };

		PopT* getPopulation()
		{
#ifdef DEBUG
			std::cout << "SelectArgs::" << __func__ << "\n";
#endif
			return population;
		};

		//Setters---------------------------------------------------------------
		void setRet(IndT val)
		{
#ifdef DEBUG
			std::cout << "SelectArgs::" << __func__ << "\n";
#endif
			ret = val;
		};

		//Operators-------------------------------------------------------------
		SelectArgs& operator=(const SelectArgs& args)
		{
			population = args.population;
			ret = args.ret;

			return *this;
		};
};

template<class IndT, class PopT>
class AbstractGenAlg
{
	protected:
		PopT population;
		IndT best;
		int maxGenerations;
		float targetFitness;
		int timeLimit;//maximum runtime, in minutes

	protected:
		//Constructors----------------------------------------------------------
		//Default constructor
		AbstractGenAlg(): maxGenerations(0), targetFitness(0), timeLimit(0) {}

		//Valued constructor
		AbstractGenAlg(
				PopT& p,
				int mGens = DEFAULT_MGENS,
				float tFtns = DEFAULT_TFTNS,
				int tLimit = DEFAULT_TLIMT) :
			population(p),
			maxGenerations(mGens),
			targetFitness(tFtns),
			timeLimit(tLimit)
		{
#ifdef DEBUG
			std::cout << __func__ << "\n";
#endif
		};

		//Copy constructor
		AbstractGenAlg(const AbstractGenAlg<IndT, PopT>& src):
			population(src.population),
			best(src.best),
			maxGenerations(src.maxGenerations),
			targetFitness(src.targetFitness),
			timeLimit(src.timeLimit) {}

	public:
		//Getters---------------------------------------------------------------
		IndT getBest() { return best; };
		int getTimeLimit() { return timeLimit; };

		//Setters---------------------------------------------------------------
		void setBest(const IndT& b)
		{
			best = b;
#ifdef DEBUG
			std::cout << __func__ << "\t" << best.Check() << "\n";
#endif
		};

		//Abstract methods------------------------------------------------------
		virtual IndT run() = 0;
};

template<class IndT, class PopT>
class ThreadGenAlg : public AbstractGenAlg<IndT, PopT>
{
	private:
		pthread_t* selectParent;
		SelectArgs<IndT, PopT> selectArgs;
		IndT* nextPop;

	protected:
		class TimeCheckThread : public Thread<int>
		{
			private:
				pthread_t target;
				int timeLimit;
				IndT* best;

			public:
				TimeCheckThread(pthread_t tgt, int tLimit, IndT* bst): Thread<int>(), target(tgt), timeLimit(tLimit), best(bst){}

				int run()
				{
					sleep(60 * timeLimit);
					std::cout << "\033[31mTime is up\033[0m\n";
					std::cout << *best << "\n" ;
					retVal = pthread_kill(target, SIGTERM);
					return retVal;
				}
		};

		//Thread routines-------------------------------------------------------
		static void* SelectThread(void* args)
		{
#ifdef DEBUG
			std::cout << __func__ << "(" << pthread_self()%10 << ")\n";
#endif
			st_args->setRet(st_args->getPopulation()->Select());

			return NULL;
		}

	public:
		//Constructors----------------------------------------------------------
		//Default construtor
		ThreadGenAlg() {}

		//Valued constructor
		ThreadGenAlg( PopT& p, int mGens = DEFAULT_MGENS, float tFtns = DEFAULT_TFTNS, int tLimit = DEFAULT_TLIMT) : AbstractGenAlg<IndT, PopT>(p, mGens, tFtns, tLimit)
		{
#ifdef DEBUG
			std::cout << __func__ << "\n";
#endif
			selectParent = new pthread_t[this->population.getSize()];
			nextPop = new IndT[this->population.getSize()];
		};

		//Copy constructor
		ThreadGenAlg(const ThreadGenAlg<IndT, PopT>& src):
			AbstractGenAlg<IndT, PopT> (src) {}

		//Destructor
		virtual ~ThreadGenAlg()
		{
			delete[] selectParent;
			delete[] nextPop;
		}

		//Other methods
		IndT run()
		{
			int nbSelectThreads = 4;
			IndT* parents = new IndT[nbSelectThreads];
			SelectArgs<IndT, PopT> *args = new SelectArgs<IndT, PopT>[nbSelectThreads];
			TimeCheckThread timer(pthread_self(), this->timeLimit, &this->best);
			srand(time(0));

			//start timer
			timer.start();
			//pthread_create(&timeCheckThread, NULL, timeCheck, &tcArgs);

			//fill arguments structure for selection threads
			for(int i = 0; i < nbSelectThreads; i++)
				args[i] = SelectArgs<IndT, PopT>(&(this->population));

			//main loop
			for(int currentGeneration = 0;
					currentGeneration < this->maxGenerations
					&& this->best.getFitness() < this->targetFitness;
					currentGeneration++)
			{
				std::cout << "\033[33mgeneration " << currentGeneration << " out of " << this->maxGenerations << "\033[0m\n";
				std::cout << "\033[33mbest fitness: " << this->best.getFitness() << " / " << this->targetFitness << "\033[0m\n" ;
				assert(nbSelectThreads%2==0);

				//build next population
				for(int i = 0; i <= this->population.getSize(); i += nbSelectThreads)
				{
#ifdef DEBUG
					//check population state
					std::cout << "check population state\n";
					for(int j = 0; j < this->population.getSize(); j++)
					{
						if(this->population[j].Check() != 1)
						{
							std::cout << "invalid population !!!\n";
							exit(-1);
						}

					}
#endif
					//selection
					for(int j = 0; j < nbSelectThreads; j++)
						pthread_create(&selectParent[j], NULL, SelectThread, &args[j]);

					for(int j = 0; j < nbSelectThreads; j++)
						pthread_join(selectParent[j], NULL);

					for(int j = 0; j < nbSelectThreads; j++)
						parents[j] = args[j].getRet();

					//mating and mutation
					for(int j = 0; j < nbSelectThreads; j += 2)
					{
						nextPop[(i+j)%this->population.getSize()] = parents[j].Mate(parents[j+1]).Mutate();
						nextPop[(i+j+1)%this->population.getSize()] = parents[j+1].Mate(parents[j]).Mutate();
					}

#ifdef DEBUG
					std::cout << "update champion\n";
#endif
					//std::cout << "update champion\n";
					if(this->population.getChampion().getFitness() > this->best.getFitness())
						this->setBest(this->population.getChampion());

					if(this->best.getFitness() == 100)
						return this->best;
				}
				std::cout << "\n";

#ifdef DEBUG
					//check population state
					std::cout << "check next population state\n";
					for(int j = 0; j < this->population.getSize(); j++)
					{
						if(nextPop[j].Check() != 1)
						{
							std::cout << "next population is invalid!!!\n";
							exit(-1);
						}

					}
				std::cout << "assign new pop (" << nextPop << ")\n";
#endif
				//assign new population
				this->population.setPop(nextPop);
				this->population.resetChampion();
			}

			delete[] args;
			delete[] parents;
			return this->best;
		};
};
#endif
