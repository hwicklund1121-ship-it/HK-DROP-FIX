// Optional camera-only overlay for the working back-rise clip test.
modded class PlayerBase
{
    private int m_HKRightDropSourceTag = -1;
    private int m_HKRightDropCancelTag = -1;
    private float m_HKRightDropTail;
    private bool m_HKRightDropWasActive;

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
            m_HKRightDropTail = 0;
            m_HKRightDropWasActive = false;
            return handled;
        }

        HumanAnimInterface anim = GetAnimInterface();
        if (!anim)
            return handled;
        if (m_HKRightDropSourceTag < 0)
            m_HKRightDropSourceTag = anim.BindTag("TagHKBackCancelSelected");
        if (m_HKRightDropCancelTag < 0)
            m_HKRightDropCancelTag = anim.BindTag("TagThrowingCancel");

        // The source tag can linger after blend-out. Require the cancel tag
        // as well, then cover its short outgoing blend with a bounded tail.
        bool cancelActive = false;
        if (m_HKRightDropSourceTag >= 0 && m_HKRightDropCancelTag >= 0)
            cancelActive = anim.IsTag(m_HKRightDropSourceTag) && anim.IsTag(m_HKRightDropCancelTag);
        if (cancelActive)
            m_HKRightDropTail = 0.3;
        else
            m_HKRightDropTail = Math.Max(0.0, m_HKRightDropTail - pDt);

        bool active = m_HKRightDropTail > 0;
        if (active)
        {
            // True selects the right shoulder. ONE_FRAME expires by itself;
            // the underlying user's shoulder choice is not rewritten.
            input.Override3rdIsRightShoulder(HumanInputControllerOverrideType.ONE_FRAME, true);
        }
        if (active != m_HKRightDropWasActive)
            Print("[HKRIGHTDROP] active=" + active.ToString());
        m_HKRightDropWasActive = active;
        return handled;
    }
};
