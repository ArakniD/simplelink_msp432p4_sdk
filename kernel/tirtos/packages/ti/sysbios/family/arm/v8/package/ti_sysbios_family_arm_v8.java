/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-F14
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_sysbios_family_arm_v8
{
    static final String VERS = "@(#) xdc-F14\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> icfgs = new ArrayList<Object>();
    ArrayList<String> inherits = new ArrayList<String>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "ti.sysbios.interfaces");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.rov");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.arm.v8", new Value.Obj("ti.sysbios.family.arm.v8", pkgP));
    }

    void MPU$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8.MPU", new Value.Obj("ti.sysbios.family.arm.v8.MPU", po));
        pkgV.bind("MPU", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU$$RegionAttrsView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8.MPU.RegionAttrsView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU$$DeviceRegs", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8.MPU.DeviceRegs", new Proto.Str(spo, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.AccessPerm", new Proto.Enm("ti.sysbios.family.arm.v8.MPU.AccessPerm"));
        om.bind("ti.sysbios.family.arm.v8.MPU.Shareable", new Proto.Enm("ti.sysbios.family.arm.v8.MPU.Shareable"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU$$RegionAttrs", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8.MPU.RegionAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU$$RegionEntry", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8.MPU.RegionEntry", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8.MPU$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8.MPU.Module_State", new Proto.Str(spo, false));
    }

    void MPU$$CONSTS()
    {
        // module MPU
        om.bind("ti.sysbios.family.arm.v8.MPU.deviceRegs", new Extern("ti_sysbios_family_arm_v8_MPU_deviceRegs", "ti_sysbios_family_arm_v8_MPU_DeviceRegs*", false, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_PRIV", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_PRIV", xdc.services.intern.xsr.Enum.intValue(0x0L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_ANY", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_ANY", xdc.services.intern.xsr.Enum.intValue(0x1L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_PRIV", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_PRIV", xdc.services.intern.xsr.Enum.intValue(0x2L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_ANY", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_ANY", xdc.services.intern.xsr.Enum.intValue(0x3L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.Shareable_NONE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.Shareable_NONE", xdc.services.intern.xsr.Enum.intValue(0x0L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.Shareable_OUTER", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.Shareable_OUTER", xdc.services.intern.xsr.Enum.intValue(0x2L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.Shareable_INNER", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable", "ti.sysbios.family.arm.v8"), "ti.sysbios.family.arm.v8.MPU.Shareable_INNER", xdc.services.intern.xsr.Enum.intValue(0x3L)+0));
        om.bind("ti.sysbios.family.arm.v8.MPU.disable", new Extern("ti_sysbios_family_arm_v8_MPU_disable__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.enable", new Extern("ti_sysbios_family_arm_v8_MPU_enable__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.initRegionAttrs", new Extern("ti_sysbios_family_arm_v8_MPU_initRegionAttrs__E", "xdc_Void(*)(ti_sysbios_family_arm_v8_MPU_RegionAttrs*)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.isEnabled", new Extern("ti_sysbios_family_arm_v8_MPU_isEnabled__E", "xdc_Bool(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.setMAIR", new Extern("ti_sysbios_family_arm_v8_MPU_setMAIR__E", "xdc_Void(*)(xdc_UInt8,xdc_UInt8)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.setRegion", new Extern("ti_sysbios_family_arm_v8_MPU_setRegion__E", "xdc_Void(*)(xdc_UInt8,xdc_Ptr,xdc_Ptr,ti_sysbios_family_arm_v8_MPU_RegionAttrs*)", true, false));
        om.bind("ti.sysbios.family.arm.v8.MPU.startup", new Extern("ti_sysbios_family_arm_v8_MPU_startup__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void MPU$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void MPU$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn MPU.initRegionAttrsMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8.MPU$$initRegionAttrsMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8.MPU.Module", "ti.sysbios.family.arm.v8"), null, 1, 1, false));
                fxn.addArg(0, "regionAttrs", new Proto.Adr("ti_sysbios_family_arm_v8_MPU_RegionAttrs*", "PS"), $$UNDEF);
        // fxn MPU.setRegionMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8.MPU$$setRegionMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8.MPU.Module", "ti.sysbios.family.arm.v8"), null, 4, 4, false));
                fxn.addArg(0, "regionId", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF);
                fxn.addArg(1, "regionBaseAddr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF);
                fxn.addArg(2, "regionEndAddr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF);
                fxn.addArg(3, "attrs", (Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrs", "ti.sysbios.family.arm.v8"), $$DEFAULT);
    }

    void MPU$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8.MPU.DeviceRegs", "ti.sysbios.family.arm.v8");
        sizes.clear();
        sizes.add(Global.newArray("TYPE", "UInt32"));
        sizes.add(Global.newArray("CTRL", "UInt32"));
        sizes.add(Global.newArray("RNR", "UInt32"));
        sizes.add(Global.newArray("RBAR", "UInt32"));
        sizes.add(Global.newArray("RLAR", "UInt32"));
        sizes.add(Global.newArray("RBAR_A1", "UInt32"));
        sizes.add(Global.newArray("RLAR_A1", "UInt32"));
        sizes.add(Global.newArray("RBAR_A2", "UInt32"));
        sizes.add(Global.newArray("RLAR_A2", "UInt32"));
        sizes.add(Global.newArray("RBAR_A3", "UInt32"));
        sizes.add(Global.newArray("RLAR_A3", "UInt32"));
        sizes.add(Global.newArray("res0", "UInt32"));
        sizes.add(Global.newArray("MAIR0", "UInt32"));
        sizes.add(Global.newArray("MAIR1", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8.MPU.DeviceRegs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8.MPU.DeviceRegs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8.MPU.DeviceRegs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrs", "ti.sysbios.family.arm.v8");
        sizes.clear();
        sizes.add(Global.newArray("enable", "UShort"));
        sizes.add(Global.newArray("shareable", "Nti.sysbios.family.arm.v8.MPU.Shareable;;0x0;0x2;0x3"));
        sizes.add(Global.newArray("executable", "UShort"));
        sizes.add(Global.newArray("accessPerm", "Nti.sysbios.family.arm.v8.MPU.AccessPerm;;0x0;0x1;0x2;0x3"));
        sizes.add(Global.newArray("attrIndx", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionEntry", "ti.sysbios.family.arm.v8");
        sizes.clear();
        sizes.add(Global.newArray("rbar", "UInt32"));
        sizes.add(Global.newArray("rlar", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionEntry']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionEntry']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8.MPU.RegionEntry'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8.MPU.Module_State", "ti.sysbios.family.arm.v8");
        sizes.clear();
        sizes.add(Global.newArray("regionEntry", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8.MPU.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8.MPU.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8.MPU.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void MPU$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8/MPU.xs");
        om.bind("ti.sysbios.family.arm.v8.MPU$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU.Module", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.arm.v8"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.arm.v8"), $$UNDEF, "wh");
            po.addFld("defaultAttrs", (Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrs", "ti.sysbios.family.arm.v8"), Global.newObject("enable", true, "shareable", om.find("ti.sysbios.family.arm.v8.MPU.Shareable_NONE"), "executable", true, "accessPerm", om.find("ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_PRIV"), "attrIndx", 0L), "w");
            po.addFld("A_nullPointer", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8"), Global.newObject("msg", "A_nullPointer: Pointer is null"), "w");
            po.addFld("A_invalidRegionId", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8"), Global.newObject("msg", "A_invalidRegionId: MPU Region number passed is invalid."), "w");
            po.addFld("A_unalignedBaseAddr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8"), Global.newObject("msg", "A_unalignedBaseAddr: MPU region base address not aligned."), "w");
            po.addFld("A_unalignedEndAddr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8"), Global.newObject("msg", "A_unalignedEndAddr: MPU region end address not aligned."), "w");
            po.addFld("enableMPU", $$T_Bool, false, "w");
            po.addFld("enableBackgroundRegion", $$T_Bool, true, "w");
            po.addFld("MAIR0", Proto.Elm.newCNum("(xdc_UInt8)"), 0x00L, "w");
            po.addFld("MAIR1", Proto.Elm.newCNum("(xdc_UInt8)"), 0x04L, "w");
            po.addFld("MAIR2", Proto.Elm.newCNum("(xdc_UInt8)"), 0x08L, "w");
            po.addFld("MAIR3", Proto.Elm.newCNum("(xdc_UInt8)"), 0x0CL, "w");
            po.addFld("MAIR4", Proto.Elm.newCNum("(xdc_UInt8)"), 0x44L, "w");
            po.addFld("MAIR5", Proto.Elm.newCNum("(xdc_UInt8)"), 0x4FL, "w");
            po.addFld("MAIR6", Proto.Elm.newCNum("(xdc_UInt8)"), 0xBBL, "w");
            po.addFld("MAIR7", Proto.Elm.newCNum("(xdc_UInt8)"), 0xFFL, "w");
            po.addFld("numRegions", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
            po.addFld("regionEntry", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionEntry", "ti.sysbios.family.arm.v8"), false), $$DEFAULT, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8.MPU$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8.MPU$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8.MPU$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8.MPU$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                po.addFxn("initRegionAttrsMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8.MPU$$initRegionAttrsMeta", "ti.sysbios.family.arm.v8"), Global.get(cap, "initRegionAttrsMeta"));
                po.addFxn("setRegionMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8.MPU$$setRegionMeta", "ti.sysbios.family.arm.v8"), Global.get(cap, "setRegionMeta"));
        // struct MPU.RegionAttrsView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$RegionAttrsView", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.RegionAttrsView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("RegionId", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("Enabled", $$T_Bool, $$UNDEF, "w");
                po.addFld("BaseAddress", $$T_Str, $$UNDEF, "w");
                po.addFld("EndAddress", $$T_Str, $$UNDEF, "w");
                po.addFld("Shareable", $$T_Str, $$UNDEF, "w");
                po.addFld("Executable", $$T_Bool, $$UNDEF, "w");
                po.addFld("AccessPerm", $$T_Str, $$UNDEF, "w");
        // struct MPU.DeviceRegs
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$DeviceRegs", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.DeviceRegs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("TYPE", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CTRL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RNR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RBAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RLAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RBAR_A1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RLAR_A1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RBAR_A2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RLAR_A2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RBAR_A3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RLAR_A3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("res0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MAIR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MAIR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct MPU.RegionAttrs
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$RegionAttrs", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.RegionAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("enable", $$T_Bool, $$UNDEF, "w");
                po.addFld("shareable", (Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable", "ti.sysbios.family.arm.v8"), $$UNDEF, "w");
                po.addFld("executable", $$T_Bool, $$UNDEF, "w");
                po.addFld("accessPerm", (Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"), $$UNDEF, "w");
                po.addFld("attrIndx", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
        // struct MPU.RegionEntry
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$RegionEntry", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.RegionEntry", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("rbar", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("rlar", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct MPU.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$Module_State", "ti.sysbios.family.arm.v8");
        po.init("ti.sysbios.family.arm.v8.MPU.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("regionEntry", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionEntry", "ti.sysbios.family.arm.v8"), false), $$DEFAULT, "w");
    }

    void MPU$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU", "ti.sysbios.family.arm.v8");
        vo.bind("DeviceRegs$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8.MPU.DeviceRegs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$DeviceRegs", "ti.sysbios.family.arm.v8");
        vo.bind("RegionAttrs$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8.MPU.RegionAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$RegionAttrs", "ti.sysbios.family.arm.v8");
        vo.bind("RegionEntry$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8.MPU.RegionEntry", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$RegionEntry", "ti.sysbios.family.arm.v8");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8.MPU.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU$$Module_State", "ti.sysbios.family.arm.v8");
        po.bind("regionEntry$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8.MPU.RegionEntry", "isScalar", false));
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.arm.v8.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.arm.v8"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.arm.v8"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.arm.v8"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.arm.v8"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.arm.v8"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.arm.v8"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.arm.v8"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.arm.v8", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.arm.v8");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.arm.v8.");
        pkgV.bind("$vers", Global.newArray(2, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.arm.v8'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.arm.v8$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.arm.v8$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.arm.v8$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void MPU$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU", "ti.sysbios.family.arm.v8");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU.Module", "ti.sysbios.family.arm.v8");
        vo.init2(po, "ti.sysbios.family.arm.v8.MPU", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8.MPU$$capsule", "ti.sysbios.family.arm.v8"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8", "ti.sysbios.family.arm.v8"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("RegionAttrsView", om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrsView", "ti.sysbios.family.arm.v8"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrsView", "ti.sysbios.family.arm.v8"));
        vo.bind("DeviceRegs", om.findStrict("ti.sysbios.family.arm.v8.MPU.DeviceRegs", "ti.sysbios.family.arm.v8"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8.MPU.DeviceRegs", "ti.sysbios.family.arm.v8"));
        vo.bind("deviceRegs", om.findStrict("ti.sysbios.family.arm.v8.MPU.deviceRegs", "ti.sysbios.family.arm.v8"));
        vo.bind("AccessPerm", om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm", "ti.sysbios.family.arm.v8"));
        vo.bind("Shareable", om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable", "ti.sysbios.family.arm.v8"));
        vo.bind("RegionAttrs", om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrs", "ti.sysbios.family.arm.v8"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionAttrs", "ti.sysbios.family.arm.v8"));
        mcfgs.add("defaultAttrs");
        mcfgs.add("A_nullPointer");
        mcfgs.add("A_invalidRegionId");
        mcfgs.add("A_unalignedBaseAddr");
        mcfgs.add("A_unalignedEndAddr");
        mcfgs.add("enableMPU");
        mcfgs.add("enableBackgroundRegion");
        mcfgs.add("MAIR0");
        mcfgs.add("MAIR1");
        mcfgs.add("MAIR2");
        mcfgs.add("MAIR3");
        mcfgs.add("MAIR4");
        mcfgs.add("MAIR5");
        mcfgs.add("MAIR6");
        mcfgs.add("MAIR7");
        mcfgs.add("numRegions");
        vo.bind("RegionEntry", om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionEntry", "ti.sysbios.family.arm.v8"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8.MPU.RegionEntry", "ti.sysbios.family.arm.v8"));
        mcfgs.add("regionEntry");
        icfgs.add("regionEntry");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v8.MPU.Module_State", "ti.sysbios.family.arm.v8"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8.MPU.Module_State", "ti.sysbios.family.arm.v8"));
        vo.bind("AccessPerm_RW_PRIV", om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_PRIV", "ti.sysbios.family.arm.v8"));
        vo.bind("AccessPerm_RW_ANY", om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm_RW_ANY", "ti.sysbios.family.arm.v8"));
        vo.bind("AccessPerm_RO_PRIV", om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_PRIV", "ti.sysbios.family.arm.v8"));
        vo.bind("AccessPerm_RO_ANY", om.findStrict("ti.sysbios.family.arm.v8.MPU.AccessPerm_RO_ANY", "ti.sysbios.family.arm.v8"));
        vo.bind("Shareable_NONE", om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable_NONE", "ti.sysbios.family.arm.v8"));
        vo.bind("Shareable_OUTER", om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable_OUTER", "ti.sysbios.family.arm.v8"));
        vo.bind("Shareable_INNER", om.findStrict("ti.sysbios.family.arm.v8.MPU.Shareable_INNER", "ti.sysbios.family.arm.v8"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("disable", om.findStrict("ti.sysbios.family.arm.v8.MPU.disable", "ti.sysbios.family.arm.v8"));
        vo.bind("enable", om.findStrict("ti.sysbios.family.arm.v8.MPU.enable", "ti.sysbios.family.arm.v8"));
        vo.bind("initRegionAttrs", om.findStrict("ti.sysbios.family.arm.v8.MPU.initRegionAttrs", "ti.sysbios.family.arm.v8"));
        vo.bind("isEnabled", om.findStrict("ti.sysbios.family.arm.v8.MPU.isEnabled", "ti.sysbios.family.arm.v8"));
        vo.bind("setMAIR", om.findStrict("ti.sysbios.family.arm.v8.MPU.setMAIR", "ti.sysbios.family.arm.v8"));
        vo.bind("setRegion", om.findStrict("ti.sysbios.family.arm.v8.MPU.setRegion", "ti.sysbios.family.arm.v8"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.arm.v8.MPU.startup", "ti.sysbios.family.arm.v8"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8_MPU_Module__startupDone__E", "ti_sysbios_family_arm_v8_MPU_disable__E", "ti_sysbios_family_arm_v8_MPU_enable__E", "ti_sysbios_family_arm_v8_MPU_initRegionAttrs__E", "ti_sysbios_family_arm_v8_MPU_isEnabled__E", "ti_sysbios_family_arm_v8_MPU_setMAIR__E", "ti_sysbios_family_arm_v8_MPU_setRegion__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray("A_nullPointer", "A_invalidRegionId", "A_unalignedBaseAddr", "A_unalignedEndAddr"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("MPU", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("MPU");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8.MPU", "ti.sysbios.family.arm.v8"));
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8.MPU", "ti.sysbios.family.arm.v8");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"MpuRegionAttrsView", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewMpuRegionAttrs", "structName", "RegionAttrsView")})}))));
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8.MPU")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.arm.v8")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        MPU$$OBJECTS();
        MPU$$CONSTS();
        MPU$$CREATES();
        MPU$$FUNCTIONS();
        MPU$$SIZES();
        MPU$$TYPES();
        if (isROV) {
            MPU$$ROV();
        }//isROV
        $$SINGLETONS();
        MPU$$SINGLETONS();
        $$INITIALIZATION();
    }
}
