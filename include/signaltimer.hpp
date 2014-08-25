#ifndef SIG_SIGNALTIMER_HPP
# define SIG_SIGNALTIMER_HPP

# include "signal.hpp"
# include <pthread.h>

# if defined(_WIN32) || defined(_WIN64)
#  include <windows.h>
# else
#  include <unistd.h>
# endif

namespace sig
{
	enum class TimerInterval { usec = 0, msec = 1000, sec = 1000000 };

	template <typename ... Params>
	class SignalTimer : public Signal<Params...>
	{
	private:
		bool		_stop = false;
		int			_interval = 1000;
		pthread_t	_thread_id;
		bool		_in_thread;

	public:
		SignalTimer(int max_connections = 0) : Signal<Params...>(max_connections) {};
		virtual ~SignalTimer() {
			this->_stop = true;
			if (_in_thread)
			{
				void *ret;
				pthread_join(this->_thread_id, &ret);
			}
		}

		void	start() {
			while (!_stop)
			{
				this->onTick();
				#if defined(_WIN32) || defined(_WIN64)
				HANDLE timer; 
				LARGE_INTEGER ft; 
				ft.QuadPart = -(10*this->_interval);
				timer = CreateWaitableTimer(NULL, TRUE, NULL); 
				SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
				WaitForSingleObject(timer, INFINITE); 
				CloseHandle(timer); 
				#else
				usleep(this->_interval);
				#endif
			}
		}

		inline bool	startThread() {
			return (pthread_create(&this->_thread_id, nullptr, [](void *data){
				SignalTimer<Params...> *ptr = (SignalTimer<Params...>*)data;
				ptr->_in_thread = true;
				ptr->start();
				ptr->_in_thread = false;
				return ((void *)nullptr);
			}, (void *)this) == 0);
		}

		inline void	stop() {
			_stop = true;
		}
		inline void setInterval(int interval, TimerInterval int_type = TimerInterval::msec) noexcept {
			this->_interval = interval * (int)int_type;
		}
		inline int interval() const noexcept {
			return (this->_interval);
		}
		virtual void onTick() = 0;
	};
}

#endif