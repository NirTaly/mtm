#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

// #include <exception>     /* std::runtime_error */
#include <stdexcept>     /* std::runtime_error */
#include <string>

namespace mtm
{
	class Exception : public std::exception {};
	
	class InvalidDate : public Exception {};

	class NegativeDays : public Exception {};

} // namespace mtm

#endif     /* __DATE_WRAP_H__ */
