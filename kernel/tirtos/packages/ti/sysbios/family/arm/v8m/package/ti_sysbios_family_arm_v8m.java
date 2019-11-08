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

public class ti_sysbios_family_arm_v8m
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
        Global.callFxn("loadPackage", xdcO, "ti.sysbios");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m", new Value.Obj("ti.sysbios.family.arm.v8m", pkgP));
    }

    void Hwi$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi", new Value.Obj("ti.sysbios.family.arm.v8m.Hwi", po));
        pkgV.bind("Hwi", vo);
        // decls 
        om.bind("ti.sysbios.family.arm.v8m.Hwi.HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.arm.v8m"));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.arm.v8m"));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.arm.v8m"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$CCR", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.CCR", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$NVIC", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.NVIC", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$ExcContext", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.ExcContext", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$Struct2__", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Struct2__", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$BasicView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.BasicView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$DetailedView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.DetailedView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.ModuleView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$VectorTableView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.VectorTableView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$InterruptObj", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.InterruptObj", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$Instance_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sysbios.family.arm.v8m.Hwi.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$Object", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Hwi$$Params", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Params", new Proto.Str(po, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Handle", insP);
        if (isROV) {
            om.bind("ti.sysbios.family.arm.v8m.Hwi.Object", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance_State", "ti.sysbios.family.arm.v8m"));
        }//isROV
    }

    void IntrinsicsSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport", new Value.Obj("ti.sysbios.family.arm.v8m.IntrinsicsSupport", po));
        pkgV.bind("IntrinsicsSupport", vo);
        // decls 
    }

    void TaskSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.TaskSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.TaskSupport", new Value.Obj("ti.sysbios.family.arm.v8m.TaskSupport", po));
        pkgV.bind("TaskSupport", vo);
        // decls 
    }

    void Timer$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer", new Value.Obj("ti.sysbios.family.arm.v8m.Timer", po));
        pkgV.bind("Timer", vo);
        // decls 
        om.bind("ti.sysbios.family.arm.v8m.Timer.StartMode", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode", "ti.sysbios.family.arm.v8m"));
        om.bind("ti.sysbios.family.arm.v8m.Timer.RunMode", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode", "ti.sysbios.family.arm.v8m"));
        om.bind("ti.sysbios.family.arm.v8m.Timer.Status", om.findStrict("ti.sysbios.interfaces.ITimer.Status", "ti.sysbios.family.arm.v8m"));
        om.bind("ti.sysbios.family.arm.v8m.Timer.PeriodType", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType", "ti.sysbios.family.arm.v8m"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$BasicView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.BasicView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.ModuleView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$DeviceView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.DeviceView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$Instance_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.Instance_State", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.Module_State", new Proto.Str(spo, false));
        // insts 
        Object insP = om.bind("ti.sysbios.family.arm.v8m.Timer.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$Object", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.Object", new Proto.Str(po, false));
        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Timer$$Params", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.Timer.Params", new Proto.Str(po, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.Handle", insP);
        if (isROV) {
            om.bind("ti.sysbios.family.arm.v8m.Timer.Object", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance_State", "ti.sysbios.family.arm.v8m"));
        }//isROV
    }

    void TimestampProvider$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.TimestampProvider", new Value.Obj("ti.sysbios.family.arm.v8m.TimestampProvider", po));
        pkgV.bind("TimestampProvider", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", new Proto.Str(spo, false));
    }

    void Power$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.Power.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.Power", new Value.Obj("ti.sysbios.family.arm.v8m.Power", po));
        pkgV.bind("Power", vo);
        // decls 
    }

    void SAU$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU", new Value.Obj("ti.sysbios.family.arm.v8m.SAU", po));
        pkgV.bind("SAU", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU$$RegionsView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegionsView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU$$RegsView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegsView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU$$DeviceRegs", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.SAU.DeviceRegs", new Proto.Str(spo, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegionType", new Proto.Enm("ti.sysbios.family.arm.v8m.SAU.RegionType"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU$$RegionEntry", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegionEntry", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v8m.SAU$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v8m.SAU.Module_State", new Proto.Str(spo, false));
    }

    void Hwi$$CONSTS()
    {
        // module Hwi
        om.bind("ti.sysbios.family.arm.v8m.Hwi.nvic", new Extern("ti_sysbios_family_arm_v8m_Hwi_nvic", "ti_sysbios_family_arm_v8m_Hwi_NVIC*", false, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.getStackInfo", new Extern("ti_sysbios_family_arm_v8m_Hwi_getStackInfo__E", "xdc_Bool(*)(ti_sysbios_interfaces_IHwi_StackInfo*,xdc_Bool)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.getCoreStackInfo", new Extern("ti_sysbios_family_arm_v8m_Hwi_getCoreStackInfo__E", "xdc_Bool(*)(ti_sysbios_interfaces_IHwi_StackInfo*,xdc_Bool,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.startup", new Extern("ti_sysbios_family_arm_v8m_Hwi_startup__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.switchFromBootStack", new Extern("ti_sysbios_family_arm_v8m_Hwi_switchFromBootStack__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.post", new Extern("ti_sysbios_family_arm_v8m_Hwi_post__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.getTaskSP", new Extern("ti_sysbios_family_arm_v8m_Hwi_getTaskSP__E", "xdc_Char*(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.disableInterrupt", new Extern("ti_sysbios_family_arm_v8m_Hwi_disableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.enableInterrupt", new Extern("ti_sysbios_family_arm_v8m_Hwi_enableInterrupt__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.restoreInterrupt", new Extern("ti_sysbios_family_arm_v8m_Hwi_restoreInterrupt__E", "xdc_Void(*)(xdc_UInt,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.clearInterrupt", new Extern("ti_sysbios_family_arm_v8m_Hwi_clearInterrupt__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.construct2", new Extern("ti_sysbios_family_arm_v8m_Hwi_construct2__E", "ti_sysbios_family_arm_v8m_Hwi_Handle(*)(ti_sysbios_family_arm_v8m_Hwi_Struct2__*,xdc_Int,xdc_Void(*)(xdc_UArg),ti_sysbios_family_arm_v8m_Hwi_Params*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.disableFxn", new Extern("ti_sysbios_family_arm_v8m_Hwi_disableFxn__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.enableFxn", new Extern("ti_sysbios_family_arm_v8m_Hwi_enableFxn__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.restoreFxn", new Extern("ti_sysbios_family_arm_v8m_Hwi_restoreFxn__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.plug", new Extern("ti_sysbios_family_arm_v8m_Hwi_plug__E", "xdc_Void(*)(xdc_UInt,xdc_Void*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.getHandle", new Extern("ti_sysbios_family_arm_v8m_Hwi_getHandle__E", "ti_sysbios_family_arm_v8m_Hwi_Handle(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.setPriority", new Extern("ti_sysbios_family_arm_v8m_Hwi_setPriority__E", "xdc_Void(*)(xdc_UInt,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excSetBuffers", new Extern("ti_sysbios_family_arm_v8m_Hwi_excSetBuffers__E", "xdc_Void(*)(xdc_Ptr,xdc_Ptr)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.initNVIC", new Extern("ti_sysbios_family_arm_v8m_Hwi_initNVIC__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.initStacks", new Extern("ti_sysbios_family_arm_v8m_Hwi_initStacks__E", "xdc_Void(*)(xdc_Ptr)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.postInit", new Extern("ti_sysbios_family_arm_v8m_Hwi_postInit__I", "xdc_Int(*)(ti_sysbios_family_arm_v8m_Hwi_Object*,xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excHandlerAsm", new Extern("ti_sysbios_family_arm_v8m_Hwi_excHandlerAsm__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excHandler", new Extern("ti_sysbios_family_arm_v8m_Hwi_excHandler__I", "xdc_Void(*)(xdc_UInt*,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excHandlerMin", new Extern("ti_sysbios_family_arm_v8m_Hwi_excHandlerMin__I", "xdc_Void(*)(xdc_UInt*,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excHandlerMax", new Extern("ti_sysbios_family_arm_v8m_Hwi_excHandlerMax__I", "xdc_Void(*)(xdc_UInt*,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excFillContext", new Extern("ti_sysbios_family_arm_v8m_Hwi_excFillContext__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excNmi", new Extern("ti_sysbios_family_arm_v8m_Hwi_excNmi__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excHardFault", new Extern("ti_sysbios_family_arm_v8m_Hwi_excHardFault__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excMemFault", new Extern("ti_sysbios_family_arm_v8m_Hwi_excMemFault__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excBusFault", new Extern("ti_sysbios_family_arm_v8m_Hwi_excBusFault__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excUsageFault", new Extern("ti_sysbios_family_arm_v8m_Hwi_excUsageFault__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excSvCall", new Extern("ti_sysbios_family_arm_v8m_Hwi_excSvCall__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excDebugMon", new Extern("ti_sysbios_family_arm_v8m_Hwi_excDebugMon__I", "xdc_Void(*)(xdc_UInt*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excReserved", new Extern("ti_sysbios_family_arm_v8m_Hwi_excReserved__I", "xdc_Void(*)(xdc_UInt*,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excNoIsr", new Extern("ti_sysbios_family_arm_v8m_Hwi_excNoIsr__I", "xdc_Void(*)(xdc_UInt*,xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.excDumpRegs", new Extern("ti_sysbios_family_arm_v8m_Hwi_excDumpRegs__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.pendSV", new Extern("ti_sysbios_family_arm_v8m_Hwi_pendSV__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.setStackLimit", new Extern("ti_sysbios_family_arm_v8m_Hwi_setStackLimit__I", "xdc_Void(*)(xdc_Ptr)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.dispatch", new Extern("ti_sysbios_family_arm_v8m_Hwi_dispatch__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.romInitNVIC", new Extern("ti_sysbios_family_arm_v8m_Hwi_romInitNVIC__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.dispatchC", new Extern("ti_sysbios_family_arm_v8m_Hwi_dispatchC__I", "xdc_UInt(*)(xdc_UArg,xdc_UInt32,xdc_UInt32,ti_sysbios_family_arm_v8m_Hwi_Object*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.doSwiRestore", new Extern("ti_sysbios_family_arm_v8m_Hwi_doSwiRestore__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.doTaskRestore", new Extern("ti_sysbios_family_arm_v8m_Hwi_doTaskRestore__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.enableNonSecureFloatingPoint", new Extern("ti_sysbios_family_arm_v8m_Hwi_enableNonSecureFloatingPoint__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Hwi.enableNonSecureFaultHandlers", new Extern("ti_sysbios_family_arm_v8m_Hwi_enableNonSecureFaultHandlers__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void IntrinsicsSupport$$CONSTS()
    {
        // module IntrinsicsSupport
        om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport.maxbit", new Extern("ti_sysbios_family_arm_v8m_IntrinsicsSupport_maxbit__E", "xdc_UInt(*)(xdc_UInt)", true, false));
    }

    void TaskSupport$$CONSTS()
    {
        // module TaskSupport
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.start", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_start__E", "xdc_Ptr(*)(xdc_Ptr,xdc_Void(*)(xdc_Void),xdc_Void(*)(xdc_Void),xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.swap", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_swap__E", "xdc_Void(*)(xdc_Ptr*,xdc_Ptr*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.checkStack", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_checkStack__E", "xdc_Bool(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.stackUsed", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_stackUsed__E", "xdc_SizeT(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.getStackAlignment", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_getStackAlignment__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport.getDefaultStackSize", new Extern("ti_sysbios_family_arm_v8m_TaskSupport_getDefaultStackSize__E", "xdc_SizeT(*)(xdc_Void)", true, false));
    }

    void Timer$$CONSTS()
    {
        // module Timer
        om.bind("ti.sysbios.family.arm.v8m.Timer.MAX_PERIOD", 0x00ffffffL);
        om.bind("ti.sysbios.family.arm.v8m.Timer.NUM_TIMER_DEVICES", 1L);
        om.bind("ti.sysbios.family.arm.v8m.Timer.TIMER_CLOCK_DIVIDER", 1L);
        om.bind("ti.sysbios.family.arm.v8m.Timer.MIN_SWEEP_PERIOD", 1L);
        om.bind("ti.sysbios.family.arm.v8m.Timer.getNumTimers", new Extern("ti_sysbios_family_arm_v8m_Timer_getNumTimers__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.getStatus", new Extern("ti_sysbios_family_arm_v8m_Timer_getStatus__E", "ti_sysbios_interfaces_ITimer_Status(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.startup", new Extern("ti_sysbios_family_arm_v8m_Timer_startup__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.getHandle", new Extern("ti_sysbios_family_arm_v8m_Timer_getHandle__E", "ti_sysbios_family_arm_v8m_Timer_Handle(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.getTickCount", new Extern("ti_sysbios_family_arm_v8m_Timer_getTickCount__E", "xdc_UInt32(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.initDevice", new Extern("ti_sysbios_family_arm_v8m_Timer_initDevice__I", "xdc_Void(*)(ti_sysbios_family_arm_v8m_Timer_Object*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.postInit", new Extern("ti_sysbios_family_arm_v8m_Timer_postInit__I", "xdc_Int(*)(ti_sysbios_family_arm_v8m_Timer_Object*,xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.oneShotStub", new Extern("ti_sysbios_family_arm_v8m_Timer_oneShotStub__I", "xdc_Void(*)(xdc_UArg)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.Timer.periodicStub", new Extern("ti_sysbios_family_arm_v8m_Timer_periodicStub__I", "xdc_Void(*)(xdc_UArg)", true, false));
    }

    void TimestampProvider$$CONSTS()
    {
        // module TimestampProvider
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.get32", new Extern("ti_sysbios_family_arm_v8m_TimestampProvider_get32__E", "xdc_Bits32(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.get64", new Extern("ti_sysbios_family_arm_v8m_TimestampProvider_get64__E", "xdc_Void(*)(xdc_runtime_Types_Timestamp64*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.getFreq", new Extern("ti_sysbios_family_arm_v8m_TimestampProvider_getFreq__E", "xdc_Void(*)(xdc_runtime_Types_FreqHz*)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.startTimer", new Extern("ti_sysbios_family_arm_v8m_TimestampProvider_startTimer__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider.rolloverFunc", new Extern("ti_sysbios_family_arm_v8m_TimestampProvider_rolloverFunc__E", "xdc_Void(*)(xdc_UArg)", true, false));
    }

    void Power$$CONSTS()
    {
        // module Power
        om.bind("ti.sysbios.family.arm.v8m.Power.idleCPU", new Extern("ti_sysbios_family_arm_v8m_Power_idleCPU__E", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void SAU$$CONSTS()
    {
        // module SAU
        om.bind("ti.sysbios.family.arm.v8m.SAU.deviceRegs", new Extern("ti_sysbios_family_arm_v8m_SAU_deviceRegs", "ti_sysbios_family_arm_v8m_SAU_DeviceRegs*", false, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegionType_NS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType", "ti.sysbios.family.arm.v8m"), "ti.sysbios.family.arm.v8m.SAU.RegionType_NS", 0));
        om.bind("ti.sysbios.family.arm.v8m.SAU.RegionType_NSC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType", "ti.sysbios.family.arm.v8m"), "ti.sysbios.family.arm.v8m.SAU.RegionType_NSC", 1));
        om.bind("ti.sysbios.family.arm.v8m.SAU.disable", new Extern("ti_sysbios_family_arm_v8m_SAU_disable__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.enable", new Extern("ti_sysbios_family_arm_v8m_SAU_enable__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.isEnabled", new Extern("ti_sysbios_family_arm_v8m_SAU_isEnabled__E", "xdc_Bool(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.configureRegion", new Extern("ti_sysbios_family_arm_v8m_SAU_configureRegion__E", "xdc_Void(*)(xdc_UInt8,xdc_Ptr,xdc_SizeT,ti_sysbios_family_arm_v8m_SAU_RegionType)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.enableRegion", new Extern("ti_sysbios_family_arm_v8m_SAU_enableRegion__E", "xdc_Void(*)(xdc_UInt8)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.disableRegion", new Extern("ti_sysbios_family_arm_v8m_SAU_disableRegion__E", "xdc_Void(*)(xdc_UInt8)", true, false));
        om.bind("ti.sysbios.family.arm.v8m.SAU.startup", new Extern("ti_sysbios_family_arm_v8m_SAU_startup__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void Hwi$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Hwi$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sysbios.family.arm.v8m.Hwi.create() called before xdc.useModule('ti.sysbios.family.arm.v8m.Hwi')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.intNum, inst.$args.hwiFxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.Hwi$$create", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module", "ti.sysbios.family.arm.v8m"), om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance", "ti.sysbios.family.arm.v8m"), 3, 2, false));
                        fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Params", "ti.sysbios.family.arm.v8m"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Hwi$$create = function( intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.arm.v8m.Hwi'];\n");
                sb.append("var __inst = xdc.om['ti.sysbios.family.arm.v8m.Hwi.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sysbios.family.arm.v8m']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sysbios.family.arm.v8m.Hwi'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [intNum, hwiFxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.arm.v8m.Hwi'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.Hwi$$construct", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module", "ti.sysbios.family.arm.v8m"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Object", "ti.sysbios.family.arm.v8m"), null);
                        fxn.addArg(1, "intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "hwiFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Params", "ti.sysbios.family.arm.v8m"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Hwi$$construct = function( __obj, intNum, hwiFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.arm.v8m.Hwi'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {intNum:intNum, hwiFxn:hwiFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sysbios.family.arm.v8m.Hwi'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.arm.v8m.Hwi'].__initObject(__inst);\n");
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

    void TaskSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Timer$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        if (isCFG) {
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Timer$$__initObject = function( inst ) {\n");
                sb.append("if (!this.$used) {\n");
                    sb.append("throw new Error(\"Function ti.sysbios.family.arm.v8m.Timer.create() called before xdc.useModule('ti.sysbios.family.arm.v8m.Timer')\");\n");
                sb.append("}\n");
                sb.append("var name = xdc.module('xdc.runtime.Text').defineRopeCord(inst.instance.name);\n");
                sb.append("inst.$object.$$bind('__name', name);\n");
                sb.append("this.instance$static$init.$fxn.apply(inst, [inst.$object, inst.$args.id, inst.$args.tickFxn, inst, inst.$module]);\n");
                sb.append("inst.$seal();\n");
            sb.append("};\n");
            Global.eval(sb.toString());
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.Timer$$create", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module", "ti.sysbios.family.arm.v8m"), om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance", "ti.sysbios.family.arm.v8m"), 3, 2, false));
                        fxn.addArg(0, "id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(1, "tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(2, "__params", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Params", "ti.sysbios.family.arm.v8m"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Timer$$create = function( id, tickFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.arm.v8m.Timer'];\n");
                sb.append("var __inst = xdc.om['ti.sysbios.family.arm.v8m.Timer.Instance'].$$make();\n");
                sb.append("__inst.$$bind('$package', xdc.om['ti.sysbios.family.arm.v8m']);\n");
                sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
                sb.append("__inst.$$bind('$category', 'Instance');\n");
                sb.append("__inst.$$bind('$args', {id:id, tickFxn:tickFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$instances.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', new xdc.om['ti.sysbios.family.arm.v8m.Timer'].Instance_State);\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("var save = xdc.om.$curpkg;\n");
                sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
                sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [id, tickFxn]);\n");
                sb.append("xdc.om.$$bind('$curpkg', save);\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.arm.v8m.Timer'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return __inst;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
        if (isCFG) {
            fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.Timer$$construct", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module", "ti.sysbios.family.arm.v8m"), null, 4, 2, false));
                        fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Object", "ti.sysbios.family.arm.v8m"), null);
                        fxn.addArg(1, "id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF);
                        fxn.addArg(2, "tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF);
                        fxn.addArg(3, "__params", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Params", "ti.sysbios.family.arm.v8m"), Global.newObject());
            sb = new StringBuilder();
            sb.append("ti$sysbios$family$arm$v8m$Timer$$construct = function( __obj, id, tickFxn, __params ) {\n");
                sb.append("var __mod = xdc.om['ti.sysbios.family.arm.v8m.Timer'];\n");
                sb.append("var __inst = __obj;\n");
                sb.append("__inst.$$bind('$args', {id:id, tickFxn:tickFxn});\n");
                sb.append("__inst.$$bind('$module', __mod);\n");
                sb.append("__mod.$objects.$add(__inst);\n");
                sb.append("__inst.$$bind('$object', xdc.om['ti.sysbios.family.arm.v8m.Timer'].Instance_State.$$make(__inst.$$parent, __inst.$name));\n");
                sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
                sb.append("__inst.$$bless();\n");
                sb.append("if (xdc.om.$$phase >= 5) xdc.om['ti.sysbios.family.arm.v8m.Timer'].__initObject(__inst);\n");
                sb.append("__inst.$$bind('$$phase', xdc.om.$$phase);\n");
                sb.append("return null;\n");
            sb.append("}\n");
            Global.eval(sb.toString());
        }//isCFG
    }

    void TimestampProvider$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Power$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void SAU$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Hwi$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn Hwi.inUseMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.Hwi$$inUseMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module", "ti.sysbios.family.arm.v8m"), $$T_Bool, 1, 1, false));
                fxn.addArg(0, "intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF);
    }

    void IntrinsicsSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TaskSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Timer$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TimestampProvider$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Power$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SAU$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn SAU.configureRegionMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.SAU$$configureRegionMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module", "ti.sysbios.family.arm.v8m"), null, 4, 4, false));
                fxn.addArg(0, "regionId", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF);
                fxn.addArg(1, "regionBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF);
                fxn.addArg(2, "regionSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF);
                fxn.addArg(3, "regionType", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType", "ti.sysbios.family.arm.v8m"), $$UNDEF);
        // fxn SAU.disableRegionMeta
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.v8m.SAU$$disableRegionMeta", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module", "ti.sysbios.family.arm.v8m"), null, 1, 1, false));
                fxn.addArg(0, "regionId", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF);
    }

    void Hwi$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.CCR", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("STKOFHFNMIGN", "UInt8"));
        sizes.add(Global.newArray("BFHFNMIGN", "UInt8"));
        sizes.add(Global.newArray("DIV_0_TRP", "UInt8"));
        sizes.add(Global.newArray("UNALIGN_TRP", "UInt8"));
        sizes.add(Global.newArray("USERSETMPEND", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.CCR']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.CCR']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.CCR'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.NVIC", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("RES_00", "UInt32"));
        sizes.add(Global.newArray("ICTR", "UInt32"));
        sizes.add(Global.newArray("RES_08", "UInt32"));
        sizes.add(Global.newArray("RES_0C", "UInt32"));
        sizes.add(Global.newArray("STCSR", "UInt32"));
        sizes.add(Global.newArray("STRVR", "UInt32"));
        sizes.add(Global.newArray("STCVR", "UInt32"));
        sizes.add(Global.newArray("STCALIB", "UInt32"));
        sizes.add(Global.newArray("RES_20", "A56;UInt32"));
        sizes.add(Global.newArray("ISER", "A16;UInt32"));
        sizes.add(Global.newArray("RES_140", "A16;UInt32"));
        sizes.add(Global.newArray("ICER", "A16;UInt32"));
        sizes.add(Global.newArray("RES_1C0", "A16;UInt32"));
        sizes.add(Global.newArray("ISPR", "A16;UInt32"));
        sizes.add(Global.newArray("RES_240", "A16;UInt32"));
        sizes.add(Global.newArray("ICPR", "A16;UInt32"));
        sizes.add(Global.newArray("RES_2C0", "A16;UInt32"));
        sizes.add(Global.newArray("IABR", "A16;UInt32"));
        sizes.add(Global.newArray("RES_340", "A16;UInt32"));
        sizes.add(Global.newArray("ITNS", "A16;UInt32"));
        sizes.add(Global.newArray("RES_3C0", "A16;UInt32"));
        sizes.add(Global.newArray("IPR", "A492;UInt8"));
        sizes.add(Global.newArray("RES_5F0", "A453;UInt32"));
        sizes.add(Global.newArray("CPUIDBR", "UInt32"));
        sizes.add(Global.newArray("ICSR", "UInt32"));
        sizes.add(Global.newArray("VTOR", "UInt32"));
        sizes.add(Global.newArray("AIRCR", "UInt32"));
        sizes.add(Global.newArray("SCR", "UInt32"));
        sizes.add(Global.newArray("CCR", "UInt32"));
        sizes.add(Global.newArray("SHPR", "A12;UInt8"));
        sizes.add(Global.newArray("SHCSR", "UInt32"));
        sizes.add(Global.newArray("MMFSR", "UInt8"));
        sizes.add(Global.newArray("BFSR", "UInt8"));
        sizes.add(Global.newArray("UFSR", "UInt16"));
        sizes.add(Global.newArray("HFSR", "UInt32"));
        sizes.add(Global.newArray("DFSR", "UInt32"));
        sizes.add(Global.newArray("MMAR", "UInt32"));
        sizes.add(Global.newArray("BFAR", "UInt32"));
        sizes.add(Global.newArray("AFSR", "UInt32"));
        sizes.add(Global.newArray("PFR0", "UInt32"));
        sizes.add(Global.newArray("PFR1", "UInt32"));
        sizes.add(Global.newArray("DFR0", "UInt32"));
        sizes.add(Global.newArray("AFR0", "UInt32"));
        sizes.add(Global.newArray("MMFR0", "UInt32"));
        sizes.add(Global.newArray("MMFR1", "UInt32"));
        sizes.add(Global.newArray("MMFR2", "UInt32"));
        sizes.add(Global.newArray("MMFR3", "UInt32"));
        sizes.add(Global.newArray("ISAR0", "UInt32"));
        sizes.add(Global.newArray("ISAR1", "UInt32"));
        sizes.add(Global.newArray("ISAR2", "UInt32"));
        sizes.add(Global.newArray("ISAR3", "UInt32"));
        sizes.add(Global.newArray("ISAR4", "UInt32"));
        sizes.add(Global.newArray("ISAR5", "UInt32"));
        sizes.add(Global.newArray("CLIDR", "UInt32"));
        sizes.add(Global.newArray("CTR", "UInt32"));
        sizes.add(Global.newArray("CCSIDR", "UInt32"));
        sizes.add(Global.newArray("CSSELR", "UInt32"));
        sizes.add(Global.newArray("CPACR", "UInt32"));
        sizes.add(Global.newArray("NSACR", "UInt32"));
        sizes.add(Global.newArray("MPU_TYPE", "UInt32"));
        sizes.add(Global.newArray("MPU_CTRL", "UInt32"));
        sizes.add(Global.newArray("MPU_RNR", "UInt32"));
        sizes.add(Global.newArray("MPU_RBAR", "UInt32"));
        sizes.add(Global.newArray("MPU_RLAR", "UInt32"));
        sizes.add(Global.newArray("MPU_RBAR_An", "UInt32"));
        sizes.add(Global.newArray("MPU_RLAR_An", "UInt32"));
        sizes.add(Global.newArray("RES_DAC", "A5;UInt32"));
        sizes.add(Global.newArray("MPU_MAIR0", "UInt32"));
        sizes.add(Global.newArray("MPU_MAIR1", "UInt32"));
        sizes.add(Global.newArray("RES_DC8", "A2;UInt32"));
        sizes.add(Global.newArray("SAU_CTRL", "UInt32"));
        sizes.add(Global.newArray("SAU_TYPE", "UInt32"));
        sizes.add(Global.newArray("SAU_RNR", "UInt32"));
        sizes.add(Global.newArray("SAU_RBAR", "UInt32"));
        sizes.add(Global.newArray("SAU_RLAR", "UInt32"));
        sizes.add(Global.newArray("SFSR", "UInt32"));
        sizes.add(Global.newArray("SFAR", "UInt32"));
        sizes.add(Global.newArray("RES_DEC", "UInt32"));
        sizes.add(Global.newArray("DHCSR", "UInt32"));
        sizes.add(Global.newArray("DCRSR", "UInt32"));
        sizes.add(Global.newArray("DCRDR", "UInt32"));
        sizes.add(Global.newArray("DEMCR", "UInt32"));
        sizes.add(Global.newArray("RES_E00", "UInt32"));
        sizes.add(Global.newArray("DAUTHCTRL", "UInt32"));
        sizes.add(Global.newArray("DSCSR", "UInt32"));
        sizes.add(Global.newArray("RES_E0C", "A61;UInt32"));
        sizes.add(Global.newArray("STI", "UInt32"));
        sizes.add(Global.newArray("RES_F04", "A12;UInt32"));
        sizes.add(Global.newArray("FPCCR", "UInt32"));
        sizes.add(Global.newArray("FPCAR", "UInt32"));
        sizes.add(Global.newArray("FPDSCR", "UInt32"));
        sizes.add(Global.newArray("MVFR0", "UInt32"));
        sizes.add(Global.newArray("MVFR1", "UInt32"));
        sizes.add(Global.newArray("MVFR2", "UInt32"));
        sizes.add(Global.newArray("RES_F48", "UInt32"));
        sizes.add(Global.newArray("ICIALLU", "UInt32"));
        sizes.add(Global.newArray("RES_F54", "UInt32"));
        sizes.add(Global.newArray("ICIMVAU", "UInt32"));
        sizes.add(Global.newArray("DCIMVAC", "UInt32"));
        sizes.add(Global.newArray("DCISW", "UInt32"));
        sizes.add(Global.newArray("DCCMVAU", "UInt32"));
        sizes.add(Global.newArray("DCCMVAC", "UInt32"));
        sizes.add(Global.newArray("DCCSW", "UInt32"));
        sizes.add(Global.newArray("DCCIMVAC", "UInt32"));
        sizes.add(Global.newArray("DCCISW", "UInt32"));
        sizes.add(Global.newArray("BPIALL", "UInt32"));
        sizes.add(Global.newArray("RES_F7C", "A13;UInt32"));
        sizes.add(Global.newArray("DLAR", "UInt32"));
        sizes.add(Global.newArray("DLSR", "UInt32"));
        sizes.add(Global.newArray("DAUTHSTATUS", "UInt32"));
        sizes.add(Global.newArray("DDEVARCH", "UInt32"));
        sizes.add(Global.newArray("RES_FC0", "A3;UInt32"));
        sizes.add(Global.newArray("DDEVTYPE", "UInt32"));
        sizes.add(Global.newArray("DPIDR4", "UInt32"));
        sizes.add(Global.newArray("DPIDR5", "UInt32"));
        sizes.add(Global.newArray("DPIDR6", "UInt32"));
        sizes.add(Global.newArray("DPIDR7", "UInt32"));
        sizes.add(Global.newArray("DPIDR0", "UInt32"));
        sizes.add(Global.newArray("DPIDR1", "UInt32"));
        sizes.add(Global.newArray("DPIDR2", "UInt32"));
        sizes.add(Global.newArray("DPIDR3", "UInt32"));
        sizes.add(Global.newArray("DCIDR0", "UInt32"));
        sizes.add(Global.newArray("DCIDR1", "UInt32"));
        sizes.add(Global.newArray("DCIDR2", "UInt32"));
        sizes.add(Global.newArray("DCIDR3", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.NVIC']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.NVIC']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.NVIC'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ExcContext", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("threadType", "Nti.sysbios.BIOS.ThreadType;;;;;"));
        sizes.add(Global.newArray("threadHandle", "UPtr"));
        sizes.add(Global.newArray("threadStack", "UPtr"));
        sizes.add(Global.newArray("threadStackSize", "USize"));
        sizes.add(Global.newArray("r0", "UPtr"));
        sizes.add(Global.newArray("r1", "UPtr"));
        sizes.add(Global.newArray("r2", "UPtr"));
        sizes.add(Global.newArray("r3", "UPtr"));
        sizes.add(Global.newArray("r4", "UPtr"));
        sizes.add(Global.newArray("r5", "UPtr"));
        sizes.add(Global.newArray("r6", "UPtr"));
        sizes.add(Global.newArray("r7", "UPtr"));
        sizes.add(Global.newArray("r8", "UPtr"));
        sizes.add(Global.newArray("r9", "UPtr"));
        sizes.add(Global.newArray("r10", "UPtr"));
        sizes.add(Global.newArray("r11", "UPtr"));
        sizes.add(Global.newArray("r12", "UPtr"));
        sizes.add(Global.newArray("sp", "UPtr"));
        sizes.add(Global.newArray("lr", "UPtr"));
        sizes.add(Global.newArray("pc", "UPtr"));
        sizes.add(Global.newArray("psr", "UPtr"));
        sizes.add(Global.newArray("ICSR", "UPtr"));
        sizes.add(Global.newArray("MMFSR", "UPtr"));
        sizes.add(Global.newArray("BFSR", "UPtr"));
        sizes.add(Global.newArray("UFSR", "UPtr"));
        sizes.add(Global.newArray("HFSR", "UPtr"));
        sizes.add(Global.newArray("DFSR", "UPtr"));
        sizes.add(Global.newArray("MMAR", "UPtr"));
        sizes.add(Global.newArray("BFAR", "UPtr"));
        sizes.add(Global.newArray("AFSR", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.ExcContext']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.ExcContext']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.ExcContext'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Struct2__", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("fxns", "UPtr"));
        sizes.add(Global.newArray("arg", "UIArg"));
        sizes.add(Global.newArray("fxn", "UFxn"));
        sizes.add(Global.newArray("irp", "UIArg"));
        sizes.add(Global.newArray("priority", "UInt8"));
        sizes.add(Global.newArray("intNum", "TInt16"));
        sizes.add(Global.newArray("hookEnv", "UPtr"));
        sizes.add(Global.newArray("name", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Struct2__']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Struct2__']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Struct2__'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("arg", "UIArg"));
        sizes.add(Global.newArray("fxn", "UFxn"));
        sizes.add(Global.newArray("irp", "UIArg"));
        sizes.add(Global.newArray("priority", "UInt8"));
        sizes.add(Global.newArray("intNum", "TInt16"));
        sizes.add(Global.newArray("hookEnv", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("taskSP", "UPtr"));
        sizes.add(Global.newArray("excActive", "UShort"));
        sizes.add(Global.newArray("excContext", "UPtr"));
        sizes.add(Global.newArray("excStack", "UPtr"));
        sizes.add(Global.newArray("isrStack", "UPtr"));
        sizes.add(Global.newArray("isrStackBase", "UPtr"));
        sizes.add(Global.newArray("isrStackSize", "UPtr"));
        sizes.add(Global.newArray("vectorTableBase", "UPtr"));
        sizes.add(Global.newArray("swiTaskKeys", "UInt"));
        sizes.add(Global.newArray("dispatchTable", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Hwi.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void IntrinsicsSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void TaskSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Timer$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("__fxns", "UPtr"));
        sizes.add(Global.newArray("staticInst", "UShort"));
        sizes.add(Global.newArray("id", "TInt"));
        sizes.add(Global.newArray("runMode", "Nti.sysbios.interfaces.ITimer.RunMode;;;;"));
        sizes.add(Global.newArray("startMode", "Nti.sysbios.interfaces.ITimer.StartMode;;;"));
        sizes.add(Global.newArray("period", "UInt"));
        sizes.add(Global.newArray("periodType", "Nti.sysbios.interfaces.ITimer.PeriodType;;;"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("arg", "UIArg"));
        sizes.add(Global.newArray("tickFxn", "UFxn"));
        sizes.add(Global.newArray("extFreq", "Sxdc.runtime.Types;FreqHz"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Instance_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Instance_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Instance_State'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("tickCount", "UInt"));
        sizes.add(Global.newArray("availMask", "UInt"));
        sizes.add(Global.newArray("handle", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.Timer.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void TimestampProvider$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("timer", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.TimestampProvider.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.TimestampProvider.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.TimestampProvider.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Power$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void SAU$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.SAU.DeviceRegs", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("CTRL", "UInt32"));
        sizes.add(Global.newArray("TYPE", "UInt32"));
        sizes.add(Global.newArray("RNR", "UInt32"));
        sizes.add(Global.newArray("RBAR", "UInt32"));
        sizes.add(Global.newArray("RLAR", "UInt32"));
        sizes.add(Global.newArray("SFSR", "UInt32"));
        sizes.add(Global.newArray("SFAR", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.SAU.DeviceRegs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.SAU.DeviceRegs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.SAU.DeviceRegs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionEntry", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("rbar", "UInt32"));
        sizes.add(Global.newArray("rlar", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.SAU.RegionEntry']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.SAU.RegionEntry']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.SAU.RegionEntry'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module_State", "ti.sysbios.family.arm.v8m");
        sizes.clear();
        sizes.add(Global.newArray("regionEntry", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v8m.SAU.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v8m.SAU.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v8m.SAU.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Hwi$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/Hwi.xs");
        om.bind("ti.sysbios.family.arm.v8m.Hwi$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Module", om.findStrict("ti.sysbios.interfaces.IHwi.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("NUM_INTERRUPTS", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
            po.addFld("NUM_PRIORITIES", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.arm.v8m"), $$UNDEF, "wh");
            po.addFld("LM_begin", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.arm.v8m"), Global.newObject("mask", Global.eval("0x0100 | 0x0200"), "msg", "LM_begin: hwi: 0x%x, func: 0x%x, preThread: %d, intNum: %d, irp: 0x%x"), "w");
            po.addFld("LD_end", (Proto)om.findStrict("xdc.runtime.Log$$Event", "ti.sysbios.family.arm.v8m"), Global.newObject("mask", 0x0200L, "msg", "LD_end: hwi: 0x%x"), "w");
            po.addFld("A_unsupportedMaskingOption", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "A_unsupportedMaskingOption: unsupported maskSetting."), "w");
            po.addFld("E_alreadyDefined", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_alreadyDefined: Hwi already defined: intr# %d"), "w");
            po.addFld("E_hwiLimitExceeded", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_hwiLimitExceeded: Too many interrupts defined"), "w");
            po.addFld("E_exception", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_exception: id = %d, pc = %08x.\nTo see more exception detail, set ti.sysbios.family.arm.v8m.Hwi.enableException = true or,\nexamine the Exception view for the ti.sysbios.family.arm.v8m.Hwi module using ROV."), "w");
            po.addFld("E_noIsr", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_noIsr: id = %d, pc = %08x"), "w");
            po.addFld("E_NMI", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_NMI: %s"), "w");
            po.addFld("E_hardFault", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_hardFault: %s"), "w");
            po.addFld("E_memFault", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_memFault: %s, address: %08x"), "w");
            po.addFld("E_busFault", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_busFault: %s, address: %08x"), "w");
            po.addFld("E_usageFault", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_usageFault: %s"), "w");
            po.addFld("E_svCall", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_svCall: svNum = %d"), "w");
            po.addFld("E_debugMon", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_debugMon: %s"), "w");
            po.addFld("E_reserved", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_reserved: %s %d"), "w");
            po.addFld("dispatchTableSize", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "wh");
            po.addFld("vectorTableAddress", new Proto.Adr("xdc_Ptr", "Pv"), 0x00000000L, "wh");
            po.addFld("resetVectorAddress", new Proto.Adr("xdc_Ptr", "Pv"), 0x00000000L, "wh");
            po.addFld("resetFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("nmiFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("hardFaultFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("memFaultFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("busFaultFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("usageFaultFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("svCallFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("debugMonFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("reservedFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "wh");
            po.addFld("nullIsrFunc", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "w");
            po.addFld("excHandlerFunc", new Proto.Adr("xdc_Void(*)(xdc_UInt*,xdc_UInt)", "PFv"), om.find("ti.sysbios.family.arm.v8m.Hwi.excHandlerMax"), "w");
            po.addFld("enableException", $$T_Bool, true, "wh");
            po.addFld("excContextBuffer", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "wh");
            po.addFld("excStackBuffer", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "wh");
            po.addFld("excHookFunc", new Proto.Adr("xdc_Void(*)(ti_sysbios_family_arm_v8m_Hwi_ExcContext*)", "PFv"), null, "w");
            po.addFld("nvicCCR", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.CCR", "ti.sysbios.family.arm.v8m"), Global.newObject("STKOFHFNMIGN", 0L, "BFHFNMIGN", 0L, "DIV_0_TRP", 0L, "UNALIGN_TRP", 0L, "USERSETMPEND", 0L), "wh");
            po.addFld("disablePriority", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
            po.addFld("priGroup", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "w");
            po.addFld("enableNonSecureFloatingPointAccess", $$T_Bool, true, "wh");
            po.addFld("enableNonSecureFaultHandling", $$T_Bool, true, "wh");
            po.addFld("numSparseInterrupts", Proto.Elm.newCNum("(xdc_UInt)"), 0L, "w");
            po.addFld("isTiva", $$T_Bool, false, "wh");
            po.addFld("swiDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("swiRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("taskDisable", new Proto.Adr("xdc_UInt(*)(xdc_Void)", "PFn"), $$UNDEF, "w");
            po.addFld("taskRestoreHwi", new Proto.Adr("xdc_Void(*)(xdc_UInt)", "PFv"), $$UNDEF, "w");
            po.addFld("ccr", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
            po.addFld("hooks", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.HookSet", "ti.sysbios.family.arm.v8m"), true), Global.newArray(new Object[]{}), "w");
            po.addFld("interrupt", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.InterruptObj", "ti.sysbios.family.arm.v8m"), false), $$DEFAULT, "wh");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$create", "ti.sysbios.family.arm.v8m"), Global.get("ti$sysbios$family$arm$v8m$Hwi$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$construct", "ti.sysbios.family.arm.v8m"), Global.get("ti$sysbios$family$arm$v8m$Hwi$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Hwi$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "addHookSet");
                if (fxn != null) po.addFxn("addHookSet", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$addHookSet", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "viewGetStackInfo");
                if (fxn != null) po.addFxn("viewGetStackInfo", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.IHwi$$viewGetStackInfo", "ti.sysbios.family.arm.v8m"), fxn);
                po.addFxn("inUseMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$inUseMeta", "ti.sysbios.family.arm.v8m"), Global.get(cap, "inUseMeta"));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Instance", om.findStrict("ti.sysbios.interfaces.IHwi.Instance", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), 255L, "w");
            po.addFld("maskSetting", (Proto)om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.arm.v8m"), om.find("ti.sysbios.interfaces.IHwi.MaskingOption_LOWER"), "w");
            po.addFld("useDispatcher", $$T_Bool, true, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Params", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Params", om.findStrict("ti.sysbios.interfaces.IHwi$$Params", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), 255L, "w");
            po.addFld("maskSetting", (Proto)om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.arm.v8m"), om.find("ti.sysbios.interfaces.IHwi.MaskingOption_LOWER"), "w");
            po.addFld("useDispatcher", $$T_Bool, true, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Object", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Object", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance", "ti.sysbios.family.arm.v8m"));
        // typedef Hwi.VectorFuncPtr
        om.bind("ti.sysbios.family.arm.v8m.Hwi.VectorFuncPtr", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"));
        // typedef Hwi.ExceptionHookFuncPtr
        om.bind("ti.sysbios.family.arm.v8m.Hwi.ExceptionHookFuncPtr", new Proto.Adr("xdc_Void(*)(ti_sysbios_family_arm_v8m_Hwi_ExcContext*)", "PFv"));
        // struct Hwi.CCR
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$CCR", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.CCR", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("STKOFHFNMIGN", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("BFHFNMIGN", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("DIV_0_TRP", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("UNALIGN_TRP", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("USERSETMPEND", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct Hwi.NVIC
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$NVIC", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.NVIC", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("RES_00", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ICTR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_08", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_0C", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("STCSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("STRVR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("STCVR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("STCALIB", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_20", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(56L)), $$DEFAULT, "w");
                po.addFld("ISER", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_140", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("ICER", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_1C0", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("ISPR", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_240", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("ICPR", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_2C0", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("IABR", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_340", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("ITNS", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("RES_3C0", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(16L)), $$DEFAULT, "w");
                po.addFld("IPR", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt8)"), false, xdc.services.intern.xsr.Enum.intValue(492L)), $$DEFAULT, "w");
                po.addFld("RES_5F0", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(453L)), $$DEFAULT, "w");
                po.addFld("CPUIDBR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ICSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("VTOR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("AIRCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SHPR", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt8)"), false, xdc.services.intern.xsr.Enum.intValue(12L)), $$DEFAULT, "w");
                po.addFld("SHCSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMFSR", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("BFSR", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("UFSR", Proto.Elm.newCNum("(xdc_UInt16)"), $$UNDEF, "w");
                po.addFld("HFSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DFSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("BFAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("AFSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("PFR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("PFR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DFR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("AFR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMFR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMFR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMFR2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MMFR3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR4", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ISAR5", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CLIDR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CTR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CCSIDR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CSSELR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("CPACR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("NSACR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_TYPE", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_CTRL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_RNR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_RBAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_RLAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_RBAR_An", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_RLAR_An", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_DAC", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(5L)), $$DEFAULT, "w");
                po.addFld("MPU_MAIR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MPU_MAIR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_DC8", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(2L)), $$DEFAULT, "w");
                po.addFld("SAU_CTRL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SAU_TYPE", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SAU_RNR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SAU_RBAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SAU_RLAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SFSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SFAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_DEC", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DHCSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCRSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCRDR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DEMCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_E00", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DAUTHCTRL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DSCSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_E0C", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(61L)), $$DEFAULT, "w");
                po.addFld("STI", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_F04", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(12L)), $$DEFAULT, "w");
                po.addFld("FPCCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("FPCAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("FPDSCR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MVFR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MVFR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("MVFR2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_F48", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ICIALLU", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_F54", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("ICIMVAU", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCIMVAC", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCISW", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCCMVAU", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCCMVAC", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCCSW", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCCIMVAC", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCCISW", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("BPIALL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_F7C", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(13L)), $$DEFAULT, "w");
                po.addFld("DLAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DLSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DAUTHSTATUS", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DDEVARCH", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RES_FC0", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt32)"), false, xdc.services.intern.xsr.Enum.intValue(3L)), $$DEFAULT, "w");
                po.addFld("DDEVTYPE", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR4", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR5", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR6", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR7", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DPIDR3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCIDR0", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCIDR1", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCIDR2", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("DCIDR3", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct Hwi.ExcContext
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$ExcContext", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.ExcContext", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("threadType", (Proto)om.findStrict("ti.sysbios.BIOS.ThreadType", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
                po.addFld("threadHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("threadStack", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("threadStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("r0", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r1", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r2", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r3", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r4", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r5", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r6", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r7", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r8", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r9", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r10", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r11", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("r12", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("sp", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("lr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("pc", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("psr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("ICSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("MMFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("BFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("UFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("HFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("DFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("MMAR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("BFAR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("AFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct Hwi.Struct2__
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Struct2__", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Struct2__", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int16)"), $$UNDEF, "w");
                po.addFld("hookEnv", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("name", new Proto.Adr("xdc_runtime_Types_CordAddr__*", "PE"), $$UNDEF, "w");
        // typedef Hwi.Struct2
        om.bind("ti.sysbios.family.arm.v8m.Hwi.Struct2", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Struct2__", "ti.sysbios.family.arm.v8m"));
        // struct Hwi.BasicView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$BasicView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("halHwiHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("label", $$T_Str, $$UNDEF, "w");
                po.addFld("type", $$T_Str, $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("group", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("subPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("fxn", $$T_Str, $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
        // struct Hwi.DetailedView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$DetailedView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.DetailedView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("halHwiHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("label", $$T_Str, $$UNDEF, "w");
                po.addFld("type", $$T_Str, $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("group", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("subPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("fxn", $$T_Str, $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("status", $$T_Str, $$UNDEF, "w");
        // struct Hwi.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$ModuleView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("options", new Proto.Arr($$T_Str, false, xdc.services.intern.xsr.Enum.intValue(4L)), $$DEFAULT, "w");
                po.addFld("processorState", $$T_Str, $$UNDEF, "w");
                po.addFld("activeInterrupt", $$T_Str, $$UNDEF, "w");
                po.addFld("pendingInterrupt", $$T_Str, $$UNDEF, "w");
                po.addFld("exception", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiStackPeak", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("hwiStackBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct Hwi.VectorTableView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$VectorTableView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.VectorTableView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("vectorNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("vector", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("vectorLabel", $$T_Str, $$UNDEF, "w");
                po.addFld("type", $$T_Str, $$UNDEF, "w");
                po.addFld("priority", $$T_Str, $$UNDEF, "w");
                po.addFld("preemptPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("subPriority", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("status", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiFxn", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiArg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("hwiIrp", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // typedef Hwi.ExcHandlerFuncPtr
        om.bind("ti.sysbios.family.arm.v8m.Hwi.ExcHandlerFuncPtr", new Proto.Adr("xdc_Void(*)(xdc_UInt*,xdc_UInt)", "PFv"));
        // typedef Hwi.HandlerFuncPtr
        om.bind("ti.sysbios.family.arm.v8m.Hwi.HandlerFuncPtr", new Proto.Adr("xdc_Void(*)(ti_sysbios_family_arm_v8m_Hwi_Handle,xdc_UInt)", "PFv"));
        // struct Hwi.InterruptObj
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$InterruptObj", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.InterruptObj", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("name", $$T_Str, $$UNDEF, "w");
                po.addFld("used", $$T_Bool, $$UNDEF, "w");
                po.addFld("useDispatcher", $$T_Bool, $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("hwi", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Handle", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        // struct Hwi.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Instance_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("fxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("irp", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("priority", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Int16)"), $$UNDEF, "w");
                po.addFld("hookEnv", new Proto.Arr(new Proto.Adr("xdc_Ptr", "Pv"), false), $$DEFAULT, "w");
        // struct Hwi.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Module_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Hwi.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("taskSP", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("excActive", $$T_Bool, $$UNDEF, "w");
                po.addFld("excContext", new Proto.Adr("ti_sysbios_family_arm_v8m_Hwi_ExcContext*", "PS"), $$UNDEF, "w");
                po.addFld("excStack", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("isrStack", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("isrStackBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("isrStackSize", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("vectorTableBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("swiTaskKeys", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("dispatchTable", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void IntrinsicsSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/IntrinsicsSupport.xs");
        om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.IntrinsicsSupport.Module", om.findStrict("ti.sysbios.interfaces.IIntrinsicsSupport.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void TaskSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/TaskSupport.xs");
        om.bind("ti.sysbios.family.arm.v8m.TaskSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.TaskSupport.Module", om.findStrict("ti.sysbios.interfaces.ITaskSupport.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("defaultStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), 2048L, "r");
            po.addFld("stackAlignment", Proto.Elm.newCNum("(xdc_UInt)"), 8L, "r");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TaskSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TaskSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TaskSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TaskSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "stackUsed$view");
                if (fxn != null) po.addFxn("stackUsed$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$stackUsed$view", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "getCallStack$view");
                if (fxn != null) po.addFxn("getCallStack$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$getCallStack$view", "ti.sysbios.family.arm.v8m"), fxn);
    }

    void Timer$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/Timer.xs");
        om.bind("ti.sysbios.family.arm.v8m.Timer$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Module", om.findStrict("ti.sysbios.interfaces.ITimer.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0x00ffffffL, "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("TIMER_CLOCK_DIVIDER", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("MIN_SWEEP_PERIOD", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.arm.v8m"), $$UNDEF, "wh");
            po.addFld("E_invalidTimer", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_invalidTimer: Invalid Timer Id %d"), "w");
            po.addFld("E_notAvailable", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_notAvailable: Timer not available %d"), "w");
            po.addFld("E_cannotSupport", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "E_cannotSupport: Timer cannot support requested period %d"), "w");
            po.addFld("anyMask", Proto.Elm.newCNum("(xdc_UInt)"), 0x1L, "w");
            po.addFld("timerInUse", $$T_Bool, false, "wh");
            po.addFld("startupNeeded", Proto.Elm.newCNum("(xdc_UInt)"), false, "w");
        }//isCFG
        if (isCFG) {
                        po.addFxn("create", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$create", "ti.sysbios.family.arm.v8m"), Global.get("ti$sysbios$family$arm$v8m$Timer$$create"));
                        po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$construct", "ti.sysbios.family.arm.v8m"), Global.get("ti$sysbios$family$arm$v8m$Timer$$construct"));
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Timer$$instance$static$init", true);
        if (fxn != null) po.addFxn("instance$static$init", $$T_Met, fxn);
                fxn = Global.get(cap, "viewGetCurrentClockTick");
                if (fxn != null) po.addFxn("viewGetCurrentClockTick", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITimer$$viewGetCurrentClockTick", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "getFreqMeta");
                if (fxn != null) po.addFxn("getFreqMeta", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITimer$$getFreqMeta", "ti.sysbios.family.arm.v8m"), fxn);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Instance", om.findStrict("ti.sysbios.interfaces.ITimer.Instance", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0x00ffffffL, "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("TIMER_CLOCK_DIVIDER", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("MIN_SWEEP_PERIOD", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("hwiParams", new Proto.Adr("ti_sysbios_family_arm_v8m_Hwi_Params*", "PS"), null, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Params", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Params", om.findStrict("ti.sysbios.interfaces.ITimer$$Params", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("ANY", Proto.Elm.newCNum("(xdc_UInt)"), Global.eval("~0"), "rh");
                po.addFld("MAX_PERIOD", Proto.Elm.newCNum("(xdc_UInt)"), 0x00ffffffL, "rh");
                po.addFld("NUM_TIMER_DEVICES", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("TIMER_CLOCK_DIVIDER", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
                po.addFld("MIN_SWEEP_PERIOD", Proto.Elm.newCNum("(xdc_Int)"), 1L, "rh");
        if (isCFG) {
            po.addFld("hwiParams", new Proto.Adr("ti_sysbios_family_arm_v8m_Hwi_Params*", "PS"), null, "w");
                        po.addFld("instance", (Proto)om.findStrict("xdc.runtime.IInstance.Params", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        }//isCFG
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Object", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Object", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance", "ti.sysbios.family.arm.v8m"));
        // struct Timer.BasicView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$BasicView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("halTimerHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("label", $$T_Str, $$UNDEF, "w");
                po.addFld("id", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("device", $$T_Str, $$UNDEF, "w");
                po.addFld("startMode", $$T_Str, $$UNDEF, "w");
                po.addFld("runMode", $$T_Str, $$UNDEF, "w");
                po.addFld("period", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("periodType", $$T_Str, $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("tickFxn", new Proto.Arr($$T_Str, false), $$DEFAULT, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("extFreq", $$T_Str, $$UNDEF, "w");
                po.addFld("hwiHandle", $$T_Str, $$UNDEF, "w");
                po.addFxn("$xml", $$T_Met, Global.get("$$xml"));
        // struct Timer.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$ModuleView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("availMask", $$T_Str, $$UNDEF, "w");
                po.addFld("tickCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct Timer.DeviceView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$DeviceView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.DeviceView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("id", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("device", $$T_Str, $$UNDEF, "w");
                po.addFld("devAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("runMode", $$T_Str, $$UNDEF, "w");
                po.addFld("period", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("currCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("remainingCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct Timer.Instance_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Instance_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Instance_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("staticInst", $$T_Bool, $$UNDEF, "w");
                po.addFld("id", Proto.Elm.newCNum("(xdc_Int)"), $$UNDEF, "w");
                po.addFld("runMode", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.RunMode", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
                po.addFld("startMode", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.StartMode", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
                po.addFld("period", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("periodType", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.PeriodType", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
                po.addFld("tickFxn", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
                po.addFld("extFreq", (Proto)om.findStrict("xdc.runtime.Types.FreqHz", "ti.sysbios.family.arm.v8m"), $$DEFAULT, "w");
                po.addFld("hwi", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Handle", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
        // struct Timer.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Module_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Timer.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("tickCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("availMask", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("handle", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Handle", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
    }

    void TimestampProvider$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/TimestampProvider.xs");
        om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.TimestampProvider.Module", om.findStrict("ti.sysbios.interfaces.ITimestamp.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("configTimer", $$T_Bool, false, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.TimestampProvider$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getFreqMeta");
                if (fxn != null) po.addFxn("getFreqMeta", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITimestamp$$getFreqMeta", "ti.sysbios.family.arm.v8m"), fxn);
        // struct TimestampProvider.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider$$Module_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("timer", (Proto)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Handle", "ti.sysbios.family.arm.v8m"), $$UNDEF, "w");
    }

    void Power$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/Power.xs");
        om.bind("ti.sysbios.family.arm.v8m.Power$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Power.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.Power.Module", om.findStrict("ti.sysbios.interfaces.IPower.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("idle", $$T_Bool, false, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Power$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Power$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Power$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.Power$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void SAU$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/SAU.xs");
        om.bind("ti.sysbios.family.arm.v8m.SAU$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.arm.v8m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.arm.v8m"), $$UNDEF, "wh");
            po.addFld("A_nullPointer", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "A_nullPointer: Pointer is null"), "w");
            po.addFld("A_invalidRegionId", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "A_invalidRegionId: SAU Region number passed is invalid."), "w");
            po.addFld("A_unalignedBaseAddr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "A_unalignedBaseAddr: SAU region base address not aligned."), "w");
            po.addFld("A_unalignedEndAddr", (Proto)om.findStrict("xdc.runtime.Assert$$Id", "ti.sysbios.family.arm.v8m"), Global.newObject("msg", "A_unalignedEndAddr: SAU region end address not aligned."), "w");
            po.addFld("enableSAU", $$T_Bool, false, "w");
            po.addFld("allSecureWhenSauDisabled", $$T_Bool, true, "w");
            po.addFld("numRegions", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
            po.addFld("regionEntry", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionEntry", "ti.sysbios.family.arm.v8m"), false), $$DEFAULT, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.SAU$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.SAU$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.SAU$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v8m.SAU$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                po.addFxn("configureRegionMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$configureRegionMeta", "ti.sysbios.family.arm.v8m"), Global.get(cap, "configureRegionMeta"));
                po.addFxn("disableRegionMeta", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$disableRegionMeta", "ti.sysbios.family.arm.v8m"), Global.get(cap, "disableRegionMeta"));
        // struct SAU.RegionsView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$RegionsView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.RegionsView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("RegionIdx", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("Enabled", $$T_Bool, $$UNDEF, "w");
                po.addFld("BaseAddress", $$T_Str, $$UNDEF, "w");
                po.addFld("EndAddress", $$T_Str, $$UNDEF, "w");
                po.addFld("RegionType", $$T_Str, $$UNDEF, "w");
        // struct SAU.RegsView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$RegsView", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.RegsView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("SREGION", Proto.Elm.newCNum("(xdc_UInt8)"), $$UNDEF, "w");
                po.addFld("ENABLE", $$T_Bool, $$UNDEF, "w");
                po.addFld("ALLNS", $$T_Str, $$UNDEF, "w");
                po.addFld("SFSR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("SFAR", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct SAU.DeviceRegs
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$DeviceRegs", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.DeviceRegs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("CTRL", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("TYPE", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RNR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RBAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("RLAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SFSR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("SFAR", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct SAU.RegionEntry
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$RegionEntry", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.RegionEntry", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("rbar", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("rlar", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct SAU.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$Module_State", "ti.sysbios.family.arm.v8m");
        po.init("ti.sysbios.family.arm.v8m.SAU.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("regionEntry", new Proto.Arr((Proto)om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionEntry", "ti.sysbios.family.arm.v8m"), false), $$DEFAULT, "w");
    }

    void Hwi$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Instance_State", "ti.sysbios.family.arm.v8m");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("CCR$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.CCR", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$CCR", "ti.sysbios.family.arm.v8m");
        vo.bind("NVIC$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.NVIC", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$NVIC", "ti.sysbios.family.arm.v8m");
        vo.bind("ExcContext$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.ExcContext", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$ExcContext", "ti.sysbios.family.arm.v8m");
        vo.bind("Struct2__$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.Struct2__", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Struct2__", "ti.sysbios.family.arm.v8m");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Instance_State", "ti.sysbios.family.arm.v8m");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$Module_State", "ti.sysbios.family.arm.v8m");
        po.bind("excContext$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Hwi.ExcContext", "isScalar", false));
    }

    void IntrinsicsSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport", "ti.sysbios.family.arm.v8m");
    }

    void TaskSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport", "ti.sysbios.family.arm.v8m");
    }

    void Timer$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Instance_State", "ti.sysbios.family.arm.v8m");
        po.addFld("__fxns", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        vo.bind("Instance_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Timer.Instance_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Instance_State", "ti.sysbios.family.arm.v8m");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.Timer.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer$$Module_State", "ti.sysbios.family.arm.v8m");
    }

    void TimestampProvider$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider", "ti.sysbios.family.arm.v8m");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider$$Module_State", "ti.sysbios.family.arm.v8m");
    }

    void Power$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Power", "ti.sysbios.family.arm.v8m");
    }

    void SAU$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU", "ti.sysbios.family.arm.v8m");
        vo.bind("DeviceRegs$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.SAU.DeviceRegs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$DeviceRegs", "ti.sysbios.family.arm.v8m");
        vo.bind("RegionEntry$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.SAU.RegionEntry", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$RegionEntry", "ti.sysbios.family.arm.v8m");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.SAU.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU$$Module_State", "ti.sysbios.family.arm.v8m");
        po.bind("regionEntry$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v8m.SAU.RegionEntry", "isScalar", false));
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.arm.v8m.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.arm.v8m"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v8m/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.arm.v8m"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.arm.v8m"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.arm.v8m", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.arm.v8m");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.arm.v8m.");
        pkgV.bind("$vers", Global.newArray(2, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.arm.v8m'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.arm.v8m$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.arm.v8m$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.arm.v8m$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void Hwi$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.Hwi", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.Hwi$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("Instance", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance", "ti.sysbios.family.arm.v8m"));
        vo.bind("Params", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Params", "ti.sysbios.family.arm.v8m"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Params", "ti.sysbios.family.arm.v8m")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Handle", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.IHwi.FuncPtr", "ti.sysbios.family.arm.v8m"));
        vo.bind("Irp", om.findStrict("ti.sysbios.interfaces.IHwi.Irp", "ti.sysbios.family.arm.v8m"));
        vo.bind("HookSet", om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.HookSet", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption", "ti.sysbios.family.arm.v8m"));
        vo.bind("StackInfo", om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.interfaces.IHwi.StackInfo", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("dispatcherAutoNestingSupport");
        mcfgs.add("dispatcherSwiSupport");
        mcfgs.add("dispatcherTaskSupport");
        mcfgs.add("dispatcherIrpTrackingSupport");
        mcfgs.add("NUM_INTERRUPTS");
        mcfgs.add("NUM_PRIORITIES");
        vo.bind("VectorFuncPtr", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.VectorFuncPtr", "ti.sysbios.family.arm.v8m"));
        vo.bind("ExceptionHookFuncPtr", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ExceptionHookFuncPtr", "ti.sysbios.family.arm.v8m"));
        vo.bind("CCR", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.CCR", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.CCR", "ti.sysbios.family.arm.v8m"));
        vo.bind("NVIC", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.NVIC", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.NVIC", "ti.sysbios.family.arm.v8m"));
        vo.bind("nvic", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.nvic", "ti.sysbios.family.arm.v8m"));
        vo.bind("ExcContext", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ExcContext", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ExcContext", "ti.sysbios.family.arm.v8m"));
        vo.bind("Struct2__", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Struct2__", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Struct2__", "ti.sysbios.family.arm.v8m"));
        vo.bind("Struct2", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Struct2", "ti.sysbios.family.arm.v8m"));
        vo.bind("BasicView", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.BasicView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.BasicView", "ti.sysbios.family.arm.v8m"));
        vo.bind("DetailedView", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.DetailedView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.DetailedView", "ti.sysbios.family.arm.v8m"));
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ModuleView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ModuleView", "ti.sysbios.family.arm.v8m"));
        vo.bind("VectorTableView", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.VectorTableView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.VectorTableView", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("LM_begin");
        mcfgs.add("LD_end");
        mcfgs.add("A_unsupportedMaskingOption");
        mcfgs.add("E_alreadyDefined");
        mcfgs.add("E_hwiLimitExceeded");
        mcfgs.add("E_exception");
        mcfgs.add("E_noIsr");
        mcfgs.add("E_NMI");
        mcfgs.add("E_hardFault");
        mcfgs.add("E_memFault");
        mcfgs.add("E_busFault");
        mcfgs.add("E_usageFault");
        mcfgs.add("E_svCall");
        mcfgs.add("E_debugMon");
        mcfgs.add("E_reserved");
        mcfgs.add("nullIsrFunc");
        vo.bind("ExcHandlerFuncPtr", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.ExcHandlerFuncPtr", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("excHandlerFunc");
        mcfgs.add("excHookFunc");
        mcfgs.add("disablePriority");
        mcfgs.add("priGroup");
        mcfgs.add("numSparseInterrupts");
        icfgs.add("numSparseInterrupts");
        icfgs.add("isTiva");
        mcfgs.add("swiDisable");
        icfgs.add("swiDisable");
        mcfgs.add("swiRestoreHwi");
        icfgs.add("swiRestoreHwi");
        mcfgs.add("taskDisable");
        icfgs.add("taskDisable");
        mcfgs.add("taskRestoreHwi");
        icfgs.add("taskRestoreHwi");
        mcfgs.add("ccr");
        icfgs.add("ccr");
        mcfgs.add("hooks");
        icfgs.add("hooks");
        vo.bind("HandlerFuncPtr", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.HandlerFuncPtr", "ti.sysbios.family.arm.v8m"));
        vo.bind("InterruptObj", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.InterruptObj", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.InterruptObj", "ti.sysbios.family.arm.v8m"));
        icfgs.add("interrupt");
        vo.bind("Instance_State", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Instance_State", "ti.sysbios.family.arm.v8m"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Module_State", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption_NONE", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_NONE", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption_ALL", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_ALL", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption_SELF", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_SELF", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption_BITMASK", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_BITMASK", "ti.sysbios.family.arm.v8m"));
        vo.bind("MaskingOption_LOWER", om.findStrict("ti.sysbios.interfaces.IHwi.MaskingOption_LOWER", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sysbios.family.arm.v8m.Hwi$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sysbios$family$arm$v8m$Hwi$$__initObject"));
        }//isCFG
        vo.bind("getStackInfo", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.getStackInfo", "ti.sysbios.family.arm.v8m"));
        vo.bind("getCoreStackInfo", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.getCoreStackInfo", "ti.sysbios.family.arm.v8m"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.startup", "ti.sysbios.family.arm.v8m"));
        vo.bind("switchFromBootStack", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.switchFromBootStack", "ti.sysbios.family.arm.v8m"));
        vo.bind("post", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.post", "ti.sysbios.family.arm.v8m"));
        vo.bind("getTaskSP", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.getTaskSP", "ti.sysbios.family.arm.v8m"));
        vo.bind("disableInterrupt", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.disableInterrupt", "ti.sysbios.family.arm.v8m"));
        vo.bind("enableInterrupt", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.enableInterrupt", "ti.sysbios.family.arm.v8m"));
        vo.bind("restoreInterrupt", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.restoreInterrupt", "ti.sysbios.family.arm.v8m"));
        vo.bind("clearInterrupt", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.clearInterrupt", "ti.sysbios.family.arm.v8m"));
        vo.bind("construct2", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.construct2", "ti.sysbios.family.arm.v8m"));
        vo.bind("disableFxn", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.disableFxn", "ti.sysbios.family.arm.v8m"));
        vo.bind("enableFxn", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.enableFxn", "ti.sysbios.family.arm.v8m"));
        vo.bind("restoreFxn", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.restoreFxn", "ti.sysbios.family.arm.v8m"));
        vo.bind("plug", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.plug", "ti.sysbios.family.arm.v8m"));
        vo.bind("getHandle", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.getHandle", "ti.sysbios.family.arm.v8m"));
        vo.bind("setPriority", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.setPriority", "ti.sysbios.family.arm.v8m"));
        vo.bind("excSetBuffers", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excSetBuffers", "ti.sysbios.family.arm.v8m"));
        vo.bind("initNVIC", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.initNVIC", "ti.sysbios.family.arm.v8m"));
        vo.bind("initStacks", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.initStacks", "ti.sysbios.family.arm.v8m"));
        vo.bind("postInit", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.postInit", "ti.sysbios.family.arm.v8m"));
        vo.bind("excHandlerAsm", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excHandlerAsm", "ti.sysbios.family.arm.v8m"));
        vo.bind("excHandler", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excHandler", "ti.sysbios.family.arm.v8m"));
        vo.bind("excHandlerMin", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excHandlerMin", "ti.sysbios.family.arm.v8m"));
        vo.bind("excHandlerMax", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excHandlerMax", "ti.sysbios.family.arm.v8m"));
        vo.bind("excFillContext", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excFillContext", "ti.sysbios.family.arm.v8m"));
        vo.bind("excNmi", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excNmi", "ti.sysbios.family.arm.v8m"));
        vo.bind("excHardFault", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excHardFault", "ti.sysbios.family.arm.v8m"));
        vo.bind("excMemFault", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excMemFault", "ti.sysbios.family.arm.v8m"));
        vo.bind("excBusFault", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excBusFault", "ti.sysbios.family.arm.v8m"));
        vo.bind("excUsageFault", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excUsageFault", "ti.sysbios.family.arm.v8m"));
        vo.bind("excSvCall", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excSvCall", "ti.sysbios.family.arm.v8m"));
        vo.bind("excDebugMon", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excDebugMon", "ti.sysbios.family.arm.v8m"));
        vo.bind("excReserved", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excReserved", "ti.sysbios.family.arm.v8m"));
        vo.bind("excNoIsr", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excNoIsr", "ti.sysbios.family.arm.v8m"));
        vo.bind("excDumpRegs", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.excDumpRegs", "ti.sysbios.family.arm.v8m"));
        vo.bind("pendSV", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.pendSV", "ti.sysbios.family.arm.v8m"));
        vo.bind("setStackLimit", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.setStackLimit", "ti.sysbios.family.arm.v8m"));
        vo.bind("dispatch", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.dispatch", "ti.sysbios.family.arm.v8m"));
        vo.bind("romInitNVIC", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.romInitNVIC", "ti.sysbios.family.arm.v8m"));
        vo.bind("dispatchC", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.dispatchC", "ti.sysbios.family.arm.v8m"));
        vo.bind("doSwiRestore", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.doSwiRestore", "ti.sysbios.family.arm.v8m"));
        vo.bind("doTaskRestore", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.doTaskRestore", "ti.sysbios.family.arm.v8m"));
        vo.bind("enableNonSecureFloatingPoint", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.enableNonSecureFloatingPoint", "ti.sysbios.family.arm.v8m"));
        vo.bind("enableNonSecureFaultHandlers", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.enableNonSecureFaultHandlers", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_Hwi_Handle__label__E", "ti_sysbios_family_arm_v8m_Hwi_Module__startupDone__E", "ti_sysbios_family_arm_v8m_Hwi_Object__create__E", "ti_sysbios_family_arm_v8m_Hwi_Object__delete__E", "ti_sysbios_family_arm_v8m_Hwi_Object__get__E", "ti_sysbios_family_arm_v8m_Hwi_Object__first__E", "ti_sysbios_family_arm_v8m_Hwi_Object__next__E", "ti_sysbios_family_arm_v8m_Hwi_Params__init__E", "ti_sysbios_family_arm_v8m_Hwi_getStackInfo__E", "ti_sysbios_family_arm_v8m_Hwi_getCoreStackInfo__E", "ti_sysbios_family_arm_v8m_Hwi_startup__E", "ti_sysbios_family_arm_v8m_Hwi_switchFromBootStack__E", "ti_sysbios_family_arm_v8m_Hwi_post__E", "ti_sysbios_family_arm_v8m_Hwi_getTaskSP__E", "ti_sysbios_family_arm_v8m_Hwi_disableInterrupt__E", "ti_sysbios_family_arm_v8m_Hwi_enableInterrupt__E", "ti_sysbios_family_arm_v8m_Hwi_restoreInterrupt__E", "ti_sysbios_family_arm_v8m_Hwi_clearInterrupt__E", "ti_sysbios_family_arm_v8m_Hwi_getFunc__E", "ti_sysbios_family_arm_v8m_Hwi_setFunc__E", "ti_sysbios_family_arm_v8m_Hwi_getHookContext__E", "ti_sysbios_family_arm_v8m_Hwi_setHookContext__E", "ti_sysbios_family_arm_v8m_Hwi_getIrp__E", "ti_sysbios_family_arm_v8m_Hwi_construct2__E", "ti_sysbios_family_arm_v8m_Hwi_disable__E", "ti_sysbios_family_arm_v8m_Hwi_enable__E", "ti_sysbios_family_arm_v8m_Hwi_restore__E", "ti_sysbios_family_arm_v8m_Hwi_disableFxn__E", "ti_sysbios_family_arm_v8m_Hwi_enableFxn__E", "ti_sysbios_family_arm_v8m_Hwi_restoreFxn__E", "ti_sysbios_family_arm_v8m_Hwi_plug__E", "ti_sysbios_family_arm_v8m_Hwi_getHandle__E", "ti_sysbios_family_arm_v8m_Hwi_setPriority__E", "ti_sysbios_family_arm_v8m_Hwi_excSetBuffers__E", "ti_sysbios_family_arm_v8m_Hwi_initNVIC__E", "ti_sysbios_family_arm_v8m_Hwi_initStacks__E", "ti_sysbios_family_arm_v8m_Hwi_reconfig__E"));
        vo.bind("$$logEvtCfgs", Global.newArray("LM_begin", "LD_end"));
        vo.bind("$$errorDescCfgs", Global.newArray("E_alreadyDefined", "E_hwiLimitExceeded", "E_exception", "E_noIsr", "E_NMI", "E_hardFault", "E_memFault", "E_busFault", "E_usageFault", "E_svCall", "E_debugMon", "E_reserved"));
        vo.bind("$$assertDescCfgs", Global.newArray("A_unsupportedMaskingOption"));
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "./Hwi.xdt");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sysbios.family.arm.v8m.Hwi.Object", "ti.sysbios.family.arm.v8m"));
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

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.IntrinsicsSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
        vo.bind("maxbit", om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport.maxbit", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_IntrinsicsSupport_Module__startupDone__E", "ti_sysbios_family_arm_v8m_IntrinsicsSupport_maxbit__E"));
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

    void TaskSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.TaskSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITaskSupport.FuncPtr", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("defaultStackSize");
        mcfgs.add("stackAlignment");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
        vo.bind("start", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.start", "ti.sysbios.family.arm.v8m"));
        vo.bind("swap", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.swap", "ti.sysbios.family.arm.v8m"));
        vo.bind("checkStack", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.checkStack", "ti.sysbios.family.arm.v8m"));
        vo.bind("stackUsed", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.stackUsed", "ti.sysbios.family.arm.v8m"));
        vo.bind("getStackAlignment", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.getStackAlignment", "ti.sysbios.family.arm.v8m"));
        vo.bind("getDefaultStackSize", om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport.getDefaultStackSize", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_TaskSupport_Module__startupDone__E", "ti_sysbios_family_arm_v8m_TaskSupport_start__E", "ti_sysbios_family_arm_v8m_TaskSupport_swap__E", "ti_sysbios_family_arm_v8m_TaskSupport_checkStack__E", "ti_sysbios_family_arm_v8m_TaskSupport_stackUsed__E", "ti_sysbios_family_arm_v8m_TaskSupport_getStackAlignment__E", "ti_sysbios_family_arm_v8m_TaskSupport_getDefaultStackSize__E"));
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
        pkgV.bind("TaskSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TaskSupport");
    }

    void Timer$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.Timer", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.Timer$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("Instance", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance", "ti.sysbios.family.arm.v8m"));
        vo.bind("Params", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Params", "ti.sysbios.family.arm.v8m"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.sysbios.family.arm.v8m.Timer.Params", "ti.sysbios.family.arm.v8m")).newInstance());
        vo.bind("Handle", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Handle", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITimer.FuncPtr", "ti.sysbios.family.arm.v8m"));
        vo.bind("StartMode", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode", "ti.sysbios.family.arm.v8m"));
        vo.bind("RunMode", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode", "ti.sysbios.family.arm.v8m"));
        vo.bind("Status", om.findStrict("ti.sysbios.interfaces.ITimer.Status", "ti.sysbios.family.arm.v8m"));
        vo.bind("PeriodType", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType", "ti.sysbios.family.arm.v8m"));
        vo.bind("BasicView", om.findStrict("ti.sysbios.family.arm.v8m.Timer.BasicView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Timer.BasicView", "ti.sysbios.family.arm.v8m"));
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.arm.v8m.Timer.ModuleView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Timer.ModuleView", "ti.sysbios.family.arm.v8m"));
        vo.bind("DeviceView", om.findStrict("ti.sysbios.family.arm.v8m.Timer.DeviceView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Timer.DeviceView", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("E_invalidTimer");
        mcfgs.add("E_notAvailable");
        mcfgs.add("E_cannotSupport");
        mcfgs.add("anyMask");
        icfgs.add("timerInUse");
        mcfgs.add("startupNeeded");
        icfgs.add("startupNeeded");
        vo.bind("Instance_State", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Timer.Instance_State", "ti.sysbios.family.arm.v8m"));
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.Timer.Module_State", "ti.sysbios.family.arm.v8m"));
        vo.bind("StartMode_AUTO", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode_AUTO", "ti.sysbios.family.arm.v8m"));
        vo.bind("StartMode_USER", om.findStrict("ti.sysbios.interfaces.ITimer.StartMode_USER", "ti.sysbios.family.arm.v8m"));
        vo.bind("RunMode_CONTINUOUS", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_CONTINUOUS", "ti.sysbios.family.arm.v8m"));
        vo.bind("RunMode_ONESHOT", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_ONESHOT", "ti.sysbios.family.arm.v8m"));
        vo.bind("RunMode_DYNAMIC", om.findStrict("ti.sysbios.interfaces.ITimer.RunMode_DYNAMIC", "ti.sysbios.family.arm.v8m"));
        vo.bind("Status_INUSE", om.findStrict("ti.sysbios.interfaces.ITimer.Status_INUSE", "ti.sysbios.family.arm.v8m"));
        vo.bind("Status_FREE", om.findStrict("ti.sysbios.interfaces.ITimer.Status_FREE", "ti.sysbios.family.arm.v8m"));
        vo.bind("PeriodType_MICROSECS", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType_MICROSECS", "ti.sysbios.family.arm.v8m"));
        vo.bind("PeriodType_COUNTS", om.findStrict("ti.sysbios.interfaces.ITimer.PeriodType_COUNTS", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
            vo.bind("$$meta_iobj", om.has("ti.sysbios.family.arm.v8m.Timer$$instance$static$init", null) ? 1 : 0);
            vo.bind("__initObject", Global.get("ti$sysbios$family$arm$v8m$Timer$$__initObject"));
        }//isCFG
        vo.bind("getNumTimers", om.findStrict("ti.sysbios.family.arm.v8m.Timer.getNumTimers", "ti.sysbios.family.arm.v8m"));
        vo.bind("getStatus", om.findStrict("ti.sysbios.family.arm.v8m.Timer.getStatus", "ti.sysbios.family.arm.v8m"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.arm.v8m.Timer.startup", "ti.sysbios.family.arm.v8m"));
        vo.bind("getHandle", om.findStrict("ti.sysbios.family.arm.v8m.Timer.getHandle", "ti.sysbios.family.arm.v8m"));
        vo.bind("getTickCount", om.findStrict("ti.sysbios.family.arm.v8m.Timer.getTickCount", "ti.sysbios.family.arm.v8m"));
        vo.bind("initDevice", om.findStrict("ti.sysbios.family.arm.v8m.Timer.initDevice", "ti.sysbios.family.arm.v8m"));
        vo.bind("postInit", om.findStrict("ti.sysbios.family.arm.v8m.Timer.postInit", "ti.sysbios.family.arm.v8m"));
        vo.bind("oneShotStub", om.findStrict("ti.sysbios.family.arm.v8m.Timer.oneShotStub", "ti.sysbios.family.arm.v8m"));
        vo.bind("periodicStub", om.findStrict("ti.sysbios.family.arm.v8m.Timer.periodicStub", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_Timer_Handle__label__E", "ti_sysbios_family_arm_v8m_Timer_Module__startupDone__E", "ti_sysbios_family_arm_v8m_Timer_Object__create__E", "ti_sysbios_family_arm_v8m_Timer_Object__delete__E", "ti_sysbios_family_arm_v8m_Timer_Object__get__E", "ti_sysbios_family_arm_v8m_Timer_Object__first__E", "ti_sysbios_family_arm_v8m_Timer_Object__next__E", "ti_sysbios_family_arm_v8m_Timer_Params__init__E", "ti_sysbios_family_arm_v8m_Timer_getNumTimers__E", "ti_sysbios_family_arm_v8m_Timer_getStatus__E", "ti_sysbios_family_arm_v8m_Timer_startup__E", "ti_sysbios_family_arm_v8m_Timer_getMaxTicks__E", "ti_sysbios_family_arm_v8m_Timer_setNextTick__E", "ti_sysbios_family_arm_v8m_Timer_start__E", "ti_sysbios_family_arm_v8m_Timer_stop__E", "ti_sysbios_family_arm_v8m_Timer_setPeriod__E", "ti_sysbios_family_arm_v8m_Timer_setPeriodMicroSecs__E", "ti_sysbios_family_arm_v8m_Timer_getPeriod__E", "ti_sysbios_family_arm_v8m_Timer_getCount__E", "ti_sysbios_family_arm_v8m_Timer_getFreq__E", "ti_sysbios_family_arm_v8m_Timer_getFunc__E", "ti_sysbios_family_arm_v8m_Timer_setFunc__E", "ti_sysbios_family_arm_v8m_Timer_trigger__E", "ti_sysbios_family_arm_v8m_Timer_getExpiredCounts__E", "ti_sysbios_family_arm_v8m_Timer_getExpiredTicks__E", "ti_sysbios_family_arm_v8m_Timer_getCurrentTick__E", "ti_sysbios_family_arm_v8m_Timer_getHandle__E", "ti_sysbios_family_arm_v8m_Timer_getTickCount__E", "ti_sysbios_family_arm_v8m_Timer_reconfig__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_invalidTimer", "E_notAvailable", "E_cannotSupport"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.sysbios.family.arm.v8m.Timer.Object", "ti.sysbios.family.arm.v8m"));
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Timer", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Timer");
    }

    void TimestampProvider$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.TimestampProvider", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        mcfgs.add("configTimer");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.Module_State", "ti.sysbios.family.arm.v8m"));
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
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
        vo.bind("get32", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.get32", "ti.sysbios.family.arm.v8m"));
        vo.bind("get64", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.get64", "ti.sysbios.family.arm.v8m"));
        vo.bind("getFreq", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.getFreq", "ti.sysbios.family.arm.v8m"));
        vo.bind("startTimer", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.startTimer", "ti.sysbios.family.arm.v8m"));
        vo.bind("rolloverFunc", om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider.rolloverFunc", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_TimestampProvider_Module__startupDone__E", "ti_sysbios_family_arm_v8m_TimestampProvider_get32__E", "ti_sysbios_family_arm_v8m_TimestampProvider_get64__E", "ti_sysbios_family_arm_v8m_TimestampProvider_getFreq__E", "ti_sysbios_family_arm_v8m_TimestampProvider_startTimer__E", "ti_sysbios_family_arm_v8m_TimestampProvider_rolloverFunc__E"));
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

    void Power$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Power", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Power.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.Power", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.Power$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        mcfgs.add("idle");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
        vo.bind("idleCPU", om.findStrict("ti.sysbios.family.arm.v8m.Power.idleCPU", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_Power_Module__startupDone__E", "ti_sysbios_family_arm_v8m_Power_idleCPU__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "./Power.xdt");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./Power.xdt");
        pkgV.bind("Power", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Power");
    }

    void SAU$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU", "ti.sysbios.family.arm.v8m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module", "ti.sysbios.family.arm.v8m");
        vo.init2(po, "ti.sysbios.family.arm.v8m.SAU", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v8m.SAU$$capsule", "ti.sysbios.family.arm.v8m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v8m", "ti.sysbios.family.arm.v8m"));
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
        vo.bind("RegionsView", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionsView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionsView", "ti.sysbios.family.arm.v8m"));
        vo.bind("RegsView", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegsView", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegsView", "ti.sysbios.family.arm.v8m"));
        vo.bind("DeviceRegs", om.findStrict("ti.sysbios.family.arm.v8m.SAU.DeviceRegs", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.SAU.DeviceRegs", "ti.sysbios.family.arm.v8m"));
        vo.bind("deviceRegs", om.findStrict("ti.sysbios.family.arm.v8m.SAU.deviceRegs", "ti.sysbios.family.arm.v8m"));
        vo.bind("RegionType", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType", "ti.sysbios.family.arm.v8m"));
        mcfgs.add("A_nullPointer");
        mcfgs.add("A_invalidRegionId");
        mcfgs.add("A_unalignedBaseAddr");
        mcfgs.add("A_unalignedEndAddr");
        mcfgs.add("enableSAU");
        mcfgs.add("allSecureWhenSauDisabled");
        mcfgs.add("numRegions");
        vo.bind("RegionEntry", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionEntry", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionEntry", "ti.sysbios.family.arm.v8m"));
        icfgs.add("regionEntry");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module_State", "ti.sysbios.family.arm.v8m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v8m.SAU.Module_State", "ti.sysbios.family.arm.v8m"));
        vo.bind("RegionType_NS", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType_NS", "ti.sysbios.family.arm.v8m"));
        vo.bind("RegionType_NSC", om.findStrict("ti.sysbios.family.arm.v8m.SAU.RegionType_NSC", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v8m")).add(vo);
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
        vo.bind("disable", om.findStrict("ti.sysbios.family.arm.v8m.SAU.disable", "ti.sysbios.family.arm.v8m"));
        vo.bind("enable", om.findStrict("ti.sysbios.family.arm.v8m.SAU.enable", "ti.sysbios.family.arm.v8m"));
        vo.bind("isEnabled", om.findStrict("ti.sysbios.family.arm.v8m.SAU.isEnabled", "ti.sysbios.family.arm.v8m"));
        vo.bind("configureRegion", om.findStrict("ti.sysbios.family.arm.v8m.SAU.configureRegion", "ti.sysbios.family.arm.v8m"));
        vo.bind("enableRegion", om.findStrict("ti.sysbios.family.arm.v8m.SAU.enableRegion", "ti.sysbios.family.arm.v8m"));
        vo.bind("disableRegion", om.findStrict("ti.sysbios.family.arm.v8m.SAU.disableRegion", "ti.sysbios.family.arm.v8m"));
        vo.bind("startup", om.findStrict("ti.sysbios.family.arm.v8m.SAU.startup", "ti.sysbios.family.arm.v8m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v8m_SAU_Module__startupDone__E", "ti_sysbios_family_arm_v8m_SAU_disable__E", "ti_sysbios_family_arm_v8m_SAU_enable__E", "ti_sysbios_family_arm_v8m_SAU_isEnabled__E", "ti_sysbios_family_arm_v8m_SAU_configureRegion__E", "ti_sysbios_family_arm_v8m_SAU_enableRegion__E", "ti_sysbios_family_arm_v8m_SAU_disableRegion__E"));
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
        pkgV.bind("SAU", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SAU");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
            Object srcP = ((XScriptO)om.findStrict("xdc.runtime.IInstance", "ti.sysbios.family.arm.v8m")).findStrict("PARAMS", "ti.sysbios.family.arm.v8m");
            Scriptable dstP;

            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sysbios.family.arm.v8m.Hwi", "ti.sysbios.family.arm.v8m")).findStrict("PARAMS", "ti.sysbios.family.arm.v8m");
            Global.put(dstP, "instance", srcP);
            dstP = (Scriptable)((XScriptO)om.findStrict("ti.sysbios.family.arm.v8m.Timer", "ti.sysbios.family.arm.v8m")).findStrict("PARAMS", "ti.sysbios.family.arm.v8m");
            Global.put(dstP, "instance", srcP);
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.Hwi", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.IntrinsicsSupport", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.TaskSupport", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.Timer", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.TimestampProvider", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.Power", "ti.sysbios.family.arm.v8m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v8m.SAU", "ti.sysbios.family.arm.v8m"));
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Hwi", "ti.sysbios.family.arm.v8m");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.INSTANCE"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")}), Global.newArray(new Object[]{"Detailed", Global.newObject("type", om.find("xdc.rov.ViewInfo.INSTANCE"), "viewInitFxn", "viewInitDetailed", "structName", "DetailedView")}), Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")}), Global.newArray(new Object[]{"Exception", Global.newObject("type", om.find("xdc.rov.ViewInfo.TREE"), "viewInitFxn", "viewInitException", "structName", "ExcContext")}), Global.newArray(new Object[]{"Vector Table", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitVectorTable", "structName", "VectorTableView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.Timer", "ti.sysbios.family.arm.v8m");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.INSTANCE"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")}), Global.newArray(new Object[]{"Device", Global.newObject("type", om.find("xdc.rov.ViewInfo.INSTANCE"), "viewInitFxn", "viewInitDevice", "structName", "DeviceView")}), Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v8m.SAU", "ti.sysbios.family.arm.v8m");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"SauRegions", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewSauRegions", "structName", "RegionsView")}), Global.newArray(new Object[]{"SauRegisters", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewSauRegisters", "structName", "RegsView")})}))));
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.Hwi")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.IntrinsicsSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.TaskSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.Timer")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.TimestampProvider")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.Power")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v8m.SAU")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.arm.v8m")).add(pkgV);
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
        Hwi$$OBJECTS();
        IntrinsicsSupport$$OBJECTS();
        TaskSupport$$OBJECTS();
        Timer$$OBJECTS();
        TimestampProvider$$OBJECTS();
        Power$$OBJECTS();
        SAU$$OBJECTS();
        Hwi$$CONSTS();
        IntrinsicsSupport$$CONSTS();
        TaskSupport$$CONSTS();
        Timer$$CONSTS();
        TimestampProvider$$CONSTS();
        Power$$CONSTS();
        SAU$$CONSTS();
        Hwi$$CREATES();
        IntrinsicsSupport$$CREATES();
        TaskSupport$$CREATES();
        Timer$$CREATES();
        TimestampProvider$$CREATES();
        Power$$CREATES();
        SAU$$CREATES();
        Hwi$$FUNCTIONS();
        IntrinsicsSupport$$FUNCTIONS();
        TaskSupport$$FUNCTIONS();
        Timer$$FUNCTIONS();
        TimestampProvider$$FUNCTIONS();
        Power$$FUNCTIONS();
        SAU$$FUNCTIONS();
        Hwi$$SIZES();
        IntrinsicsSupport$$SIZES();
        TaskSupport$$SIZES();
        Timer$$SIZES();
        TimestampProvider$$SIZES();
        Power$$SIZES();
        SAU$$SIZES();
        Hwi$$TYPES();
        IntrinsicsSupport$$TYPES();
        TaskSupport$$TYPES();
        Timer$$TYPES();
        TimestampProvider$$TYPES();
        Power$$TYPES();
        SAU$$TYPES();
        if (isROV) {
            Hwi$$ROV();
            IntrinsicsSupport$$ROV();
            TaskSupport$$ROV();
            Timer$$ROV();
            TimestampProvider$$ROV();
            Power$$ROV();
            SAU$$ROV();
        }//isROV
        $$SINGLETONS();
        Hwi$$SINGLETONS();
        IntrinsicsSupport$$SINGLETONS();
        TaskSupport$$SINGLETONS();
        Timer$$SINGLETONS();
        TimestampProvider$$SINGLETONS();
        Power$$SINGLETONS();
        SAU$$SINGLETONS();
        $$INITIALIZATION();
    }
}
