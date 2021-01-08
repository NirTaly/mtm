#ifndef __CUSTOM_EVENT_H__
#define __CUSTOM_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "base_event.h"
#include "dv.h"

namespace mtm
{
	template <class CanRegister>
	class CustomEvent : public BaseEvent
	{
	public:
		CustomEvent(const DateWrap& date, const std::string& name);
		~CustomEvent() = default;
		CustomEvent(const CustomEvent&);
		CustomEvent& operator=(const CustomEvent&) = delete;

		void registerParticipant(size_t participant) throw(AlreadyRegiste, RegistrationBlocked) override;
		
		CustomEvent* clone() override;
	};

	template <class CanRegister>
	CustomEvent::CustomEvent(const DateWrap& date, const std::string& name) :
		BaseEvent(date, name)
	{	}

	template <class CanRegister>
	CustomEvent::CustomEvent(const CustomEvent& other) : BaseEvent(other)
	{ }

	template <class CanRegister>
	void CustomEvent::registerParticipant(size_t participant) throw(AlreadyRegistered, RegistrationBlocked) override
	{
		if (CanRegister(participant))
		{
			BaseEvent::registerParticipant(participant);
		}
	}

	template <class CanRegister>
	CustomEvent* clone() override
	{
		return new CustomEvent(*this);
	}

} // namespace mtm
#endif     /* __CUSTOM_EVENT_H__ */