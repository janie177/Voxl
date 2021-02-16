#pragma once
#include <map>
#include <memory>
#include <typeindex>
#include <functional>

namespace utilities
{
	//Forward declare
	class Event;
	enum class EventType;

	/*
		Data type used to correctly cast when an event is called.
	*/
	class ListenerData
	{
	public:
		virtual void notifyListener(Event& event) = 0;
	};

	template<class T>
	class ConcreteListenerData : public ListenerData
	{
	public:
		ConcreteListenerData(const std::function<void(T&)>& function) : function(function)
		{

		}

		void notifyListener(Event& event) override
		{
			//Call the correct function with the correct type.
			function(static_cast<T&>(event));
		}
		
	private:
		std::function<void(T&)> function;
	};

	/*
		Event manager handles all event listening.
		Events can be created and called, and listeners can be registered.
	*/
	class EventManager
	{
	public:
		
		EventManager();

		/*
			Add a listener for the given event function.
		*/
		template<class T>
		unsigned registerListener(std::function<void(T&)> listener);

		/*
			Unregister the listener with the given ID.
			If the given ID was not found, false is returned.
			If the given IF was found and removed, true is returned.
		*/
		bool unregisterListener(unsigned id);

		/*
			Call an event, notifying all active listeners.
		*/
		template<class T>
		void callEvent(T& event);

		~EventManager() = default;

	private:
		//Map containing each event type, and a set of listeners ordered by ID.
		std::map<std::type_index, std::map<unsigned, std::unique_ptr<ListenerData>>> listeners;
		std::map<unsigned, std::type_index> typeOfID;

		unsigned idCounter;
	};


	template<class T>
	inline unsigned EventManager::registerListener(std::function<void(T&)> listener)
	{
		static_assert(std::is_base_of<Event, T>::value, "Error: Trying to register listener that has invalid event type.");

		auto type = std::type_index(typeid(T));
		unsigned id = idCounter;

		typeOfID.insert(std::make_pair(id, type));
		
		//Find the listener map.
		auto listenerMap = listeners.find(type);
		if (listenerMap != listeners.end())
		{
			//The listener map exists for this type.
			//Insert it.
			listenerMap->second.insert(std::make_pair(id, std::make_unique<ConcreteListenerData<T>>(listener)));
		}
		else
		{
			//There was no listener of this type yet.
			//Make a new map for it.
			auto inserted = listeners.insert(std::make_pair(type, std::map<unsigned, std::unique_ptr<ListenerData>>()));
			inserted.first->second.insert(std::make_pair(id, std::make_unique<ConcreteListenerData<T>>(listener)));
		}

		++idCounter;
		return id;
	}

	template<class T>
	inline void EventManager::callEvent(T & event)
	{
		static_assert(std::is_base_of<Event, T>::value, "Error: Trying to call event that is not an event!");

		//Get the ID for the given class
		auto id = std::type_index(typeid(event));

		//Look for subscribed listeners.
		auto itr = listeners.find(id);
		if (itr != listeners.end())
		{
			//Call the event for each listener.
			auto listener = itr->second.begin();
			while (listener != itr->second.end())
			{
				listener->second->notifyListener(event);
				++listener;
			}
		}
	}
}