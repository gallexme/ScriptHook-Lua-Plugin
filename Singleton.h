#include <assert.h>
template<class T>
class Singleton
{
private:
	static T* m_instance;

	Singleton(const Singleton&);

private:
	inline static void EnsureInstance()
	{
		if (!m_instance)
		{
			m_instance = new(std::nothrow) T;

			assert(m_instance);
		}
	}

protected:
	Singleton() {}
	~Singleton() {}

public:
	static T* GetInstance()
	{
		EnsureInstance();

		return m_instance;
	}
};

template<class T>
T* Singleton<T>::m_instance = nullptr;