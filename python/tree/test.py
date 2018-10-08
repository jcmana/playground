import tree
import tree_adv
import tree_adv_python3


"""
a = tree_adv.DirectTree()
a.next1 = tree_adv.DirectTree()
a.next2 = tree_adv.DirectTree()

print(dir(a))

print(a.get_parent())
"""

b = tree_adv_python3.DirectTree()


b.node1 = tree_adv_python3.DirectTree()
b.node2 = tree_adv_python3.DirectTree()

"""
b.node1.node1 = tree_adv_python3.DirectTree()
b.node1.node2 = tree_adv_python3.DirectTree()

b.node2.attr1 = int(662)
"""

print(b.node1)
print(b.node1)

print(dir(b))

