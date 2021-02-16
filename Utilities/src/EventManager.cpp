#include "event/EventManager.h"

namespace utilities
{
	EventManager::EventManager() : idCounter(0)
	{

	}

	bool EventManager::unregisterListener(unsigned id)
	{
		//Find the type index that belongs to this ID.
		auto foundType = typeOfID.find(id);
		if (foundType != typeOfID.end())
		{
			//Find the map containing listeners for the found type.
			auto foundListenerMap = listeners.find(foundType->second);
			if (foundListenerMap != listeners.end())
			{
				//Find the listener stored for the type that has the given ID.
				auto foundListener = foundListenerMap->second.find(id);
				if (foundListener != foundListenerMap->second.end())
				{
					//Remove the listener from the map.
					foundListenerMap->second.erase(foundListener);
					typeOfID.erase(foundType);
					return true;
				}
			}
		}
		return false;
	}
}