class A():
    def __init__(self):
        self.attr = "jebem"

    def __getattribute__(self, name):
        return self.attr

    def __setattr__(self, name, value):
        self.attr = value
