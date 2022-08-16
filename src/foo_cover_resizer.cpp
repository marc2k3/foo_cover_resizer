#include "stdafx.hpp"

#pragma comment(lib, "Windowscodecs.lib")

wil::com_ptr_nothrow<IWICImagingFactory> g_imaging_factory;

namespace resizer
{
	DECLARE_COMPONENT_VERSION(
		component_name,
		"1.0.0",
		"Copyright (C) 2022 marc2003\n\n"
		"Build: " __TIME__ ", " __DATE__
	);

	class InstallationValidator : public component_installation_validator
	{
	public:
		bool is_installed_correctly() override
		{
			return test_my_name("foo_cover_resizer.dll") && core_version_info_v2::get()->test_version(2, 0, 0, 0);
		}
	};

	FB2K_SERVICE_FACTORY(InstallationValidator);

	namespace settings
	{
		cfg_int type(guid_cfg_type, 0);
		cfg_int format(guid_cfg_format, 0);
		cfg_int size(guid_cfg_size, 500);
	}

	class InitQuit : public initquit
	{
	public:
		void on_init() override
		{
			g_imaging_factory = wil::CoCreateInstanceNoThrow<IWICImagingFactory>(CLSID_WICImagingFactory);
		}

		void on_quit() override
		{
			g_imaging_factory.reset();
		}
	};

	FB2K_SERVICE_FACTORY(InitQuit);
}
