#include "bst.cpp"

template <typename K, typename V>
class Map
{
    friend std::ostream& operator<<(std::ostream& os, const Map<K, V> &map)
    {
        return os << map._tree;
    }

    friend void swap(Map<K, V>& first, Map<K, V>& second) noexcept
    {
        using std::swap;

        swap(first._tree, second._tree); 
    }

    class MapElem
    {
        friend std::ostream& operator<<(std::ostream& os, const MapElem &map_el)
        {
            return os << std::endl << "Key: " << map_el.key_value.first << " Value: " << map_el.key_value.second;
        }

        public:
        std::pair<K, V> key_value; 

        MapElem() = default;
        MapElem(const std::pair<K, V>& key_value) : key_value(key_value) {}
        MapElem(const K& key, const V& value) { key_value = std::pair<K, V>(key, value); }

        bool operator==(const MapElem &elem) const { return key_value.first == elem.key_value.first; }
        bool operator<(const MapElem &elem) const { return key_value.first < elem.key_value.first; }
        bool operator<=(const MapElem &elem) const { return key_value.first <= elem.key_value.first; }
        bool operator>(const MapElem &elem) const { return key_value.first > elem.key_value.first; }
    };

    class Iterator
    {
        friend class Map<K, V>;

        Iterator() = default;

        public:
            bool operator==(const Iterator &it) const;
            bool operator!=(const Iterator &it) const { return !(*this == it); }
            Iterator& operator++();
            Iterator operator++(int);
            V& operator*() const;
            V* operator->() const;   
            operator bool() const { return !(this->_currNode == nullptr); } 

        private:
            Iterator(typename BSTree<MapElem>::Node* node, const Map<K, V>* map) 
                : _currNode(node), _map(map) {} 

            typename BSTree<MapElem>::Node* _currNode;
            const Map<K, V>* _map;
    };

    class ConstIterator
    {
        friend class Map<K, V>;

        ConstIterator() = default;

        public:
            bool operator==(const ConstIterator &it) const;
            bool operator!=(const ConstIterator &it) const { return !(*this == it); }
            ConstIterator& operator++();
            ConstIterator operator++(int);
            const V& operator*() const;
            const V* operator->() const;   
            operator bool() const { return !(this->_currNode == nullptr); } 

        private:
            ConstIterator(typename BSTree<MapElem>::Node* node, const Map<K, V>* map) 
                : _currNode(node), _map(map) {} 

            typename BSTree<MapElem>::Node* _currNode;
            const Map<K, V>* _map;
    };
  
    public:
        Map() = default;
        Map(const Map& other) = default;
        Map(Map&& other) noexcept = default;
        ~Map() = default;

        Map<K, V>& operator=(const Map<K, V>& other);
        Map<K, V>& operator=(Map<K, V>&& other);
        V& operator[](const K& key);

        std::size_t size() const { return _tree.size(); }
        std::pair<Iterator, bool> insert(const std::pair<K, V> &key_value);
        std::pair<Iterator, bool> insert(const K& key, const V& value);
        ConstIterator find(const K& key) const;
        Iterator find(const K& key);
        bool remove(const K& key);
        
        ConstIterator begin() const;
        Iterator begin();
        ConstIterator end() const;
        Iterator end();

    private: 
        BSTree<MapElem> _tree;
};

/*
    Iterator
*/
template <typename K, typename V>
bool Map<K, V>::Iterator::operator==(const Iterator &it) const
{
    return _map->_tree.getRoot() == it._map->_tree.getRoot() && _currNode == it._currNode;
}

template <typename K, typename V>
typename Map<K, V>::Iterator& Map<K, V>::Iterator::operator++()
{
 typename BSTree<MapElem>::Node* temp;
    if(_currNode == nullptr)
    {
        _currNode = _map->_tree.getRoot();

        if(_currNode == nullptr)
        {
            throw std::invalid_argument("MySet<T>::Iterator::operator++\n : Set is empty");
        }

        while(_currNode->left != nullptr)
        {
            _currNode = _currNode->left;
        }
    }
    else if (_currNode->right != nullptr)
    {
         _currNode = _currNode->right;

        while(_currNode->left!=nullptr)
        {
            _currNode = _currNode->left;  
        }
    }
    else
    {
        temp = _map->_tree.findParent(_currNode->value); 

        while(temp != nullptr && _currNode == temp->right)
        {
            _currNode = temp;
            temp = _map->_tree.findParent(temp->value);
        }

        _currNode = temp;
    }
    return *this;  
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Iterator::operator++(int)
{
    Iterator temp(*this);
    ++(*this);
    return temp;
}

template <typename K, typename V>
auto Map<K, V>::Iterator::operator*() const -> V& 
{
    return _currNode->value.key_value.second;
}

template <typename K, typename V>
auto Map<K, V>::Iterator::operator->() const -> V*
{
    return &_currNode->value.key_value.second;
}

/*
    ConstIterator
*/
template <typename K, typename V>
bool Map<K, V>::ConstIterator::operator==(const ConstIterator &it) const
{
    return _map->_tree.getRoot() == it._map->_tree.getRoot() && _currNode == it._currNode;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator& Map<K, V>::ConstIterator::operator++()
{
    if(_currNode == nullptr){
    _currNode = _map->_tree.getRoot();
    if(_currNode == nullptr){
       throw "Set is empty";
    }

    while(_currNode->left != nullptr){
      _currNode = _currNode->left;
    }

    return *this;
  }

  if(_currNode->right != nullptr){
    _currNode = _currNode->right;
    while(_currNode->left != nullptr){
      _currNode = _currNode->left;
    }
  }else{
    typename BSTree<MapElem>::Node* temp;
    temp = _map->_tree.findParent(_currNode->value);
    while(temp != nullptr && _currNode == temp->right){
      _currNode = temp;  
      temp = _map->_tree.findParent(temp->value);
    }
    _currNode = temp;
  }    

  return *this;
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::ConstIterator::operator++(int)
{
    ConstIterator temp(*this);
    ++(*this);
    return temp;
}

template <typename K, typename V>
auto Map<K, V>::ConstIterator::operator*() const -> const V&
{
    return _currNode->value.key_value.second;
}

template <typename K, typename V>
auto Map<K, V>::ConstIterator::operator->() const -> const V*
{
    return &_currNode->value.key_value.second;
}

/* 
    Map
*/
template <typename K, typename V>
Map<K, V>& Map<K, V>::operator=(const Map<K, V>& other)
{
    Map<K, V> temp(other);
    swap(*this, temp);
    return *this;
}

template <typename K, typename V>
Map<K, V>& Map<K, V>::operator=(Map<K, V>&& other)
{
    swap(*this, other);
    return *this;
}

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
    typename BSTree<MapElem>::Node* search = _tree.search(MapElem(key, V{}));
    if(search != nullptr)
        return search->value.key_value.second;
    else
        return _tree.insert(MapElem(key, V{}))->value.key_value.second;
}

template <typename K, typename V>
std::pair<typename Map<K, V>::Iterator, bool> Map<K, V>::insert(const std::pair<K, V>& key_value)
{
    typename BSTree<MapElem>::Node* search = _tree.search(key_value);
    if(search == nullptr)
	{
	    typename BSTree<MapElem>::Node* temp = _tree.insert(MapElem(key_value));
        return std::pair<Iterator, bool>(Iterator(temp, this), true);
	}
	else
	{
        return std::pair<Iterator, bool>(Iterator(search, this), false);
	}
}

template <typename K, typename V>
std::pair<typename Map<K, V>::Iterator, bool> Map<K, V>::insert(const K& key, const V& value)
{
    
    MapElem map_elem = MapElem(key, value); 
    typename BSTree<MapElem>::Node* search = _tree.search(map_elem);
    if(search == nullptr)
	{
	    typename BSTree<MapElem>::Node* temp = _tree.insert(map_elem);
        return std::pair<Iterator, bool>(Iterator(temp, this), true);
	}
	else
	{
        return std::pair<Iterator, bool>(Iterator(search, this), false);
	}
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::find(const K& key) const
{
    
    MapElem map_elem = MapElem(key, V{}); 
    typename BSTree<MapElem>::Node* search = _tree.search(map_elem);
    if(search != nullptr)
	{
        return ConstIterator(search, this);
	}
	else
	{
        return ConstIterator(nullptr, this);
	}

} 

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
    MapElem map_elem = MapElem(key, V{}); 
    typename BSTree<MapElem>::Node* search = _tree.search(map_elem);
    if(search != nullptr)
	{
        return Iterator(search, this);
	}
	else
	{
        return Iterator(nullptr, this);
	}

}

template <typename K, typename V>
bool Map<K, V>::remove(const K& key)
{
    _tree.remove(_tree.getRoot(), MapElem(key, V{}));
} 

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() 
{  
    typename BSTree<MapElem>::Node* temp = _tree.getRoot();
    if(temp != nullptr) 
    {
        while(temp->left != nullptr)
        {
            temp = temp->left;
        }
        return typename Map<K, V>::Iterator(temp, this);
    } 
    else 
    {
        return this->end();
    }
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() 
{
    return Iterator(nullptr, this);
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::begin() const
{  
    typename BSTree<MapElem>::Node* temp = _tree.getRoot();
    if(temp != nullptr) {
        while(temp->left != nullptr)
        {
            temp = temp->left;
        }
        return typename Map<K, V>::ConstIterator(temp, this);
    } else {
        return this->end();
    }
}

template <typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::end() const
{
    return ConstIterator(nullptr, this);
}