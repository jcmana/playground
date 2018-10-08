class S
{
	public:
		static S & GetInstance()
		{
			static S instance;
			return instance;
		}

	private:
		S();
		S(S const&);
		void operator=(S const&);
};

S::S()
{
}


int main(void)
{
	S & s = S::GetInstance();
}