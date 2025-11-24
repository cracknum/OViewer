#ifndef INFORMATION_PARSER_H
#define INFORMATION_PARSER_H
#include "Macros.h"

#include <itkMetaDataObject.h>
#include <itkObject.h>
#include "DicomTags.h"
#include "DicomExport.h"
namespace itk
{
class MetaDataDictionary;
}
class DICOM_API InformationParser: public itk::Object
{
public:
  itkTypeMacro(InformationParser, itk::Object);
  itkSetPointerDeclare(InformationParser);

  using MetaDataStringType = itk::MetaDataObject<std::string>;
  virtual void parseInfo(const itk::MetaDataDictionary& metaData) = 0;
protected:
  InformationParser() = default;
  ~InformationParser() override = default;
  void PrintSelf(std::ostream & os, itk::Indent indent) const override{Superclass::PrintSelf(os, indent);};

  template<typename T = std::string>
  static T parseTag(const itk::MetaDataDictionary& metaData, const std::string& key)
  {
    const auto it = metaData.Find(key);
    if (it != metaData.End())
    {
      const typename itk::MetaDataObject<T>::Pointer var = dynamic_cast<itk::MetaDataObject<T>*>(it->second.get());
      return var == nullptr ? "N/A" : var->GetMetaDataObjectValue();
    }
    return "N/A";
  }

  static std::string parseTag(const itk::MetaDataDictionary& metaData, const std::string& key)
  {
    return parseTag<std::string>(metaData, key);
  }
};

#endif //INFORMATION_PARSER_H
