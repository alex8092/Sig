#ifndef SIG_SIGNALSWITCHER_HPP
# define SIG_SIGNALSWITCHER_HPP

# include "signal.hpp"

namespace sig
{
	template <typename ... Params>
	class SignalSwitcher : public Signal<Params...>
	{
		typedef Signal<Params...>		Child;
	private:
		typename Child::iterator 		_current;

	protected:
		virtual void	onRemoveIterator(typename Child::iterator it) noexcept {
			if (this->_current == it)
				++this->_current;
		}

	public:
		SignalSwitcher(int max_connections = 0) : Signal<Params...>(max_connections) {
			this->_current = this->_binds.end();
		}

		virtual void emit(Params... parameters) noexcept {
			if (this->_current == this->_binds.end())
				this->_current = this->_binds.begin();
			if (this->_current != this->_binds.end())
			{
				static_cast<typename Child::iterator>(this->_current)->second(parameters...);
				++this->_current;
			}
		}
	};
}

#endif