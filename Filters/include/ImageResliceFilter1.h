#ifndef IMAGERESLICEFILTER1_H
#define IMAGERESLICEFILTER1_H
#include <array>
#include <glm/glm.hpp>
#include <memory>

struct Volume;
class ImageResliceFilter1
{
public:
  void doFilter(std::shared_ptr<Volume> volume, const glm::mat4& planeWorldMatrix);

protected:
  std::array<glm::vec3, 2> calculatePlaneLocalBounds();

private:
  std::shared_ptr<Volume> mVolume;
};

#endif // IMAGERESLICEFILTER1_H
