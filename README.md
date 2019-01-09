Reverse engineering the Superior Freedom remote control
=======================================================

An attempt to reimplement the protocol between the computer and this USB-programmable universal remote control in Linux.

The USB exchange
----------------

The host sends reports on endpoint 1 out.
The remote answers each with a report from endpoing 1 in.

As specified in the report description:

- the reports sent from the host are 40 bytes long
  (marked (S) in usbmon)
- the reports from the remote are 20 bytes long
  (marked (C) in usbmon)
- features are not used; they are declared as 2 bytes long but not sent

Report ID
---------

According to USB HID, the first byte of each report is its ID, identifying the kind of report this one is. In this protocol, the IDs appears to be:

```
0x31	first report
0x56	answer to the first report

0x36	request for the devices currently programmed in the remote
0x52	answer: which devices the remote currently controls

0x33	???
0x34	remote control programming
0x35	end of programming
0x41	answer to the above three reports
```

First request
-------------

Maybe only used only to uniquely identify the remote. This request may be sent more than once, the first with packet `31 00 00 ...`, the others with `31 06 00 ...`, but maybe only the first byte really counts (the report ID).

The answer is always the same.

Request for devices currently programmed
----------------------------------------

This is how the host program knows with devices the remote currently controls. The request is a report `36 06 00 ....`. The answer is a report of ID `0x52`, followed by four bytes for each of the four programmable remotes. No part of these correspond to the "Code" as shown in the Windows program.

Programming
-----------

These reports are only sent when one of the devices the remote controls is changed. This programming operation is done by sending reports of ID `0x33` and ID `0x34`. The reports of ID `0x33` are almost all the same, like:

```
333f3f3f3f343f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f003434343400343434
```

Seen a whole, and removing the first byte (the report ID), they form a block that starts and ends with some `0x3f`, but the large central part contains only `0x34`.

Actual programming seems to be done only with reports of ID `0x34`. Following the byte `0x34`, these report start with a 2-byte progressive number that begins with `0x1000` and is increased by `0x0020` for each next report. In other words, the first report begins with `341000`, the second with `341020`, the third with `341040`, etc.

After this progressive number, the first reports begins with `706dffff0fff` followed by the identifier of the four remotes, the same four-byte numbers that is given back to reports of ID `0x52`.

All this reports together (without the report ID and the progressive numbers) form five blocks of varying length separated by zeros.

End of programming
------------------

Programming a new device in the remote ends with report ID `0x35`. This report begins with the report ID followed by the counter, restarted to `0x1000`. The four bytes `04404966` follow. The rest of the report may be irrelevant, since it content is the same as previous report, like a buffer that is not overwritten because its content does not matter.

Programs
--------

In this repository:

- **hidapi** sends reports and receives their answer; it currently only the reports of `0x31` and `0x36`, which normally occur when Windows program reads the current programming of the remote but does not change it

- **programming** extracts the blocks of data used by the Windows program to change the devices the remote controls; it reads the kind of data produced from `/sys/kernel/debug/usb/usbmon/3u` (if the remote is on USB bus 3); it removes the report IDs and the progressive numbers; with -33 or -35, it reads reports of ID `0x33` or `0x35`, otherwise those of ID `0x34`

