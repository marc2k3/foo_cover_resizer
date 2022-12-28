#pragma once

extern wil::com_ptr_nothrow<IWICImagingFactory> g_imaging_factory;

namespace Component
{
	static constexpr const char* name = "Cover Resizer";
	static constexpr const char* wic_error = "WIC initialisation failure. If running Windows 7, please make sure it is fully updated.";

	static constexpr uint32_t threaded_process_flags = threaded_process::flag_show_progress | threaded_process::flag_show_delayed | threaded_process::flag_show_item | threaded_process::flag_show_abort;
}

namespace guids
{
	static constexpr GUID context_group_resize = { 0x21ad11b8, 0xf695, 0x4b31, { 0x84, 0x54, 0x18, 0x72, 0x40, 0xab, 0x79, 0xac } };
	static constexpr GUID context_group_utils = { 0x7abaf27f, 0xdffe, 0x4b62, { 0x85, 0x24, 0xe1, 0xb4, 0x4f, 0x3d, 0x77, 0x8a } };

	static constexpr GUID context_command_resize = { 0xf1abb39b, 0x9036, 0x43d5, { 0x9a, 0xff, 0xec, 0x37, 0xa5, 0x3a, 0x5f, 0x8e } };
	static constexpr GUID context_command_resize_and_attach = { 0xa2bfe876, 0x9a94, 0x4dde, { 0xa0, 0x5e, 0x57, 0xca, 0x63, 0x96, 0xab, 0x51 } };

	static constexpr GUID context_command_convert = { 0xd5776a5e, 0x1b53, 0x4e8d, { 0xa6, 0x16, 0x35, 0xd9, 0x7e, 0x9e, 0x86, 0x78 } };
	static constexpr GUID context_command_convert_and_attach = { 0xfd0d9234, 0xd8ee, 0x4452, { 0x85, 0x2f, 0x42, 0x3f, 0x38, 0x1a, 0x13, 0xcf } };
	static constexpr GUID context_command_remove_all_except_front = { 0xc157f1fe, 0xf264, 0x4ffc, { 0x86, 0xaf, 0xb1, 0x70, 0x46, 0xa4, 0x9e, 0xad } };

	static constexpr GUID cfg_type = { 0x196dbedf, 0x7d74, 0x4c7c, { 0xa3, 0xc0, 0x94, 0xcb, 0xf3, 0x63, 0xd5, 0x19 } };
	static constexpr GUID cfg_format = { 0xacf066f8, 0xbcbc, 0x4083, { 0x8c, 0x8, 0xca, 0x83, 0xc5, 0x53, 0x29, 0x43 } };
	static constexpr GUID cfg_size = { 0x696f0d7, 0x4f25, 0x4723, { 0xb2, 0x29, 0x2d, 0xdd, 0xa9, 0x4f, 0xb1, 0x5e } };
	static constexpr GUID cfg_quality = { 0x8c0194ec, 0xd0fd, 0x476c, { 0xa0, 0xa5, 0xc9, 0xcf, 0xb1, 0x17, 0x83, 0x40 } };

	static constexpr GUID foo_cover_info_command = { 0x1fac520f, 0x63e7, 0x46e4, { 0xb8, 0x22, 0x50, 0x7c, 0x92, 0x7b, 0x53, 0x37 } };
}

namespace settings
{
	extern cfg_int type;
	extern cfg_int format;
	extern cfg_int size;
	extern cfg_int quality;
}

enum class Format
{
	JPG,
	WEBP,
};

struct Formats
{
	const Format id;
	const std::wstring label;
};

static const std::vector<Formats> formats =
{
	{ Format::JPG, L"JPG" },
	{ Format::WEBP, L"WEBP" },
};

struct ContextItem
{
	const GUID* guid;
	const pfc::string8 name;
};
