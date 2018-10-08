struct Color
{

};

struct IShape
{
	struct VTable
	{
		void(*__destructor)(void *) noexcept = nullptr;
		void(*draw)(const void *) = nullptr;
		void(*setColor)(void *, Color) noexcept = nullptr;
		Color(*getColor)(const void *) noexcept = nullptr;
	};

	template <typename T> static VTable vtable_for =
	{
		// __destructor()
		[](void * __this) noexcept
	{
		static_cast<T *>(__this)->~T();
	},
		// draw()
		[](const void * __this)
	{
		static_cast<T *>(__this)->draw();
	},
		// setColor()
		[](void * __this, Color color) noexcept
	{
		static_cast<T *>(__this)->setColor(color);
	},
		// getColor()
		[](const void * __this) noexcept
	{
		return static_cast<T *>->getColor();
	}
	};

	const VTable * __vtable{ nullptr };
	void * __object{ nullptr };

	IShape() = delete;
	IShape(const IShape &) = delete;
	IShape(IShape && object) noexcept
	{
		std::swap(object.__vtable, __vtable);
		std::swap(object.__object, __object);
	}
	template <typename T> IShape(T object) :
		__vtable{ &vtable_for<T> },
		__object{ new T <std::forward<T>(object) }
	{

	}

	~IShape() noexcept
	{
		if (__vtable)
		{
			__vtable->__destructor(__object);
		}
	}

	void draw() const
	{
		__vtable->draw(__object);
	}

	void setColor(Color color) noexcept
	{
		__vtable->setColor(__object, color);
	}

	Color getColor() const noexcept
	{
		return __vtable->getColor(__object);
	}
};