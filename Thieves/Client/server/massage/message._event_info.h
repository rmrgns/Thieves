#pragma once

namespace client_fw
{
	// �޼��� ó�� �̺�Ʈ
	class MessageEventInfo
	{
	public:
		MessageEventInfo(unsigned int event_id)
			: m_event_id(event_id) {}

	protected:
		unsigned int m_event_id;

	public:
		unsigned int GetEventID() const { return m_event_id; }
	};
}
