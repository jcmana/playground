#include <boost/di.hpp>

int main()
{
    const auto injector = boost::di::make_injector(
        boost::di::bind<int>.to(42),
        boost::di::bind<double>.to(87.0)
    );
}