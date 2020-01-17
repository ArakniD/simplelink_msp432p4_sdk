/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-H32
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_sysbios_family_arm_msp432_init
{
    static final String VERS = "@(#) xdc-H32\n";

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
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.arm.msp432.init.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.arm.msp432.init", new Value.Obj("ti.sysbios.family.arm.msp432.init", pkgP));
    }

    void Boot$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.msp432.init.Boot.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.msp432.init.Boot", new Value.Obj("ti.sysbios.family.arm.msp432.init.Boot", po));
        pkgV.bind("Boot", vo);
        // decls 
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", new Proto.Enm("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt"));
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.msp432.init.Boot$$ModuleView", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.ModuleView", new Proto.Str(spo, false));
    }

    void Boot$$CONSTS()
    {
        // module Boot
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Low", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", "ti.sysbios.family.arm.msp432.init"), "ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Low", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Medium", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", "ti.sysbios.family.arm.msp432.init"), "ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Medium", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_High", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", "ti.sysbios.family.arm.msp432.init"), "ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_High", xdc.services.intern.xsr.Enum.intValue(2L)+0));
        om.bind("ti.sysbios.family.arm.msp432.init.Boot.init", new Extern("ti_sysbios_family_arm_msp432_init_Boot_init__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void Boot$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Boot$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn Boot.registerFreqListener
        fxn = (Proto.Fxn)om.bind("ti.sysbios.family.arm.msp432.init.Boot$$registerFreqListener", new Proto.Fxn(om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.Module", "ti.sysbios.family.arm.msp432.init"), null, 0, -1, false));
    }

    void Boot$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Boot$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/msp432/init/Boot.xs");
        om.bind("ti.sysbios.family.arm.msp432.init.Boot$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.Module", "ti.sysbios.family.arm.msp432.init");
        po.init("ti.sysbios.family.arm.msp432.init.Boot.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.arm.msp432.init"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.sysbios.family.arm.msp432.init"), $$UNDEF, "wh");
            po.addFld("configureClocks", $$T_Bool, $$UNDEF, "wh");
            po.addFld("speedSelect", (Proto)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", "ti.sysbios.family.arm.msp432.init"), om.find("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_High"), "wh");
            po.addFld("enableLFXT", $$T_Bool, false, "w");
            po.addFld("bypassLFXT", $$T_Bool, false, "w");
            po.addFld("enableHFXT", $$T_Bool, false, "w");
            po.addFld("bypassHFXT", $$T_Bool, false, "w");
            po.addFld("disableWatchdog", $$T_Bool, true, "wh");
            po.addFld("computedCpuFrequency", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.msp432.init.Boot$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.msp432.init.Boot$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.msp432.init.Boot$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.msp432.init.Boot$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                po.addFxn("registerFreqListener", (Proto.Fxn)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot$$registerFreqListener", "ti.sysbios.family.arm.msp432.init"), Global.get(cap, "registerFreqListener"));
        // struct Boot.ModuleView
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot$$ModuleView", "ti.sysbios.family.arm.msp432.init");
        po.init("ti.sysbios.family.arm.msp432.init.Boot.ModuleView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("configureClocks", $$T_Bool, $$UNDEF, "w");
                po.addFld("disableWatchdog", $$T_Bool, $$UNDEF, "w");
    }

    void Boot$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot", "ti.sysbios.family.arm.msp432.init");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.arm.msp432.init.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.arm.msp432.init"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/msp432/init/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.arm.msp432.init"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.arm.msp432.init"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.arm.msp432.init"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.arm.msp432.init"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.arm.msp432.init"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.arm.msp432.init"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.arm.msp432.init", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.arm.msp432.init");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.arm.msp432.init.");
        pkgV.bind("$vers", Global.newArray(1, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.arm.msp432.init'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.arm.msp432.init$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.arm.msp432.init$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.arm.msp432.init$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/debug/ti.sysbios.family.arm.msp432.init.aem4f',\n");
            sb.append("'lib/debug/ti.sysbios.family.arm.msp432.init.am4fg',\n");
            sb.append("'lib/debug/ti.sysbios.family.arm.msp432.init.arm4f',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/debug/ti.sysbios.family.arm.msp432.init.aem4f', {target: 'ti.targets.arm.elf.M4F', suffix: 'em4f'}],\n");
            sb.append("['lib/debug/ti.sysbios.family.arm.msp432.init.am4fg', {target: 'gnu.targets.arm.M4F', suffix: 'm4fg'}],\n");
            sb.append("['lib/debug/ti.sysbios.family.arm.msp432.init.arm4f', {target: 'iar.targets.arm.M4F', suffix: 'rm4f'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void Boot$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot", "ti.sysbios.family.arm.msp432.init");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.Module", "ti.sysbios.family.arm.msp432.init");
        vo.init2(po, "ti.sysbios.family.arm.msp432.init.Boot", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot$$capsule", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm.msp432.init", "ti.sysbios.family.arm.msp432.init"));
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
        vo.bind("SpeedOpt", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("ModuleView", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.ModuleView", "ti.sysbios.family.arm.msp432.init"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.ModuleView", "ti.sysbios.family.arm.msp432.init"));
        mcfgs.add("enableLFXT");
        mcfgs.add("bypassLFXT");
        mcfgs.add("enableHFXT");
        mcfgs.add("bypassHFXT");
        icfgs.add("computedCpuFrequency");
        vo.bind("SpeedOpt_Low", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Low", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("SpeedOpt_Medium", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_Medium", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("SpeedOpt_High", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.SpeedOpt_High", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm.msp432.init")).add(vo);
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
        vo.bind("init", om.findStrict("ti.sysbios.family.arm.msp432.init.Boot.init", "ti.sysbios.family.arm.msp432.init"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_msp432_init_Boot_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "./Boot.xdt");
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./Boot.xdt");
        pkgV.bind("Boot", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Boot");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot", "ti.sysbios.family.arm.msp432.init"));
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.msp432.init.Boot", "ti.sysbios.family.arm.msp432.init");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Module", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE"), "viewInitFxn", "viewInitModule", "structName", "ModuleView")})}))));
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.arm.msp432.init.Boot")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.arm.msp432.init")).add(pkgV);
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
        Boot$$OBJECTS();
        Boot$$CONSTS();
        Boot$$CREATES();
        Boot$$FUNCTIONS();
        Boot$$SIZES();
        Boot$$TYPES();
        if (isROV) {
            Boot$$ROV();
        }//isROV
        $$SINGLETONS();
        Boot$$SINGLETONS();
        $$INITIALIZATION();
    }
}
