/*
 * C++ wrapper for C Pthread library
 */
#ifndef THREAD_H
#define THREAD_H
#include <errno.h>
#include <pthread.h>
#include <iostream>
#warning TODO: proper error management

template<class T>
class Thread
{
	protected:
		pthread_t id;//The thread number
		pthread_attr_t attr;//The thread attributes (e.g detached, joinable,...)

		void handle_err(int errCode);
		static void* routine(void* p);//calls run

	public:
		T retVal;
		Thread(bool detached = false);
		pthread_t getID();
		int getDetachState();
		void setDetached(bool detached = true);
		void join();
		void detach();
		void start();//calls pthread_create, which calls routine
		virtual T run() = 0;
};

/**
 * Constructor
 * @param detached(=false) whether the thread should be detached or not
 */
template<class T>
Thread<T>::Thread(bool detached)
{
	pthread_attr_init(&attr);
	setDetached(detached);
}

/**
 * @return the thread number
 */
template<class T>
pthread_t Thread<T>::getID()
{
	return id;
}

/**
 * Get the detach state attribute in attr
 */
template<class T>
int Thread<T>::getDetachState()
{
	int dState;
	pthread_attr_getdetachstate(&attr, &dState);
	return dState;
}

/**
 * Sets the detach state
 * @param detached(=true) whether the thread should be detached or not
 */
template<class T>
void Thread<T>::setDetached(bool detached)
{
	int dState = detached ? PTHREAD_CREATE_DETACHED:PTHREAD_CREATE_JOINABLE;
	pthread_attr_setdetachstate(&attr, dState);
}

/**
 * Wait for the thread to terminate
 */
template<class T>
void Thread<T>::join()
{
	void* p = &retVal;
	if(getDetachState() == PTHREAD_CREATE_JOINABLE)
	{
		int errn = pthread_join(id, &p);
		handle_err(errn);
	}
	//else throw exception
}

/**
 * detach the thread
 */
template<class T>
void Thread<T>::detach()
{
	if(getDetachState() != PTHREAD_CREATE_DETACHED)
	{
		setDetached();
		int errn = pthread_detach(id);
		handle_err(errn);
	}
}

/**
 * Effectively start the thread
 */
template<class T>
void Thread<T>::start()
{
	pthread_create(&id, &attr, routine, this);
	pthread_attr_destroy(&attr);
}

/**
 * Error handling function. Temporary
 */
template<class T>
void Thread<T>::handle_err(int errCode)
{
	switch(errCode)
	{
		case EAGAIN:
			std::cerr << "Insufficient resources to create another thread, or a system-imposed limit on the number of threads was encountered\n";
			break;
		case EDEADLK:
			std::cerr << "A deadlock was detected or 'thread' argument specifies the calling thread\n";
			break;
		case EINVAL:
			std::cerr << "Invalid settings or another thread is already wainting to join 'thread'\n";
			break;
		case EPERM:
			std::cerr << "No permission to set th specified scheduling policy and parameters\n";
			break;
		case ESRCH:
			std::cerr << "No thread with the specified ID could be found\n";
			break;
	}
}

/**
 * static method called by pthread_create() to start the thread
 * @param pThread pointer to the thread to start
 * @return pThread
 */
template<class T>
void* Thread<T>::routine(void* pThread)
{
	Thread* t = static_cast<Thread*>(pThread);
	t->retVal = t->run();
	pthread_exit(&t->retVal);

	return pThread;
}
#endif
