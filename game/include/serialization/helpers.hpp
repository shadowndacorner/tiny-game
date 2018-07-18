#pragma once

namespace tiny
{
	namespace serialization
	{
		template <typename A, typename N, typename T>
		inline bool write_safe(A& archive, const N& name, T& object)
		{
			try
			{
				archive(cereal::make_nvp(name, object));
			}
			catch (cereal::Exception& ex)
			{
				return false;
			}
			return true;
		}

		template <typename A, typename N, typename T, typename F>
		inline bool write_safe(A& archive, const N& name, T& object, F func)
		{
			try
			{
				archive(cereal::make_nvp(name, object));
			}
			catch (cereal::Exception& ex)
			{
				object = func();
				return false;
			}
			return true;
		}
	}
}

#define SERIALIZE_METHOD(type) template<typename Archive>\
inline void serialize(Archive& archive, type& object)

#define WRITE_VAR(value) tiny::serialization::write_safe(archive, #value, object.value)
#define WRITE_VAR_WITH_DEFAULT_FUNC(value, def) tiny::serialization::write_safe(archive, #value, object.value, def)
#define WRITE_REQUIREFD_VAR(value) if (!WRITE_VAR(value)){\
std::cerr << "Failed to write value " << value << " to archive" << std::endl \
return; }