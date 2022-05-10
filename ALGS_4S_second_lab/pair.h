#ifndef PAIR_H
#define PAIR_H


template <typename T1, typename T2>
struct Pair
{
public:
	T1 first;
	T2 second;

	Pair(const T1& first, const T2& second)
	{
		this->first = first;
		this->second = second;
	}

	Pair(const Pair& pair)
	{
		this->first = pair.first;
		this->second = pair.second;
	}

	~Pair() { }
};

template <typename T1, typename T2>
Pair<T1, T2> makepair(T1 first, T2 second)
{
	return Pair<T1, T2>(first, second);
}

#endif
