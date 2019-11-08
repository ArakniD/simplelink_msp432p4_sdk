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

public class ti_sysbios_family_arm_v7m
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
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.arm.v7m.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.arm.v7m", new Value.Obj("ti.sysbios.family.arm.v7m", pkgP));
    }

    void MemProtect$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v7m.MemProtect.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v7m.MemProtect", new Value.Obj("ti.sysbios.family.arm.v7m.MemProtect", po));
        pkgV.bind("MemProtect", vo);
        // decls 
    }

    void SysCall$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.v7m.SysCall.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.v7m.SysCall", new Value.Obj("ti.sysbios.family.arm.v7m.SysCall", po));
        pkgV.bind("SysCall", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.v7m.SysCall$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.v7m.SysCall.Module_State", new Proto.Str(spo, false));
    }

    void MemProtect$$CONSTS()
    {
        // module MemProtect
    }

    void SysCall$$CONSTS()
    {
        // module SysCall
        om.bind("ti.sysbios.family.arm.v7m.SysCall.NUM_SYSCALLS", 3L);
        om.bind("ti.sysbios.family.arm.v7m.SysCall.requestFailed", new Extern("ti_sysbios_family_arm_v7m_SysCall_requestFailed__I", "xdc_Void(*)(xdc_UArg)", true, false));
        om.bind("ti.sysbios.family.arm.v7m.SysCall.enterPrivModeI", new Extern("ti_sysbios_family_arm_v7m_SysCall_enterPrivModeI__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v7m.SysCall.enterUnprivModeI", new Extern("ti_sysbios_family_arm_v7m_SysCall_enterUnprivModeI__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v7m.SysCall.restorePrivModeI", new Extern("ti_sysbios_family_arm_v7m_SysCall_restorePrivModeI__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.v7m.SysCall.svcHandler", new Extern("ti_sysbios_family_arm_v7m_SysCall_svcHandler__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void MemProtect$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void SysCall$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void MemProtect$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SysCall$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void MemProtect$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void SysCall$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.v7m.SysCall.Module_State", "ti.sysbios.family.arm.v7m");
        sizes.clear();
        sizes.add(Global.newArray("sysCallTable", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.v7m.SysCall.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.v7m.SysCall.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.v7m.SysCall.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void MemProtect$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v7m/MemProtect.xs");
        om.bind("ti.sysbios.family.arm.v7m.MemProtect$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.MemProtect.Module", "ti.sysbios.family.arm.v7m");
        po.init("ti.sysbios.family.arm.v7m.MemProtect.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.arm.v7m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.MemProtect$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.MemProtect$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.MemProtect$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.MemProtect$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void SysCall$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v7m/SysCall.xs");
        om.bind("ti.sysbios.family.arm.v7m.SysCall$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall.Module", "ti.sysbios.family.arm.v7m");
        po.init("ti.sysbios.family.arm.v7m.SysCall.Module", om.findStrict("ti.sysbios.interfaces.ISysCall.Module", "ti.sysbios.family.arm.v7m"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("NUM_SYSCALLS", Proto.Elm.newCNum("(xdc_UInt16)"), 3L, "rh");
        if (isCFG) {
            po.addFld("E_requestFailed", (Proto)om.findStrict("xdc.runtime.Error$$Id", "ti.sysbios.family.arm.v7m"), Global.newObject("msg", "E_requestFailed: Request (Svc #%d) to switch CPU privilege denied."), "w");
            po.addFld("requestFailedFunc", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"), $$UNDEF, "w");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.SysCall$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.SysCall$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.SysCall$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.v7m.SysCall$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // typedef SysCall.FuncPtr
        om.bind("ti.sysbios.family.arm.v7m.SysCall.FuncPtr", new Proto.Adr("xdc_Void(*)(xdc_UArg)", "PFv"));
        // struct SysCall.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall$$Module_State", "ti.sysbios.family.arm.v7m");
        po.init("ti.sysbios.family.arm.v7m.SysCall.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("sysCallTable", new Proto.Arr(new Proto.Adr("xdc_Ptr", "Pv"), false), $$DEFAULT, "w");
    }

    void MemProtect$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v7m.MemProtect", "ti.sysbios.family.arm.v7m");
    }

    void SysCall$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall", "ti.sysbios.family.arm.v7m");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.v7m.SysCall.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall$$Module_State", "ti.sysbios.family.arm.v7m");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.arm.v7m.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.arm.v7m"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/v7m/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.arm.v7m"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.arm.v7m"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.arm.v7m"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.arm.v7m"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.arm.v7m"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.arm.v7m"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.arm.v7m", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.arm.v7m");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.arm.v7m.");
        pkgV.bind("$vers", Global.newArray(2, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.arm.v7m'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.arm.v7m$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.arm.v7m$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.arm.v7m$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/ti.sysbios.family.arm.v7m.aem4f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/ti.sysbios.family.arm.v7m.aem4f', {target: 'ti.targets.arm.elf.M4F', suffix: 'em4f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void MemProtect$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v7m.MemProtect", "ti.sysbios.family.arm.v7m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.MemProtect.Module", "ti.sysbios.family.arm.v7m");
        vo.init2(po, "ti.sysbios.family.arm.v7m.MemProtect", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v7m.MemProtect$$capsule", "ti.sysbios.family.arm.v7m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v7m", "ti.sysbios.family.arm.v7m"));
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
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v7m")).add(vo);
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
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v7m_MemProtect_Module__startupDone__E"));
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
        pkgV.bind("MemProtect", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("MemProtect");
    }

    void SysCall$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall", "ti.sysbios.family.arm.v7m");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.v7m.SysCall.Module", "ti.sysbios.family.arm.v7m");
        vo.init2(po, "ti.sysbios.family.arm.v7m.SysCall", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.v7m.SysCall$$capsule", "ti.sysbios.family.arm.v7m"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.v7m", "ti.sysbios.family.arm.v7m"));
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
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.FuncPtr", "ti.sysbios.family.arm.v7m"));
        mcfgs.add("E_requestFailed");
        mcfgs.add("requestFailedFunc");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.Module_State", "ti.sysbios.family.arm.v7m"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.v7m.SysCall.Module_State", "ti.sysbios.family.arm.v7m"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.v7m")).add(vo);
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
        vo.bind("requestFailed", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.requestFailed", "ti.sysbios.family.arm.v7m"));
        vo.bind("enterPrivModeI", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.enterPrivModeI", "ti.sysbios.family.arm.v7m"));
        vo.bind("enterUnprivModeI", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.enterUnprivModeI", "ti.sysbios.family.arm.v7m"));
        vo.bind("restorePrivModeI", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.restorePrivModeI", "ti.sysbios.family.arm.v7m"));
        vo.bind("svcHandler", om.findStrict("ti.sysbios.family.arm.v7m.SysCall.svcHandler", "ti.sysbios.family.arm.v7m"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_v7m_SysCall_Module__startupDone__E", "ti_sysbios_family_arm_v7m_SysCall_enterPrivMode__E", "ti_sysbios_family_arm_v7m_SysCall_enterUnprivMode__E", "ti_sysbios_family_arm_v7m_SysCall_restorePrivMode__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray("E_requestFailed"));
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("SysCall", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SysCall");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v7m.MemProtect", "ti.sysbios.family.arm.v7m"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.v7m.SysCall", "ti.sysbios.family.arm.v7m"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v7m.MemProtect")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.v7m.SysCall")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.arm.v7m")).add(pkgV);
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
        MemProtect$$OBJECTS();
        SysCall$$OBJECTS();
        MemProtect$$CONSTS();
        SysCall$$CONSTS();
        MemProtect$$CREATES();
        SysCall$$CREATES();
        MemProtect$$FUNCTIONS();
        SysCall$$FUNCTIONS();
        MemProtect$$SIZES();
        SysCall$$SIZES();
        MemProtect$$TYPES();
        SysCall$$TYPES();
        if (isROV) {
            MemProtect$$ROV();
            SysCall$$ROV();
        }//isROV
        $$SINGLETONS();
        MemProtect$$SINGLETONS();
        SysCall$$SINGLETONS();
        $$INITIALIZATION();
    }
}
