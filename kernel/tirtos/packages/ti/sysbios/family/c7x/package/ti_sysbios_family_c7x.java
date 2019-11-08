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

public class ti_sysbios_family_c7x
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
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.c7x", new Value.Obj("ti.sysbios.family.c7x", pkgP));
    }

    void Cache$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Cache.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.Cache", new Value.Obj("ti.sysbios.family.c7x.Cache", po));
        pkgV.bind("Cache", vo);
        // decls 
        om.bind("ti.sysbios.family.c7x.Cache.Type", om.findStrict("ti.sysbios.interfaces.ICache.Type", "ti.sysbios.family.c7x"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Cache$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Cache.ModuleView", new Proto.Str(spo, false));
        om.bind("ti.sysbios.family.c7x.Cache.Mode", new Proto.Enm("ti.sysbios.family.c7x.Cache.Mode"));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size", new Proto.Enm("ti.sysbios.family.c7x.Cache.L1Size"));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size", new Proto.Enm("ti.sysbios.family.c7x.Cache.L2Size"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Cache$$Size", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Cache.Size", new Proto.Str(spo, false));
    }

    void Clobber$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Clobber.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.Clobber", new Value.Obj("ti.sysbios.family.c7x.Clobber", po));
        pkgV.bind("Clobber", vo);
        // decls 
    }

    void Exception$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Exception.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.Exception", new Value.Obj("ti.sysbios.family.c7x.Exception", po));
        pkgV.bind("Exception", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Exception$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Exception.ModuleView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Exception$$Context", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Exception.Context", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Exception$$Status", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Exception.Status", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Exception$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Exception.Module_State", new Proto.Str(spo, false));
    }

    void Hwi$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.Hwi", new Value.Obj("ti.sysbios.family.c7x.Hwi", po));
        pkgV.bind("Hwi", vo);
        // decls 
        om.bind("ti.sysbios.family.c7x.Hwi.HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.c7x"));
        om.bind("ti.sysbios.family.c7x.Hwi.MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.c7x"));
        om.bind("ti.sysbios.family.c7x.Hwi.StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.c7x"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$BasicView", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.BasicView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.ModuleView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$NonDispatchedInterrupt", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.NonDispatchedInterrupt", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$InterruptObj", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.InterruptObj", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$Instance_State", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sysbios.family.c7x.Hwi.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$Object", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Hwi$$Params", new Proto.Obj());
        om.bind("ti.sysbios.family.c7x.Hwi.Params", new Proto.Str(po, false));
        om.bind("ti.sysbios.family.c7x.Hwi.Handle", insP);
        if (isROV) {
            om.bind("ti.sysbios.family.c7x.Hwi.Object", om.findStrict("ti.sysbios.family.c7x.Hwi.Instance_State", "ti.sysbios.family.c7x"));
        }//isROV
    }

    void IntrinsicsSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.IntrinsicsSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.IntrinsicsSupport", new Value.Obj("ti.sysbios.family.c7x.IntrinsicsSupport", po));
        pkgV.bind("IntrinsicsSupport", vo);
        // decls 
    }

    void Settings$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.Settings.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.Settings", new Value.Obj("ti.sysbios.family.c7x.Settings", po));
        pkgV.bind("Settings", vo);
        // decls 
    }

    void TaskSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.TaskSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.TaskSupport", new Value.Obj("ti.sysbios.family.c7x.TaskSupport", po));
        pkgV.bind("TaskSupport", vo);
        // decls 
    }

    void TimestampProvider$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.c7x.TimestampProvider.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.c7x.TimestampProvider", new Value.Obj("ti.sysbios.family.c7x.TimestampProvider", po));
        pkgV.bind("TimestampProvider", vo);
        // decls 
    }

    void Cache$$CONSTS()
    {
        // module Cache
        om.bind("ti.sysbios.family.c7x.Cache.Mode_FREEZE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.Mode", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.Mode_FREEZE", 0));
        om.bind("ti.sysbios.family.c7x.Cache.Mode_BYPASS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.Mode", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.Mode_BYPASS", 1));
        om.bind("ti.sysbios.family.c7x.Cache.Mode_NORMAL", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.Mode", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.Mode_NORMAL", 2));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size_0K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L1Size_0K", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size_4K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L1Size_4K", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size_8K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L1Size_8K", xdc.services.intern.xsr.Enum.intValue(2L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size_16K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L1Size_16K", xdc.services.intern.xsr.Enum.intValue(3L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L1Size_32K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L1Size_32K", xdc.services.intern.xsr.Enum.intValue(4L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_0K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_0K", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_32K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_32K", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_64K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_64K", xdc.services.intern.xsr.Enum.intValue(2L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_128K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_128K", xdc.services.intern.xsr.Enum.intValue(3L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_256K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_256K", xdc.services.intern.xsr.Enum.intValue(4L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_512K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_512K", xdc.services.intern.xsr.Enum.intValue(5L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.L2Size_1024K", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), "ti.sysbios.family.c7x.Cache.L2Size_1024K", xdc.services.intern.xsr.Enum.intValue(6L)+0));
        om.bind("ti.sysbios.family.c7x.Cache.PC", 1L);
        om.bind("ti.sysbios.family.c7x.Cache.WTE", 2L);
        om.bind("ti.sysbios.family.c7x.Cache.PCX", 4L);
        om.bind("ti.sysbios.family.c7x.Cache.PFX", 8L);
        om.bind("ti.sysbios.family.c7x.Cache.enable", new Extern("ti_sysbios_family_c7x_Cache_enable__E", "xdc_Void(*)(xdc_Bits16)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.inv", new Extern("ti_sysbios_family_c7x_Cache_inv__E", "xdc_Void(*)(xdc_Ptr,xdc_SizeT,xdc_Bits16,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wb", new Extern("ti_sysbios_family_c7x_Cache_wb__E", "xdc_Void(*)(xdc_Ptr,xdc_SizeT,xdc_Bits16,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wbInv", new Extern("ti_sysbios_family_c7x_Cache_wbInv__E", "xdc_Void(*)(xdc_Ptr,xdc_SizeT,xdc_Bits16,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wait", new Extern("ti_sysbios_family_c7x_Cache_wait__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.disable", new Extern("ti_sysbios_family_c7x_Cache_disable__E", "xdc_Void(*)(xdc_Bits16)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.getSize", new Extern("ti_sysbios_family_c7x_Cache_getSize__E", "xdc_Void(*)(ti_sysbios_family_c7x_Cache_Size*)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setSize", new Extern("ti_sysbios_family_c7x_Cache_setSize__E", "xdc_Void(*)(ti_sysbios_family_c7x_Cache_Size*)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wbAll", new Extern("ti_sysbios_family_c7x_Cache_wbAll__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wbL1dAll", new Extern("ti_sysbios_family_c7x_Cache_wbL1dAll__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wbInvAll", new Extern("ti_sysbios_family_c7x_Cache_wbInvAll__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.wbInvL1dAll", new Extern("ti_sysbios_family_c7x_Cache_wbInvL1dAll__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL2CFG", new Extern("ti_sysbios_family_c7x_Cache_setL2CFG__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.getL2CFG", new Extern("ti_sysbios_family_c7x_Cache_getL2CFG__E", "xdc_ULong(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL1DCFG", new Extern("ti_sysbios_family_c7x_Cache_setL1DCFG__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.getL1DCFG", new Extern("ti_sysbios_family_c7x_Cache_getL1DCFG__E", "xdc_ULong(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL2WB", new Extern("ti_sysbios_family_c7x_Cache_setL2WB__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL2WBINV", new Extern("ti_sysbios_family_c7x_Cache_setL2WBINV__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL1DWB", new Extern("ti_sysbios_family_c7x_Cache_setL1DWB__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.setL1DWBINV", new Extern("ti_sysbios_family_c7x_Cache_setL1DWBINV__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Cache.startup", new Extern("ti_sysbios_family_c7x_Cache_startup__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void Clobber$$CONSTS()
    {
        // module Clobber
        om.bind("ti.sysbios.family.c7x.Clobber.trashRegs", new Extern("ti_sysbios_family_c7x_Clobber_trashRegs__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Clobber.postIntr", new Extern("ti_sysbios_family_c7x_Clobber_postIntr__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Clobber.checkRegs", new Extern("ti_sysbios_family_c7x_Clobber_checkRegs__E", "xdc_UInt(*)(xdc_UInt)", true, false));
    }

    void Exception$$CONSTS()
    {
        // module Exception
        om.bind("ti.sysbios.family.c7x.Exception.EFRSXF", 0x00000001L);
        om.bind("ti.sysbios.family.c7x.Exception.EFRIXF", 0x00000002L);
        om.bind("ti.sysbios.family.c7x.Exception.EFREXF", 0x40000000L);
        om.bind("ti.sysbios.family.c7x.Exception.EFRNXF", 0x80000000L);
        om.bind("ti.sysbios.family.c7x.Exception.ECRSXF", 0x00000001L);
        om.bind("ti.sysbios.family.c7x.Exception.ECRIXF", 0x00000002L);
        om.bind("ti.sysbios.family.c7x.Exception.ECREXF", 0x40000000L);
        om.bind("ti.sysbios.family.c7x.Exception.ECRNXF", 0x80000000L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRIFX", 0x00000001L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRFPX", 0x00000002L);
        om.bind("ti.sysbios.family.c7x.Exception.IERREPX", 0x00000004L);
        om.bind("ti.sysbios.family.c7x.Exception.IERROPX", 0x00000008L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRRCX", 0x00000010L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRRAX", 0x00000020L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRPRX", 0x00000040L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRLBX", 0x00000080L);
        om.bind("ti.sysbios.family.c7x.Exception.IERRMSX", 0x00000100L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRGEE", 0x00000004L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRXEN", 0x00000008L);
        om.bind("ti.sysbios.family.c7x.Exception.TSREXC", 0x00000400L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRCXM", 0x000000C0L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRCXMSHIFT", 6L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRCXMSV", 0x00000000L);
        om.bind("ti.sysbios.family.c7x.Exception.TSRCXMUS", 0x00000040L);
        om.bind("ti.sysbios.family.c7x.Exception.sizeContextBuf", 320L);
        om.bind("ti.sysbios.family.c7x.Exception.EVTPMCCMPA", 120L);
        om.bind("ti.sysbios.family.c7x.Exception.EVTDMCCMPA", 122L);
        om.bind("ti.sysbios.family.c7x.Exception.EVTUMCCMPA", 124L);
        om.bind("ti.sysbios.family.c7x.Exception.EVTEMCCMPA", 126L);
        om.bind("ti.sysbios.family.c7x.Exception.getLastStatus", new Extern("ti_sysbios_family_c7x_Exception_getLastStatus__E", "xdc_Void(*)(ti_sysbios_family_c7x_Exception_Status*)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.clearLastStatus", new Extern("ti_sysbios_family_c7x_Exception_clearLastStatus__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.setReturnPtr", new Extern("ti_sysbios_family_c7x_Exception_setReturnPtr__E", "xdc_Void(*)(xdc_Void)(*)(xdc_Void(*)(xdc_Void))", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.evtEvtClear", new Extern("ti_sysbios_family_c7x_Exception_evtEvtClear__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.evtExpMaskEnable", new Extern("ti_sysbios_family_c7x_Exception_evtExpMaskEnable__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.dispatch", new Extern("ti_sysbios_family_c7x_Exception_dispatch__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.handler", new Extern("ti_sysbios_family_c7x_Exception_handler__I", "xdc_Void(*)(xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.internalHandler", new Extern("ti_sysbios_family_c7x_Exception_internalHandler__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.externalHandler", new Extern("ti_sysbios_family_c7x_Exception_externalHandler__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.nmiHandler", new Extern("ti_sysbios_family_c7x_Exception_nmiHandler__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Exception.decodeMpfsr", new Extern("ti_sysbios_family_c7x_Exception_decodeMpfsr__I", "xdc_Void(*)(xdc_UInt)", true, false));
    }

    void Hwi$$CONSTS()
    {
        // module Hwi
        om.bind("ti.sysbios.family.c7x.Hwi.NUM_INTERRUPTS", 64L);
        om.bind("ti.sysbios.family.c7x.Hwi.getStackInfo", new Extern("ti_sysbios_family_c7x_Hwi_getStackInfo__E", "xdc_Bool(*)(ti_sysbios_interfaces_IHwi_StackInfo*,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.getCoreStackInfo", new Extern("ti_sysbios_family_c7x_Hwi_getCoreStackInfo__E", "xdc_Bool(*)(ti_sysbios_interfaces_IHwi_StackInfo*,xdc_Bool,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.startup", new Extern("ti_sysbios_family_c7x_Hwi_startup__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.switchFromBootStack", new Extern("ti_sysbios_family_c7x_Hwi_switchFromBootStack__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.post", new Extern("ti_sysbios_family_c7x_Hwi_post__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.getTaskSP", new Extern("ti_sysbios_family_c7x_Hwi_getTaskSP__E", "xdc_Char*(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.disableInterrupt", new Extern("ti_sysbios_family_c7x_Hwi_disableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.enableInterrupt", new Extern("ti_sysbios_family_c7x_Hwi_enableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.restoreInterrupt", new Extern("ti_sysbios_family_c7x_Hwi_restoreInterrupt__E", "xdc_Void(*)(xdc_UInt,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.clearInterrupt", new Extern("ti_sysbios_family_c7x_Hwi_clearInterrupt__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.eventMap", new Extern("ti_sysbios_family_c7x_Hwi_eventMap__E", "xdc_Void(*)(xdc_Int,xdc_Int)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.plug", new Extern("ti_sysbios_family_c7x_Hwi_plug__E", "xdc_Void(*)(xdc_UInt,xdc_Void(*)(xdc_Void))", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.getEventId", new Extern("ti_sysbios_family_c7x_Hwi_getEventId__E", "xdc_Int(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.getHandle", new Extern("ti_sysbios_family_c7x_Hwi_getHandle__E", "ti_sysbios_family_c7x_Hwi_Handle(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.disableIER", new Extern("ti_sysbios_family_c7x_Hwi_disableIER__E", "xdc_ULong(*)(xdc_ULong)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.enableIER", new Extern("ti_sysbios_family_c7x_Hwi_enableIER__E", "xdc_ULong(*)(xdc_ULong)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.restoreIER", new Extern("ti_sysbios_family_c7x_Hwi_restoreIER__E", "xdc_ULong(*)(xdc_ULong)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.setPriority", new Extern("ti_sysbios_family_c7x_Hwi_setPriority__E", "xdc_Void(*)(xdc_UInt,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.setCOP", new Extern("ti_sysbios_family_c7x_Hwi_setCOP__E", "xdc_Void(*)(xdc_Int)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.getIsrStackAddress", new Extern("ti_sysbios_family_c7x_Hwi_getIsrStackAddress__I", "xdc_Char*(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.dispatchC", new Extern("ti_sysbios_family_c7x_Hwi_dispatchC__I", "xdc_Void(*)(xdc_Int)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.dispatchCore", new Extern("ti_sysbios_family_c7x_Hwi_dispatchCore__I", "xdc_Void(*)(xdc_Int)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.unPluggedInterrupt", new Extern("ti_sysbios_family_c7x_Hwi_unPluggedInterrupt__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.switchAndDispatch", new Extern("ti_sysbios_family_c7x_Hwi_switchAndDispatch__I", "xdc_Void(*)(xdc_Int)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.setupSC", new Extern("ti_sysbios_family_c7x_Hwi_setupSC__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.Hwi.postInit", new Extern("ti_sysbios_family_c7x_Hwi_postInit__I", "xdc_Int(*)(ti_sysbios_family_c7x_Hwi_Object*,xdc_runtime_Error_Block*)", true, false));
    }

    void IntrinsicsSupport$$CONSTS()
    {
        // module IntrinsicsSupport
        om.bind("ti.sysbios.family.c7x.IntrinsicsSupport.maxbit", new Extern("ti_sysbios_family_c7x_IntrinsicsSupport_maxbit__E", "xdc_UInt(*)(xdc_UInt)", true, false));
    }

    void Settings$$CONSTS()
    {
        // module Settings
    }

    void TaskSupport$$CONSTS()
    {
        // module TaskSupport
        om.bind("ti.sysbios.family.c7x.TaskSupport.start", new Extern("ti_sysbios_family_c7x_TaskSupport_start__E", "xdc_Ptr(*)(xdc_Ptr,xdc_Void(*)(xdc_Void),xdc_Void(*)(xdc_Void),xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.c7x.TaskSupport.swap", new Extern("ti_sysbios_family_c7x_TaskSupport_swap__E", "xdc_Void(*)(xdc_Ptr*,xdc_Ptr*)", true, false));
        om.bind("ti.sysbios.family.c7x.TaskSupport.checkStack", new Extern("ti_sysbios_family_c7x_TaskSupport_checkStack__E", "xdc_Bool(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.c7x.TaskSupport.stackUsed", new Extern("ti_sysbios_family_c7x_TaskSupport_stackUsed__E", "xdc_SizeT(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.c7x.TaskSupport.getStackAlignment", new Extern("ti_sysbios_family_c7x_TaskSupport_getStackAlignment__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.TaskSupport.getDefaultStackSize", new Extern("ti_sysbios_family_c7x_TaskSupport_getDefaultStackSize__E", "xdc_SizeT(*)(xdc_Void)", true, false));
    }

    void TimestampProvider$$CONSTS()
    {
        // module TimestampProvider
        om.bind("ti.sysbios.family.c7x.TimestampProvider.get32", new Extern("ti_sysbios_family_c7x_TimestampProvider_get32__E", "xdc_Bits32(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.c7x.TimestampProvider.get64", new Extern("ti_sysbios_family_c7x_TimestampProvider_get64__E", "xdc_Void(*)(xdc_runtime_Types_Timestamp64*)", true, false));
        om.bind("ti.sysbios.family.c7x.TimestampProvider.getFreq", new Extern("ti_sysbios_family_c7x_TimestampProvider_getFreq__E", "xdc_Void(*)(xdc_runtime_Types_FreqHz*)", true, false));
    }

    void Cache$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Clobber$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Exception$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Hwi$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$c7x$Hwi$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sysbios.family.c7x.Hwi.create() called before xdc.useModule('ti.sysbios.family.c7x.Hwi')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.intNum, inst.$args.hwiFxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.c7x.Hwi$$create", new Proto.Fxn(om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x"), om.findStrict("ti.sysbios.family.c7x.Hwi.Instance", "ti.sysbios.family.c7x"), 3, 2, false));
                        fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.Params", "ti.sysbios.family.c7x"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$c7x$Hwi$$create = function( intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.c7x.Hwi'];\n");
                sb.append("var __inst = xdc.om['ti.sysbios.family.c7x.Hwi.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sysbios.family.c7x']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sysbios.family.c7x.Hwi'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [intNum, hwiFxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.c7x.Hwi'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.c7x.Hwi$$construct", new Proto.Fxn(om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sysbios.family.c7x.Hwi$$Object", "ti.sysbios.family.c7x"), null);
                        fxn.addArg(1, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.Params", "ti.sysbios.family.c7x"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$c7x$Hwi$$construct = function( __obj, intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.c7x.Hwi'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sysbios.family.c7x.Hwi'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.c7x.Hwi'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void IntrinsicsSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Settings$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TaskSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TimestampProvider$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Cache$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Clobber$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Exception$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Hwi$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn Hwi.inUseMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.c7x.Hwi$$inUseMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x"), $$T_Bool, 1, 1, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF);
        // fxn Hwi.eventMapMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.c7x.Hwi$$eventMapMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x"), null, 2, 2, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                fxn.addArg(1, "eventId", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
        // fxn Hwi.plugMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.c7x.Hwi$$plugMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x"), null, 2, 2, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF);
                fxn.addArg(1, "fxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF);
    }

    void IntrinsicsSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Settings$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TaskSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TimestampProvider$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Cache$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Cache.Size", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("l1pSize", "Nti.sysbios.family.c7x.Cache.L1Size;;0;1;2;3;4"));
        sizes.add(Global.newArray("l1dSize", "Nti.sysbios.family.c7x.Cache.L1Size;;0;1;2;3;4"));
        sizes.add(Global.newArray("l2Size", "Nti.sysbios.family.c7x.Cache.L2Size;;0;1;2;3;4;5;6"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Cache.Size']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Cache.Size']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Cache.Size'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Clobber$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Exception$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Exception.Context", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("ILC", "UPtr"));
        sizes.add(Global.newArray("RILC", "UPtr"));
        sizes.add(Global.newArray("AMR", "UPtr"));
        sizes.add(Global.newArray("SSR", "UPtr"));
        sizes.add(Global.newArray("IRP", "UPtr"));
        sizes.add(Global.newArray("NRP", "UPtr"));
        sizes.add(Global.newArray("ITSR", "UPtr"));
        sizes.add(Global.newArray("NTSR", "UPtr"));
        sizes.add(Global.newArray("EFR", "UPtr"));
        sizes.add(Global.newArray("IERR", "UPtr"));
        sizes.add(Global.newArray("B30", "UPtr"));
        sizes.add(Global.newArray("B31", "UPtr"));
        sizes.add(Global.newArray("B28", "UPtr"));
        sizes.add(Global.newArray("B29", "UPtr"));
        sizes.add(Global.newArray("B26", "UPtr"));
        sizes.add(Global.newArray("B27", "UPtr"));
        sizes.add(Global.newArray("B24", "UPtr"));
        sizes.add(Global.newArray("B25", "UPtr"));
        sizes.add(Global.newArray("B22", "UPtr"));
        sizes.add(Global.newArray("B23", "UPtr"));
        sizes.add(Global.newArray("B20", "UPtr"));
        sizes.add(Global.newArray("B21", "UPtr"));
        sizes.add(Global.newArray("B18", "UPtr"));
        sizes.add(Global.newArray("B19", "UPtr"));
        sizes.add(Global.newArray("B16", "UPtr"));
        sizes.add(Global.newArray("B17", "UPtr"));
        sizes.add(Global.newArray("B14", "UPtr"));
        sizes.add(Global.newArray("B15", "UPtr"));
        sizes.add(Global.newArray("B12", "UPtr"));
        sizes.add(Global.newArray("B13", "UPtr"));
        sizes.add(Global.newArray("B10", "UPtr"));
        sizes.add(Global.newArray("B11", "UPtr"));
        sizes.add(Global.newArray("B8", "UPtr"));
        sizes.add(Global.newArray("B9", "UPtr"));
        sizes.add(Global.newArray("B6", "UPtr"));
        sizes.add(Global.newArray("B7", "UPtr"));
        sizes.add(Global.newArray("B4", "UPtr"));
        sizes.add(Global.newArray("B5", "UPtr"));
        sizes.add(Global.newArray("B2", "UPtr"));
        sizes.add(Global.newArray("B3", "UPtr"));
        sizes.add(Global.newArray("B0", "UPtr"));
        sizes.add(Global.newArray("B1", "UPtr"));
        sizes.add(Global.newArray("A30", "UPtr"));
        sizes.add(Global.newArray("A31", "UPtr"));
        sizes.add(Global.newArray("A28", "UPtr"));
        sizes.add(Global.newArray("A29", "UPtr"));
        sizes.add(Global.newArray("A26", "UPtr"));
        sizes.add(Global.newArray("A27", "UPtr"));
        sizes.add(Global.newArray("A24", "UPtr"));
        sizes.add(Global.newArray("A25", "UPtr"));
        sizes.add(Global.newArray("A22", "UPtr"));
        sizes.add(Global.newArray("A23", "UPtr"));
        sizes.add(Global.newArray("A20", "UPtr"));
        sizes.add(Global.newArray("A21", "UPtr"));
        sizes.add(Global.newArray("A18", "UPtr"));
        sizes.add(Global.newArray("A19", "UPtr"));
        sizes.add(Global.newArray("A16", "UPtr"));
        sizes.add(Global.newArray("A17", "UPtr"));
        sizes.add(Global.newArray("A14", "UPtr"));
        sizes.add(Global.newArray("A15", "UPtr"));
        sizes.add(Global.newArray("A12", "UPtr"));
        sizes.add(Global.newArray("A13", "UPtr"));
        sizes.add(Global.newArray("A10", "UPtr"));
        sizes.add(Global.newArray("A11", "UPtr"));
        sizes.add(Global.newArray("A8", "UPtr"));
        sizes.add(Global.newArray("A9", "UPtr"));
        sizes.add(Global.newArray("A6", "UPtr"));
        sizes.add(Global.newArray("A7", "UPtr"));
        sizes.add(Global.newArray("A4", "UPtr"));
        sizes.add(Global.newArray("A5", "UPtr"));
        sizes.add(Global.newArray("A2", "UPtr"));
        sizes.add(Global.newArray("A3", "UPtr"));
        sizes.add(Global.newArray("A0", "UPtr"));
        sizes.add(Global.newArray("A1", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Exception.Context']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Exception.Context']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Exception.Context'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Exception.Status", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("efr", "UInt32"));
        sizes.add(Global.newArray("nrp", "UInt32"));
        sizes.add(Global.newArray("ntsr", "UInt32"));
        sizes.add(Global.newArray("ierr", "UInt32"));
        sizes.add(Global.newArray("excContext", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Exception.Status']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Exception.Status']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Exception.Status'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Exception.Module_State", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("efr", "UInt32"));
        sizes.add(Global.newArray("nrp", "UInt32"));
        sizes.add(Global.newArray("ntsr", "UInt32"));
        sizes.add(Global.newArray("ierr", "UInt32"));
        sizes.add(Global.newArray("returnHook", "UFxn"));
        sizes.add(Global.newArray("excContext", "UPtr"));
        sizes.add(Global.newArray("scratch", "A16;TChar"));
        sizes.add(Global.newArray("excPtr", "UPtr"));
        sizes.add(Global.newArray("contextBuf", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Exception.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Exception.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Exception.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Hwi$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Hwi.Instance_State", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("disableMask", "ULong"));
        sizes.add(Global.newArray("restoreMask", "ULong"));
        sizes.add(Global.newArray("arg", "UIArg"));
        sizes.add(Global.newArray("fxn", "UFxn"));
        sizes.add(Global.newArray("intNum", "TInt"));
        sizes.add(Global.newArray("priority", "TInt"));
        sizes.add(Global.newArray("irp", "UIArg"));
        sizes.add(Global.newArray("hookEnv", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Hwi.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Hwi.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Hwi.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.c7x.Hwi.Module_State", "ti.sysbios.family.c7x");
        sizes.clear();
        sizes.add(Global.newArray("intEvents", "A64;TChar"));
        sizes.add(Global.newArray("ierMask", "ULong"));
        sizes.add(Global.newArray("intNum", "TInt"));
        sizes.add(Global.newArray("taskSP", "UPtr"));
        sizes.add(Global.newArray("isrStack", "UPtr"));
        sizes.add(Global.newArray("vectorTableBase", "UPtr"));
        sizes.add(Global.newArray("scw", "TInt"));
        sizes.add(Global.newArray("dispatchTable", "A64;UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.c7x.Hwi.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.c7x.Hwi.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.c7x.Hwi.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void IntrinsicsSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Settings$$SIZES()
    {
    }

    void TaskSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void TimestampProvider$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Cache$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/Cache.xs");
        om.bind("ti.sysbios.family.c7x.Cache$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Cache.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Cache.Module", om.findStrict("ti.sysbios.interfaces.ICache.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("PC", Proto.Elm.newCNum("(xdc_UInt32)"), 1L, "rh");
                po.addFld("WTE", Proto.Elm.newCNum("(xdc_UInt32)"), 2L, "rh");
                po.addFld("PCX", Proto.Elm.newCNum("(xdc_UInt32)"), 4L, "rh");
                po.addFld("PFX", Proto.Elm.newCNum("(xdc_UInt32)"), 8L, "rh");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.c7x"), $$UNDEF, "wh");
            po.addFld("initSize", (Proto)om.findStrict("ti.sysbios.family.c7x.Cache.Size", "ti.sysbios.family.c7x"), Global.newObject("l1pSize", om.find("ti.sysbios.family.c7x.Cache.L1Size_32K"), "l1dSize", om.find("ti.sysbios.family.c7x.Cache.L1Size_32K"), "l2Size", om.find("ti.sysbios.family.c7x.Cache.L2Size_0K")), "w");
            po.addFld("atomicBlockSize", Proto.Elm.newCNum("(xdc_UInt32)"), 1024L, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Cache$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Cache$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Cache$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Cache$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct Cache.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Cache$$ModuleView", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Cache.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("L1PCacheSize", $$T_Str, $$UNDEF, "w");
                po.addFld("L1PMode", $$T_Str, $$UNDEF, "w");
                po.addFld("L1DCacheSize", $$T_Str, $$UNDEF, "w");
                po.addFld("L1DMode", $$T_Str, $$UNDEF, "w");
                po.addFld("L2CacheSize", $$T_Str, $$UNDEF, "w");
                po.addFld("L2Mode", $$T_Str, $$UNDEF, "w");
        // struct Cache.Size
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Cache$$Size", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Cache.Size", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("l1pSize", (Proto)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), $$UNDEF, "w");
                po.addFld("l1dSize", (Proto)om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"), $$UNDEF, "w");
                po.addFld("l2Size", (Proto)om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"), $$UNDEF, "w");
    }

    void Clobber$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/Clobber.xs");
        om.bind("ti.sysbios.family.c7x.Clobber$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Clobber.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Clobber.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("postIntrCheck", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Clobber$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Clobber$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Clobber$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Clobber$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void Exception$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/Exception.xs");
        om.bind("ti.sysbios.family.c7x.Exception$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Exception.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("EFRSXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000001L, "rh");
                po.addFld("EFRIXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000002L, "rh");
                po.addFld("EFREXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x40000000L, "rh");
                po.addFld("EFRNXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x80000000L, "rh");
                po.addFld("ECRSXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000001L, "rh");
                po.addFld("ECRIXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000002L, "rh");
                po.addFld("ECREXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x40000000L, "rh");
                po.addFld("ECRNXF", Proto.Elm.newCNum("(xdc_Bits32)"), 0x80000000L, "rh");
                po.addFld("IERRIFX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000001L, "rh");
                po.addFld("IERRFPX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000002L, "rh");
                po.addFld("IERREPX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000004L, "rh");
                po.addFld("IERROPX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000008L, "rh");
                po.addFld("IERRRCX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000010L, "rh");
                po.addFld("IERRRAX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000020L, "rh");
                po.addFld("IERRPRX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000040L, "rh");
                po.addFld("IERRLBX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000080L, "rh");
                po.addFld("IERRMSX", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000100L, "rh");
                po.addFld("TSRGEE", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000004L, "rh");
                po.addFld("TSRXEN", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000008L, "rh");
                po.addFld("TSREXC", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000400L, "rh");
                po.addFld("TSRCXM", Proto.Elm.newCNum("(xdc_Bits32)"), 0x000000C0L, "rh");
                po.addFld("TSRCXMSHIFT", Proto.Elm.newCNum("(xdc_Bits32)"), 6L, "rh");
                po.addFld("TSRCXMSV", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000000L, "rh");
                po.addFld("TSRCXMUS", Proto.Elm.newCNum("(xdc_Bits32)"), 0x00000040L, "rh");
                po.addFld("sizeContextBuf", Proto.Elm.newCNum("(xdc_UInt32)"), 320L, "rh");
                po.addFld("EVTPMCCMPA", Proto.Elm.newCNum("(xdc_UInt)"), 120L, "rh");
                po.addFld("EVTDMCCMPA", Proto.Elm.newCNum("(xdc_UInt)"), 122L, "rh");
                po.addFld("EVTUMCCMPA", Proto.Elm.newCNum("(xdc_UInt)"), 124L, "rh");
                po.addFld("EVTEMCCMPA", Proto.Elm.newCNum("(xdc_UInt)"), 126L, "rh");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.c7x"), $$UNDEF, "wh");
            po.addFld("E_exceptionMin", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_exceptionMin: pc = 0x%08x, sp = 0x%08x.\nTo see more exception detail, use ROV or set 'ti.sysbios.family.c64p.Exception.enablePrint = true;'"), "w");
            po.addFld("E_exceptionMax", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_exceptionMax: pc = 0x%08x, sp = 0x%08x."), "w");
            po.addFld("useInternalBuffer", $$T_Bool, false, "w");
            po.addFld("enableExternalMPC", $$T_Bool, false, "w");
            po.addFld("enablePrint", $$T_Bool, true, "w");
            po.addFld("exceptionHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), null, "w");
            po.addFld("internalHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), null, "w");
            po.addFld("externalHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), null, "w");
            po.addFld("nmiHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), null, "w");
            po.addFld("returnHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), null, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Exception$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Exception$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Exception$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Exception$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct Exception.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$ModuleView", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Exception.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("exception", $$T_Str, $$UNDEF, "w");
        // typedef Exception.FuncPtr
        om.bind("ti.sysbios.family.c7x.Exception.FuncPtr", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"));
        // struct Exception.Context
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Context", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Exception.Context", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ILC", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("RILC", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("AMR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("SSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("IRP", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("NRP", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("ITSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("NTSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("EFR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("IERR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B30", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B31", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B28", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B29", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B26", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B27", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B24", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B25", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B22", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B23", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B20", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B21", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B18", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B19", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B16", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B17", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B14", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B15", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B12", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B13", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B10", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B11", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B8", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B9", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B6", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B7", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B4", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B5", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B2", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B3", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B0", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("B1", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A30", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A31", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A28", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A29", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A26", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A27", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A24", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A25", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A22", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A23", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A20", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A21", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A18", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A19", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A16", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A17", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A14", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A15", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A12", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A13", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A10", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A11", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A8", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A9", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A6", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A7", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A4", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A5", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A2", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A3", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A0", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("A1", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct Exception.Status
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Status", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Exception.Status", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("efr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("nrp", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("ntsr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("ierr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("excContext", new Proto.Adr("ti_sysbios_family_c7x_Exception_Context*", "PS"), $$UNDEF, "w");
        // struct Exception.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Module_State", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Exception.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("efr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("nrp", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("ntsr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("ierr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("returnHook", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "w");
                po.addFld("excContext", new Proto.Adr("ti_sysbios_family_c7x_Exception_Context*", "PS"), $$UNDEF, "w");
                po.addFld("scratch", new Proto.Arr(Proto.Elm.newCNum("(xdc_Char)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("excPtr", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("contextBuf", new Proto.Arr(Proto.Elm.newCNum("(xdc_Char)"), false), $$DEFAULT, "w");
    }

    void Hwi$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/Hwi.xs");
        om.bind("ti.sysbios.family.c7x.Hwi$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Module", om.findStrict("ti.sysbios.interfaces.IHwi.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 64L, "rh");
        if (isCFG) {
            po.addFld("DEFAULT_INT_PRIORITY", Proto.Elm.newCNum("(xdc_UInt)"), 6L, "w");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.c7x"), $$UNDEF, "wh");
            po.addFld("nonDispatchedInterrupts", new Proto.Map((Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.c7x")), $$DEFAULT, "wh");
            po.addFld("resetVectorAddress", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "wh");
            po.addFld("E_alreadyDefined", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_alreadyDefined: Hwi already defined: intr# %d"), "w");
            po.addFld("E_handleNotFound", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_handleNotFound: Hwi handle not found: 0x%x"), "w");
            po.addFld("E_allocSCFailed", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_allocSCFailed: Alloc secure context failed"), "w");
            po.addFld("E_registerSCFailed", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_registerSCFailed: Register secure context failed"), "w");
            po.addFld("E_invalidIntNum", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "E_invalidIntNum: Invalid interrupt number: intr# %d"), "w");
            po.addFld("LM_begin", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.c7x"), Global.newObject("mask", Global.eval("0x0100 | 0x0200"), "msg", "LM_begin: hwi: 0x%x, func: 0x%x, preThread: %d, intNum: %d, irp: 0x%x"), "w");
            po.addFld("LD_end", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.c7x"), Global.newObject("mask", 0x0200L, "msg", "LD_end: hwi: 0x%x"), "w");
            po.addFld("enableException", $$T_Bool, true, "w");
            po.addFld("swiDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("swiRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("taskDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("taskRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("hooks", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.HookSet", "ti.sysbios.family.c7x"), true), Global.newArray(new Object[]{}), "w");
            po.addFld("interrupt", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.InterruptObj", "ti.sysbios.family.c7x"), false, xdc.services.intern.xsr.Enum.intValue(64L)), $$DEFAULT, "wh");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sysbios.family.c7x.Hwi$$create", "ti.sysbios.family.c7x"), Global.get("ti$sysbios$family$c7x$Hwi$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sysbios.family.c7x.Hwi$$construct", "ti.sysbios.family.c7x"), Global.get("ti$sysbios$family$c7x$Hwi$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Hwi$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "addHookSet");
                if (fxn != null) po.addFxn("addHookSet", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$addHookSet", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "viewGetStackInfo");
                if (fxn != null) po.addFxn("viewGetStackInfo", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$viewGetStackInfo", "ti.sysbios.family.c7x"), fxn);
                po.addFxn("inUseMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.c7x.Hwi$$inUseMeta", "ti.sysbios.family.c7x"), Global.get(cap, "inUseMeta"));
                po.addFxn("eventMapMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.c7x.Hwi$$eventMapMeta", "ti.sysbios.family.c7x"), Global.get(cap, "eventMapMeta"));
                po.addFxn("plugMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.c7x.Hwi$$plugMeta", "ti.sysbios.family.c7x"), Global.get(cap, "plugMeta"));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi.Instance", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Instance", om.findStrict("ti.sysbios.interfaces.IHwi.Instance", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 64L, "rh");
        if (isCFG) {
            po.addFld("disableMask", Proto.Elm.newCNum("(xdc_ULong)"), 0L, "w");
            po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_ULong)"), 0L, "w");
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-1"), "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.c7x"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Params", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Params", om.findStrict("ti.sysbios.interfaces.IHwi$$Params", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), 64L, "rh");
        if (isCFG) {
            po.addFld("disableMask", Proto.Elm.newCNum("(xdc_ULong)"), 0L, "w");
            po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_ULong)"), 0L, "w");
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), Global.eval("-1"), "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.c7x"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Object", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Object", om.findStrict("ti.sysbios.family.c7x.Hwi.Instance", "ti.sysbios.family.c7x"));
        // typedef Hwi.PlugFuncPtr
        om.bind("ti.sysbios.family.c7x.Hwi.PlugFuncPtr", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"));
        // struct Hwi.BasicView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$BasicView", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("halHwiHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("label", $$T_Str, $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("fxn", $$T_Str, $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("eventId", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("disableMask", $$T_Str, $$UNDEF, "w");
                po.addFld("restoreMask", $$T_Str, $$UNDEF, "w");
        // struct Hwi.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$ModuleView", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("options", new Proto.Arr($$T_Str, false, xdc.services.intern.xsr.Enum.intValue(4L)), $$DEFAULT, "w");
                po.addFld("hwiStackPeak", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("hwiStackBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct Hwi.NonDispatchedInterrupt
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$NonDispatchedInterrupt", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.NonDispatchedInterrupt", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "w");
                po.addFld("enableInt", $$T_Bool, $$UNDEF, "w");
                po.addFld("eventId", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
        // struct Hwi.InterruptObj
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$InterruptObj", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.InterruptObj", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("name", $$T_Str, $$UNDEF, "w");
                po.addFld("used", $$T_Bool, $$UNDEF, "w");
                po.addFld("useDispatcher", $$T_Bool, $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("pfxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "w");
        // struct Hwi.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Instance_State", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("disableMask", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("restoreMask", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("hookEnv", new Proto.Arr(new Proto.Adr("xdc_Ptr", "Pv"), false), $$DEFAULT, "w");
        // struct Hwi.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Module_State", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Hwi.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("intEvents", new Proto.Arr(Proto.Elm.newCNum("(xdc_Char)"), false, xdc.services.intern.xsr.Enum.intValue(64L)), $$DEFAULT, "w");
                po.addFld("ierMask", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("taskSP", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("isrStack", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("vectorTableBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("scw", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("dispatchTable", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.c7x.Hwi.Handle", "ti.sysbios.family.c7x"), false, xdc.services.intern.xsr.Enum.intValue(64L)), $$DEFAULT, "w");
    }

    void IntrinsicsSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/IntrinsicsSupport.xs");
        om.bind("ti.sysbios.family.c7x.IntrinsicsSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.IntrinsicsSupport.Module", om.findStrict("ti.sysbios.interfaces.IIntrinsicsSupport.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.IntrinsicsSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.IntrinsicsSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.IntrinsicsSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.IntrinsicsSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void Settings$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/Settings.xs");
        om.bind("ti.sysbios.family.c7x.Settings$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Settings.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.Settings.Module", om.findStrict("ti.sysbios.interfaces.ISettings.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Settings$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Settings$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.Settings$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getDefaultCoreDelegate");
                if (fxn != null) po.addFxn("getDefaultCoreDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultCoreDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultHwiDelegate");
                if (fxn != null) po.addFxn("getDefaultHwiDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultHwiDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultTimerDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultClockTimerDelegate");
                if (fxn != null) po.addFxn("getDefaultClockTimerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultClockTimerDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultTimerSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerSupportDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultTimestampDelegate");
                if (fxn != null) po.addFxn("getDefaultTimestampDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimestampDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultTaskSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTaskSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTaskSupportDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultIntrinsicsSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultIntrinsicsSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultIntrinsicsSupportDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultCacheDelegate");
                if (fxn != null) po.addFxn("getDefaultCacheDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultCacheDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultPowerDelegate");
                if (fxn != null) po.addFxn("getDefaultPowerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultPowerDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultSecondsDelegate");
                if (fxn != null) po.addFxn("getDefaultSecondsDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultSecondsDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultSysCallDelegate");
                if (fxn != null) po.addFxn("getDefaultSysCallDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultSysCallDelegate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultBootModule");
                if (fxn != null) po.addFxn("getDefaultBootModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultBootModule", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultMmuModule");
                if (fxn != null) po.addFxn("getDefaultMmuModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultMmuModule", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getFamilySettingsXml");
                if (fxn != null) po.addFxn("getFamilySettingsXml", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getFamilySettingsXml", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getDefaultClockTickPeriod");
                if (fxn != null) po.addFxn("getDefaultClockTickPeriod", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultClockTickPeriod", "ti.sysbios.family.c7x"), fxn);
    }

    void TaskSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/TaskSupport.xs");
        om.bind("ti.sysbios.family.c7x.TaskSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.TaskSupport.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.TaskSupport.Module", om.findStrict("ti.sysbios.interfaces.ITaskSupport.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("defaultStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), 0x4000L, "r");
            po.addFld("stackAlignment", Proto.Elm.newCNum("(xdc_UInt)"), 0x2000L, "r");
            po.addFld("A_stackSizeTooSmall", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.c7x"), Global.newObject("msg", "A_stackSizeTooSmall: Task stack size must be >= 16KB."), "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TaskSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TaskSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TaskSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TaskSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "stackUsed$view");
                if (fxn != null) po.addFxn("stackUsed$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$stackUsed$view", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getCallStack$view");
                if (fxn != null) po.addFxn("getCallStack$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$getCallStack$view", "ti.sysbios.family.c7x"), fxn);
    }

    void TimestampProvider$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/TimestampProvider.xs");
        om.bind("ti.sysbios.family.c7x.TimestampProvider$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.TimestampProvider.Module", "ti.sysbios.family.c7x");
        po.init("ti.sysbios.family.c7x.TimestampProvider.Module", om.findStrict("ti.sysbios.interfaces.ITimestamp.Module", "ti.sysbios.family.c7x"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TimestampProvider$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TimestampProvider$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TimestampProvider$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.c7x.TimestampProvider$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getFreqMeta");
                if (fxn != null) po.addFxn("getFreqMeta", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITimestamp$$getFreqMeta", "ti.sysbios.family.c7x"), fxn);
    }

    void Cache$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Cache", "ti.sysbios.family.c7x");
        vo.bind("Size$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Cache.Size", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Cache$$Size", "ti.sysbios.family.c7x");
    }

    void Clobber$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Clobber", "ti.sysbios.family.c7x");
    }

    void Exception$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Exception", "ti.sysbios.family.c7x");
        vo.bind("Context$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Exception.Context", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Context", "ti.sysbios.family.c7x");
        vo.bind("Status$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Exception.Status", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Status", "ti.sysbios.family.c7x");
        po.bind("excContext$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Exception.Context", "isScalar", false));
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Exception.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception$$Module_State", "ti.sysbios.family.c7x");
        po.bind("excContext$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Exception.Context", "isScalar", false));
        po.bind("contextBuf$fetchDesc", Global.newObject("type", "xdc.rov.support.ScalarStructs.S_Char", "isScalar", true));
    }

    void Hwi$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Instance_State", "ti.sysbios.family.c7x");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Hwi.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Instance_State", "ti.sysbios.family.c7x");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.c7x.Hwi.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi$$Module_State", "ti.sysbios.family.c7x");
    }

    void IntrinsicsSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport", "ti.sysbios.family.c7x");
    }

    void Settings$$ROV()
    {
    }

    void TaskSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.TaskSupport", "ti.sysbios.family.c7x");
    }

    void TimestampProvider$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.TimestampProvider", "ti.sysbios.family.c7x");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.c7x.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.c7x"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/c7x/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.c7x"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.c7x"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.c7x", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.c7x");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.c7x.");
        pkgV.bind("$vers", Global.newArray(2, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.c7x'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.c7x$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.c7x$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.c7x$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void Cache$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Cache", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Cache.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.Cache", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.Cache$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("Type", om.findStrict("ti.sysbios.interfaces.ICache.Type", "ti.sysbios.family.c7x"));
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.c7x.Cache.ModuleView", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Cache.ModuleView", "ti.sysbios.family.c7x"));
        vo.bind("Mode", om.findStrict("ti.sysbios.family.c7x.Cache.Mode", "ti.sysbios.family.c7x"));
        vo.bind("L1Size", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size", "ti.sysbios.family.c7x"));
        vo.bind("L2Size", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size", "ti.sysbios.family.c7x"));
        vo.bind("Size", om.findStrict("ti.sysbios.family.c7x.Cache.Size", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Cache.Size", "ti.sysbios.family.c7x"));
        mcfgs.add("initSize");
        mcfgs.add("atomicBlockSize");
        vo.bind("Type_L1P", om.findStrict("ti.sysbios.interfaces.ICache.Type_L1P", "ti.sysbios.family.c7x"));
        vo.bind("Type_L1D", om.findStrict("ti.sysbios.interfaces.ICache.Type_L1D", "ti.sysbios.family.c7x"));
        vo.bind("Type_L1", om.findStrict("ti.sysbios.interfaces.ICache.Type_L1", "ti.sysbios.family.c7x"));
        vo.bind("Type_L2P", om.findStrict("ti.sysbios.interfaces.ICache.Type_L2P", "ti.sysbios.family.c7x"));
        vo.bind("Type_L2D", om.findStrict("ti.sysbios.interfaces.ICache.Type_L2D", "ti.sysbios.family.c7x"));
        vo.bind("Type_L2", om.findStrict("ti.sysbios.interfaces.ICache.Type_L2", "ti.sysbios.family.c7x"));
        vo.bind("Type_ALLP", om.findStrict("ti.sysbios.interfaces.ICache.Type_ALLP", "ti.sysbios.family.c7x"));
        vo.bind("Type_ALLD", om.findStrict("ti.sysbios.interfaces.ICache.Type_ALLD", "ti.sysbios.family.c7x"));
        vo.bind("Type_ALL", om.findStrict("ti.sysbios.interfaces.ICache.Type_ALL", "ti.sysbios.family.c7x"));
        vo.bind("Mode_FREEZE", om.findStrict("ti.sysbios.family.c7x.Cache.Mode_FREEZE", "ti.sysbios.family.c7x"));
        vo.bind("Mode_BYPASS", om.findStrict("ti.sysbios.family.c7x.Cache.Mode_BYPASS", "ti.sysbios.family.c7x"));
        vo.bind("Mode_NORMAL", om.findStrict("ti.sysbios.family.c7x.Cache.Mode_NORMAL", "ti.sysbios.family.c7x"));
        vo.bind("L1Size_0K", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size_0K", "ti.sysbios.family.c7x"));
        vo.bind("L1Size_4K", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size_4K", "ti.sysbios.family.c7x"));
        vo.bind("L1Size_8K", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size_8K", "ti.sysbios.family.c7x"));
        vo.bind("L1Size_16K", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size_16K", "ti.sysbios.family.c7x"));
        vo.bind("L1Size_32K", om.findStrict("ti.sysbios.family.c7x.Cache.L1Size_32K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_0K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_0K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_32K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_32K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_64K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_64K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_128K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_128K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_256K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_256K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_512K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_512K", "ti.sysbios.family.c7x"));
        vo.bind("L2Size_1024K", om.findStrict("ti.sysbios.family.c7x.Cache.L2Size_1024K", "ti.sysbios.family.c7x"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("enable", om.findStrict("ti.sysbios.family.c7x.Cache.enable", "ti.sysbios.family.c7x"));
        vo.bind("inv", om.findStrict("ti.sysbios.family.c7x.Cache.inv", "ti.sysbios.family.c7x"));
        vo.bind("wb", om.findStrict("ti.sysbios.family.c7x.Cache.wb", "ti.sysbios.family.c7x"));
        vo.bind("wbInv", om.findStrict("ti.sysbios.family.c7x.Cache.wbInv", "ti.sysbios.family.c7x"));
        vo.bind("wait", om.findStrict("ti.sysbios.family.c7x.Cache.wait", "ti.sysbios.family.c7x"));
        vo.bind("disable", om.findStrict("ti.sysbios.family.c7x.Cache.disable", "ti.sysbios.family.c7x"));
        vo.bind("getSize", om.findStrict("ti.sysbios.family.c7x.Cache.getSize", "ti.sysbios.family.c7x"));
        vo.bind("setSize", om.findStrict("ti.sysbios.family.c7x.Cache.setSize", "ti.sysbios.family.c7x"));
        vo.bind("wbAll", om.findStrict("ti.sysbios.family.c7x.Cache.wbAll", "ti.sysbios.family.c7x"));
        vo.bind("wbL1dAll", om.findStrict("ti.sysbios.family.c7x.Cache.wbL1dAll", "ti.sysbios.family.c7x"));
        vo.bind("wbInvAll", om.findStrict("ti.sysbios.family.c7x.Cache.wbInvAll", "ti.sysbios.family.c7x"));
        vo.bind("wbInvL1dAll", om.findStrict("ti.sysbios.family.c7x.Cache.wbInvL1dAll", "ti.sysbios.family.c7x"));
        vo.bind("setL2CFG", om.findStrict("ti.sysbios.family.c7x.Cache.setL2CFG", "ti.sysbios.family.c7x"));
        vo.bind("getL2CFG", om.findStrict("ti.sysbios.family.c7x.Cache.getL2CFG", "ti.sysbios.family.c7x"));
        vo.bind("setL1DCFG", om.findStrict("ti.sysbios.family.c7x.Cache.setL1DCFG", "ti.sysbios.family.c7x"));
        vo.bind("getL1DCFG", om.findStrict("ti.sysbios.family.c7x.Cache.getL1DCFG", "ti.sysbios.family.c7x"));
        vo.bind("setL2WB", om.findStrict("ti.sysbios.family.c7x.Cache.setL2WB", "ti.sysbios.family.c7x"));
        vo.bind("setL2WBINV", om.findStrict("ti.sysbios.family.c7x.Cache.setL2WBINV", "ti.sysbios.family.c7x"));
        vo.bind("setL1DWB", om.findStrict("ti.sysbios.family.c7x.Cache.setL1DWB", "ti.sysbios.family.c7x"));
        vo.bind("setL1DWBINV", om.findStrict("ti.sysbios.family.c7x.Cache.setL1DWBINV", "ti.sysbios.family.c7x"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.c7x.Cache.startup", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_Cache_Module__startupDone__E", "ti_sysbios_family_c7x_Cache_enable__E", "ti_sysbios_family_c7x_Cache_inv__E", "ti_sysbios_family_c7x_Cache_wb__E", "ti_sysbios_family_c7x_Cache_wbInv__E", "ti_sysbios_family_c7x_Cache_wait__E", "ti_sysbios_family_c7x_Cache_disable__E", "ti_sysbios_family_c7x_Cache_getSize__E", "ti_sysbios_family_c7x_Cache_setSize__E", "ti_sysbios_family_c7x_Cache_wbAll__E", "ti_sysbios_family_c7x_Cache_wbL1dAll__E", "ti_sysbios_family_c7x_Cache_wbInvAll__E", "ti_sysbios_family_c7x_Cache_wbInvL1dAll__E", "ti_sysbios_family_c7x_Cache_setL2CFG__E", "ti_sysbios_family_c7x_Cache_getL2CFG__E", "ti_sysbios_family_c7x_Cache_setL1DCFG__E", "ti_sysbios_family_c7x_Cache_getL1DCFG__E", "ti_sysbios_family_c7x_Cache_setL2WB__E", "ti_sysbios_family_c7x_Cache_setL2WBINV__E", "ti_sysbios_family_c7x_Cache_setL1DWB__E", "ti_sysbios_family_c7x_Cache_setL1DWBINV__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Cache", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Cache");
    }

    void Clobber$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Clobber", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Clobber.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.Clobber", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.Clobber$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        mcfgs.add("postIntrCheck");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
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
        vo.bind("trashRegs", om.findStrict("ti.sysbios.family.c7x.Clobber.trashRegs", "ti.sysbios.family.c7x"));
        vo.bind("postIntr", om.findStrict("ti.sysbios.family.c7x.Clobber.postIntr", "ti.sysbios.family.c7x"));
        vo.bind("checkRegs", om.findStrict("ti.sysbios.family.c7x.Clobber.checkRegs", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_Clobber_Module__startupDone__E", "ti_sysbios_family_c7x_Clobber_trashRegs__E", "ti_sysbios_family_c7x_Clobber_postIntr__E", "ti_sysbios_family_c7x_Clobber_checkRegs__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        loggables.add(Global.newObject("name", "trashRegs", "entry", "0x%x", "exit", ""));
        loggables.add(Global.newObject("name", "postIntr", "entry", "0x%x", "exit", ""));
        loggables.add(Global.newObject("name", "checkRegs", "entry", "0x%x", "exit", "0x%x"));
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Clobber", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Clobber");
    }

    void Exception$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Exception", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Exception.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.Exception", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.Exception$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.c7x.Exception.ModuleView", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Exception.ModuleView", "ti.sysbios.family.c7x"));
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.family.c7x.Exception.FuncPtr", "ti.sysbios.family.c7x"));
        vo.bind("Context", om.findStrict("ti.sysbios.family.c7x.Exception.Context", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Exception.Context", "ti.sysbios.family.c7x"));
        vo.bind("Status", om.findStrict("ti.sysbios.family.c7x.Exception.Status", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Exception.Status", "ti.sysbios.family.c7x"));
        mcfgs.add("E_exceptionMin");
        mcfgs.add("E_exceptionMax");
        mcfgs.add("useInternalBuffer");
        mcfgs.add("enableExternalMPC");
        mcfgs.add("enablePrint");
        mcfgs.add("exceptionHook");
        mcfgs.add("internalHook");
        mcfgs.add("externalHook");
        mcfgs.add("nmiHook");
        mcfgs.add("returnHook");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.c7x.Exception.Module_State", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Exception.Module_State", "ti.sysbios.family.c7x"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
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
        vo.bind("getLastStatus", om.findStrict("ti.sysbios.family.c7x.Exception.getLastStatus", "ti.sysbios.family.c7x"));
        vo.bind("clearLastStatus", om.findStrict("ti.sysbios.family.c7x.Exception.clearLastStatus", "ti.sysbios.family.c7x"));
        vo.bind("setReturnPtr", om.findStrict("ti.sysbios.family.c7x.Exception.setReturnPtr", "ti.sysbios.family.c7x"));
        vo.bind("evtEvtClear", om.findStrict("ti.sysbios.family.c7x.Exception.evtEvtClear", "ti.sysbios.family.c7x"));
        vo.bind("evtExpMaskEnable", om.findStrict("ti.sysbios.family.c7x.Exception.evtExpMaskEnable", "ti.sysbios.family.c7x"));
        vo.bind("dispatch", om.findStrict("ti.sysbios.family.c7x.Exception.dispatch", "ti.sysbios.family.c7x"));
        vo.bind("handler", om.findStrict("ti.sysbios.family.c7x.Exception.handler", "ti.sysbios.family.c7x"));
        vo.bind("internalHandler", om.findStrict("ti.sysbios.family.c7x.Exception.internalHandler", "ti.sysbios.family.c7x"));
        vo.bind("externalHandler", om.findStrict("ti.sysbios.family.c7x.Exception.externalHandler", "ti.sysbios.family.c7x"));
        vo.bind("nmiHandler", om.findStrict("ti.sysbios.family.c7x.Exception.nmiHandler", "ti.sysbios.family.c7x"));
        vo.bind("decodeMpfsr", om.findStrict("ti.sysbios.family.c7x.Exception.decodeMpfsr", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_Exception_Module__startupDone__E", "ti_sysbios_family_c7x_Exception_getLastStatus__E", "ti_sysbios_family_c7x_Exception_clearLastStatus__E", "ti_sysbios_family_c7x_Exception_setReturnPtr__E", "ti_sysbios_family_c7x_Exception_evtEvtClear__E", "ti_sysbios_family_c7x_Exception_evtExpMaskEnable__E", "ti_sysbios_family_c7x_Exception_dispatch__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_exceptionMin", "E_exceptionMax"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Exception", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Exception");
    }

    void Hwi$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.Hwi", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.Hwi$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("Instance", om.findStrict("ti.sysbios.family.c7x.Hwi.Instance", "ti.sysbios.family.c7x"));
        vo.bind("Params", om.findStrict("ti.sysbios.family.c7x.Hwi.Params", "ti.sysbios.family.c7x"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sysbios.family.c7x.Hwi.Params", "ti.sysbios.family.c7x")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sysbios.family.c7x.Hwi.Handle", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.IHwi.FuncPtr", "ti.sysbios.family.c7x"));
        vo.bind("Irp", om.findStrict("ti.sysbios.interfaces.IHwi.Irp", "ti.sysbios.family.c7x"));
        vo.bind("HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.c7x"));
        vo.bind("StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.c7x"));
        mcfgs.add("dispatcherAutoNestingSupport");
        mcfgs.add("dispatcherSwiSupport");
        mcfgs.add("dispatcherTaskSupport");
        mcfgs.add("dispatcherIrpTrackingSupport");
        mcfgs.add("DEFAULT_INT_PRIORITY");
        vo.bind("PlugFuncPtr", om.findStrict("ti.sysbios.family.c7x.Hwi.PlugFuncPtr", "ti.sysbios.family.c7x"));
        vo.bind("BasicView", om.findStrict("ti.sysbios.family.c7x.Hwi.BasicView", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.BasicView", "ti.sysbios.family.c7x"));
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.c7x.Hwi.ModuleView", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.ModuleView", "ti.sysbios.family.c7x"));
        vo.bind("NonDispatchedInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.NonDispatchedInterrupt", "ti.sysbios.family.c7x"));
        mcfgs.add("E_alreadyDefined");
        mcfgs.add("E_handleNotFound");
        mcfgs.add("E_allocSCFailed");
        mcfgs.add("E_registerSCFailed");
        mcfgs.add("E_invalidIntNum");
        mcfgs.add("LM_begin");
        mcfgs.add("LD_end");
        mcfgs.add("enableException");
        mcfgs.add("swiDisable");
        icfgs.add("swiDisable");
        mcfgs.add("swiRestoreHwi");
        icfgs.add("swiRestoreHwi");
        mcfgs.add("taskDisable");
        icfgs.add("taskDisable");
        mcfgs.add("taskRestoreHwi");
        icfgs.add("taskRestoreHwi");
        mcfgs.add("hooks");
        icfgs.add("hooks");
        vo.bind("InterruptObj", om.findStrict("ti.sysbios.family.c7x.Hwi.InterruptObj", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.InterruptObj", "ti.sysbios.family.c7x"));
        icfgs.add("interrupt");
        vo.bind("Instance_State", om.findStrict("ti.sysbios.family.c7x.Hwi.Instance_State", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.Instance_State", "ti.sysbios.family.c7x"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.c7x.Hwi.Module_State", "ti.sysbios.family.c7x"));
        tdefs.add(om.findStrict("ti.sysbios.family.c7x.Hwi.Module_State", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption_NONE", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_NONE", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption_ALL", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_ALL", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption_SELF", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_SELF", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption_BITMASK", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_BITMASK", "ti.sysbios.family.c7x"));
        vo.bind("MaskingOption_LOWER", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_LOWER", "ti.sysbios.family.c7x"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", om.has("ti.sysbios.family.c7x.Hwi$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sysbios$family$c7x$Hwi$$__initObject"));
        }//isCFG
        vo.bind("getStackInfo", om.findStrict("ti.sysbios.family.c7x.Hwi.getStackInfo", "ti.sysbios.family.c7x"));
        vo.bind("getCoreStackInfo", om.findStrict("ti.sysbios.family.c7x.Hwi.getCoreStackInfo", "ti.sysbios.family.c7x"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.c7x.Hwi.startup", "ti.sysbios.family.c7x"));
        vo.bind("switchFromBootStack", om.findStrict("ti.sysbios.family.c7x.Hwi.switchFromBootStack", "ti.sysbios.family.c7x"));
        vo.bind("post", om.findStrict("ti.sysbios.family.c7x.Hwi.post", "ti.sysbios.family.c7x"));
        vo.bind("getTaskSP", om.findStrict("ti.sysbios.family.c7x.Hwi.getTaskSP", "ti.sysbios.family.c7x"));
        vo.bind("disableInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.disableInterrupt", "ti.sysbios.family.c7x"));
        vo.bind("enableInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.enableInterrupt", "ti.sysbios.family.c7x"));
        vo.bind("restoreInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.restoreInterrupt", "ti.sysbios.family.c7x"));
        vo.bind("clearInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.clearInterrupt", "ti.sysbios.family.c7x"));
        vo.bind("eventMap", om.findStrict("ti.sysbios.family.c7x.Hwi.eventMap", "ti.sysbios.family.c7x"));
        vo.bind("plug", om.findStrict("ti.sysbios.family.c7x.Hwi.plug", "ti.sysbios.family.c7x"));
        vo.bind("getEventId", om.findStrict("ti.sysbios.family.c7x.Hwi.getEventId", "ti.sysbios.family.c7x"));
        vo.bind("getHandle", om.findStrict("ti.sysbios.family.c7x.Hwi.getHandle", "ti.sysbios.family.c7x"));
        vo.bind("disableIER", om.findStrict("ti.sysbios.family.c7x.Hwi.disableIER", "ti.sysbios.family.c7x"));
        vo.bind("enableIER", om.findStrict("ti.sysbios.family.c7x.Hwi.enableIER", "ti.sysbios.family.c7x"));
        vo.bind("restoreIER", om.findStrict("ti.sysbios.family.c7x.Hwi.restoreIER", "ti.sysbios.family.c7x"));
        vo.bind("setPriority", om.findStrict("ti.sysbios.family.c7x.Hwi.setPriority", "ti.sysbios.family.c7x"));
        vo.bind("setCOP", om.findStrict("ti.sysbios.family.c7x.Hwi.setCOP", "ti.sysbios.family.c7x"));
        vo.bind("getIsrStackAddress", om.findStrict("ti.sysbios.family.c7x.Hwi.getIsrStackAddress", "ti.sysbios.family.c7x"));
        vo.bind("dispatchC", om.findStrict("ti.sysbios.family.c7x.Hwi.dispatchC", "ti.sysbios.family.c7x"));
        vo.bind("dispatchCore", om.findStrict("ti.sysbios.family.c7x.Hwi.dispatchCore", "ti.sysbios.family.c7x"));
        vo.bind("unPluggedInterrupt", om.findStrict("ti.sysbios.family.c7x.Hwi.unPluggedInterrupt", "ti.sysbios.family.c7x"));
        vo.bind("switchAndDispatch", om.findStrict("ti.sysbios.family.c7x.Hwi.switchAndDispatch", "ti.sysbios.family.c7x"));
        vo.bind("setupSC", om.findStrict("ti.sysbios.family.c7x.Hwi.setupSC", "ti.sysbios.family.c7x"));
        vo.bind("postInit", om.findStrict("ti.sysbios.family.c7x.Hwi.postInit", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_Hwi_Handle__label__E", "ti_sysbios_family_c7x_Hwi_Module__startupDone__E", "ti_sysbios_family_c7x_Hwi_Object__create__E", "ti_sysbios_family_c7x_Hwi_Object__delete__E", "ti_sysbios_family_c7x_Hwi_Object__get__E", "ti_sysbios_family_c7x_Hwi_Object__first__E", "ti_sysbios_family_c7x_Hwi_Object__next__E", "ti_sysbios_family_c7x_Hwi_Params__init__E", "ti_sysbios_family_c7x_Hwi_getStackInfo__E", "ti_sysbios_family_c7x_Hwi_getCoreStackInfo__E", "ti_sysbios_family_c7x_Hwi_startup__E", "ti_sysbios_family_c7x_Hwi_switchFromBootStack__E", "ti_sysbios_family_c7x_Hwi_post__E", "ti_sysbios_family_c7x_Hwi_getTaskSP__E", "ti_sysbios_family_c7x_Hwi_disableInterrupt__E", "ti_sysbios_family_c7x_Hwi_enableInterrupt__E", "ti_sysbios_family_c7x_Hwi_restoreInterrupt__E", "ti_sysbios_family_c7x_Hwi_clearInterrupt__E", "ti_sysbios_family_c7x_Hwi_getFunc__E", "ti_sysbios_family_c7x_Hwi_setFunc__E", "ti_sysbios_family_c7x_Hwi_getHookContext__E", "ti_sysbios_family_c7x_Hwi_setHookContext__E", "ti_sysbios_family_c7x_Hwi_getIrp__E", "ti_sysbios_family_c7x_Hwi_disable__E", "ti_sysbios_family_c7x_Hwi_enable__E", "ti_sysbios_family_c7x_Hwi_restore__E", "ti_sysbios_family_c7x_Hwi_eventMap__E", "ti_sysbios_family_c7x_Hwi_plug__E", "ti_sysbios_family_c7x_Hwi_getEventId__E", "ti_sysbios_family_c7x_Hwi_getHandle__E", "ti_sysbios_family_c7x_Hwi_disableIER__E", "ti_sysbios_family_c7x_Hwi_enableIER__E", "ti_sysbios_family_c7x_Hwi_restoreIER__E", "ti_sysbios_family_c7x_Hwi_setPriority__E", "ti_sysbios_family_c7x_Hwi_setCOP__E", "ti_sysbios_family_c7x_Hwi_reconfig__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("LM_begin", "LD_end"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_alreadyDefined", "E_handleNotFound", "E_allocSCFailed", "E_registerSCFailed", "E_invalidIntNum"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "./Hwi.xdt");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sysbios.family.c7x.Hwi.Object", "ti.sysbios.family.c7x"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./Hwi.xdt");
        pkgV.bind("Hwi", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Hwi");
    }

    void IntrinsicsSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.IntrinsicsSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("maxbit", om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport.maxbit", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_IntrinsicsSupport_Module__startupDone__E", "ti_sysbios_family_c7x_IntrinsicsSupport_maxbit__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("IntrinsicsSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("IntrinsicsSupport");
    }

    void Settings$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Settings", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.Settings.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.Settings", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.Settings$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sysbios.family.c7x.Settings$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("Settings", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Settings");
    }

    void TaskSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.TaskSupport", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.TaskSupport.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.TaskSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.TaskSupport$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITaskSupport.FuncPtr", "ti.sysbios.family.c7x"));
        mcfgs.add("defaultStackSize");
        mcfgs.add("stackAlignment");
        mcfgs.add("A_stackSizeTooSmall");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("start", om.findStrict("ti.sysbios.family.c7x.TaskSupport.start", "ti.sysbios.family.c7x"));
        vo.bind("swap", om.findStrict("ti.sysbios.family.c7x.TaskSupport.swap", "ti.sysbios.family.c7x"));
        vo.bind("checkStack", om.findStrict("ti.sysbios.family.c7x.TaskSupport.checkStack", "ti.sysbios.family.c7x"));
        vo.bind("stackUsed", om.findStrict("ti.sysbios.family.c7x.TaskSupport.stackUsed", "ti.sysbios.family.c7x"));
        vo.bind("getStackAlignment", om.findStrict("ti.sysbios.family.c7x.TaskSupport.getStackAlignment", "ti.sysbios.family.c7x"));
        vo.bind("getDefaultStackSize", om.findStrict("ti.sysbios.family.c7x.TaskSupport.getDefaultStackSize", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_TaskSupport_Module__startupDone__E", "ti_sysbios_family_c7x_TaskSupport_start__E", "ti_sysbios_family_c7x_TaskSupport_swap__E", "ti_sysbios_family_c7x_TaskSupport_checkStack__E", "ti_sysbios_family_c7x_TaskSupport_stackUsed__E", "ti_sysbios_family_c7x_TaskSupport_getStackAlignment__E", "ti_sysbios_family_c7x_TaskSupport_getDefaultStackSize__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray("A_stackSizeTooSmall"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("TaskSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TaskSupport");
    }

    void TimestampProvider$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.TimestampProvider", "ti.sysbios.family.c7x");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.c7x.TimestampProvider.Module", "ti.sysbios.family.c7x");
        vo.init2(po, "ti.sysbios.family.c7x.TimestampProvider", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.c7x.TimestampProvider$$capsule", "ti.sysbios.family.c7x"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.c7x", "ti.sysbios.family.c7x"));
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
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.c7x")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("get32", om.findStrict("ti.sysbios.family.c7x.TimestampProvider.get32", "ti.sysbios.family.c7x"));
        vo.bind("get64", om.findStrict("ti.sysbios.family.c7x.TimestampProvider.get64", "ti.sysbios.family.c7x"));
        vo.bind("getFreq", om.findStrict("ti.sysbios.family.c7x.TimestampProvider.getFreq", "ti.sysbios.family.c7x"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_c7x_TimestampProvider_Module__startupDone__E", "ti_sysbios_family_c7x_TimestampProvider_get32__E", "ti_sysbios_family_c7x_TimestampProvider_get64__E", "ti_sysbios_family_c7x_TimestampProvider_getFreq__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("TimestampProvider", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TimestampProvider");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sysbios.family.c7x")).findStrict("PARAMS", "ti.sysbios.family.c7x");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sysbios.family.c7x.Hwi", "ti.sysbios.family.c7x")).findStrict("PARAMS", "ti.sysbios.family.c7x");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.Cache", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.Clobber", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.Exception", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.Hwi", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.IntrinsicsSupport", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.Settings", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.TaskSupport", "ti.sysbios.family.c7x"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.c7x.TimestampProvider", "ti.sysbios.family.c7x"));
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Cache", "ti.sysbios.family.c7x");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Exception", "ti.sysbios.family.c7x");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Exception", Global.newObject("type", om.find("xdc.rov.ViewInfo.TREE"), "viewInitFxn", "viewInitException", "structName", "Context")}), Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.c7x.Hwi", "ti.sysbios.family.c7x");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.INSTANCE"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")}), Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")})}))));
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.Cache")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.Clobber")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.Exception")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.Hwi")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.IntrinsicsSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.Settings")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.TaskSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.c7x.TimestampProvider")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.c7x")).add(pkgV);
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
        Cache$$OBJECTS();
        Clobber$$OBJECTS();
        Exception$$OBJECTS();
        Hwi$$OBJECTS();
        IntrinsicsSupport$$OBJECTS();
        Settings$$OBJECTS();
        TaskSupport$$OBJECTS();
        TimestampProvider$$OBJECTS();
        Cache$$CONSTS();
        Clobber$$CONSTS();
        Exception$$CONSTS();
        Hwi$$CONSTS();
        IntrinsicsSupport$$CONSTS();
        Settings$$CONSTS();
        TaskSupport$$CONSTS();
        TimestampProvider$$CONSTS();
        Cache$$CREATES();
        Clobber$$CREATES();
        Exception$$CREATES();
        Hwi$$CREATES();
        IntrinsicsSupport$$CREATES();
        Settings$$CREATES();
        TaskSupport$$CREATES();
        TimestampProvider$$CREATES();
        Cache$$FUNCTIONS();
        Clobber$$FUNCTIONS();
        Exception$$FUNCTIONS();
        Hwi$$FUNCTIONS();
        IntrinsicsSupport$$FUNCTIONS();
        Settings$$FUNCTIONS();
        TaskSupport$$FUNCTIONS();
        TimestampProvider$$FUNCTIONS();
        Cache$$SIZES();
        Clobber$$SIZES();
        Exception$$SIZES();
        Hwi$$SIZES();
        IntrinsicsSupport$$SIZES();
        Settings$$SIZES();
        TaskSupport$$SIZES();
        TimestampProvider$$SIZES();
        Cache$$TYPES();
        Clobber$$TYPES();
        Exception$$TYPES();
        Hwi$$TYPES();
        IntrinsicsSupport$$TYPES();
        Settings$$TYPES();
        TaskSupport$$TYPES();
        TimestampProvider$$TYPES();
        if (isROV) {
            Cache$$ROV();
            Clobber$$ROV();
            Exception$$ROV();
            Hwi$$ROV();
            IntrinsicsSupport$$ROV();
            Settings$$ROV();
            TaskSupport$$ROV();
            TimestampProvider$$ROV();
        }//isROV
        $$SINGLETONS();
        Cache$$SINGLETONS();
        Clobber$$SINGLETONS();
        Exception$$SINGLETONS();
        Hwi$$SINGLETONS();
        IntrinsicsSupport$$SINGLETONS();
        Settings$$SINGLETONS();
        TaskSupport$$SINGLETONS();
        TimestampProvider$$SINGLETONS();
        $$INITIALIZATION();
    }
}
