#pragma once
#include <map>
#include <cassert>
#include <iostream>

#include "BaseState.h"

namespace utilities
{

	/*
		Specialize so that only Enumerations can be used as StateType.
	*/
	template<class StateType, class T, class = void>
	class FSM
	{

	};

	/*
		Finite State Machine class that uses an enumeration as state type (StateType).
		T is the class that owns the FSM that might have data that needs to be accessible.

		A default state and first tick are provided in the constructor.
		Update is called every tick, or at an interval.
		The amount of ticks between update calls is automatically passed to states.
	*/
	template<class StateType, class T>
	class FSM<StateType, T, typename std::enable_if<std::is_enum<StateType>::value>::type>
	{
	private:
		//State is now the type of BaseState with the correct StateType and Object Type.
		using State = BaseState<StateType, T>;

	public:
		FSM(StateType defaultState, unsigned currentTick) : currentState(nullptr), currentStateType(defaultState), defaultState(defaultState), lastUpdateTick(currentTick)
		{

		}

		/*
		 * FSM needs a deep copy, which is provided using a clone function per state.
		 */
		FSM(const FSM& other)
		{
			defaultState = other.defaultState;
			currentStateType = other.currentStateType;
			lastUpdateTick = other.lastUpdateTick;

			for(auto& pair : other.states)
			{
				states[pair.first] = pair.second->clone();
			}

			//Make sure that the current state is actually valid.
			assert(states.find(currentStateType) != states.end() && "Error: Trying to copy state machine that has invalid current state.");
			currentState = states[currentStateType];
		}

		/*
		 * Get the currently active state type.
		 */
		StateType getCurrentStateType() const
		{
			return currentStateType;
		}

		//Update the current state.
		void update(unsigned tick, T& object)
		{
			if (currentState != nullptr)
			{
				//Calculate how many ticks passed since the last update.
				unsigned ticksPassed = tick - lastUpdateTick;
				lastUpdateTick = tick;

				//Update the current state and receive an update.
				StateType received = currentState->onUpdate(tick, ticksPassed, object);

				//State change requested from current state. Apply directly.
				if (received != currentStateType)
				{
					//Find the requested state and make sure it exists.
					auto found = states.find(received);
					assert(found != states.end() && "Error: Unregistered state requested!");

					//Leave the old state and enter the new one.
					currentState->onLeave(object);
					currentState = found->second;
					currentStateType = received;
					currentState->onEnter(object);
				}
			}
			else
			{
				//No state was set. Set the default state and then update it.
				requestState(defaultState, object);
				update(tick, object);
			}
		}

		//Request a state change. Pimpl.
		bool requestState(StateType type, T& object)
		{
			//Already state happening.
			if (currentState != nullptr)
			{
				//Ask the state if it wants to change. If not, do nothing.
				if (!currentState->onRequestStateChange(type))
				{
					return false;
				}
				//State wants to change. End old state.
				else
				{
					currentState->onLeave(object);
				}
			}

			//Look for the requested state and make sure it is registered.
			auto found = states.find(type);
			assert(found != states.end() && "Trying to set state that doesn't exist!");

			//Set the new state as active and init it.
			currentState = found->second;
			currentStateType = type;
			currentState->onEnter(object);
			return true;
		}

		//Memory management.
		//Make sure each state can only register once.
		void registerState(StateType type, State* state)
		{
			auto found = states.find(type);
			assert(found == states.end() && "Trying to register state that was already registered!");
			states[type] = state;
		}

		/*
		 * Get the last tick at which this state machine was update.
		 */
		unsigned getLastUpdateTick() const
		{
			return lastUpdateTick;
		}

		/*
		 * Get the default state for this state machine.
		 */
		StateType getDefaultState() const
		{
			return defaultState;
		}

		//Destruct and free states.
		~FSM()
		{
			for(auto& pair : states)
			{
				delete pair.second;
			}
		}

	private:
		std::map<StateType, State*> states;
		State* currentState;
		StateType currentStateType;

		StateType defaultState;
		unsigned lastUpdateTick;
	};
}
