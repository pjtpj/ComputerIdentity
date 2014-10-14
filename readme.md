ComputerIdentity
================

We have a client that wants to make sure that his agents cannot give out their logins to anybody 
else. I made some suggestions of social ways to deal with the problem, but he's really worried 
and he's going to pay me to develop some code to prevent it. Well, the customer is always 
right, eh? 

So, my initial though is to do a MAC address logging and checking similar to that done by the 
license monitoring code in Search Engine Commando. Basically, if the user's MAC address in the 
database is empty, then we allow the login, read the MAC address (see below), then 
store it in ListingsTech database. If the user's MAC address is already in the ListingsTech 
database, and the MACs match, then access is allowed. If the MACs don't match, the account will 
be locked out (customer's desired outcome). I envision three fields added to the User account 
associated with an agent:

- Validate MAC Address
- Invlaid MAC Addresses Deactivates login

Unfortunately, reading the MAC address from a web app will require an ActiveX control (IE) or 
Plug-in (Firefox, Opera) because as far as I know, due to security considerations, JavaScript 
cannot read system level stuff like this. Writing an ActiveX control is not ultra-hard. I've 
already done that for my digital camera upload control (currenly know as the dcupload project). 
Getting a Plug-in for Firefox looks harder. There is a Plug-in that will allow you to use an 
ActiveX control, but that looks almost as hard to deal with as just writing a Plug-in. So, the 
Active X/Plug-In will work but it's brutish and won't support Macs or Linux. I can't think of 
a way to make it work with just IP addresses. 

Ideally, the plug-in will work on browsers other than Firefox. I did find some pages about 2 
different standard plug-in APIs that the various browsers supposedly support:

  http://www.opera.com/docs/pluginapi/ - However, I could find no other references to 
    the "netscape4 plug-in API".

  http://www.mozilla.org/projects/plugins/npruntime.html - This looks like some kind of standard, 
    but I couldn't find any supporting SDKs or other references. 

The SDK I did find is linked at the bottom of this page: http://www.mozilla.org/projects/plugins/npruntime.html. 
It may not support Opera, etc.., but it's the one I plan to try right now.

In addition to plugins, Firefox also support "Extensions". However, from what I can tell 
extensions must be written in JavaScript and so won't have any obvious way to access MAC 
addresses. Also, I think only plugins can be semi-automatically installed.

Firefox Plugins do have a historacle quirk of requiring a MIME type. I believe this is left 
over from the days when the only purpose of a plugin was to replace "helper apps". If we have 
to register a MIME type, a possible MIME type we could adopt is 
application/x-computeridentity (application/x- is a generic prefix for a non-registered MIME type).

I have a code signing certificate for Teztech, Inc. Here is how to sign the ActiveX control. I'm not sure 
if just the DLL has to be signed, the CAB or both. To create the cab, change to the source code directory 
and run this command:

	cd \wksrc\ComputerIdentity
	cabarc -c N ComputerIdentity.cab ComputerIdentity.inf ReleaseMinDependency\ComputerIdentity.dll
	signcode /spc \wksrc\TezTech\CodeSigningCertificate\mycert.spc /v \wksrc\TezTech\CodeSigningCertificate\mykey.pvk -t "http://timestamp.verisign.com/scripts/timstamp.dll" /n "Computer Identity" /i "http://www.listingstech.com/controls/ComputerIdentity.aspx" ComputerIdentity.cab
	chktrust ComputerIdentity.cab
	copy ComputerIdentity.cab \wksrc\listings\www\controls\  (or wherever)

Obviously, you will have to find a copies of cabarc, signcode and chktrust and put them on your path. 

ComputerIdentityCtrl

This is the project for the Computer Identity ActiveX control. To create the project, I pretty much
just followed the instructions for creating an ActiveX control in MSDN (in the ATL section). I used 
the example without "attributes". Attributes make sense, but I didn't see that I needed them for 
this project. After creating the project, adding the control and adding the properties with
the wizards, I went back into the project properties to select static linking, no unicode and no 
debugging output. I also added a post build event to create and sign the control's cab. 

ComputerIdentityPlugin

This is the project for the Computer Identity Plugin for Firefox. It is possible that the same
DLL might work for Opera (and other Windows browsers), but the JavaScript install mechanism and
.xpi archive file don't seem to be supported. Oh well, next time.

To create the initial DLL project, I started with the Win32 DLL Wizard, but then I had to 
go back and laboriously copy every project setting from the scriptable sample I already had
working. Also, I found out the hard way you have to be really careful about the version 
resources. Use a working project as an example. They have to be in the percise format that 
Firefox can read. It took me hours to figure out that I needed BLOCK "040904e4" instead of the 
BLOCK "XXX" number that the DLL Wizard used. I added a post build event to create the plugin's xpi.
