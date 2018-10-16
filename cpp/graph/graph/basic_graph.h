#pragma once

#include <cstdarg>
#include <vector>
#include <map>
#include <utility>

/// = = = = = = = = = = = = = CLASS TEMPLATE GRAPH
// TODO: Rename class to 'basic_graph'
template <typename NODE, typename LINK, typename ID>
class graph
{
public:
	using _Tnodes = std::map<ID, NODE>;
	using _Tlinks = std::map<ID, std::map<ID, LINK>>;
	using _Tthis = graph<NODE, LINK, ID>;

public:
	graph();
		
	bool operator==(graph *);
	bool operator!=(graph *);
		
	/// CREATE, REMOVE NODE
	bool create(ID, NODE);
	void remove(ID);
		
	/// LINK, UNLINK NODES
	bool link(ID, ID, LINK);
	void unlink(ID, ID);
		
	/// NODE ACCESS
	NODE & node(ID);
	NODE & operator[](ID);
		
	/// NODE LINKS
	std::map<ID,LINK> to(ID);
	std::map<ID,LINK> in(ID);
		
	std::map<ID,LINK> from(ID);
	std::map<ID,LINK> out(ID);

	/// NODES, LINKS MAP(copy)
	_Tnodes & nodes();
	_Tlinks & links();

	class iterator;
		
	/// START ITERATOR
	iterator start(ID);
	iterator start(int, ID, ...);
	iterator start(std::vector<ID>);
		
	class iterator : public std::iterator<std::bidirectional_iterator_tag, NODE>
	{
	public:
		static constexpr int GRAPH_IT_DEAD = -1;
		static constexpr int GRAPH_IT_DONE = 0;
		static constexpr int GRAPH_IT_GOOD = 1;

	public:
		_Tthis *			_graph;
		ID				_current;
		std::vector<ID>		_path;
		std::vector<ID>		_visited;
		unsigned int		_state;
				
		std::pair<ID,NODE &> * _pair;
				
		iterator();
		~iterator();
				
		iterator & operator=(iterator);
				
		bool operator==(iterator);
		bool operator!=(iterator);
				
		iterator & operator++();
				
		std::pair<ID,NODE &> operator*();
		std::pair<ID,NODE &> * operator->();
				
		static iterator done();
			
				
		bool alive();
				
		friend class graph;
	};

protected:
	_Tnodes _nodes;
	_Tlinks _links;
};

/// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
/// = = = = = = = = = = = = = CLASS GRAPH IMPLEMENTATION
/// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

template <class NODE, class LINK, class ID> graph<NODE,LINK,ID>::graph()
{
	_nodes.clear();
	_links.clear();
}

template <class NODE, class LINK, class ID> bool graph<NODE,LINK,ID>::create(ID id,NODE n)
{
	if (_nodes.count(id) == 1)
	{
		return false;
	}

	_nodes[id] = n;

	return true;
}
template <class NODE, class LINK, class ID> void graph<NODE,LINK,ID>::remove(ID id)
{
	// links 'to' this node
	_links.erase(id);
	// links 'from' this node
	for(typename std::map<ID,std::map<ID,LINK> >::iterator m = _links.begin(); m != _links.end(); ++m)
		m->second.erase(id);
	// node itself
	_nodes.erase(id);
}

template <class NODE, class LINK, class ID> bool graph<NODE,LINK,ID>::link(ID id_from,ID id_to,LINK l)
{
	if( _nodes.count(id_from) == 0 || _nodes.count(id_to) == 0 )
		return false;
	else
		_links[id_to][id_from] = l;
	return true;
}
template <class NODE, class LINK, class ID> void graph<NODE,LINK,ID>::unlink(ID id_from,ID id_to)
{
	if( _links.count(id_to) == 0 )
		return;
	_links[id_to].erase(id_from);
}

template <class NODE, class LINK, class ID> NODE & graph<NODE,LINK,ID>::node(ID id)
{
	return _nodes[id];
}
template <class NODE, class LINK, class ID> NODE & graph<NODE,LINK,ID>::operator[](ID id)
{
	return node(id);
}

template <class NODE, class LINK, class ID> std::map<ID,LINK> graph<NODE,LINK,ID>::to(ID id)
{
	return _links[id];
}
template <class NODE, class LINK, class ID> std::map<ID,LINK> graph<NODE,LINK,ID>::in(ID id)
{
	return to(id);
}
template <class NODE, class LINK, class ID> std::map<ID,LINK> graph<NODE,LINK,ID>::from(ID id)
{
	std::map<ID,LINK> result;
	
	for(typename std::map<ID,std::map<ID,LINK> >::iterator m = _links.begin(); m != _links.end(); ++m)
	{
		typename std::map<ID,LINK>::iterator n = m->second.find(id);
		if( n != m->second.end() )
			result[m->first] = n->second;
	}
	
	return result;
}
template <class NODE, class LINK, class ID> std::map<ID,LINK> graph<NODE,LINK,ID>::out(ID id)
{
	return from(id);
}

template <class NODE, class LINK, class ID> std::map<ID,NODE> & graph<NODE,LINK,ID>::nodes()
{
	return _nodes;
}
template <class NODE, class LINK, class ID> std::map<ID,std::map<ID,LINK>> & graph<NODE,LINK,ID>::links()
{
	return _links;
}


template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator graph<NODE,LINK,ID>::start(ID n)
{
	std::vector<ID> ns;
	ns.push_back(n);
	return start(ns);
}
template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator graph<NODE,LINK,ID>::start(int n,ID first...)
{
	va_list vargs;
	va_start(vargs,first);
	std::vector<ID> res;
	res.push_back(first);
	for(int i = 1; i < n; ++i)
		res.push_back(va_arg(vargs,ID));
	va_end(vargs);

	return start(res);
}
template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator graph<NODE,LINK,ID>::start(std::vector<ID> ns)
{
	std::cout << "graph::start" << std::endl;
	iterator it;
	it._state = iterator::GRAPH_IT_GOOD;
	for(int i = 0; i < ns.size(); ++i)
		if( _nodes.count(ns[i]) == 0 )
			it._state = iterator::GRAPH_IT_DEAD;
	it._graph = this;
	it._current = ns[0];
	it._visited.clear();
	it._path.insert(it._path.begin(),ns.begin()+1,ns.end());
	it._pair = new std::pair<ID,NODE &>(it._current,it._graph->node(it._current));
	
	return it;
}




/// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
/// = = = = = = = = = = = = = CLASS ITERATOR IMPLEMENTATION
/// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 

/// = = = = = = = = = = = = = CONSTRUCTORS, DESTRUCTORS
template <class NODE, class LINK, class ID> graph<NODE,LINK,ID>::iterator::iterator()
{
	std::cout << "graph::iterator::iterator" << std::endl;
	
	_state = iterator::GRAPH_IT_DEAD;
	_pair = new std::pair<ID,NODE &>;
}

template <class NODE, class LINK, class ID> graph<NODE,LINK,ID>::iterator::~iterator()
{
	std::cout << "graph::iterator::~iterator" << std::endl;
	
	std::cout << "   deleting _pair" << std::endl;
	if( _pair != NULL ) 
		delete _pair;
	
	std::cout << std::endl;
}

/// = = = = = = = = = = = = = OPERATOR =
template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator & 
	graph<NODE,LINK,ID>::iterator::operator=(graph<NODE,LINK,ID>::iterator i)
{
	std::cout << "graph::iterator::operator=" << std::endl;
	
	_graph = i._graph;
	_current = i._current;
	_visited = i._visited ;
	_path = i._path;
	_state = i._state;
	_pair = i._pair;
}

/// = = = = = = = = = = = = = OPERATORS ==, !=
template <class NODE, class LINK, class ID> bool graph<NODE,LINK,ID>::iterator::operator==(iterator i)
{
	return (	(	_graph == i._graph && 
				_current == i._current && 
				_visited == i._visited &&
				_path == i._path &&
				_state == i._state ) || 
			(	_path.size() == 0 &&
				_state == GRAPH_IT_DONE &&
				i._state == GRAPH_IT_DONE )
		 ) ? true : false;
}
template <class NODE, class LINK, class ID> bool graph<NODE,LINK,ID>::iterator::operator!=(iterator i)
{
	return ! operator==(i);
}

/// = = = = = = = = = = = = = OPERATOR ++
template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator & graph<NODE,LINK,ID>::iterator::operator++()
{
	std::cout << "graph::iterator::operator++" << std::endl;
	
	// generate outgoing nodes
	std::map<ID,LINK> mfrom = _graph->from(_current);
	// append them to path
	for(typename std::map<ID,LINK>::iterator i = mfrom.begin(); i != mfrom.end(); ++i)
	{
		bool duplicate = false;
		// check visited for existence
		for(typename std::vector<ID>::iterator v = _visited.begin(); v != _visited.end(); ++v)
			if( *v == i->first )
				duplicate = true;
		// check path for existence
		for(typename std::vector<ID>::iterator p = _path.begin(); p != _path.end(); ++p)
			if( *p == i->first )
				duplicate = true;
		// check current for existence
		if( _current == i->first )
			duplicate = true;
		// if ID is not duplicate, add to path
		if( !duplicate )
			_path.push_back(i->first);
	}
	// append current to visited
	_visited.push_back(_current);
	// change current node
	if( _path.size() == 0 )
	{
		_state = GRAPH_IT_DONE;
	}
	else
	{
		// actualize current ID
		_current = _path.front();
		// remove from path
		_path.erase(_path.begin());
		// construct pair form current
		_pair = new std::pair<ID,NODE &>(_current,_graph->node(_current));
	}
	
	// return reference to self
// 	cout << "iterator::reference" << endl;
	return *this;
}

/// = = = = = = = = = = = = = OPERATOR *, ->
template <class NODE, class LINK, class ID> std::pair<ID,NODE &> graph<NODE,LINK,ID>::iterator::operator*()
{
// 	return make_pair<ID,NODE &>(_current,_graph->node(_current));

// 	cout << "iterator::_pair = " << _pair->first << "," << _pair->second << endl;
	
	std::cout << "graph::iterator::operator*" << std::endl;
	
	return *_pair;
}
template <class NODE, class LINK, class ID> std::pair<ID,NODE &> * graph<NODE,LINK,ID>::iterator::operator->()
{
// 	pair<ID,NODE &> temppair = make_pair<ID,NODE &>(_current,_graph->node(_current));
// 	pair<ID,NODE &> * ptpair = &temppair;
// 	return ptpair;
	
	std::cout << "graph::iterator::operator->" << std::endl;

	return _pair;
}

/// = = = = = = = = = = = = = 'DONE' CONSTRUCTOR
template <class NODE, class LINK, class ID> typename graph<NODE,LINK,ID>::iterator graph<NODE,LINK,ID>::iterator::done()
{
	graph<NODE,LINK,ID>::iterator i;
	// i._graph = this;
	// i._current - don't care
	i._path.clear();
	// i._visited - don't care
	i._state = GRAPH_IT_DONE;
	return i;
}


template <class NODE, class LINK, class ID> bool graph<NODE,LINK,ID>::iterator::alive()
{
	if( _state == GRAPH_IT_DEAD || _state == GRAPH_IT_DONE )
		return false;
	else
		return true;
}
