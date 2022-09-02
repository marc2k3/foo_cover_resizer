#pragma once

namespace resizer
{
	class CDialogSettings : public CDialogImpl<CDialogSettings>
	{
	public:
		CDialogSettings(bool show_size) : m_show_size(show_size) {}

		BEGIN_MSG_MAP_EX(CDialogSettings)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
			COMMAND_CODE_HANDLER_EX(EN_UPDATE, OnUpdate)
		END_MSG_MAP()

		enum { IDD = IDD_DIALOG_SETTINGS };

		BOOL OnInitDialog(CWindow, LPARAM)
		{
			m_button_ok = GetDlgItem(IDOK);
			m_combo_type = GetDlgItem(IDC_COMBO_TYPE);
			m_combo_format = GetDlgItem(IDC_COMBO_FORMAT);
			m_edit_size = GetDlgItem(IDC_EDIT_SIZE);

			const size_t count = album_art_ids::num_types();
			for (size_t i = 0; i < count; ++i)
			{
				m_combo_type.AddString(string_wide_from_utf8_fast(album_art_ids::query_capitalized_name(i)));
			}

			for (const auto& [id, label] : formats)
			{
				m_combo_format.AddString(label.data());
			}

			m_combo_type.SetCurSel(static_cast<int>(settings::type));
			m_combo_format.SetCurSel(static_cast<int>(settings::format));

			if (m_show_size)
			{
				pfc::setWindowText(m_edit_size, pfc::format_int(settings::size));
			}
			else
			{
				GetDlgItem(IDC_LABEL_SIZE).ShowWindow(SW_HIDE);
				m_edit_size.ShowWindow(SW_HIDE);
			}

			m_hooks.AddDialogWithControls(*this);
			CenterWindow();
			return TRUE;
		}

		int GetSize()
		{
			pfc::string8 str = pfc::getWindowText(m_edit_size);
			return pfc::atoui_ex(str, str.get_length());
		}

		void OnCloseCmd(UINT, int nID, CWindow)
		{
			if (nID == IDOK)
			{
				settings::type = m_combo_type.GetCurSel();
				settings::format = m_combo_format.GetCurSel();

				if (m_show_size)
				{
					settings::size = GetSize();
				}
			}

			EndDialog(nID);
		}

		void OnUpdate(UINT, int, CWindow)
		{
			m_button_ok.EnableWindow(GetSize() >= 200);
		}

		CButton m_button_ok;
		CComboBox m_combo_format, m_combo_type;
		CEdit m_edit_size;
		bool m_show_size{};
		fb2k::CCoreDarkModeHooks m_hooks;
	};
}
