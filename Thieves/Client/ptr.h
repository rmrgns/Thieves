#pragma once
#include <memory>

namespace client_fw
{
	//CreateUPtr 템플릿 함수는 std::unique_ptr를 만드는 래퍼 함수
	template<class T>
	using UPtr = std::unique_ptr<T>;
	template<class T, class ... Args>
	constexpr UPtr<T> CreateUPtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	//CreateSPtr 템플릿 함수는 std::shared_ptr를 만드는 래퍼 함수
	template<typename T>
	using SPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SPtr<T> CreateSPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WPtr = std::weak_ptr<T>;
	//ToUnderlying 템플릿 함수는 C++11에서 추가된 열거형 타입의 내부 값(underlying value)을 가져오는 함수
	template<typename Enum>
	constexpr typename std::underlying_type<Enum>::type ToUnderlying(Enum e) noexcept
	{
		return static_cast<typename std::underlying_type<Enum>::type>(e);
	}
}

//문자열을 받아들여서, 해당 문자열을 해시코드(Hash code)로 변환해주는 함수
constexpr unsigned int HashCode(const char* str)
{
	return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * HashCode(str + 1) : 8603;
}