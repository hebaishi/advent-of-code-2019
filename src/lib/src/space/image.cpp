#include <space/image.hpp>

#include <vector>
#include <iostream>

constexpr const char Transparent = '2';
constexpr const char Black = '0';
constexpr const char White = '1';


namespace space {

Image::Image(const std::string& data, size_t width, size_t height) : data_(data), width_(width), height_(height) {}

std::string_view Image::GetLayer(size_t index) {
  auto layer = std::string_view(data_);
  size_t pixels_per_layer = width_ * height_;
  return layer.substr(pixels_per_layer * index, pixels_per_layer);
}

size_t Image::GetNumLayers() {
  return data_.length() / (width_ * height_);
}

std::string Image::Render() {
  std::vector<std::string_view> layers;
  for (size_t index = 0 ; index < GetNumLayers() ; index++)
    layers.push_back(GetLayer(index));


  std::string output;
  for (size_t index = 0 ; index < (width_* height_) ; index++) {
    char current_pixel = layers[0][index];
    for (const auto& layer : layers) {
      if (current_pixel == Transparent) {
        current_pixel = layer[index];
      }
    }
    output.push_back(current_pixel);
  }

  return output;
}

void Image::Print(const std::string& image) {
  std::string_view image_view(image);
  for (size_t index = 0 ; index < (image.length() / width_) ; index++) {
    std::cout << image_view.substr(index * width_, width_);
    std::cout << std::endl;
  }

}

}
