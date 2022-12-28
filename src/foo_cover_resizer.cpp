#include "stdafx.hpp"

#pragma comment(lib, "Windowscodecs.lib")

wil::com_ptr_nothrow<IWICImagingFactory> g_imaging_factory;

namespace settings
{
	cfg_int type(guids::cfg_type, 0);
	cfg_int format(guids::cfg_format, 0);
	cfg_int size(guids::cfg_size, 500);
	cfg_int quality(guids::cfg_quality, 95);
}

namespace
{
	DECLARE_COMPONENT_VERSION(
		Component::name,
		"1.0.4",
		"Copyright (C) 2022-2023 marc2003\n\n"
		"Build: " __TIME__ ", " __DATE__
	);

	VALIDATE_COMPONENT_FILENAME("foo_cover_resizer.dll");

	class InitQuit : public initquit
	{
	public:
		void on_init() final
		{
			g_imaging_factory = wil::CoCreateInstanceNoThrow<IWICImagingFactory>(CLSID_WICImagingFactory);
		}

		void on_quit() final
		{
			g_imaging_factory.reset();
		}
	};

	FB2K_SERVICE_FACTORY(InitQuit);
}
