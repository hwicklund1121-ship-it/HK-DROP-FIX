// Client-only sprint roll correction. No bone index or camera selection changes.
#ifndef SERVER
modded class DayZPlayerCamera3rdPersonErcSpr
{
    override float GetCurrentRoll()
    {
        return 0.0;
    }
}
#endif
