#include "Messaging.h"

VectorSender::VectorSender()
{
}

VectorSender::~VectorSender()
{
}

void VectorSender::sendMessage(float x1, float y1, float x2, float y2)
{
	VectorMessage newMessage;
	newMessage.x1 = x1;
	newMessage.y1 = y1;
	newMessage.y2 = y2;
	newMessage.x2 = x2;

	for (unsigned int i = 0; i < mListeners.size(); i++)
	{
		mListeners[i]->recieveMessage(newMessage);
	}
}

void VectorSender::recieveListener(VectorListener* listener)
{
	mListeners.push_back(listener);
}

VectorListener::VectorListener()
{
}

VectorListener::~VectorListener()
{
}

void VectorListener::listenToSender(VectorSender* sender)
{
	sender->recieveListener(this);
}

std::vector<float> VectorListener::getMessage()
{
	std::vector<float> messageVector;

	for (unsigned int i = 0; i < mMessages.size(); i++)
	{
		messageVector.push_back(mMessages[i].x1);
		messageVector.push_back(mMessages[i].x2);
		messageVector.push_back(mMessages[i].y1);
		messageVector.push_back(mMessages[i].y2);
	}

	mMessages.clear();

	return messageVector;
}

void VectorListener::recieveMessage(VectorMessage newMessage)
{
	mMessages.push_back(newMessage);
}
