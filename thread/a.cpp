#include <iostream>
#include "thread.cpp"

using namespace std;

class TestThread : public Thread<int>
{
	private:
		int arg1;
		float arg2;
		char arg3;

	public:
		int retour;
		TestThread(int a1, float a2, char a3): Thread(), arg1(a1), arg2(a2), arg3(a3)
		{

		};

		void run()
		{
			for(int i = 0; i < 3; i++)
				cout << getID() << ": " << arg1 << " " << arg2 << " " << arg3 << "\n";
			retVal = getID() % 100;
		}

};

int main()
{
	TestThread t1(11, 12, '3'), t2(21, 22, 'R');
	t1.start();
	t1.detach();
	t2.start();
	t1.join();
	t2.join();
	cout << t1.retVal << " " << t2.retVal << "\n";

	return 0;
}

