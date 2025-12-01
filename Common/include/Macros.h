#ifndef MACROS_H
#define MACROS_H

#define itkSetPointerDeclare(name)                                                                 \
  using Self = name;                                                                               \
  using Pointer = itk::SmartPointer<Self>;                                                         \
  using ConstPointer = itk::SmartPointer<const Self>;

#define DISALLOW_COPY_AND_MOVE(TypeName)                                                           \
  TypeName(const TypeName&) = delete;                                                              \
  TypeName& operator=(const TypeName&) = delete;                                                   \
  TypeName(TypeName&&) = delete;                                                                   \
  TypeName& operator=(TypeName&&) = delete

#endif // MACROS_H
