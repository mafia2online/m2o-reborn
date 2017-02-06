#pragma once

template <class T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default;

public:
	// Delete copy constructor and assignment operator so there is no copy of the Singleton
	Singleton(const Singleton&) = delete;
	Singleton &operator=(const Singleton&) = delete;

	static T &Instance()
	{
		/* static declaration guarantees:
		* - instance declaration is thread-safe (since C++11)
		* - instance is created the first time the program encounters the declaration
		* - instance is destroyed at program termination (after main() returns)
		* */

		static T instance;
		return instance;
	}
};