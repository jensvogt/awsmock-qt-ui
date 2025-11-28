function Component() {}

Component.prototype.install = function() {
    this.createShortcuts();
}

Component.prototype.uninstall = function() {
    // Optional cleanup
}

Component.prototype.createShortcuts = function()
{
    if (systemInfo.productType === "windows") {
        var iconPath = installer.value("TargetDir") + "/awsmock-qt-ui.exe";
        var programs = installer.environmentVariable("PROGRAMDATA");
        var desktop = installer.environmentVariable("PUBLIC") + "\\Desktop";

        // Desktop shortcut
        component.addOperation("CreateShortcut", iconPath, desktop + "\\AWSMock UI.lnk",
            "workingDirectory=" + installer.value("TargetDir"), "iconPath=" + iconPath);

        // Start menu shortcut
        component.addOperation("CreateShortcut", iconPath,
            programs + "\\Microsoft\\Windows\\Start Menu\\Programs\\AWSMock UI.lnk",
            "workingDirectory=" + installer.value("TargetDir"), "iconPath=" + iconPath
        );
    }
}
