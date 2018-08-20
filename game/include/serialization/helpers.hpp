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

#define SERIALIZE_VAR(value) tiny::serialization::write_safe(archive, #value, object.value)
#define SERIALIZE_NAMED_VAR(name, value) tiny::serialization::write_safe(archive, name, object.value)
#define SERIALIZE_VAR_WITH_DEFAULT_FUNC(value, def) tiny::serialization::write_safe(archive, #value, object.value, def)
#define SERIALIZE_NAMED_VAR_WITH_DEFAULT_FUNC(name, value, def) tiny::serialization::write_safe(archive, name, object.value, def)
#define SERIALIZE_REQUIRED_VAR(value) if (!SERIALIZE_VAR(value)){\
std::cerr << "Failed to serialize value " << value << " to archive" << std::endl \
return; }

#define SERIALIZE_REQUIRED_NAMED_VAR(value) if (!SERIALIZE_NAMED_VAR(name, value)){\
std::cerr << "Failed to serialize value " << value << " to archive" << std::endl \
return; }