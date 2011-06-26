var g_updateIdentityFunction;
var g_serverName;
var g_errorID;
var g_statusID;
var g_browserID;
var g_useStatus = false;
var g_useBrowser = false;

var g_writeCtrl = false;
var g_writePlugin = false;

function GetComputerIdentity(updateFunction, serverName, errorID, statusID, browserID)
{
    g_updateIdentityFunction = updateFunction;
    g_serverName = serverName;
    g_useStatus  = typeof statusID  != 'undefined'; 
    g_useBrowser = typeof browserID != 'undefined'; 
    g_errorID    = errorID;
    if (g_useStatus);
        g_statusID = statusID;
    if (g_useBrowser)
        g_browserID  = browserID;
    
    try
    {  
        if (navigator.appName.indexOf("Internet Explorer") == -1)
        {
            if (g_useBrowser)
                document.getElementById(g_browserID).innerHTML = "Non-IE Browser";
            
            if (navigator.platform != 'Win32')
                throw "Computer Identity is only supported on Windows operating systems. Contact your office administrator for more information.";
                
            if (typeof InstallTrigger != 'undefined')
            {
                if (g_useStatus)
                    document.getElementById(g_statusID).innerHTML = "Checking if updates are enabled...";
    
                if (g_useStatus)                
                    document.getElementById(g_statusID).innerHTML = "Checking version...";                
                if (InstallTrigger.CompareVersion('@teztech.com/ComputerIdentityPlugin', 1,0,0,12 ) < 0)
                {
                    if (!InstallTrigger.UpdateEnabled())
                        throw "Please enable Software Installation in your browser so that the Computer Identity Plugin can be installed. Contact your office administrator for more information.";
                
                    if (g_useStatus)
                        document.getElementById(g_statusID).innerHTML = "Installing plugin...";
                    var pluginUrl = 'http://$ServerName$/controls/ComputerIdentityPlugin.xpi'.replace('$ServerName$', serverName);
                    InstallTrigger.install({'Computer Identity' : pluginUrl},  OnComputerIdentityInstallStatus);
                    if (g_useStatus)
                        document.getElementById(g_statusID).innerHTML += "InstallTrigger.install has completed";
                }
                else
                {
                    if (g_useStatus)
                        document.getElementById(g_statusID).innerHTML = "Correct version of plugin is installed";
                    WritePlugin();
                    CheckIdentity();
                }
            }
            else
            {
                if (g_useStatus)
                    document.getElementById(g_statusID).innerHTML = "No InstallTrigger support - assuming that plugin is installed";
                WritePlugin();
                CheckIdentity(true);
            }
        }
        else
        {
            if (g_useBrowser)
                document.getElementById(g_browserID).innerHTML = "IE Browser";
            WriteCtrl();
            IECheckIdentity();
        }
    }
    catch(e)
    {
        var message = "Cannot install Computer Identity. The error message returned by the operating system is: " + e.message;
        document.getElementById(g_errorID).innerHTML = message;
        document.getElementById(g_errorID).style.display = "";
    }
}

function WriteCtrl()
{
   var output =
'<object id="ComputerIdentity" classid="clsid:4B631C42-F4D9-42B8-9E4E-5FEA45DF488D" codebase="http://$ServerName$/controls/ComputerIdentityCtrl.cab#version=1,0,0,4" \
    width="0" height="0" visible="false"> \
</object>';
    output = output.replace("$ServerName$", g_serverName);
    var div = document.createElement('div');
    div.style.display = 'none';
    div.style.position = 'absolute';
    div.style.top = '0';
    div.style.left = '0';
    div.innerHTML = output;
    document.getElementById(g_errorID).appendChild(div);
    if (g_useStatus)
        document.getElementById(g_statusID).innerHTML = "Computer Identity object added to DOM";
}

function WritePlugin()
{
    var output =
'<object id="ComputerIdentity" style="position:absolute;top:0;left:0;" type="application/x-computeridentity" codebase="http://$ServerName$/controls/ComputerIdentityPlugin.xpi" \
    width="0" height="0"> \
</object>';
    output = output.replace("$ServerName$", g_serverName);
    var div = document.createElement('div');
    div.style.position = 'absolute';
    div.style.top = '0';
    div.style.left = '0';
    div.innerHTML = output;
    document.body.appendChild(div);
}

function OnComputerIdentityInstallStatus(url, status)
{
    var message;
    switch(status)
    {
        case 0:
            message = 'Computer Identity has been successfully installed.';
            break;
        case 999:
            message = 'Please restart the browser to complete Computer Identity installation.';
            break;
        case -210:
            message = 'Computer Identity installation has been canceled by user.';
            break;
        default:
            message = 'Computer Identity installation has failed. Error code: ' + status;
            break;
    }
    
    if (status == 0)
    {
        if (g_useStatus)
            document.getElementById(g_statusID).innerHTML = message;
        WritePlugin();
        CheckIdentity();
    }
    else
    {
        document.getElementById(g_errorID).innerHTML = message;
        document.getElementById(g_errorID).style.display = "";
        // alert(message);
    }
    
    g_installTriggerComplete = true;
    
}

function IECheckIdentity()
{
    // IE requires a timer because we don't know when the ActiveX control will be installed
    
    if (typeof document.getElementById('ComputerIdentity').Identity != 'undefined')
    {
        var computerIdentity = document.getElementById('ComputerIdentity').Identity;
        g_updateIdentityFunction(computerIdentity);
    }
    else
    {
        setTimeout('IECheckIdentity()', 500);
    }
}

function CheckIdentity(assumePluginArg)
{
    var assumePlugin = typeof assumePluginArg != 'undefined' ? assumePluginArg : false; 
    
    if(assumePlugin && (typeof document.getElementById('ComputerIdentity') == 'undefined' || typeof document.getElementById('ComputerIdentity').Identity == 'undefined'))
    {
        var message = 
'Your browser does not appear to support automatic installation of plugins. You may wish to try installing the Computer Identity plugin \
manually from the following URL: <a href="http://$ServerName$/controls/ComputerIdentityPlugin.xpi">http://$ServerName$/controls/ComputerIdentityPlugin.xpi</a>';
        message = message.replace('$ServerName$', g_serverName);
        message = message.replace('$ServerName$', g_serverName);

        document.getElementById(g_errorID).innerHTML = message;
        document.getElementById(g_errorID).style.display = "";
    }
    else
    {
        var computerIdentity = document.getElementById('ComputerIdentity').Identity;
        g_updateIdentityFunction(computerIdentity);
    }
}
