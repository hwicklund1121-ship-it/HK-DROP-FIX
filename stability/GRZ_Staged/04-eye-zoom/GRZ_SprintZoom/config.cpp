class CfgPatches
{
    class GRZ_SprintZoom
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "GRZ_Movement"};
    };
};
class CfgMods
{
    class GRZ_SprintZoom
    {
        dir = "GRZ_SprintZoom";
        name = "GRZ_SprintZoom - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_SprintZoom/scripts/4_World"};
            };
        };
    };
};
