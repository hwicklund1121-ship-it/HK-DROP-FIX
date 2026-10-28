// Original GRZ panel embedded in the native Settings > Game tab.
class GRZCameraSettingsPanel extends ScriptedWidgetEventHandler
{
    private Widget m_Root;
    private OptionsMenu m_Menu;
    private float m_SavedFov;
    private float m_SavedView;
    private float m_SavedObjects;
    private SliderWidget m_Fov;
    private SliderWidget m_View;
    private SliderWidget m_Objects;
    private TextWidget m_FovValue;
    private TextWidget m_ViewValue;
    private TextWidget m_ObjectsValue;
    private TextWidget m_Status;

    void GRZCameraSettingsPanel(Widget parent, OptionsMenu menu)
    {
        m_Root = GetGame().GetWorkspace().CreateWidgets("GRZ_Movement/gui/layouts/grz_camera.layout", parent);
        m_Fov = SliderWidget.Cast(m_Root.FindAnyWidget("GRZFov"));
        m_View = SliderWidget.Cast(m_Root.FindAnyWidget("GRZView"));
        m_Objects = SliderWidget.Cast(m_Root.FindAnyWidget("GRZObjects"));
        m_FovValue = TextWidget.Cast(m_Root.FindAnyWidget("GRZFovValue"));
        m_ViewValue = TextWidget.Cast(m_Root.FindAnyWidget("GRZViewValue"));
        m_ObjectsValue = TextWidget.Cast(m_Root.FindAnyWidget("GRZObjectsValue"));
        m_Status = TextWidget.Cast(m_Root.FindAnyWidget("GRZStatus"));
        m_Fov.SetMinMax(0.9, 2.0);
        m_Fov.SetStep(0.001);
        m_View.SetMinMax(500, 5000);
        m_View.SetStep(50);
        m_Objects.SetMinMax(500, 5000);
        m_Objects.SetStep(50);
        GRZCameraPreferences saved = GRZCameraPreferences.Current();
        m_Fov.SetCurrent(Math.Clamp(GetDayZGame().GetUserFOV(), 0.9, 2.0));
        m_View.SetCurrent(saved.ViewDistance);
        m_Objects.SetCurrent(saved.ObjectDistance);
        RefreshValues();
        m_Root.SetHandler(this);
        m_Menu = menu;
        CaptureSavedValues();
    }

    private void RefreshValues()
    {
        // Objects cannot render farther than the overall view distance.
        if (m_Objects.GetCurrent() > m_View.GetCurrent())
            m_Objects.SetCurrent(m_View.GetCurrent());
        m_FovValue.SetText(m_Fov.GetCurrent().ToString());
        m_ViewValue.SetText(Math.Round(m_View.GetCurrent()).ToString() + " m");
        m_ObjectsValue.SetText(Math.Round(m_Objects.GetCurrent()).ToString() + " m");
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w == m_Fov || w == m_View || w == m_Objects)
        {
            RefreshValues();
            if (m_Menu)
                m_Menu.OnChanged();
            return true;
        }
        return super.OnChange(w, x, y, finished);
    }


    private void CaptureSavedValues()
    {
        m_SavedFov = m_Fov.GetCurrent();
        m_SavedView = m_View.GetCurrent();
        m_SavedObjects = m_Objects.GetCurrent();
    }

    bool IsChanged()
    {
        return Math.AbsFloat(m_Fov.GetCurrent() - m_SavedFov) > 0.0001 || Math.AbsFloat(m_View.GetCurrent() - m_SavedView) > 0.1 || Math.AbsFloat(m_Objects.GetCurrent() - m_SavedObjects) > 0.1;
    }

    bool Save()
    {
        if (!IsChanged())
            return true;
        GRZCameraPreferences preferences = new GRZCameraPreferences();
        preferences.Enabled = true;
        preferences.FieldOfView = m_Fov.GetCurrent();
        preferences.ViewDistance = m_View.GetCurrent();
        preferences.ObjectDistance = m_Objects.GetCurrent();
        string error;
        if (!GRZCameraPreferences.SaveAndApply(preferences, error))
        {
            m_Status.SetText("Could not save GRZ settings. Check your DayZ profile folder.");
            Print("[GRZ SETTINGS] " + error);
            return false;
        }
        CaptureSavedValues();
        m_Status.SetText("GRZ camera settings saved.");
        return true;
    }

    void Revert()
    {
        m_Fov.SetCurrent(m_SavedFov);
        m_View.SetCurrent(m_SavedView);
        m_Objects.SetCurrent(m_SavedObjects);
        RefreshValues();
    }

    void Defaults()
    {
        m_Fov.SetCurrent(1.0);
        m_View.SetCurrent(2000);
        m_Objects.SetCurrent(2000);
        RefreshValues();
    }
}

modded class OptionsMenuGame
{
    private ref GRZCameraSettingsPanel m_GRZCameraPanel;

    void OptionsMenuGame(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
    {
        GRZCameraPreferences.Load();
        Widget settings = m_Root.FindAnyWidget("game_settings_root");
        if (settings)
        {
            m_GRZCameraPanel = new GRZCameraSettingsPanel(settings, menu);
            settings.Update();
            // GRZ's enhanced slider replaces the standard clamped FOV control.
            Widget nativeFov = m_Root.FindAnyWidget("fov_setting_option");
            if (nativeFov)
                nativeFov.Enable(false);
        }
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
        // Native options may set render distances; apply GRZ after them.
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
