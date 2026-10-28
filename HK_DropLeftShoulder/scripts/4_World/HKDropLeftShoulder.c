// Optional camera-only overlay for the working back-rise clip test.
modded class PlayerBase
{
    private int m_HKLeftDropSourceTag = -1;
    private int m_HKLeftDropCancelTag = -1;
    private float m_HKLeftDropTail;
    private bool m_HKLeftDropWasActive;

    override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
    {
        bool handled = super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
        if (handled)
            return handled;
        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return handled;

        HumanInputController input = GetInputController();
        if (!input)
            return handled;
        if (!IsAlive() || pCurrentCommandID != DayZPlayerConstants.COMMANDID_MOVE || pCurrentCommandFinished)
        {
            m_HKLeftDropTail = 0;
            m_HKLeftDropWasActive = false;
            return handled;
        }

        HumanAnimInterface anim = GetAnimInterface();
        if (!anim)
            return handled;
        if (m_HKLeftDropSourceTag < 0)
            m_HKLeftDropSourceTag = anim.BindTag("TagHKBackCancelSelected");
        if (m_HKLeftDropCancelTag < 0)
            m_HKLeftDropCancelTag = anim.BindTag("TagThrowingCancel");

        // The source tag can linger after blend-out. Require the cancel tag
        // as well, then cover its short outgoing blend with a bounded tail.
        bool cancelActive = false;
        if (m_HKLeftDropSourceTag >= 0 && m_HKLeftDropCancelTag >= 0)
            cancelActive = anim.IsTag(m_HKLeftDropSourceTag) && anim.IsTag(m_HKLeftDropCancelTag);
        if (cancelActive)
            m_HKLeftDropTail = 0.3;
        else
            m_HKLeftDropTail = Math.Max(0.0, m_HKLeftDropTail - pDt);

        bool active = m_HKLeftDropTail > 0;
        if (active)
        {
            // False selects the left shoulder. ONE_FRAME expires by itself;
            // the underlying user's shoulder choice is not rewritten.
            input.Override3rdIsRightShoulder(HumanInputControllerOverrideType.ONE_FRAME, false);
        }
        if (active != m_HKLeftDropWasActive)
            Print("[HKLEFTDROP] active=" + active.ToString());
        m_HKLeftDropWasActive = active;
        return handled;
    }
};
