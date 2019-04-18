/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 */
/*
 *  ======== MemProtect.xdc ========
 */
package ti.sysbios.hal;

/*!
 *  ======== MemProtect ========
 *  Memory protection manager.
 *
 *  SYS/BIOS Memory Protection Extensions (MPE) add memory protection capability
 *  to the SYS/BIOS kernel. With this extension enabled
 *  (see {@link ti.sysbios.BIOS#mpeEnabled BIOS.mpeEnabled}), an application can
 *  restrict the memory and peripherals a Task can access, thereby creating a
 *  sandbox. The access restricted tasks run in unprivileged mode and are
 *  therefore also restricted in terms of the type of CPU instructions they can
 *  execute. For example, an access restricted or unprivileged task cannot
 *  disable interrupts directly.
 *
 *  SYS/BIOS MPE can be used to enhance the security of applications by
 *  compartmentalizing the application into mutually isolated tasks. In the
 *  simplest configuration, the security critical portions of the application
 *  can be moved to privileged tasks and the rest of the application can be run
 *  as unprivileged tasks. In a more advanced configuration, multiple mutually
 *  isolated tasks may be used to compartmentalize the application at a finer
 *  level of granularity.
 *
 *  SYS/BIOS MPE introduces the notion of execution domains. A Domain is a
 *  protected environment for the kernel and tasks to execute within and has
 *  an Access Control List (ACL) that is defined at the time of creating the
 *  domain. The kernel and privileged tasks execute in a special domain that
 *  allows them to access the entire address space. User Task domains are more
 *  restrictive and allow a user task to only access memory regions explicitly
 *  owned by the domain or memory regions that are public and accessible to all
 *  domains.
 *
 *  A domain can be created in main() or a privileged task only. A User Task is
 *  assigned to an existing domain at the time of its creation. A User Task may
 *  create more threads which inherit its domain. A User Task cannot assign any
 *  other domain to the tasks it creates.
 *
 *  This module contains APIs used to create and/or construct domains at
 *  runtime.
 *
 *  @p(html)
 *  <h3> Calling Context </h3>
 *  <table border="1" cellpadding="3">
 *    <colgroup span="1"></colgroup> <colgroup span="5" align="center">
 *    </colgroup>
 *
 *    <tr><th> Function                 </th><th>  Hwi   </th><th>  Swi   </th>
 *    <th>  Task  </th><th>  Main  </th><th>  Startup  </th></tr>
 *    <!--                                                         -->
 *    <tr><td> MemProtect_constructDomain </td><td>   N   </td><td>   N   </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td> MemProtect_destructDomain </td><td>   N    </td><td>   N   </td>
 *    <td>   Y    </td><td>   Y    </td><td>   N    </td></tr>
 *    <tr><td colspan="6"> Definitions: <br />
 *       <ul>
 *         <li> <b>Hwi</b>: API is callable from a Hwi thread. </li>
 *         <li> <b>Swi</b>: API is callable from a Swi thread. </li>
 *         <li> <b>Task</b>: API is callable from a Task thread. </li>
 *         <li> <b>Main</b>: API is callable during any of these phases: </li>
 *           <ul>
 *             <li> In your module startup after this module is started
 *    (e.g. Hwi_Module_startupDone() returns TRUE). </li>
 *             <li> During xdc.runtime.Startup.lastFxns. </li>
 *             <li> During main().</li>
 *             <li> During BIOS.startupFxns.</li>
 *           </ul>
 *         <li> <b>Startup</b>: API is callable during any of these phases:</li>
 *           <ul>
 *             <li> During xdc.runtime.Startup.firstFxns.</li>
 *           </ul>
 *       </ul>
 *    </td></tr>
 *
 *  </table>
 *  @p
 *
 *  @a(C Synopsis)
 *  @p(code)
 *  #include <ti/sysbios/hal/MemProtect.h>
 *  @p
 *
 *  @p(html)
 *  <h3> Macros </h3>
 *  @p
 *  @p(code)
 *  // Access Privilege Flags
 *  #define MemProtect_USER_READ
 *  #define MemProtect_USER_WRITE
 *  #define MemProtect_USER_EXEC
 *
 *  #define MemProtect_PRIV_READ
 *  #define MemProtect_PRIV_WRITE
 *  #define MemProtect_PRIV_EXEC
 *
 *  // Memory Type Flags
 *  #define MemProtect_DEVICE
 *  #define MemProtect_DEVICE_UNBUFFERED
 *  #define MemProtect_NONCACHEABLE
 *  #define MemProtect_WRITEBACK
 *  #define MemProtect_WRITETHROUGH
 *  #define MemProtect_WRITEALLOCATE
 *  #define MemProtect_SHAREABLE
 *
 *  // ACL Helper Macros
 *  #define MemProtect_NUM_KERNEL_ACL_ENTRIES
 *  #define MemProtect_NUM_USER_ACL_ENTRIES
 *  #define MemProtect_NUM_ACL_ENTRIES
 *  @p
 *
 *  @p(html)
 *  <h3> Typedefs </h3>
 *  @p
 *  @p(code)
 *  typedef struct MemProtect_Acl {
 *      Ptr    baseAddress;
 *      SizeT  length;
 *      UInt32 flags;
 *  } MemProtect_Acl;
 *
 *  typedef MemProtect_Struct *MemProtect_Handle;
 *  @p
 *
 *  @p(html)
 *  <h3> Functions </h3>
 *  @p
 *  @p(code)
 *  Int MemProtect_constructDomain(MemProtect_Struct *obj, struct MemProtect_Acl *acl, UInt16 aclLength)
 * 
 *  Void MemProtect_destructDomain(MemProtect_Struct *obj)
 *  @p
 *
 *  @a(MemProtect Functions)
 *
 *  @p(html)
 *  <h3> MemProtect_constructDomain() </h3>
 *  @p
 *  Construct an execution domain with given access control permissions
 *  @p(code)
 *  Int MemProtect_constructDomain(MemProtect_Struct *obj, struct MemProtect_Acl *acl, UInt16 aclLength)
 *  @p
 *  @p(html)
 *  <h4> ARGUMENTS </h4>
 *  @p(code)
 *  obj - Pointer to a statically created MemProtect object of type MemProtect_Struct
 *  acl - Pointer to an array of ACL entries. ACL entries are of type MemProtect_Acl
 *  aclLength - Length of ACL entry array
 *  @p(html)
 *  <h4> RETURNS </h4>
 *  @p
 *  Returns 0 on success. A negative return value indicates an error occurred.
 *  @p(html)
 *  <h4> DETAILS </h4>
 *  @p
 *  This function constructs an execution domain with the access permissions
 *  specified by the Access Control List (ACL) passed to it.
 *
 *  @p(html)
 *  <br/>
 *  <h3> MemProtect_destructDomain() </h3>
 *  @p
 *  Destruct execution domain
 *  @p(code)
 *  Int MemProtect_destructDomain(MemProtect_Struct *obj)
 *  @p
 *  @p(html)
 *  <h4> ARGUMENTS </h4>
 *  @p(code)
 *  obj - Pointer to a statically created MemProtect object of type MemProtect_Struct
 *  @p(html)
 *  <h4> RETURNS </h4>
 *  @p
 *  Returns 0 on success. A negative return value indicates an error occurred.
 *  @p(html)
 *  <h4> DETAILS </h4>
 *  @p
 *  This function destructs a previously constructed execution domain.
 */

@CustomHeader
module MemProtect
{
}
