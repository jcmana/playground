enum class flags : unsigned int
{
    A = 0b0001,
    B = 0b0010,
    C = 0b0100,
};

void main()
{
    auto F = flags::A + flags::B;
}