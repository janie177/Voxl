#pragma once
#include <typeinfo>

namespace utilities
{
	/*
 * Templated service locator.
 * Creates a singleton instance for any type I provide it, and allows me to set and get the data it refers to.
 *
 * Warning: This breaks all OOP rules. It's extremely powerful and should not be abused. Use as little as possible.
 */
	template<class T>
	class ServiceLocator
	{
	public:
		/*
			Get the service stored for the given type.
		*/
		static T& getService();

		/*
			Set the service for this service locator.
		*/
		static void setService(T* service);

		/*
			Deleted methods to prevent any copying.
		*/
		ServiceLocator(ServiceLocator<T> const&) = delete;
		ServiceLocator(ServiceLocator<T>&&) = delete;
		ServiceLocator& operator=(const ServiceLocator<T>&) = delete;
		ServiceLocator& operator=(ServiceLocator<T>&&) = delete;

	private:
		static ServiceLocator<T>& getInstance();

		ServiceLocator();

		~ServiceLocator() = default;

	private:
		T* data;
	};

	template<class T>
	inline ServiceLocator<T>& ServiceLocator<T>::getInstance()
	{
		static ServiceLocator<T> instance;
		return instance;
	}

	template<class T>
	inline T& ServiceLocator<T>::getService()
	{
		ServiceLocator<T>& instance = getInstance();
#ifndef NDEBUG
		if (instance.data == nullptr)
		{
			throw std::runtime_error("Trying to access service locator service that was not set.");
		}
#endif
		return *(instance.data);
	}

	template<class T>
	inline void ServiceLocator<T>::setService(T* service)
	{
		ServiceLocator<T>& instance = getInstance();
		instance.data = service;
	}

	template<class T>
	inline ServiceLocator<T>::ServiceLocator() : data(nullptr)
	{

	}
}