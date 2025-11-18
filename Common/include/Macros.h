#ifndef MACROS_H
#define MACROS_H

#define itkSetPointerDeclare(name)                                                                 \
  using Self = name;                                                                               \
  using Pointer = itk::SmartPointer<Self>;                                                         \
  using ConstPointer = itk::SmartPointer<const Self>;                                              \

#endif // MACROS_H
