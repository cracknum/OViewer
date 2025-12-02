#ifndef DATATYPE_H
#define DATATYPE_H
#include <stdexcept>
enum class DataType
{
  CHAR,
  UNSIGNED_CHAR,
  SHORT,
  UNSIGNED_SHORT,
  FLOAT,
  DOUBLE
};

static size_t getDataTypeSize(const DataType& type)
{
  switch (type)
  {
    case DataType::CHAR:
      return sizeof(char);
    case DataType::UNSIGNED_CHAR:
      return sizeof(unsigned char);
    case DataType::SHORT:
      return sizeof(short);
    case DataType::UNSIGNED_SHORT:
      return sizeof(unsigned short);
    case DataType::FLOAT:
      return sizeof(float);
    case DataType::DOUBLE:
      return sizeof(double);
    default:
      throw std::runtime_error("unsupported data type");
  }
}

#define DataTypeTraitsMacro(enumType, real_type)                                                   \
  template <>                                                                                      \
  struct DataTypeTraits<enumType>                                                                  \
  {                                                                                                \
    using type = real_type;                                                                        \
  };

template <DataType Type>
struct DataTypeTraits;

DataTypeTraitsMacro(DataType::CHAR, char);
DataTypeTraitsMacro(DataType::UNSIGNED_CHAR, unsigned char);
DataTypeTraitsMacro(DataType::SHORT, short);
DataTypeTraitsMacro(DataType::UNSIGNED_SHORT, unsigned short);
DataTypeTraitsMacro(DataType::FLOAT, float);
DataTypeTraitsMacro(DataType::DOUBLE, double);

template <DataType dt>
using data_type = typename DataTypeTraits<dt>::type;
#endif // DATATYPE_H
