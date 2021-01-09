#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>     /* std::exception */

namespace mtm
{
	class Exception : public std::exception {};
	
	class InvalidDate : public Exception {};

	class NegativeDays : public Exception {};

	class AlreadyRegistered : public Exception {};
	
	class RegistrationBlocked : public Exception {};

	class NotRegistered : public Exception {};

	class AlreadyInvited : public Exception {};

	class PQEmpty : public Exception {};
	
	class PQNotFound : public Exception {};
	
	class NotSupported : public Exception {};
	
	class DateMismatch : public Exception {};
	
	class InvalidNumber : public Exception {};
	
	class InvalidInterval : public Exception {};
		
} // namespace mtm

#endif     /* __EXCEPTIONS_H__ */
