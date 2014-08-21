#ifndef SIG_SIG_H
# define SIG_SIG_H

# include <vector>
# include <functional>

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
				vect->push_back(std::bind(func, object, params...));
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
		std::vector<std::function<void(Params...)>>	_binds;

	public:
		Signal() = default;
		virtual ~Signal() = default;

		template <class U, class V>
		inline void	connect(U func, V *object)
		{
			sig::_priv::_bind<std::vector<std::function<void(Params...)>>, U, V, sizeof...(Params)>::Do(&this->_binds, func, object);
		}

		inline void emit(Params... parameters) noexcept {
			for (auto it : this->_binds) it(parameters...);
		}
	};
}

#endif