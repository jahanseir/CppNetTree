
#include <string>
#include <sstream>

using namespace std;


#ifndef UTILS_H_
#define UTILS_H_

template <typename T>
string to_string(T const& object)
{
    ostringstream strm;
    strm << object;
    return strm.str();
}

template <class T>
class KeyEqual
{
public:
	bool operator()(const T *obj1, const T *obj2) const
	{
		return *obj1 == *obj2;
	}
};

template <class T>
class KeyHash
{
public:
	size_t operator()(const T *obj) const
	{
		return hash<std::string>{}(to_string(*obj));
	}
};

#endif
