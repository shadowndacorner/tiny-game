#include <graphics/sprite.hpp>

using namespace tiny::graphics;
tiny::graphics::Sprite::Sprite()
{
}

Sprite::Sprite(const ImageHandle& image, const glm::vec4& bounds) :
	uv_bounds(bounds),
	image(image)
{

}

Sprite::Sprite(const Sprite& rhs) :
	uv_bounds(rhs.uv_bounds),
	image(rhs.image)
{

}

Sprite::Sprite(Sprite&& rhs) :
	uv_bounds(std::move(rhs.uv_bounds)),
	image(std::move(rhs.image))
{

}

Sprite & tiny::graphics::Sprite::operator=(const Sprite & rhs)
{
	uv_bounds = rhs.uv_bounds;
	image = rhs.image;
	return *this;
}

Sprite & tiny::graphics::Sprite::operator=(Sprite && rhs)
{
	uv_bounds = std::move(rhs.uv_bounds);
	image = std::move(rhs.image);
	return *this;
}
