class CfgPatches
{
    class HK_CrouchZoom
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts"};
    };
};

class CfgMods
{
    class HK_CrouchZoom
    {
        dir = "HK_CrouchZoom";
        name = "HK Crouch Eye Zoom";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"HK_CrouchZoom/scripts/4_World"};
            };
        };
    };
};
