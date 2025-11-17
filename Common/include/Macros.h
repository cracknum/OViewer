#ifndef MACROS_H
#define MACROS_H

#define itkSetPointerDeclare(name)                                                                 \
  using Self = name;                                                                               \
  using Pointer = itk::SmartPointer<Self>;                                                         \
  using ConstPointer = itk::SmartPointer<const Self>;

#define get(name, type)                                                                            \
  inline type get##name() const                                                                    \
  {                                                                                                \
    return m_name;                                                                                 \
  }

#endif // MACROS_H
