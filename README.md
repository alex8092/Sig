Sig
===

Signal library

Installation
------------

Put the file sig.hpp in your projects, and enjoy :)

Example
-------

	#include "sig.hpp"

	class Writer
	{
	public:
		sig::Signal<string>	 	on_write;
		sig::Signal<>			on_finished;
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