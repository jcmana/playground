#pragma once

template <typename Container>
class filter
{
public:
	using container_value_type = typename Container::value_type;
	using container_type = Container;
	using container_iterator = typename Container::iterator;

	using value_type = container_value_type;
	using predicate_type = bool(*)(const int & n);

public:
	class iterator
	{
	public:
		bool operator !=(const iterator & other);
		iterator & operator ++();
		value_type & operator *();

	protected:
		iterator(container_iterator it_begin, container_iterator it_end, predicate_type p);

	private:
		container_iterator m_it;
		container_iterator m_it_end;
		predicate_type m_predicate;

	private:
		friend filter;
	};

public:
	iterator begin();
	iterator end();

protected:
	filter(Container & c, predicate_type p);

private:
	container_type & m_container;
	predicate_type m_predicate;

private:
	template <typename Container> friend filter<Container> make_filter(Container & c, typename filter<Container>::predicate_type p);
};

template <typename Container>
filter<Container> make_filter(Container & c, typename filter<Container>::predicate_type p)
{
	return filter<Container>(c, p);
}

template <typename Container>
bool
filter<Container>::iterator::operator !=(const typename filter::iterator & other)
{
	return (m_it != other.m_it);
}

template <typename Container>
typename filter<Container>::iterator & 
filter<Container>::iterator::operator ++()
{
	do
	{
		++m_it;
		if (m_it == m_it_end) break;
	}
	while (m_predicate(*m_it) == false);

	return (*this);
}

template <typename Container>
typename filter<Container>::value_type &
filter<Container>::iterator::operator *()
{
	return (*m_it);
}

template <typename Container>
filter<Container>::iterator::iterator(container_iterator it_begin, container_iterator it_end, predicate_type p) :
	m_it(it_begin),
	m_it_end(it_end),
	m_predicate(p)
{
}

template <typename Container>
typename filter<Container>::iterator 
filter<Container>::begin()
{
	return iterator(m_container.begin(), m_container.end(), m_predicate);
}

template <typename Container>
typename filter<Container>::iterator
filter<Container>::end()
{
	return iterator(m_container.end(), m_container.end(), m_predicate);
}

template <typename Container>
filter<Container>::filter(Container & c, predicate_type p) :
	m_container(c),
	m_predicate(p)
{
}
