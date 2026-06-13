#pragma once
#include "Object.h"
#include "State.h"

class Item : public Object
{
public:
	Item() = default;
	virtual ~Item() = default;

	void SetState(ITEM_STATE i_st) { m_state = i_st; }

	[[nodiscard]] ITEM_STATE GetState() const { return m_state; }

	void SetItemCode(int code) { m_itemCode = static_cast<char>(code); }

	[[nodiscard]] int GetItemCode() const { return static_cast<int>(m_itemCode); }

private:
	char m_itemCode{ -1 }; // 아이템 번호
	ITEM_STATE m_state{ ITEM_STATE::ST_NOTUSED };
};