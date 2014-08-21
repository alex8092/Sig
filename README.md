Sig
===

Signal library

Installation
------------

Put the file sig.hpp in your projects or you can do this command in your directory
	wget https://raw.githubusercontent.com/alex8092/Sig/master/include/sig.hpp
enjoy :)

Example
-------

	#include "sig.hpp"
	#include <iostream>

	using namespace std;

	class Writer
	{
	public:
		sig::Signal<string>	 	on_write;
		sig::Signal<>			on_finished;

		bool	write(string data)
		{
			// do write
			on_write.emit(data);
			return (true);
		}
	};

	class Test
	{
	public:
		Test(Writer& write)
		{
			write.on_write.connect(&Test::on_write, this);
			write.on_finished.connect(&Test::on_finished, this);
		}

		void on_write(string data)
		{
			cout << "data write : " << data << endl;
		}

		void on_finished(void)
		{
			cout << "write job finished" << endl;
		}
	};

	int main(void)
	{
		Writer w;
		Test t(w);
		w.write("This is my first signal receive");
		return (0);
	}