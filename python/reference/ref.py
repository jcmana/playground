class C(object):

    member = "value"

    def set(self, val):
        self.member = val

    def get(self):
        return self.member


def func(obj, type):
    if type == 1:
        obj.member = "changed_by_value"
    if type == 2:
        obj.set("changed_by_method")
    if type == 3:
        return obj.get()



c = C()

print(c.member)
c.member = "hovno"
print(c.member)
func(c, 1)
print(c.member)
func(c, 2)
print(c.member)
print(func(c, 3))
