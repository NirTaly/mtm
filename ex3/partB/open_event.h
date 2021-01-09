#ifndef __OPEN_EVENT_H__
#define __OPEN_EVENT_H__

#include <string>

#include "../partA/date_wrap.h"
#include "../partA/exceptions.h"
#include "base_event.h"

namespace mtm
{
	class OpenEvent : public BaseEvent
	{
	public:
		OpenEvent(const DateWrap& date, const std::string& name): BaseEvent(date, name)
		~OpenEvent() = default;
		OpenEvent(const OpenEvent& other) : BaseEvent(other);
		OpenEvent& operator=(const OpenEvent&) = delete;

		OpenEvent* clone() override
		{
			return new OpenEvent(*this);
		}
	};

} // namespace mtm

#endif     /* __OPEN_EVENT_H__ */