#pragma once

template <typename Container>
class container_filter_if
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
		friend container_filter_if;
	};

public:
	container_filter_if(Container & c, predicate_type p);
	iterator begin();
	iterator end();
	
private:
	container_type & m_container;
	predicate_type m_predicate;

private:
	template <typename Container>
	friend container_filter_if<Container> filter_if(Container & c, typename container_filter_if<Container>::predicate_type p);
};

template <typename Container>
container_filter_if<Container> filter_if(Container & c, typename container_filter_if<Container>::predicate_type p)
{
	return container_filter_if<Container>(c, p);
}

template <typename Container>
bool
container_filter_if<Container>::iterator::operator !=(const typename container_filter_if::iterator & other)
{
	return (m_it != other.m_it);
}

template <typename Container>
typename container_filter_if<Container>::iterator &
container_filter_if<Container>::iterator::operator ++()
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
typename container_filter_if<Container>::value_type &
container_filter_if<Container>::iterator::operator *()
{
	return (*m_it);
}

template <typename Container>
container_filter_if<Container>::iterator::iterator(container_iterator it_begin, container_iterator it_end, predicate_type p) :
	m_it(it_begin),
	m_it_end(it_end),
	m_predicate(p)
{
}

template <typename Container>
container_filter_if<Container>::container_filter_if(Container & c, predicate_type p) :
	m_container(c),
	m_predicate(p)
{
}

template <typename Container>
typename container_filter_if<Container>::iterator
container_filter_if<Container>::begin()
{
	return iterator(m_container.begin(), m_container.end(), m_predicate);
}

template <typename Container>
typename container_filter_if<Container>::iterator
container_filter_if<Container>::end()
{
	return iterator(m_container.end(), m_container.end(), m_predicate);
}


