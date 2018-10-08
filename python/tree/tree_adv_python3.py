class DirectTree(object):

    def __init__(self):
        self.__node_parent = None
        self.__node_childs = {}
        self.__node_members = {}

    def __getattr__(self, name):

        print("__getattribute__(self={}, name='{}')".format(self, name))

        node_dir = object.__dir__(self)
        node_childs = object.__getattribute__(self, '__node_childs')
        node_members = object.__getattribute__(self, '_DirectTree__node_members')

        for child in node_childs.keys():
            if child == name:
                print(node_childs[name])
                return node_childs[name]

        for member in node_members.keys():
            if member == name:
                print(node_members[name])
                return node_members[name]

        raise KeyError(name)


    def __setattr__(self, name, item):

        print("__setattr__(self={}, name='{}', item='{}')".format(self, name, item))

        item_bases = type(item).__bases__

        print(object.__dir__(self))
        node_childs = object.__getattribute__(self, '_DirectTree__node_childs')
        print(node_childs.keys())
        node_members = object.__getattribute__(self, 'node_members')

        if DirectTree in item_bases:
            node_childs[name] = item
        else:
            node_members[name] = item

    def __dir__(self):
        dir = []
        dir += object.__getattribute__(self, '_DirectTree__node_childs').keys()
        dir += object.__getattribute__(self, '_DirectTree__node_members').keys()
        return dir

    def set_parent(self, value):
        self.__node_parent = value

    def get_parent(self):
        return self.__node_parent
