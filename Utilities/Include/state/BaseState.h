#pragma once

namespace utilities
{
	template<class StateType, class T>
	class BaseState
	{
	public:
		BaseState()
		{
			
		}

		BaseState(const BaseState&) = default;
		BaseState(BaseState&&) = default;
		BaseState& operator =(const BaseState&) = default;
		BaseState& operator =(BaseState&&) = default;


		/*
			Called when a state is first entered.
			This is meant for setting up the state.
		*/
		virtual void onEnter(T& object) = 0;

		/*
			Called when a state updates.
			This returns the current state type.
			A different state type can be returned if the state should change to that.

			Tick is the current game tick. TicksPassed is the amount of ticks that have passed since the last update was called.
		*/
		virtual StateType onUpdate(unsigned tick, unsigned ticksPassed, T& object) = 0;

		/*
			Called when a state is left.
			This is meant for cleaning up after a state is done.
		*/
		virtual void onLeave(T& object) = 0;

		/*
			Called when the FSM receives a request to change state.
			This can be denied by the current state.
		*/
		virtual bool onRequestStateChange(StateType state) = 0;

		/*
		 * Make a copy of this state instance.
		 */
		virtual BaseState* clone() = 0;

		//Virtual destructor because polymorphism.
		virtual ~BaseState()
		{
			
		}
	};
}
