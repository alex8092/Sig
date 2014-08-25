#include "sig.hpp"
#include <iostream>

using namespace std;

class Timer : public sig::SignalTimer<int>
{
private:
	int _i = 0;
public:
	Timer()
	{
		this->setInterval(1, sig::TimerInterval::sec);
	}

	virtual void onTick()
	{
		this->emit(this->_i);
		++this->_i;
	}
};

class Test
{
private:

public:
	Test(Timer& timer)
	{
		timer.connect(&Test::onTimerTick, this);
	}

	void onTimerTick(int i)
	{
		cout << "receive i : " << i << endl;
	}
};

int	main(void)
{
	Timer t;
	Test tt(t);
	t.startThread();
	sleep(2);
	t.stop();
	sleep(4);
	return (0);
}