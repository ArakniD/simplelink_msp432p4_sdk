# SimpleLink™ MSP432P4 SDK 2.40.00.10 Release Notes

## Introduction
The SimpleLink™ MSP432P4 SDK delivers components that enable engineers to develop applications on Texas Instruments MSP432P4 microcontroller devices. The product is comprised of multiple software components and examples of how to use these components together. In addition, examples are provided to demonstrate the use of each functional area and each supported device and as a starting point for your own projects.

The SimpleLink™ MCU portfolio offers a single development environment that delivers flexible hardware, software and tool options for customers developing wired and wireless applications. With 100 percent code reuse across host MCUs, Wi-Fi™, Bluetooth® low energy, Sub-1GHz devices and more, choose the MCU or connectivity standard that fits your design. A one-time investment with the SimpleLink software development kit (SDK) allows you to reuse often, opening the door to create unlimited applications. For more information, visit www.ti.com/simplelink.

This is version 2.40.00.10 of the SimpleLink MSP432P4 SDK.

## What’s New
Please refer to the Documentation Overview for the individual release notes for details for each SDK component

SimpleLink Platform level changes are available in SimpleLink Core SDK Release Notes

SDK Change Log provides a list of all SDK component changes

## Upgrade and Compatibility Information
If you are migrating from MSP432Ware please read the Migration Guide.

The Grlib component for MSP432P4 has been extended to provide support for all devices in the SimpleLink Platform for a centralized solution. This change has impacted the pre-built grlib library name and location in the SDK and requires a modification to the grlib link resource path in your application from previous SDK releases.Please see the SimpleLink Core SDK Upgrade and Compatibility Information for details of grlib changes.

Please see the SimpleLink Core SDK Upgrade and Compatibility Information for its updates on core sdk compatibility breaks.

## Operating Systems Support
* Windows 7, Windows 8, Windows 10
* Ubuntu 14.04 64-bit, Ubuntu 16.04 64-bit
* macOS Sierra

## Dependencies
This release was validated with the following components:

* Code Composer Studio 8.3.0
** ARM 18.1.4.LTS
** XDCTools 3.51.01.18
* GNU Code Generation Tools
** ARM gcc-arm-none-eabi-7-2017-q4-major
* IAR Code Generation Tools
** ARM 8.20.2
* Keil uVision (A subset of the SDK components support Keil IDE)
** 5.23
** TexasInstruments.MSP432E4_DFP.3.2.3.pack

This release of SDK is CMSIS 5.0.1 Compliant.

## Device Support
* MSP432P401R (Rev C and later)
* MSP432P401M (Rev C and later)
* MSP432P401V
* MSP432P401Y
* MSP432P4011
* MSP432P4111
* MSP432P411V
* MSP432P411Y
* MSP-EXP432P401R (Red Launchpad - Rev2.0)
* MSP-EXP432P4111 Launchpad

## Known Issues
* “Create New Project” wizard in CCS does not create projects using the SimpleLink MSP432P4 SDK
* Please refer to the Documentation Overview for the individual release notes for details for each SDK component

## Versioning
This product’s version follows a version format, M.mm.pp.bb, where M is a single digit Major number, mm is 2 digit minor number, pp is a 2 digit patch number, and b is an unrestricted set of digits used as an incrementing build counter.

M - Is incremented for the first release every year. 1 -> 2017, 2 -> 2018, and so on..

mm - indicates the specific quarter of the year the SDK was released. 10 -> Q1; 20 -> Q2; 30 -> Q3; 40 -> Q4

## Prior Releases
* 2.30.00.14
* 2.20.00.12
* 2.10.00.14
* 1.60.00.12
* 1.50.00.12
* 1.40.01.00
* 1.40.00.28
* 1.30.00.40
* 1.20.00.45