class CfgPatches
{
    class HK_CrouchHandsGuard
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts"};
    };
};

class CfgMods
{
    class HK_CrouchHandsGuard
    {
        dir = "HK_CrouchHandsGuard";
        name = "HK Crouch Hands Guard (experimental)";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"HK_CrouchHandsGuard/scripts/4_World"};
            };
        };
    };
};
