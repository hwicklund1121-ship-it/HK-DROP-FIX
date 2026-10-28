class CfgPatches
{
    class GRZ_RunningLean
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "GRZ_Movement"};
    };
};
class CfgMods
{
    class GRZ_RunningLean
    {
        dir = "GRZ_RunningLean";
        name = "GRZ_RunningLean - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_RunningLean/scripts/4_World"};
            };
        };
    };
};
