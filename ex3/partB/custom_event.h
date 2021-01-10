#ifndef __CUSTOM_EVENT_H__
#define __CUSTOM_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "base_event.h"

namespace mtm
{
	template <class CanRegister>
	class CustomEvent : public BaseEvent
	{
	public:
		CustomEvent(const DateWrap& date, const std::string& name, CanRegister function);
		~CustomEvent() = default;
		CustomEvent(const CustomEvent&);
		CustomEvent& operator=(const CustomEvent&) = delete;

		void registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked) override;
		
		CustomEvent* clone() const override;
	private:
		CanRegister m_function;
	};

	template <class CanRegister>
	CustomEvent<CanRegister>::CustomEvent(const DateWrap& date, const std::string& name, CanRegister function) :
		BaseEvent(date, name), m_function(function)
	{	}

	template <class CanRegister>
	CustomEvent<CanRegister>::CustomEvent(const CustomEvent& other) : BaseEvent(other), m_function(other.m_function)
	{ }

	template <class CanRegister>
	void CustomEvent<CanRegister>::registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked)
	{
		if (m_function(participant))
		{
			BaseEvent::registerParticipant(participant);
		}
	}

	template <class CanRegister>
	CustomEvent<CanRegister>* CustomEvent<CanRegister>::clone() const
	{
		return new CustomEvent(*this);
	}

} // namespace mtm
#endif     /* __CUSTOM_EVENT_H__ */