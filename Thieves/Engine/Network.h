#pragma once
class Network
{
public:
	Network();
	~Network();

public:

	void SendMessage();
	

private:
	int8 _sendData = 0;
};

