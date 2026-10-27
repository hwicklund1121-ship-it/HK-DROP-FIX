// Experimental input-level workaround, not an animation-graph replacement.
modded class PlayerBase
{
    private ref HumanMovementState m_HKCrouchHandsState = new HumanMovementState();
    private int m_HKCrouchHandsPreviousStance = -1;
    private bool m_HKCrouchHandsWasChanging;
    private bool m_HKCrouchHandsCrouchTransition;
    private bool m_HKCrouchHandsWasBlocking;

    override bool ModCommandHandlerBefore(float dt, int commandID, bool commandFinished)
    {
        // Respect a preceding mod's request to handle this tick itself.
        bool handled = super.ModCommandHandlerBefore(dt, commandID, commandFinished);
        if (handled)
            return handled;

        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return handled;

        HumanCommandMove move = GetCommand_Move();
        if (commandID != DayZPlayerConstants.COMMANDID_MOVE || !move || commandFinished)
        {
            m_HKCrouchHandsPreviousStance = -1;
            m_HKCrouchHandsWasChanging = false;
            m_HKCrouchHandsCrouchTransition = false;
            HKCrouchHandsSetBlocking(false);
            return handled;
        }

        GetMovementState(m_HKCrouchHandsState);
        int stance = m_HKCrouchHandsState.m_iStanceIdx;
        bool changing = move.IsChangingStance();
        if (!changing)
            m_HKCrouchHandsCrouchTransition = false;
        else if (!m_HKCrouchHandsWasChanging)
            m_HKCrouchHandsCrouchTransition = HKCrouchHandsIsCrouched(stance) || HKCrouchHandsIsCrouched(m_HKCrouchHandsPreviousStance);

        m_HKCrouchHandsWasChanging = changing;
        m_HKCrouchHandsPreviousStance = stance;

        HumanInputController input = GetInputController();
        bool block = changing && m_HKCrouchHandsCrouchTransition && input;
        if (block)
        {
            // Do not interfere with items, attacks, actions, emotes or throwing.
            if (GetHumanInventory().GetEntityInHands() || input.IsAttackButton() || IsFighting())
                block = false;
            if (IsRestrained() || IsUnconscious() || !IsAlive() || IsInFBEmoteState())
                block = false;
            if (GetCommand_Action() || GetCommandModifier_Action())
                block = false;
            if (GetActionManager() && GetActionManager().GetRunningAction())
                block = false;
            DayZPlayerImplementThrowing throwing = GetThrowing();
            if (throwing && (throwing.IsThrowingModeEnabled() || throwing.IsThrowingAnimationPlaying()))
                block = false;
        }

        if (block)
        {
            if (!m_HKCrouchHandsWasBlocking)
                Print("[HKCROUCHHANDS] lower_request stance=" + stance.ToString() + " input_raised=" + input.IsWeaponRaised().ToString() + " graph_raised=" + m_HKCrouchHandsState.IsRaised().ToString());

            // ONE_FRAME expires automatically; never leave a persistent lock.
            // This briefly suppresses empty-handed guard raising during the
            // transition, including an intentional raise without an attack.
            input.OverrideRaise(HumanInputControllerOverrideType.ONE_FRAME, false);
        }
        HKCrouchHandsSetBlocking(block);
        return handled;
    }

    private bool HKCrouchHandsIsCrouched(int stance)
    {
        return stance == DayZPlayerConstants.STANCEIDX_CROUCH || stance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH;
    }

    private void HKCrouchHandsSetBlocking(bool blocking)
    {
        if (m_HKCrouchHandsWasBlocking && !blocking)
            Print("[HKCROUCHHANDS] lower_request_end");
        m_HKCrouchHandsWasBlocking = blocking;
    }
};
