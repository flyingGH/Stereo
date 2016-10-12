class Base:

    def __init__(self):
        self.a = 1

    def meth(self):
        print("in Base meth", self.a)

class Sub(Base):

    def meth(self):
        print("in Sub meth")
        return super().meth()

a = Sub()
a.meth()
