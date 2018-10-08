#include <iostream>

class ICallback
{
public:
	virtual void OnWhatTheFuck() {};
};


class CHandler : public ICallback
{
// ICallback interface implementation
public:
	virtual void OnWhatTheFuck()
	{
		std::cout << "tahni" << std::endl;
	}

public:
	void StillAlive()
	{
		std::cout << "alive" << std::endl;
	}
};



int main()
{
	CHandler * h = new CHandler();
	h->StillAlive();

	ICallback * c = (ICallback *)(h);
	delete c;

	h->StillAlive();
//	h->OnWhatTheFuck();

	delete h;

	return 0;
}