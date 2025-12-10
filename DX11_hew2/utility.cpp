#include	<filesystem>
#include	<string>
#include	<Windows.h>

namespace utility {
    // std::string 用のディレクトリ取得関数
    std::filesystem::path get_directory(const std::string& path) {
        return std::filesystem::path(path).parent_path();
    }

    // std::u8string 用のディレクトリ取得関数
    std::filesystem::path get_directory(const std::u8string& path) {
        return std::filesystem::path(path).parent_path();
    }

    // std::wstring 用のディレクトリ取得関数
    std::filesystem::path get_directory(const std::wstring& path) {
        return std::filesystem::path(path).parent_path();
    }

	// ワイド文字(utf16)をｓ－ｊｉｓに
	std::string wide_to_multi_winapi(std::wstring const& src)
	{
		auto const dest_size = ::WideCharToMultiByte(
			CP_ACP,
			0U,
			src.data(),
			-1,
			nullptr,
			0,
			nullptr,
			nullptr);
		std::vector<char> dest(dest_size, '\0');
		if (::WideCharToMultiByte(
			CP_ACP,
			0U,
			src.data(),
			-1,
			dest.data(),
			static_cast<int>(dest.size()),
			nullptr,
			nullptr) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<char>::length(dest.data()));
		dest.shrink_to_fit();
		return std::string(dest.begin(), dest.end());
	}

	// utf-8をワイド文字(utf-16)に
	std::wstring utf8_to_wide_winapi(std::string const& src)
	{
		auto const dest_size = ::MultiByteToWideChar(
			CP_UTF8,			 // ソース側がUTF-8
			0U,
			src.data(),
			-1,
			nullptr,
			0U);
		std::vector<wchar_t> dest(dest_size, L'\0');
		if (::MultiByteToWideChar(CP_UTF8, 0U, src.data(), -1, dest.data(), static_cast<int>(dest.size())) == 0) {
			throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
		}
		dest.resize(std::char_traits<wchar_t>::length(dest.data()));
		dest.shrink_to_fit();
		return std::wstring(dest.begin(), dest.end());
	}

	// utf8をS-JISに
	std::string utf8_to_multi_winapi(std::string const& src)
	{
		auto const wide = utf8_to_wide_winapi(src);
		return wide_to_multi_winapi(wide);
	}
}

