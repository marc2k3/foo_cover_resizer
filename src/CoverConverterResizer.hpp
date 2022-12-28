#pragma once

class CoverConverterResizer : public threaded_process_callback
{
public:
	enum class Action
	{
		Resize,
		Convert,
	};

	CoverConverterResizer(Action action, metadb_handle_list_cref handles, Format format, const GUID& art_guid);

	void on_done(HWND, bool was_aborted) final;
	void run(threaded_process_status& status, abort_callback& abort) final;

private:
	Action m_action{};
	Format m_format{};
	GUID m_art_guid{};
	metadb_handle_list m_handles;
};
