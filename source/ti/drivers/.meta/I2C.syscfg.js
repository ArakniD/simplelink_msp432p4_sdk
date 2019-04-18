/*
 * Copyright (c) 2018 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 *  ======== I2C.syscfg.js ========
 */

/*
 * I2C speed configuration (Philips I2C specification 
 * http://www.nxp.com/documents/user_manual/UM10204.pdf):
 *    Standard-mode, Fast-mode (Fm), Fast-mode Plus (Fm+), and High-speed
 *    mode (Hs-mode) devices are downward-compatible - any device may be 
 *    operated at a lower bus speed. Ultra Fast-mode devices are not 
 *    compatible with previous versions since the bus is unidirectional.
 *        Bidirectional bus:
 *            o Standard-mode (Sm), with a bit rate up to 100 kbit/s
 *            o Fast-mode (Fm), with a bit rate up to 400 kbit/s
 *            o Fast-mode Plus (Fm+), with a bit rate up to 1 Mbit/s
 *            o High-speed mode (Hs-mode), with a bit rate up to 3.4 Mbit/s.
 *        Unidirectional bus:
 *            o Ultra Fast-mode (UFm), with a bit rate up to 5 Mbit/s
 * 
 * Each device connected to the bus is software addressable by a unique
 * address and simple master/slave relationships exist at all times.
 * 
 * A master is the device which initiates a data transfer on the bus and
 * generates the clock signals to permit that transfer. At that time,
 * any device addressed is considered a slave.
 */

"use strict";

/* get Common /ti/drivers utility functions */
let Common = system.getScript("/ti/drivers/Common.js");
let logError = Common.logError;
let logWarning = Common.logWarning;
let logInfo = Common.logInfo;

/* get /ti/drivers family name from device object */
let family = Common.device2Family(system.deviceData, "I2C");

/* generic configuration parameters for I2C instances */
let config = [
    {
        name        : "maxBitRate",
        displayName : "Maximum Bit Rate",
        description : "Maximum bit rate (Kbps) supported by the board",
        longDescription: "The maximum bit rate for an I2C bus is a function of"
            + " the electrical capacitance and resistance on the I2C signal"
            + " lines.  By default, a warning will be triggered in the event"
            + " the declared maximum speed of an attached slave is less then"
            + " this value.  If this configuration parameter is set to zero,"
            + " the assumed maximum speed for this instance is the maximum"
            + " speed supported by all attached slaves.",
        default     : 0
    },
    {
        name        : "ignoreUnusedAddressConflicts",
        displayName : "Ignore Unused Address Conflicts",
        description : "Don't warn about address conflicts between "
                      + "unused components on this bus",
        default     : false
    },
    {
        name        : "addressChecks",
        displayName : "Address Checks",
        description : "How to handle detected peripheral address conflicts",
        default     : "Fail",
        options     : [
            { name:  "Warn"},
            { name:  "Fail"},
            { name:  "Remark"}
        ]
    },
    {
        name        : "speedChecks",
        displayName : "Speed Checks",
        description : "How to handle detected bus speed conflicts",
        default     : "Fail",
        options     : [
            { name:  "Warn"},
            { name:  "Fail"},
            { name:  "Remark"}
        ]
    }
]; 

/* map validation options to validation logging functions */
let logHandlers = {"Warn": logWarning, "Fail": logError, "Remark": logInfo};

/**
 *  ======== getAddresses ========
 *  Get the range of valid I2C addresses to which a component may respond
 *
 *  Given an I2C component, extract and return the "actual" range of
 *  I2C addresses this component supports (as specified by the HW component).
 *  This is often a much smaller subset of the "full" range of addresses
 *  supported by the underlying peripheral that's documented by the
 *  peripheral's datasheet.  Boards often tie the HW peripheral's address
 *  selection pins to fixed levels to ensure all peripherals on the same
 *  I2C bus have unique addresses.
 *
 *  @param {Object} component - a board component assigned to $hardware; note,
 *                              a bus modules $hardware is an array of such
 *                              components.
 *
 *  @returns {Array} - numeric addresses declared in by the component's
 *                     meta-data specification.  If a component is not an I2C
 *                     component or no addresses are declared in the the
 *                     componnet's meta-data, an empty array is returned.
 */
function getAddresses(component)
{
    let result = null;

    /* I2C addresses are declared in the component's settings object */
    if (component != null && component.settings != null) {

        /* find the I2C signals in the component */
        if (component.settings.address != null) {

            //console.log("I2C.getAddresses(" + component.name + "): address: "
            //        + component.settings.address);

            /* address settings can be a single string or an array */
            result = component.settings.address;
            if (!(result instanceof Array)) {
                result = [result];
            }
        }
    }

    return (result == null ? [] : result);
}

/*
 *  ======== getCompAddress ========
 *  Get a component's assigned address
 */
function getCompAddress(comp) 
{
    /* first check the HW for the range of addresses */
    let addr = comp.settings.address;
    if (!(addr instanceof Array)) {
        if (addr != null) {
            return (Number(addr));
        }
    }
    else {
        if (addr.length == 1) {
            return (Number(addr[0]));
        }
        addr = addr[0]; /* the default address when there's a choice */
    }
    
    /* the HW supports > 1 possible address, so get it from SW */
    for (let moduleName in system.modules) {
        let mod = system.modules[moduleName];
        let instances = mod.$instances;
        for (let i = 0; i < instances.length; i++) {
            let xinst = instances[i];
            let xinstHW = xinst.$hardware ? xinst.$hardware.name : null;
            if (("i2cAddress" in xinst) && xinstHW == comp.name) {
                return (Number(xinst.i2cAddress));
            }
        }
    }

    /* return the default address specified by the HW */
    return (Number(addr));
}

/*
 *  ======== getCompSpeed ========
 *  Get a component's maximum supported spped 
 */
function getCompSpeed(comp, unknownDefault) 
{
    let speed = comp.settings.maxSpeed;
    if (speed != null)  {
        return (speed);
    }
    if (unknownDefault === undefined) {
        return (100); /* standard mode 100 Kbps */
    }

    return (unknownDefault); 
}

/*
 *  ======== modules ========
 *  Return all used modules
 */
function modules(inst)
{
    return [
        {name : "Power", moduleName: "/ti/drivers/Power", hidden: true}
    ];
}

/*
 *  ======== validate ========
 *  Validate this inst's configuration
 *
 *            
 *  @param inst - I2C instance to be validated
 *  @param vo   - object to hold detected validation issues
 */
function validate(inst, vo)
{
    if (inst.$hardware != null) {
        /* get _all_ HW components attached to this I2C bus */
        let components = inst.$hardware != null ? inst.$hardware.$parents : [];

        //Common.print(components, "I2C_validate(" + inst.$name + "), comp: ", 2);

        /* validate speed settings against component specified ratings */
        _validateSpeed(inst, components, vo);

        /* validate I2C addresses used */
        _validateAddrs(inst, components, vo);
    }
}

/*
 *  ========= filterHardware ========
 *  Check 'component' signals for compatibility with I2C
 *
 *  @param component - hardware object describing signals and
 *                     pins to which they're attached
 *
 *  @returns true iff component is recognized as an I2C bus
 */
function filterHardware(component)
{
    let sda, scl;

    for (let sig in component.signals) {
        let type = component.signals[sig].type;
        if (Common.typeMatches(type, ["I2C_SCL"])) scl = sig;
        if (Common.typeMatches(type, ["I2C_SDA"])) sda = sig;
    }
    let result = (scl && sda) ? true : false;

    return (result);
}

/*
 *  ======== pinmuxRequirements ========
 *  Return peripheral pin requirements as a function of an instance's config
 */
function pinmuxRequirements(inst)
{
    let i2c = {
        /* name is the name of a property that will be added to inst;
         * this property's value is an object returned by the
         * pinmux solver
         */
        name: "i2c",
        displayName: "I2C Peripheral",

        interfaceName: "I2C",            /* pinmux interface name */

        /* request specific signals for I2C */
        resources: [
            {
                name: "sdaPin",          /* config script name */
                displayName: "SDA Pin",  /* GUI name */
                interfaceNames: ["SDA"]  /* pinmux tool name */
            },
            {
                name: "sclPin",
                displayName: "SCL Pin",
                interfaceNames: ["SCL"]
            }
        ]
    };

    /* map injected pinmux solution names to sig type names */
    i2c.signalTypes = {
        sdaPin: ["I2C_SDA"], /* warning: device-specific signal name */
        sclPin: ["I2C_SCL"]  /* warning: device-specific signal name */
    };

    return ([i2c]);
}

/*
 *  ======== _addrsOverlap ========
 *  Returns true iff two address arrays have a common element
 *
 *  @param a - an array of I2C addresses
 *  @param b - another array of I2C addresses
 *
 *  @returns true iff an element of a[] equals an element of b[]
 */
function _addrsOverlap(a, b)
{
    let addrs = {};
    let i;

    for (i = 0; i < a.length; i++) {
        addrs[a[i]] = true;
    }
    for (i = 0; i < b.length; i++) {
        if (addrs[b[i]]) {
            return (true);
        }
    }

    return (false);
}

/*
 *  ======== _genConflictMsg ========
 *  Generate an I2C address conflict error message
 *
 *  When _solve() fails to find a solution, use its output to generate a
 *  meaningful/actionable message.
 *
 *  @param ca     - input array of objects returned from _solve()
 *  @param msgs   - optional output array of instance-specific message objects
 *                  of the form: {inst: <instance>, msg: <error_string>}
 *  @param header - optional prefix to add to all returned messages
 *
 *  @returns  - a string containing an informative error message,
 *              or "" if there's no error.
 */
function _genConflictMsg(ca, msgs, header)
{
    if (msgs == null) {
        msgs = [];
    }

    /* if there's no conflict, return "" */
    if (ca[0].index >= 0) {
        return ("");
    }

    let msg = (header == null ? "" : header) 
              + "Conflicting I2C addresses between ";

    /* compute the last fatal conflict's addresses (solver sets index = -1) */
    let i;
    for (i = 0; i < ca.length; i++) {
        let comp = ca[i];
        if (comp.index >= 0) {
            break;
        }
    }
    let caddrs = ca[i - 1].addrs;

    /* ignore components that don't overlap with these addresses (caddrs) */
    let overlaps = {}; /* map overlaping comps to their referencing insts */
    let prefix = "";
    for (let j = 0; j < i; j++) {
        let comp = ca[j];
        if (_addrsOverlap(caddrs, comp.addrs)) {
            msg += prefix + comp.name + ":[" + comp.addrs + "]";
            prefix = ", ";
            overlaps[comp.name] = comp.usedBy;
        }
    }

    /* reset ca[] so that _solve() looks for _any_ solution supported by HW */
    for (let i = 0; i < ca.length; i++) {
        let comp = ca[i];
        comp.index = 0;          /* reset previous search state */
        comp.addrs = comp.range; /* search the full range of HW addresses */
    }
    
    /* append a suggestion: a solution supported by the HW */
    if (_solve(ca) == false) {
        msg += "; this appears to be an irreconcilable conflict. "
               + "Try not using one or more of the following components:\n";
        for (let j = 0; j < ca.length; j++) {
            let comp = ca[j];
            let refs = [];
            if (comp.usedBy.length > 0) {
                for (let k = 0; k < comp.usedBy.length; k++) {
                    refs.push(comp.usedBy[k].$name);
                }
                msg += "    " + comp.name + " (used by: " + refs + ")\n";
            }
        }
    }
    else {
        prefix = msg;
        msg += "; try the following address assignments:\n";
        for (let cname in overlaps) {
            msg += "\t" + cname;
            let usedBy = overlaps[cname];
            if (usedBy.length > 0) {
                msg += "(used by " + usedBy[0].$name + ")";
            }
            msg += ": ";

            /* get the computed valid address assignment for comp */
            let addr;
            for (let j = 0; j < ca.length; j++) {
                let comp = ca[j];
                if (comp.name == cname) {
                    addr = "0x" + comp.addrs[comp.index].toString(16);
                    break;
                }
            }
            msg += addr + '\n';
            for (let j = 0; j < usedBy.length; j++) {
                msgs.push({
                    inst: usedBy[j], 
                    msg: prefix + "; try address: " + addr
                });
            }
        }
    }

    return (msg);
}

/*
 *  ======== _genSolutionMsg ========
 *  Generate a printable I2C address assignment
 *
 *  When _solve() finds a solution, use its output to generate a
 *  meaningful message.
 *
 *  @param ca - the array of objects returned from _solve()
 *  @returns  - a string containing an informative message,
 *              or "" in the case there is novalid solution.
 */
function _genSolutionMsg(ca, header)
{
    /* if there's a conflict, return "" */
    if (ca[0].index < 0) {
        return ("");
    }

    /* sort used components ahead of unused then sort by address choices */
    ca.sort(function (a,b) {
        if (b.usedBy.length == a.usedBy.length) {
            return a.addrs.length - b.addrs.length;
        }
        return b.usedBy.length - a.usedBy.length;
    });

    let msg = header == null ? "" : header;
    for (let i = 0; i < ca.length; i++) {
        let comp = ca[i];
        msg += "    " + comp.name + ": 0x" + comp.addrs[comp.index].toString(16)
            + ((comp.usedBy.length == 0)
                ? " (unused)" : (" (used by " + comp.usedBy[0].$name + ")"))
            + "\n";
    }

    return (msg);
}

/*
 *  ======== _makeCompAddrArray ========
 *  Return an array of component address objects, one per HW component
 * 
 *  The returned array is used to find valid I2C address assignments for
 *  each component.  Specifically:
 *     o all used components must have unique addresses allowed by the HW
 *     o unused components may share addresses with other unused components
 *     o no unused component's address can be shared with a used component
 *
 *  Components are "used" (i.e., referenced) by an instance object's
 *  $hardware config _or_ by a module's static configs. An instance, say 
 *  inst, uses a component comp iff
 *      inst.$hardware.name = comp.name
 *
 *  and a module, say Mod, uses comp iff
 *      Mod.$static.$hardware.name == comp.name
 *
 *  All modules are found via the map system.modules.  Given a module, Mod,
 *  all its instances are provided via the array Mod.$instances
 *
 *  @param components  - an array of HW components connected to a common set
 *                       of I2C resources
 *
 *  @param useInstAddr - true => a referencing instance's I2C
 *                          address is taken as the _only_ allowed address for
 *                          the referenced HW component; all other components
 *                          have the full range of addresses allowed by the
 *                          board's HW.
 *                       false => the full range of addresses are used for
 *                           all components
 *
 *  @returns an array of objects, one for each component:
 *   {   name: "<comp name>",   // comp name
 *       addrs: [<addr> ...],   // allowable addresses for comp
 *       index: <addrs index>,  // addrs[index] is the addr for comp
 *       usedBy: []             // array of all instances that use comp (may
 *                              // be an empty array)
 *   }
 */
function _makeCompAddrArray(components, useInstAddr)
{
    /* create map of comp names to array of instances bound to it */
    let c2i = {}; /* map comp names -> array of instances bound to comp */
    for (let j = 0; j < components.length; j++) {
        let comp = components[j];

        /* issue: should pre-compute "I2C instances" bound to HW */
        for (let moduleName in system.modules) {
            let mod = system.modules[moduleName];
            let instances = mod.$instances;
            for (let i = 0; i < instances.length; i++) {
                let xinst = instances[i];
                let xinstHW = xinst.$hardware ? xinst.$hardware.name : null;

                /* issue: assuming i2cAddress always has default */
                if (("i2cAddress" in xinst) && xinstHW == comp.name) {
                    if (c2i[comp.name] == null) {
                        c2i[comp.name] = [];
                    }
                    c2i[comp.name].push(xinst);
                    //console.log(comp.name + " used by " + xinst.$name);
                }
            }
        }
    }

    /* create component addr solution array */
    let ca = [];
    for (let i = 0; i < components.length; i++) {
        let comp = components[i];

        /* get actual range of allowable addresses */
        let range = getAddresses(comp);

        /* compute a specified subset of range, addrs, to pass to the solver */
        let ia = c2i[comp.name];
        let addr = null;
        if (useInstAddr && ia != null) {
            /* if inst sets an addr, it's the only choice */
            addr = Number(ia[0].i2cAddress);
        }
        let addrs = addr ? [addr] : range;
        if (addrs.length > 0) {
            ca.push({
                name: comp.name, 
                addrs: addrs, 
                index: 0, 
                range: range,
                usedBy: (ia == null) ? [] : ia
            });
        }
    }

    return (ca);
}

/*
 *  ======== _solve ========
 *  Find a valid assignment of I2C addresses for the specified components
 *
 *  @param ca  - an array of components and their allowable addresses 
 *               returned by _makeCompAddrArray
 *
 *  @returns   - true => there is a solution, false => no solution
 *               In addition, the ca array contains the solution or a
 *               set of conflicting components
 * 
 *  Elements of ca are objects of the form:
 *  {
 *      name: <comp_name>,    // name of HW component
 *      addrs: [<addr>, ...], // set of allowable addresses
 *      index: 0,             // set by solver to index into addrs or -1
 *      usedBy: []            // possibly empty array of objects with an
 *                               "i2cAddress" property
 *  ]
 *
 */
function _solve(ca)
{
    //Common.print(ca, "_solve ca:\n");

    /* sort to maximize search tree pruning in backtrack */
    ca.sort(function (a,b) {return a.addrs.length - b.addrs.length;});

    let compIndex = 0;  /* curr comp to which to give an addr */
    let failIndex = 0;  /* max index considered before failing */
    let usedAddrs = {}; /* map addr -> stack of components using addr */
    while (compIndex >= 0 && compIndex < ca.length) {
        let comp = ca[compIndex];
        let caddr = comp.addrs[comp.index];

        /* if comp addr is unique, add a reference and move on to next comp */
        if (usedAddrs[caddr] == null || usedAddrs[caddr].length == 0) {
            usedAddrs[caddr] = [compIndex];
            compIndex++;
        }
        /* if caddr is used BUT both comp and caddr's 1st user are unused .. */
        else if (ca[usedAddrs[caddr][0]].usedBy.length == 0
                 && comp.usedBy.length == 0) {
            /* an overlap is allowable: it's ok for an address to be used by
             * multiple components provided ALL such components are unused
             */
            usedAddrs[caddr].push(compIndex); /* push a ref from unused comp */
            compIndex++;
        }
        else {
            /* otherwise, must try comp's next addr choice */
            comp.index++;

            /* save backtrack start (in case we need to reset comp.index) */
            failIndex = compIndex; /* used to generate failure message */

            /* if necessary, backtrack */
            while (comp.index >= comp.addrs.length) {
                comp.index = 0; /* reset current comp index */

                /* move back to prev comp, if possible */
                compIndex--;
                if (compIndex < 0) {
                    break; /* fail, no solution */
                }
                comp = ca[compIndex];

                /* pop our reference to prev addr from the used hash */
                usedAddrs[comp.addrs[comp.index]].pop();

                /* try new addr choice on previous comp */
                comp.index++;
            }
        }
    }

    /* if no solution, mark irreconcilable components (for error msg gen) */
    if (compIndex < 0) {
        for (let i = 0; i <= failIndex; i++) {
            ca[i].index = -1; /* -1 => ca[i] is in the conflicting set */
        }
        return (false);
    }

    return (true);
}

/*
 *  ======== _validateAddrs ========
 *  Check for address conflicts and log actionable messages
 *  
 *  In general, HW components can specify a range of
 *  addresses that are initialized at runtime; e.g., the INA226
 *  on the BOOSTXL-TLV8544PIR.  We use a simple solver to tell
 *  us whether
 *      o there exists any solution (including unused components) and
 *      o if there is a solution, which components need to be actively
 *        "managed" to properly initialize the HW component so
 *        as to not conflict with addresses being actively used by
 *        the application.
 *
 *  Assumptions:
 *      o all I2C addresses are configured by module instances that have 
 *        a config property named "i2cAddress".
 *      o each module instance using an I2C bus validates that its I2C
 *        address is in the set of addresses allowed by the HW component.
 *      o the first entry in a HW component's I2C address array is its
 *        "default" address; i.e., in the absence of any SW initialization
 *        of the HW component, the device will respond to this default
 *        address.
 *
 *  Address Checks:
 *      o all _used_ components connected to the I2C bus have unique addresses.
 *            - detects BoosterPack combos that break the application, but
 *            - intentionally allows conflicts between unused components 
 *      o the set of default addresses of all unused components is disjoint
 *        from the set of addresses assigned to the used components.
 *            - detects an initialization problem (unused HW interfering with
 *              properly configured instances)
 *      o all instances bound to a common HW component on the bus are
 *        configured with a common I2C address.
 *            - detects probable user error (requires runtime I2C addr control)
 *      o all components on the bus have unique default addresses
 *            - detects potential issue using the conflicting components
 *      
 *  Validation Messages:
 *      o in the event of address conflict between two used components, a 
 *        valid assignment is computed.  If there is a solution:
 *            all references are "tagged" with an error message suggesting 
 *            the valid computed alternative
 *        otherwise:
 *            the I2C bus is tagged with an irreconcilable conflict error
 *      o in the event of a conflict between used and unused components:
 *            all references are tagged with an error that identifies the 
 *            conflicting unused component.
 *      o in the event that different addresses are used to reference the
 *        same component:
 *            all references are tagged with an error that lists all
 *            instances referencing the same component and the address 
 *            they're using
 *      o in the event that different unused components have the same
 *        default address:
 *            optionally emit a warning identifying the overlapping 
 *            components
 */
function _validateAddrs(inst, components, vo)
{
    let logConflict = logHandlers[inst.addressChecks];

    /* gather all component address and usage information */
    let ca = _makeCompAddrArray(components, true);

    /* check that all insts referencing a component use the same addr */
    for (let i = 0; i < ca.length; i++) {
        let usedBy = ca[i].usedBy;
        if (usedBy.length < 2) {
            continue; /* need 2 or more to conflict */
        }
        let addr = usedBy[0].i2cAddress;
        let msg = "";
        for (let j = 1; j < usedBy.length; j++) {
            if (addr != usedBy[j].i2cAddress) {
                msg += usedBy[j].$name + ":" + usedBy[j].i2cAddress + " ";
            }
        }
        if (msg.length > 0) {
            /* compose actionable messages */
            msg = "multiple addresses used to reference the component "
                + ca[i].name + ": "
                + usedBy[0].$name + ":" + usedBy[0].i2cAddress + " "
                + msg;

            for (let j = 0; j < usedBy.length; j++) {
                logConflict(vo, usedBy[j], "i2cAddress", msg);
            }
            return;
        }
    }

    /* check for conflicts using the address specified by instances */
    if (ca.length > 0 && _solve(ca) == false) {
        var ierrs = [];
        let msg = _genConflictMsg(ca, ierrs);

        /* put an error on this I2C inst */
        logConflict(vo, inst, "$hardware", msg);

        /* plant errors on the instances involved in the conflict */
        for (let i = 0; i < ierrs.length; i++) {
            logConflict(vo, ierrs[i].inst, "i2cAddress", ierrs[i].msg);
        }
    }

    /* check for conflicts between unused components and instances */
    let unused = {}; /* map addr -> unused component name with this addr */
    for (let i = 0; i < ca.length; i++) {
        let comp = ca[i];
        if (comp.usedBy.length == 0) {
            /* accumulate the default addresses of unused components */
            if (unused[comp.addrs[0]] == null) {
                unused[comp.addrs[0]] = [];
            }
            unused[comp.addrs[0]].push(comp.name);
            //console.log("unused component " + comp.name + " addr: "
            //            + comp.addrs[0]);
        }
    }
    for (let i = 0; i < ca.length; i++) {
        let comp = ca[i];
        if (comp.usedBy.length > 0 && comp.index >= 0) {
            /* check that used addrs differ from the unused from above */
            let addr = comp.addrs[comp.index];
            //console.log("used component " + comp.name + " addr: "+ addr);
            if (unused[addr] != null) {
                let msg = comp.name
                    + " and the unused device(s) " + unused[addr]
                    + " share the same address: 0x" + addr.toString(16);

                /* TODO: suggest solution: electrically disconnect unused
                 * or, if the unused device supports more than one addr,
                 * use a driver to initialize the unused device to the 
                 * computed valid address.
                 */
                /* plant error on all instances that use this address */
                for (let j = 0; j < comp.usedBy.length; j++) {
                    logConflict(vo, comp.usedBy[j], "i2cAddress", msg);
                }
            }
        }
    }

    /* (optionally) warn about conflicts between unused components */
    if (inst.ignoreUnusedAddressConflicts == false) {
        for (let addr in unused) {
            if (unused[addr].length > 1) {
                let msg = "unused components " 
                    + unused[addr] + " share the same address: " + addr;
                logWarning(vo, inst, "$hardware", msg);
            }
        }
    }
}

/*
 *  ======== _validateSpeed ========
 *  Verify that the user specified bus speed is not greater than the
 *  components connected to the bus
 *
 * Speed Checks:
 *      o the declared max speeds of all components on the bus are verified
 *        to be >= the I2C bus's maxBitRate 
 *  
 *  Validation Messages:
 *      o if a HW component's declared maxSeed values is less that the bus's
 *        maxBitRate:
 *            the maxBitRate configuration parameter is tagged with a message
 *            that identifies the offending components
 */
function _validateSpeed(inst, components, vo)
{
    let logConflict = logHandlers[inst.speedChecks];

    for (let j = 0; j < components.length; j++) {
        let comp = components[j];
        let speed = getCompSpeed(comp, 5000);
        if (speed < inst.maxBitRate) {
            let msg = "the component " + comp.name
                + " can only support speeds up to " + speed + " Kbps";
            logConflict(vo, inst, "maxBitRate", msg);
        }
    }
}

/*
 *  ======== base ========
 *  Define the base properties and methods
 */
let base = {
    /* generic sysconfig module interface */
    displayName:         "I2C",
    description:         "Inter-Integrated Circuit (I2C) Bus Driver",
    longDescription:     "The I2C driver provides a simplified application"
        + " interface to access peripherals on an I2C bus.",
    documentation: "/tidrivers/doxygen/html/_i2_c_8h.html",
    defaultInstanceName: "Board_I2C",

    /* instance properties and methods */
    config:              config,
    modules:             modules,    /* used modules */
    validate:            validate,

    /* pinmux interface */
    busModule:           true, /* true => this module's insts are shareable */
    filterHardware:      filterHardware,
    pinmuxRequirements:  pinmuxRequirements,

    /* I2C interface exported for other modules/templates */
    getAddresses:        getAddresses,
    getCompAddress:      getCompAddress,
    getCompSpeed:        getCompSpeed,

    /* internal methods exposed for testing only */
    _genConflictMsg:     _genConflictMsg,
    _genSolutionMsg:     _genSolutionMsg,
    _solve:              _solve
};

/* extend the base exports to include family-specific content */
let deviceI2C = system.getScript("/ti/drivers/i2c/I2C" + family);
exports = deviceI2C.extend(base);
