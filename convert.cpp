#include "convert.h"

string Convert::ToString(int value)
{
    stringstream stream;
    stream << value;
    return stream.str();
}

string Convert::ToString(long value)
{
    stringstream stream;
    stream << value;
    return stream.str();
}

