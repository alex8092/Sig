#include <iostream>
#include "sig.hpp"

using namespace std;

class Writer
{
public:
    sig::SignalSwitcher<string>     on_write;

    Writer() : on_write() {}

    bool    write(string data)
    {
        on_write.emit(data);
        return (true);
    }
};

class Test
{
private:
	Writer &   _write;
    int        _number;
public:
    Test(int number, Writer& write) : _write(write), _number(number) {
        write.on_write.connect(&Test::on_write, this);
    }
    ~Test()
    {
        _write.on_write.disconnect(this);
    }

    void on_write(string data)
    {
        cout << "Test : " << this->_number << " receive data write signal : " << data << endl;
    }
};

int main(void)
{
    Writer w;
    Test t(1, w);
    Test * y = new Test(2, w);
    Test r(3, w);
    w.write("The signal dispatcher");
    w.write("Can be useful");
    w.write("To dispatch client connection");
    w.write("in a http server for example");
    Test o(4, w);
    w.write("But in a thread pool too");
    w.write("if you know what i mean");
    w.write("I have no idea what write now");
    delete y;
    w.write("So, now, i stop this text because it's not a book :)");
    return (0);
}