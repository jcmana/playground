class DirectTree(object):

    __node_childs = {}
    __node_parent = None
    __node_member = {}

    def __getattr__(self, name):
        if name in self._DirectTree__node_childs[name]:
            return self._DirectTree__node_childs[name]
        else:
            raise KeyError(name)


    def __setattr__(self, name, value):
        self._DirectTree__node_childs[name] = value

    def __dir__(self):
        return self._DirectTree__node_childs.keys()

    def set_parent(self, value):
        self.__node_parent = value

    def get_parent(self):
        return self.__node_parent
