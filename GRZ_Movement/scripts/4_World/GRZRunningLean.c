// Preserve inherited sprint-start handling, then restore sprint lean inputs.
modded class DayZPlayerImplement
{
    override void OnSprintStart()
    {
        super.OnSprintStart();

        if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
        {
            GetUApi().GetInputByID(UALeanLeft).Unlock();
            GetUApi().GetInputByID(UALeanRight).Unlock();
        }
    }
}
