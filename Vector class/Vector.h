#pragma once
#define Vector_Debug 0

template<typename VectorType>
struct VectorIterator
{
public:
	using ValueType = typename VectorType::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

public:
	VectorIterator(PointerType ptr)
		: m_Ptr(ptr) {}

	VectorIterator& operator++()
	{
		m_Ptr++;
		return *this;
	}

	VectorIterator& operator++(int)
	{
		VectorIterator iterator = *this;
		(*this)++;
		return iterator;
	}

	VectorIterator& operator--()
	{
		return m_Ptr--;
	}

	VectorIterator& operator--(int)
	{
		VectorIterator iterator = *this;
		(*this)--;
		return iterator;
	}

	ReferenceType operator[](size_t index)
	{
		return *(m_Ptr + index);
	}

	const ReferenceType operator[](size_t index) const
	{
		return *(m_Ptr + index);
	}

	ReferenceType operator->()
	{
		return m_Ptr;
	}

	ReferenceType operator*()
	{
		return *m_Ptr;
	}

	bool operator==(const VectorIterator& other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const VectorIterator& other) const
	{
		return !(*this == other);
	}

private:
	PointerType m_Ptr = nullptr;
};

template<typename T>
class Vector
{
public:
	using ValueType = T;
	using Iterator = VectorIterator<Vector<T>>;

public:
	Vector()
	{
		ReAlloc(2);
	}

	Vector(std::initializer_list<T> list)
	{
		size_t size = list.size();
		ReAlloc(size);

		for (size_t i = 0; i < size; i++)
			m_Data[i] = list[i];
	}

	~Vector()
	{
#if Vector_Debug
		std::cerr << "Deleting Vector\n";
#endif
		::operator delete(m_Data, m_Capacity * sizeof(T));
	}

	void push_back(T& obj)
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = obj;
		m_Size++;
	}

	void push_back(const T& obj)
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = obj;
		m_Size++;
	}

	void push(T&& obj)
	{
		ReAllocIfNeeded();

		m_Data[m_Size] = std::move(obj);
		m_Size++;
	}

	template<typename... Args>
	void push_many(Args&&... args)
	{
		size_t amount = sizeof...(Args);
		ReAllocIfNeeded(amount);

		(PushNoReAlloc(args), ...);
	}

	template<typename... Args>
	void emplace(Args&&... args)
	{
		ReAllocIfNeeded();

		new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
		m_Size++;
	}

	void print() const
	{
		for (size_t i = 0; i < m_Size; i++)
		{
			if (i == 0)
			{
				std::cout << m_Data[i];
				continue;
			}
			std::cout << ' ' << m_Data[i];
		}
		std::cout << '\n';
	}

	std::ostream& print(std::ostream& stream) const
	{
		for (size_t i = 0; i < m_Size; i++)
		{
			if (i == 0)
			{
				stream << m_Data[i];
				continue;
			}
			stream << ' ' << m_Data[i];
		}
		stream << '\n';
		return stream;
	}

	T& first()
	{
		return m_Data[0];
	}

	T& last()
	{
		return m_Data[m_Size - 1];
	}

	Iterator begin()
	{
		return m_Data;
	}

	Iterator end()
	{
		return m_Data + m_Size;
	}

	size_t size() const
	{
		return m_Size;
	}

	T& operator[](size_t index)
	{
		return m_Data[index];
	}

	const T& operator[](const size_t index) const
	{
		return m_Data[index];
	}

	void reserve(size_t newCapacity)
	{
		ReAlloc(newCapacity);
	}

private:
	void ReAlloc(size_t newCapacity) noexcept
	{
#if Vector_Debug
		std::cerr << "ReAlloc to " << newCapacity << " objects\n";
#endif
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		for (size_t i = 0; i < std::min(m_Size, newCapacity); i++)
			newBlock[i] = std::move(m_Data[i]);

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));

		m_Data = newBlock;
		m_Capacity = newCapacity;
		m_Size = std::min(m_Size, newCapacity);
	}

	void ReAllocIfNeeded()
	{
		if (m_Size >= m_Capacity)
			ReAlloc(2 * m_Capacity + 1);
	}

	void ReAllocIfNeeded(size_t capacityNeeded)
	{
		if (m_Size + capacityNeeded >= m_Capacity)
			ReAlloc(m_Capacity + capacityNeeded + 1);
	}

	void PushNoReAlloc(T& object)
	{
		new(&m_Data[m_Size]) T(object);
		m_Size++;
	}

private:
	T* m_Data = nullptr;
	size_t m_Capacity = 0;
	size_t m_Size = 0;
};
