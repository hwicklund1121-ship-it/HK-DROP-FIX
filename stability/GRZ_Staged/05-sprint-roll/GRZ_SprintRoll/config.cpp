class CfgPatches
{
    class GRZ_SprintRoll
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "GRZ_Movement"};
    };
};
class CfgMods
{
    class GRZ_SprintRoll
    {
        dir = "GRZ_SprintRoll";
        name = "GRZ_SprintRoll - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_SprintRoll/scripts/4_World"};
            };
        };
    };
};
