#include <iostream>
#include "sig.hpp"

using namespace std;

class Writer
{
public:
    sig::Signal<string>     on_write;
    sig::Signal<>           on_finished;

    Writer() : on_write(1) {}

    bool    write(string data)
    {
        // do write
        on_write.emit(data);
        return (true);
    }
};

class Test
{
private:
	Writer & _write;
public:
    Test(Writer& write) : _write(write)
    {
        write.on_write.connect(&Test::on_write, this);
        write.on_finished.connect(&Test::on_finished, this);
    }

    void on_write(string data)
    {
        cout << "data write : " << data << endl;
        _write.on_write.disconnect(this);
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
    Test y(w); // The second object cannot receive signal because the signal is block to one connection max
    w.write("This is my first signal receive");
    w.write("This is the second signal, but object is disconnect");
    Test u(w); // The third object do receive the signal
    w.write("You see ?");
    return (0);
}