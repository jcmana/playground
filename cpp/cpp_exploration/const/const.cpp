#include <iostream>
#include <string>

class myclass
{
public:
    myclass(const std::string & name) :
        m_name(name)
    {
    }

    std::string name()
    {
        return m_name;
    }

    const std::string name() const
    {
        return m_name;
    }

private:
    const std::string m_name;
};

void main()
{
    myclass mca("myclass");
    const myclass mcb("const myclass");

    auto mca_name = mca.name();
    auto mcb_name = mcb.name();             // <- not const because who gives a damn about qualifier on return value

    mca_name = "yap!";
    mcb_name = "nop?";
}