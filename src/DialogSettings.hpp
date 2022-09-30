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

			const size_t count = album_art_ids::num_types();
			for (size_t i = 0; i < count; ++i)
			{
				m_combo_type.AddString(pfc::wideFromUTF8(album_art_ids::query_capitalized_name(i)));
			}

			for (const auto& [id, label] : formats)
			{
				m_combo_format.AddString(label.data());
			}

			m_combo_type.SetCurSel(static_cast<int>(settings::type));
			m_combo_format.SetCurSel(static_cast<int>(settings::format));

			pfc::setWindowText(GetDlgItem(IDC_EDIT_QUALITY), pfc::format_int(settings::quality));

			if (m_show_size)
			{
				pfc::setWindowText(GetDlgItem(IDC_EDIT_SIZE), pfc::format_int(settings::size));
			}
			else
			{
				GetDlgItem(IDC_LABEL_SIZE).ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SIZE).ShowWindow(SW_HIDE);
			}

			m_hooks.AddDialogWithControls(*this);
			CenterWindow();
			return TRUE;
		}

		int GetQuality()
		{
			return GetDlgItemInt(IDC_EDIT_QUALITY);
		}

		int GetSize()
		{
			return GetDlgItemInt(IDC_EDIT_SIZE);
		}

		void OnCloseCmd(UINT, int nID, CWindow)
		{
			if (nID == IDOK)
			{
				settings::type = m_combo_type.GetCurSel();
				settings::format = m_combo_format.GetCurSel();
				settings::quality = GetQuality();

				if (m_show_size)
				{
					settings::size = GetSize();
				}
			}

			EndDialog(nID);
		}

		void OnUpdate(UINT, int, CWindow)
		{
			m_button_ok.EnableWindow(GetDlgItem(IDC_EDIT_QUALITY).GetWindowTextLengthW() > 0 && GetQuality() <= 100 && (!m_show_size || GetSize() >= 200));
		}

		CButton m_button_ok;
		CComboBox m_combo_format, m_combo_type;
		bool m_show_size{};
		fb2k::CCoreDarkModeHooks m_hooks;
	};
}
