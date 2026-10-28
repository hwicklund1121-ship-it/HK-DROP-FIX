// Keep the view steady while the character leans beneath it. Native shoulder
// switching, aiming, stance transitions and player movement remain unchanged.
modded class DayZPlayerCamera3rdPerson
{
    void DayZPlayerCamera3rdPerson(DayZPlayer pPlayer, HumanInputController pInput)
    {
        m_fLeanDistance = 0.0;
    }
}
