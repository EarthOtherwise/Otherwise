#pragma once

#include <vector>

struct VectorMessage
{
	float x1;
	float x2;
	float y1;
	float y2;
};

class VectorListener;

class VectorSender
{
public:

	VectorSender();
	~VectorSender();

	void sendMessage(float x1, float y1, float x2, float y2);
	void recieveListener(VectorListener* listener);

private:
	std::vector<VectorListener*> mListeners;
};

class VectorListener
{
public:

	VectorListener();
	~VectorListener();

	void listenToSender(VectorSender* sender);
	std::vector<float> getMessage();
	void recieveMessage(VectorMessage newMessage);

private:
	std::vector<VectorMessage> mMessages;
};

