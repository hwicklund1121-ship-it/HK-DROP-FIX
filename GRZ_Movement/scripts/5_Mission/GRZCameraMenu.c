// Reuse DayZ's shipped settings rows and OptionSelectorSlider. Only preference
// handling and label text are GRZ code; no custom font/color/slider drawing.
class GRZCameraSettingsPanel extends ScriptedWidgetEventHandler
{
    private OptionsMenu m_Menu;
    private ref OptionSelectorSlider m_Fov;
    private ref OptionSelectorSlider m_View;
    private ref OptionSelectorSlider m_Objects;
    private float m_SavedFov;
    private float m_SavedView;
    private float m_SavedObjects;
    private bool m_Ready;

    static TextWidget FindText(Widget root, Widget excluded)
    {
        if (!root || root == excluded)
            return null;
        TextWidget text = TextWidget.Cast(root);
        if (text)
            return text;
        Widget child = root.GetChildren();
        while (child)
        {
            TextWidget found = FindText(child, excluded);
            if (found)
                return found;
            child = child.GetSibling();
        }
        return null;
    }

    private Widget CreateNativeOption(string layout, Widget parent, int order, string label, out Widget row)
    {
        // Instantiate the native layout as a temporary rowTemplate, move only its
        // unmodified FOV row into our category, then discard the unused rowTemplate.
        Widget rowTemplate = GetGame().GetWorkspace().CreateWidgets(layout);
        if (!rowTemplate)
            return null;
        rowTemplate.Show(false);
        Widget option = rowTemplate.FindAnyWidget("fov_setting_option");
        if (!option || !option.GetParent())
        {
            rowTemplate.Unlink();
            return null;
        }
        row = option.GetParent();
        TextWidget nativeLabel = FindText(row, option);
        if (nativeLabel)
            nativeLabel.SetText(label);
        row.GetParent().RemoveChild(row);
        parent.AddChild(row);
        row.SetSort(order);
        row.Show(true);
        option.SetUserID(-1);
        rowTemplate.Unlink();
        return option;
    }

    void GRZCameraSettingsPanel(Widget nativeFov, string nativeLayout, OptionsMenu menu)
    {
        Widget fovRow = nativeFov.GetParent();
        Widget rows = fovRow.GetParent();
        Widget viewRow, objectsRow;
        Widget viewOption = CreateNativeOption(nativeLayout, rows, fovRow.GetSort() + 1, "VIEW DISTANCE", viewRow);
        Widget objectsOption = CreateNativeOption(nativeLayout, rows, fovRow.GetSort() + 2, "OBJECT VIEW DISTANCE", objectsRow);
        if (!viewOption || !objectsOption)
        {
            if (viewRow)
                viewRow.Unlink();
            if (objectsRow)
                objectsRow.Unlink();
            Print("[GRZ SETTINGS] Native row rowTemplate unavailable; original controls retained.");
            return;
        }
        // Retain the vanilla selector's widgets for its normal revert/destructor
        // lifecycle, but hide them before adding the enhanced native selector.
        Widget oldSlider = nativeFov.GetChildren();
        while (oldSlider)
        {
            oldSlider.Show(false);
            oldSlider = oldSlider.GetSibling();
        }
        GRZCameraPreferences saved = GRZCameraPreferences.Current();
        m_Fov = new OptionSelectorSlider(nativeFov, Math.Clamp(GetDayZGame().GetUserFOV(), 0.9, 2.0), this, false, 0.9, 2.0);
        m_View = new OptionSelectorSlider(viewOption, saved.ViewDistance, this, false, 500, 5000);
        m_Objects = new OptionSelectorSlider(objectsOption, saved.ObjectDistance, this, false, 500, 5000);
        m_Fov.SetStep(0.001);
        m_View.SetStep(50.0 / 4500.0);
        m_Objects.SetStep(50.0 / 4500.0);
        m_Fov.m_OptionChanged.Insert(OnValueChanged);
        m_View.m_OptionChanged.Insert(OnValueChanged);
        m_Objects.m_OptionChanged.Insert(OnValueChanged);
        m_Menu = menu;
        m_Ready = true;
        CaptureSavedValues();
        rows.Update();
    }

    bool IsReady()
    {
        return m_Ready;
    }

    private void OnValueChanged(float value)
    {
        if (!m_Ready)
            return;
        if (m_Objects.GetValue() > m_View.GetValue())
            m_Objects.SetValue(m_View.GetValue(), false);
        m_Menu.OnChanged();
    }

    private void CaptureSavedValues()
    {
        m_SavedFov = m_Fov.GetValue();
        m_SavedView = m_View.GetValue();
        m_SavedObjects = m_Objects.GetValue();
    }

    bool IsChanged()
    {
        if (!m_Ready)
            return false;
        return Math.AbsFloat(m_Fov.GetValue() - m_SavedFov) > 0.0001 || Math.AbsFloat(m_View.GetValue() - m_SavedView) > 0.1 || Math.AbsFloat(m_Objects.GetValue() - m_SavedObjects) > 0.1;
    }

    void Save()
    {
        if (!IsChanged())
            return;
        GRZCameraPreferences preferences = new GRZCameraPreferences();
        preferences.Enabled = true;
        preferences.FieldOfView = m_Fov.GetValue();
        preferences.ViewDistance = m_View.GetValue();
        preferences.ObjectDistance = m_Objects.GetValue();
        string error;
        if (!GRZCameraPreferences.SaveAndApply(preferences, error))
        {
            Print("[GRZ SETTINGS] Could not save preferences: " + error);
            GetGame().GetUIManager().ShowDialog("GUNRUNNERZ FOV", "Could not save settings. Check your DayZ profile folder.", 973195, DBT_OK, DBB_OK, DMT_INFO, m_Menu);
            return;
        }
        CaptureSavedValues();
    }

    void Revert()
    {
        if (!m_Ready)
            return;
        m_Fov.SetValue(m_SavedFov, false);
        m_View.SetValue(m_SavedView, false);
        m_Objects.SetValue(m_SavedObjects, false);
    }

    void Defaults()
    {
        if (!m_Ready)
            return;
        m_Fov.SetValue(1.0, false);
        m_View.SetValue(2000, false);
        m_Objects.SetValue(2000, false);
    }
}

modded class OptionsMenuGame
{
    private ref GRZCameraSettingsPanel m_GRZCameraPanel;

    void OptionsMenuGame(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
    {
        GRZCameraPreferences.Load();
        Widget nativeFov = m_Root.FindAnyWidget("fov_setting_option");
        Widget settingsRoot = m_Root.FindAnyWidget("game_settings_root");
        if (!nativeFov || !nativeFov.GetParent() || !nativeFov.GetParent().GetParent() || !settingsRoot)
            return;
        Widget cameraRow = nativeFov.GetParent();
        Widget category = cameraRow;
        while (category.GetParent() && category.GetParent() != settingsRoot && category.GetParent() != m_Root)
        {
            Widget next = category.GetParent();
            if (next.FindAnyWidget("language_setting_option") || next.FindAnyWidget("hud_setting_option"))
                break;
            category = next;
        }
        TextWidget heading = GRZCameraSettingsPanel.FindText(category, cameraRow);
        float rowWidth, rowHeight, width, height;
        cameraRow.GetScreenSize(rowWidth, rowHeight);
        category.GetScreenSize(width, height);
        m_GRZCameraPanel = new GRZCameraSettingsPanel(nativeFov, GetLayoutName(), menu);
        if (!m_GRZCameraPanel.IsReady())
            return;
        if (heading)
            heading.SetText("GUNRUNNERZ FOV");
        // Add exactly two native row heights, preserving the native header and
        // existing margins instead of introducing a custom section or gap.
        if (rowHeight > 0)
            category.SetScreenSize(width, height + 2 * rowHeight);
        category.Update();
        settingsRoot.Update();
    }

    override bool IsChanged()
    {
        return super.IsChanged() || (m_GRZCameraPanel && m_GRZCameraPanel.IsChanged());
    }

    override void Revert()
    {
        super.Revert();
        if (m_GRZCameraPanel)
            m_GRZCameraPanel.Revert();
    }

    override void SetToDefaults()
    {
        super.SetToDefaults();
        if (m_GRZCameraPanel)
            m_GRZCameraPanel.Defaults();
    }

    void GRZSaveCameraPreferences()
    {
        if (m_GRZCameraPanel)
            m_GRZCameraPanel.Save();
    }
}

modded class OptionsMenu
{
    override void Apply()
    {
        super.Apply();
        m_GameTab.GRZSaveCameraPreferences();
        GRZCameraPreferences.ApplyDistances();
        OnChanged();
    }
}

modded class MissionGameplay
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        GRZCameraPreferences.Load();
        GRZCameraPreferences.ApplyDistances();
    }
}
