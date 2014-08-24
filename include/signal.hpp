#ifndef SIG_SIGNAL_HPP
# define SIG_SIGNAL_HPP

# include <vector>
# include <functional>
# include <map>

using namespace std::placeholders;

namespace sig
{
	template <int N>
	struct _my_placeholder {};
}

namespace std
{
    template<int N>
    struct is_placeholder<sig::_my_placeholder<N>> : public integral_constant<int, N> {};
}

namespace sig
{
	namespace _priv
	{
		template <class U, class V, class W, int Current>
		struct _bind
		{
			template <typename ...Args>
			static inline void Do(U *vect, V func, W *object, Args&&... params) noexcept {
				sig::_priv::_bind<U, V, W, Current - 1>::Do(vect, func, object, _my_placeholder<Current>{}, params...);
			}
		};

		template <class U, class V, class W>
		struct _bind<U, V, W, 0>
		{
			template <typename ...Args>
			static inline void Do(U *vect, V func, W *object, Args&&... params) noexcept {
				(*vect)[(void *)object] = std::bind(func, object, params...);
			}
		};
	}

	template <typename ... Params>
	class Signal
	{
	private:
		Signal(Signal&) = delete;
		Signal(Signal&&) = delete;
		Signal& operator=(Signal&) = delete;
		Signal&& operator=(Signal&&) = delete;
		int 			_max_connections;
		int 			_current_connections = 0;
	protected:
		std::map<void *, std::function<void(Params...)>>							_binds;
		typedef typename std::map<void *, std::function<void(Params...)>>::iterator	iterator;
		virtual void																onRemoveIterator(iterator) noexcept {};

	public:
		Signal(int max_connections = 0) : _max_connections(max_connections) {};
		virtual ~Signal() = default;

		template <class U, class V>
		bool	connect(U func, V *object)
		{
			if (this->_max_connections && this->_current_connections >= this->_max_connections)
				return (false);
			sig::_priv::_bind<std::map<void *, std::function<void(Params...)>>, U, V, sizeof...(Params)>::Do(&this->_binds, func, object);
			++this->_current_connections;
			return (true);
		}

		template <class V>
		bool 	disconnect(V *object) noexcept {
			auto it = this->_binds.find((void *)object);
			if (it == this->_binds.end())
				return (false);
			onRemoveIterator(it);
			this->_binds.erase(it);
			--this->_current_connections;
			return (true);
		}

		virtual void emit(Params... parameters) noexcept {
			for (auto it : this->_binds) it.second(parameters...);
		}
	};
}

#endif