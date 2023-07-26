#pragma once
#include <mutex>
#include "object/object.h"
#include "state.h"

class Item : public Object
{
	char m_itemCode; // 아이템 번호
	ITEM_STATE m_state;
	std::mutex item_state_lock;

public:
	Item() {
		m_itemCode = -1;
		m_state = ITEM_STATE::ST_NOTUSED;
	}

	void SetState(ITEM_STATE i_st) {
		m_state = i_st;
	}

	ITEM_STATE GetState() { return m_state; }

	void SetItemCode(int code) {
		m_itemCode = code;
	}

	int GetItemCode() {
		return m_itemCode;
	}
};

