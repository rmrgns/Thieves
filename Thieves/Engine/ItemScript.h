#pragma once
#include "MonoBehaviour.h"
class ItemScript : public MonoBehaviour
{
public:
	ItemScript(int id) : m_ID(id) {};

	virtual void Update() override;

	void SetId(int val) { m_ID = val; }
	int GetId() { return m_ID; }

private:
	int m_ID = -1;
};

