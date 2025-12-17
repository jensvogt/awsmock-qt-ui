function Component() {}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {

        var targetExe = installer.value("TargetDir") + "/awsmock-qt-ui.exe";

        var startMenuDir =
            installer.value("StartMenuDir") + "/AwsMock";

        component.addOperation(
            "CreateShortcut",
            targetExe,
            startMenuDir + "/awsmock-qt-ui.lnk",
            "workingDirectory=" + installer.value("TargetDir"),
            "iconPath=" + targetExe,
            "description=AwsMock Qt UI"
        );

        component.addOperation(
            "CreateShortcut",
            installer.value("TargetDir") + "/awsmock-qt-ui.exe",
            installer.value("DesktopDir") + "/awsmock-qt-ui.lnk",
            "workingDirectory=" + installer.value("TargetDir")
        );
    }
};