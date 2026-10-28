// Mild crouched movement reduction. A one-frame input override expires without
// locking stance, slowing animation playback, or affecting standing/prone.
modded class PlayerBase
{
    override bool ModCommandHandlerBefore(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
    {
        bool handled = super.ModCommandHandlerBefore(pDt, pCurrentCommandID, pCurrentCommandFinished);
        if (handled || !IsAlive() || pCurrentCommandID != DayZPlayerConstants.COMMANDID_MOVE)
            return handled;
        HumanCommandMove movement = GetCommand_Move();
        if (!movement || movement.IsChangingStance() || movement.IsOnBack() || movement.IsStandingFromBack() || movement.IsInRoll() || IsFighting())
            return handled;
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_CROUCH && state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
            return handled;
        HumanInputController controls = GetInputController();
        if (!controls)
            return handled;
        float speed;
        vector direction;
        controls.GetMovement(speed, direction);
        if (speed > 0)
            controls.OverrideMovementSpeed(HumanInputControllerOverrideType.ONE_FRAME, speed * 0.95);
        return handled;
    }
}
