#include <string>
#include <string_view>

namespace space {

class Image {
public:

  Image(const std::string& data, size_t width, size_t height);

  std::string_view GetLayer(size_t index);

  size_t GetNumLayers();

  std::string Render();

  void Print(const std::string& image);
private:
  std::string data_;
  size_t width_, height_;
};

}
